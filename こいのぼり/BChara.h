#pragma	warning(disable:4996)
#pragma once
//-------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-------------------------------------------
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//�ύX�s����������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������
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
	Dir				dire;	//����

	//-----�������o�ϐ���ǉ�������A�K�����������ǉ�����---��
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

	//���ʃ��\�b�h
	//
	virtual void	Anim();
	virtual void	CheckMove(ML::Vec2& e_);	//�G�ł�MoveCheck()
	virtual void	CMove(ML::Vec2&	est_);		//�ړ��̂�
	virtual ML::Vec2	ShotMoveDir(float ang_);		//�e�̈ړ�����
	virtual void	ScorePlus(int point_);
	virtual void	ScoreMinus(int point_);
};