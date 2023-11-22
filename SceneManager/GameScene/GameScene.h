#pragma once

#include <memory>
#include "Utils/Camera/Camera.h"
#include "Game/Enemy/Enemy.h"
#include "Game/FollowCamera/FollowCamera.h"
#include "Game/Ground/Ground.h"
#include "Game/Player/Player.h"
#include "Game/Skydome/Skydome.h"
#include "Game/Goal/Goal.h"
#include "Game/LockOn/LockOn.h"
#include <list>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	//DirectXCommon* dxCommon_ = nullptr;

	Camera camera_;

	std::unique_ptr<Player> player_;

	FollowCamera followCamera_;

	std::list<std::unique_ptr<Enemy>> enemies_;

	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Goal> goal_;

	std::unique_ptr<LockOn> lockOn_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
