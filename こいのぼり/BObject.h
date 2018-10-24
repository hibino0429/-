#pragma	warning(disable:4996)
#pragma once
//-------------------------------------------
//�I�u�W�F�N�g�ėp�X�[�p�[�N���X
//-------------------------------------------
#include "GameEngine_Ver3_7.h"

class BObject : public BTask
{
	//�ύX�s����������������������������
public:
	typedef shared_ptr<BObject>		SP;
	typedef weak_ptr<BObject>		WP;
public:
	//�ύX������������������������������
	ML::Vec2		pos;				//�ʒu	
	ML::Box2D		hitBase;			//�����蔻��
	ML::Box2D		crystalImg[30];		//�摜
	int				animCnt;			//�A�j���J�E���g
	int				point;				//�|�C���g

	//-----�������o�ϐ���ǉ�������A�K�����������ǉ�����---��
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

	//���ʃ��\�b�h
	virtual void	Anim();			//�A�j���[�V����
	virtual bool	HitPlayer();	//�v���C���[�Ƃ̂����蔻��
	virtual void	ScorePlus(int point_);	//�X�R�A����
}; 
