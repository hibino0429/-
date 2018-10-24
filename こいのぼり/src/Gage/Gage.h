#pragma once
#include "../../MyPG.h"
#include "../../GameEngine_Ver3_7.h"
#include <string>

class Gage
{
private:
	struct GageBox
	{
		ML::Box2D	box;
		ML::Vec2	pos;
		ML::Vec2	minPos;
		ML::Vec2	maxPos;
		string		imageName;
	};
public:
	//�T�v: ���ׂē����傫���ō쐬
	Gage(const ML::Box2D& box);
	//�T�v:�@���ꂼ��傫����ʂō쐬
	Gage(const ML::Box2D& areaBox, const ML::Box2D& underBox, ML::Box2D upBox);
	~Gage();

	//�T�v: �͈̓{�b�N�X�̍쐬
	void	CreateAreaBox(const string& imageName,const string& filePath);
	//�T�v: ���ɕ\������{�b�N�X�̍쐬
	void	CreateUnderBox(const string& imageName, const string& filePath);
	//�T�v: ��ɂ��Ԃ���{�b�N�X�̍쐬
	void	CreateUpBox(const string& imageName, const string& filePath);

	//�T�v: �ʒu�̌���
	void	SetPos(ML::Vec2 pos);

	//�X�V
	void		UpDate();
	//�`��
	void		Draw();

private:
	//�T�v: �ŏ��ƍő�̈ʒu�̍쐬
	void	CreateMinMaxPos();
	//�T�v: �Q�[�W��ψ�
	void	ChageParam();
private:
	GageBox		areaBox;	//�͈̓{�b�N�X
	GageBox		underBox;	//���ɕ\������{�b�N�X
	GageBox		upBox;		//��ɂ��Ԃ���{�b�N�X
};
