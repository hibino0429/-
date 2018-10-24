#pragma once
#include <string>
#include "../../MyPG.h"

using namespace std;

class ImageFont
{
public:
	//�T�v: �R���X�g���N�^
	//����1: �摜���ʖ�
	//����2: �t�@�C���p�X
	ImageFont(const string& name,const string& filePath);
	//�T�v: �f�X�g���N�^
	~ImageFont();

	//�摜�����̐ݒ�
	void	SetCharSize(int charSize);

	//�T�v:�w�肳�ꂽ��������摜�t�H���g�ŕ`�悷��
	//����1: 1�����ڊJ�n�ʒux
	//����2: 1�����ڊJ�n�ʒuy
	//����3: �`�敶����
	void	Draw(int x, int y, const char* msg);

private:
	string		name;		//�摜���ʖ�
	int			charSize;	//�摜�����̑傫��
};