#include "ImageFont.h"


//�T�v: �R���X�g���N�^
//����1: �摜���ʖ�
//����2: �t�@�C���p�X
ImageFont::ImageFont(const string& name, const string& filePath)
	: name(name)
	, charSize(32)
{
	DG::Image_Create(name, filePath);
}

//�T�v: �f�X�g���N�^
ImageFont::~ImageFont()
{
	DG::Image_Erase(name);
}


//�摜�����̐ݒ�
void	ImageFont::SetCharSize(int charSize)
{
	this->charSize = charSize;
}

//�T�v:�w�肳�ꂽ��������摜�t�H���g�ŕ`�悷��
//����1: 1�����ڊJ�n�ʒux
//����2: 1�����ڊJ�n�ʒuy
//����3: �`�敶����
void	ImageFont::Draw(int x, int y, const char* msg)
{
	int dx = x;
	int dy = y;
	for (int i = 0; i < (int)strlen(msg); i++)
	{
		int code = ((unsigned char)msg[i]);
		int fx = code % 16 * charSize;
		int fy = code % 16 * charSize;
		ML::Box2D	draw(dx, dy, charSize, charSize);
		ML::Box2D	src(fx, fy, charSize, charSize);
		DG::Image_Draw(name, draw, src);
		dx += charSize;
	}
}