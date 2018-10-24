#include "ImageFont.h"


//概要: コンストラクタ
//引数1: 画像識別名
//引数2: ファイルパス
ImageFont::ImageFont(const string& name, const string& filePath)
	: name(name)
	, charSize(32)
{
	DG::Image_Create(name, filePath);
}

//概要: デストラクタ
ImageFont::~ImageFont()
{
	DG::Image_Erase(name);
}


//画像文字の設定
void	ImageFont::SetCharSize(int charSize)
{
	this->charSize = charSize;
}

//概要:指定された文字列を画像フォントで描画する
//引数1: 1文字目開始位置x
//引数2: 1文字目開始位置y
//引数3: 描画文字列
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