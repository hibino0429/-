#pragma once
#include <string>
#include "../../MyPG.h"

using namespace std;

class ImageFont
{
public:
	//概要: コンストラクタ
	//引数1: 画像識別名
	//引数2: ファイルパス
	ImageFont(const string& name,const string& filePath);
	//概要: デストラクタ
	~ImageFont();

	//画像文字の設定
	void	SetCharSize(int charSize);

	//概要:指定された文字列を画像フォントで描画する
	//引数1: 1文字目開始位置x
	//引数2: 1文字目開始位置y
	//引数3: 描画文字列
	void	Draw(int x, int y, const char* msg);

private:
	string		name;		//画像識別名
	int			charSize;	//画像文字の大きさ
};