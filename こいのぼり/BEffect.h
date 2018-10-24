#pragma	warning(disable:4996)
#pragma once
//-------------------------------------------
//エフェクト汎用スーパークラス
//-------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "BEffect.h"

class BEffect : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BEffect>		SP;
	typedef weak_ptr<BEffect>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	ML::Vec2	pos;
	ML::Box2D	effImg[30];
	int			animCnt;
	float			life;
	float			timeCnt;
	float			speed;

	//-----◆メンバ変数を追加したら、必ず初期化も追加する---◆
	BEffect() : 
		pos(0, 0),
		animCnt(0),
		life(1.0f),
		timeCnt(0.1f),
		speed(3.0f)
	{
		for (int i = 0; i < 30; ++i) {
			this->effImg[i] = ML::Box2D(0, 0, 0, 0);
		}
	}
	virtual	~BEffect() {}

	//共通メソッド
	void Anim();
};
