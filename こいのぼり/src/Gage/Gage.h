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
	//概要: すべて同じ大きさで作成
	Gage(const ML::Box2D& box);
	//概要:　それぞれ大きさを別で作成
	Gage(const ML::Box2D& areaBox, const ML::Box2D& underBox, ML::Box2D upBox);
	~Gage();

	//概要: 範囲ボックスの作成
	void	CreateAreaBox(const string& imageName,const string& filePath);
	//概要: 下に表示するボックスの作成
	void	CreateUnderBox(const string& imageName, const string& filePath);
	//概要: 上にかぶせるボックスの作成
	void	CreateUpBox(const string& imageName, const string& filePath);

	//概要: 位置の決定
	void	SetPos(ML::Vec2 pos);

	//更新
	void		UpDate();
	//描画
	void		Draw();

private:
	//概要: 最小と最大の位置の作成
	void	CreateMinMaxPos();
	//概要: ゲージを変位
	void	ChageParam();
private:
	GageBox		areaBox;	//範囲ボックス
	GageBox		underBox;	//下に表示するボックス
	GageBox		upBox;		//上にかぶせるボックス
};
