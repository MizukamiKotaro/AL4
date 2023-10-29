#pragma once

#include <memory>
#include "../Camera/Camera.h"
#include "../Enemy/Enemy.h"
#include "../FollowCamera/FollowCamera.h"
#include "../Ground/Ground.h"
#include "../Player/Player.h"
#include "../Skydome/Skydome.h"
#include "../Goal/Goal.h"

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

	std::unique_ptr<Enemy> enemy_;

	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Goal> goal_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
