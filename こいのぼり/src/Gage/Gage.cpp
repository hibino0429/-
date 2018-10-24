#include "Gage.h"

//概要: すべて同じ大きさで作成
Gage::Gage(const ML::Box2D& box)
{
	areaBox.box = box;
	underBox.box = box;
	upBox.box = box;

	CreateMinMaxPos();
}
//概要:　それぞれ大きさを別で作成
Gage::Gage(const ML::Box2D& areaBox, const ML::Box2D& underBox, ML::Box2D upBox)
{
	this->areaBox.box = areaBox;
	this->underBox.box = underBox;
	this->upBox.box = upBox;

	CreateMinMaxPos();
}


Gage::~Gage()
{
	DG::Image_Erase(this->areaBox.imageName);
	DG::Image_Erase(this->underBox.imageName);
	DG::Image_Erase(this->upBox.imageName);
}

//概要: 範囲ボックスの作成
void	Gage::CreateAreaBox(const string& imageName,const string& filePath)
{
	this->areaBox.imageName = imageName;
	DG::Image_Create(this->areaBox.imageName, filePath);
}

//概要: 下に表示するボックスの作成
void	Gage::CreateUnderBox(const string& imageName, const string& filePath)
{
	this->underBox.imageName = imageName;
	DG::Image_Create(this->underBox.imageName, filePath);
}
//概要: 上にかぶせるボックスの作成
void	Gage::CreateUpBox(const string& imageName, const string& filePath)
{
	this->upBox.imageName = imageName;
	DG::Image_Create(this->upBox.imageName, filePath);
}


//概要: 位置の決定
void	Gage::SetPos(ML::Vec2 pos)
{
	areaBox.pos = pos;
	underBox.pos = pos;
	upBox.pos = pos;

	CreateMinMaxPos();
}


void	Gage::UpDate()
{
	upBox.pos.x -= 100;
	underBox.pos.x -= 30;
}

void	Gage::Draw()
{
	
	ML::Box2D	draw = this->underBox.box;
	draw.Offset(underBox.pos);
	draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
	DG::Image_Draw(this->underBox.imageName, draw, ML::Box2D(0,0,1611,113));

	ML::Box2D	draw2 = this->upBox.box;
	draw2.Offset(upBox.pos);
	draw2.Offset(-ge->camera2D.x, -ge->camera2D.y);
	DG::Image_Draw(this->upBox.imageName, draw2, ML::Box2D(0,0,1611,113));

	ML::Box2D	draw3 = this->areaBox.box;
	draw3.Offset(areaBox.pos);
	draw3.Offset(-ge->camera2D.x, -ge->camera2D.y);
	DG::Image_Draw(this->areaBox.imageName, draw3, ML::Box2D(0, 0, 1747, 178));

}

//概要: 最小と最大の位置の作成
void	Gage::CreateMinMaxPos()
{
	areaBox.minPos = ML::Vec2(areaBox.box.x - areaBox.box.w / 2, areaBox.box.y - areaBox.box.h / 2);
	areaBox.maxPos = ML::Vec2(areaBox.box.x + areaBox.box.w / 2, areaBox.box.y + areaBox.box.h / 2);
	underBox.minPos = ML::Vec2(underBox.box.x - underBox.box.w / 2, underBox.box.y - underBox.box.h / 2);
	underBox.maxPos = ML::Vec2(underBox.box.x + underBox.box.w / 2, underBox.box.y + underBox.box.h / 2);
	upBox.minPos = ML::Vec2(upBox.box.x - upBox.box.w / 2, upBox.box.y - upBox.box.h / 2);
	upBox.maxPos = ML::Vec2(upBox.box.x + upBox.box.w / 2, upBox.box.y + upBox.box.h / 2);
}

//概要: ゲージを変位
void	Gage::ChageParam()
{
	int maxHp = 100;
	int minHp = 0;
	int nowHp = 40;

	RECT upRect = { upBox.box.x,
					upBox.box.y,
					upBox.box.x + upBox.box.w,
					upBox.box.y + upBox.box.h };

	RECT areaRect = {	areaBox.box.x,
						areaBox.box.y,
						areaBox.box.x + areaBox.box.w,
						areaBox.box.y + areaBox.box.h };

	//描画エリア内に収める
	if (upRect.left < areaRect.left) { upRect.left = areaRect.left; }
	if (upRect.top < areaRect.top) { upRect.top = areaRect.top; }
	if (upRect.right > areaRect.right) { upRect.right = areaRect.right; }
	if (upRect.bottom > areaRect.bottom) { upRect.bottom = areaRect.bottom; }

}