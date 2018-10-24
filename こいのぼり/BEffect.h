#pragma	warning(disable:4996)
#pragma once
//-------------------------------------------
//�G�t�F�N�g�ėp�X�[�p�[�N���X
//-------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "BEffect.h"

class BEffect : public BTask
{
	//�ύX�s����������������������������
public:
	typedef shared_ptr<BEffect>		SP;
	typedef weak_ptr<BEffect>		WP;
public:
	//�ύX������������������������������
	ML::Vec2	pos;
	ML::Box2D	effImg[30];
	int			animCnt;
	float			life;
	float			timeCnt;
	float			speed;

	//-----�������o�ϐ���ǉ�������A�K�����������ǉ�����---��
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

	//���ʃ��\�b�h
	void Anim();
};
