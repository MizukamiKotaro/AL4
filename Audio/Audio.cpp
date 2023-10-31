#include "Audio.h"
#include <cassert>
#include <fstream>

Audio* Audio::GetInstance() {
	static Audio instance;
	return &instance;
}

void Audio::Initialize() {

	HRESULT hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));
}

void Audio::Finalize() {

	xAudio2_.Reset();

	/*for (Voice* voice : voices_) {
		delete voice;
	}*/

	for (SoundData soundData : soundDatas_) {
		Unload(&soundData);
	}

}

uint32_t Audio::LoadWave(const std::string& filename) {

	for (uint32_t soundDataNum = 0; soundDataNum < static_cast<uint32_t>(soundDatas_.size()); soundDataNum++) {

		if (soundDatas_[soundDataNum].name == filename) {
			return soundDataNum;
		}
	}


	std::ifstream file;

	file.open(filename, std::ios_base::binary);

	assert(file.is_open());

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	FormatChunk fmt = {};

	file.read((char*)&fmt, sizeof(ChunkHeader));
	if (strncmp(fmt.chunk.id, "fmt", 4) != 0) {
		assert(0);
	}

	assert(fmt.chunk.size <= sizeof(fmt.fmt));
	file.read((char*)&fmt.fmt, fmt.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	if (strncmp(data.id, "JUNK", 4) != 0) {
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	SoundData soundData = {};

	soundData.wfex = fmt.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
	soundData.name = filename;

	soundDatas_.push_back(soundData);

	return static_cast<uint32_t>(soundDatas_.size() - 1);

}

void Audio::Unload(SoundData* soundData) {

	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

uint32_t Audio::PlayWave(uint32_t soundDataHandle, bool loopFlag, float volume) {

	HRESULT hr;

	voices_.push_back(std::make_unique<Voice>());
	
	hr = xAudio2_->CreateSourceVoice(&voices_.back()->sourceVoice, &soundDatas_[soundDataHandle].wfex);
	assert(SUCCEEDED(hr));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundDatas_[soundDataHandle].pBuffer;
	buf.AudioBytes = soundDatas_[soundDataHandle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	if (loopFlag) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	hr = voices_.back()->sourceVoice->SubmitSourceBuffer(&buf);
	hr = voices_.back()->sourceVoice->Start();
	hr = voices_.back()->sourceVoice->SetVolume(volume);

	voices_.back()->handle = static_cast<uint32_t>(voices_.size() - 1);

	return voices_.back()->handle;
}

void Audio::StopWave(uint32_t voiceHandle) {

	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->handle == voiceHandle) {
			voice->sourceVoice->Stop();
			break;
		}
	}
}

void Audio::SetVolume(uint32_t voiceHandle, float volume) {
	for (const std::unique_ptr<Voice>& voice : voices_) {
		if (voice->handle == voiceHandle) {
			voice->sourceVoice->SetVolume(volume);
			break;
		}
	}
}
