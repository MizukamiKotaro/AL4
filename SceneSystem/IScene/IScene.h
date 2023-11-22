#pragma once

enum SCENE { TITLE, STAGE, CLEAR };

// シーン内での処理を行う基底クラス
class IScene
{
public:
	// シーン番号を管理する変数
	static int sceneNo_;

public:
	// 継承先で実装される関数
	// 抽象クラスなので純粋仮想関数とする
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~IScene();

	// シーン番号のゲッター
	int GetSceneNo();
};

