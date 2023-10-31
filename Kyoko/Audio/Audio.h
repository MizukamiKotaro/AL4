#pragma once

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <wrl.h>
#include <string>
#include <vector>
#include <memory>

class Audio
{

	struct ChunkHeader {
		char id[4];
		int32_t size;
	};

	struct RiffHeader {
		ChunkHeader chunk;
		char type[4];
	};

	struct FormatChunk {
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

	// 音声データ
	struct SoundData {
		// 波形フォーマット
		WAVEFORMATEX wfex;
		// バッファ
		BYTE* pBuffer;

		std::string name;

		unsigned int bufferSize;
	};

	struct Voice {
		uint32_t handle = 0u;
		IXAudio2SourceVoice* sourceVoice;
	};

	static Audio* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// WAV音声読み込み
	/// </summary>
	/// <param name="filename">WAVファイル名</param>
	/// <returns>サウンドデータハンドル</returns>
	uint32_t LoadWave(const std::string& filename);

	/// <summary>
	/// サウンドデータの解放
	/// </summary>
	/// <param name="soundData">サウンドデータ</param>
	void Unload(SoundData* soundData);

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="soundDataHandle">サウンドデータハンドル</param>
	/// <param name="loopFlag">ループ再生フラグ</param>
	/// <param name="volume">ボリューム
	/// 0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする</param>
	/// <returns>再生ハンドル</returns>
	uint32_t PlayWave(uint32_t soundDataHandle, bool loopFlag = false, float volume = 1.0f);

	/// <summary>
	/// 音声停止
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	void StopWave(uint32_t voiceHandle);

	/// <summary>
	/// 音声再生中かどうか
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	/// <returns>音声再生中かどうか</returns>
	//bool IsPlaying(uint32_t voiceHandle);

	/// <summary>
	/// 音声一時停止
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	//void PauseWave(uint32_t voiceHandle);

	/// <summary>
	/// 音声一時停止からの再開
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	//void ResumeWave(uint32_t voiceHandle);

	/// <summary>
	/// 音量設定
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	/// <param name="volume">ボリューム
	/// 0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする</param>
	void SetVolume(uint32_t voiceHandle, float volume);


private:

	Audio() = default;
	~Audio() = default;
	Audio(const Audio&) = delete;
	const Audio& operator=(const Audio&) = delete;

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;

	std::vector<SoundData> soundDatas_;
	std::vector<std::unique_ptr<Voice>> voices_;
};

