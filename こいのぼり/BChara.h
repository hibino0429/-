#pragma	warning(disable:4996)
#pragma once
//-------------------------------------------
//キャラクタ汎用スーパークラス
//-------------------------------------------
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	enum Dir {
		Left,
		Down,
		Right,
		Up,
		Non
	};

	ML::Vec2		pos;
	ML::Vec2		moveVec;
	ML::Box2D		hitBase;
	ML::Box2D		charaImg[4];
	int				animCnt;
	int				shotCnt;
	int				life;
	int				moveCnt;
	int				point;
	float			angle;
	float			speed;
	bool			hitFlag;
	Dir				dire;	//方向

	//-----◆メンバ変数を追加したら、必ず初期化も追加する---◆
	BChara() : pos(0, 0),
		moveVec(0,0),
		hitBase(0,0,0,0),
		charaImg{
		ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0)
	},
		animCnt(0),
		shotCnt(0),
		life(0),
		moveCnt(0),
		point(0),
		angle(0),
		speed(0),
		hitFlag(false),
		dire(Non)
	{}
	virtual	~BChara() {}

	//共通メソッド
	//
	virtual void	Anim();
	virtual void	CheckMove(ML::Vec2& e_);	//敵ではMoveCheck()
	virtual void	CMove(ML::Vec2&	est_);		//移動のみ
	virtual ML::Vec2	ShotMoveDir(float ang_);		//弾の移動方向
	virtual void	ScorePlus(int point_);
	virtual void	ScoreMinus(int point_);
};