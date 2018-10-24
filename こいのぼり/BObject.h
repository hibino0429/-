#pragma	warning(disable:4996)
#pragma once
//-------------------------------------------
//オブジェクト汎用スーパークラス
//-------------------------------------------
#include "GameEngine_Ver3_7.h"

class BObject : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BObject>		SP;
	typedef weak_ptr<BObject>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	ML::Vec2		pos;				//位置	
	ML::Box2D		hitBase;			//あたり判定
	ML::Box2D		crystalImg[30];		//画像
	int				animCnt;			//アニメカウント
	int				point;				//ポイント

	//-----◆メンバ変数を追加したら、必ず初期化も追加する---◆
	BObject() : pos(0, 0),
		hitBase(-8, -8, 16, 16),
		crystalImg{
		ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),
		ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),
		ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),
		ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),
		ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),
		ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0)
	},
		animCnt(0),
		point(0)
	{}
	virtual	~BObject() {}

	//共通メソッド
	virtual void	Anim();			//アニメーション
	virtual bool	HitPlayer();	//プレイヤーとのあたり判定
	virtual void	ScorePlus(int point_);	//スコア増減
}; 
