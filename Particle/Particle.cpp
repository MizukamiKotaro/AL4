#include "Particle.h"
#include <cassert>
#include <cmath>
#include <numbers>
#include <random>
#include <algorithm>

Particle::Particle()
{
	for (int i = 0; i < 10; i++) {
		models_[i] = std::make_unique<Model>("Resources/Cube", "Cube.obj");
		transforms_[i] = Transform();
		transforms_[i].scale_ *= 0.4f;
		models_[i]->transform_.parent_ = &transforms_[i];
		models_[i]->color_ = { 0.1f,0.1f,0.1f,1.0f };
	}

	isDraw_ = false;
}

void Particle::Initialize() {

	isDraw_ = false;
}

void Particle::Update() {

	if (isDraw_) {
		count_++;
		if (count_ >= 90) {
			count_ = 0;
			isDraw_ = false;
		}
		else {
			for (int i = 0; i < 10; i++) {
				transforms_[i].translate_ += velcities_[i];
				transforms_[i].rotate_.y += 0.01f;
				std::random_device rand;
				models_[i]->color_.w = float(rand() % 2);
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		transforms_[i].UpdateMatrix();
		models_[i]->Update();
	}
	
}

void Particle::Draw(const Matrix4x4& viewProjection)
{
	if (isDraw_) {
		for (int i = 0; i < 10; i++) {
			models_[i]->Draw(viewProjection);
		}
	}
}

void Particle::Begin(const Vector3& pos)
{
	isDraw_ = true;
	count_ = 0;
	for (int i = 0; i < 10; i++) {
		transforms_[i].translate_ = pos;
		std::random_device rand;
		transforms_[i].rotate_.y = 1.0f / float(rand() % 10 + 1);
		velcities_[i] = Vector3{ float(rand() % 20) - 10.0f, float(rand() % 10) + 2.0f, float(rand() % 20) - 10.0f };
		velcities_[i] = velcities_[i].Normalize() / float(rand() % 10 + 1);
	}
}
