//-------------------------------------------------------
//キャラクタ汎用スーパークラス
//-------------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map2D.h"
#include "Task_Shot00.h"
#include "Task_Shot01.h"

//-----------------------------------------------------
//めり込まない移動処理
void BChara::CheckMove(ML::Vec2& e_)
{
	auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	if (nullptr == map) { return; }	//マップがなければ判定しない

	while (e_.x != 0) {
		float preX = this->pos.x;
		if (e_.x >= 1) {
			this->pos.x += 1; e_.x -= 1;
		}
		else if (e_.x <= -1) {
			this->pos.x -= 1;	e_.x += 1;
		}
		else {
			this->pos.x += e_.x;	e_.x = 0;
		}
		ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit)) {
			this->pos.x = preX;
			break;
		}
	}
	while (e_.y != 0) {
		float preY = this->pos.y;
		if (e_.y >= 1) {
			this->pos.y += 1;	e_.y -= 1;
		}
		else if (e_.y <= -1) {
			this->pos.y -= 1;	e_.y += 1;
		}
		else {
			this->pos.y += e_.y;	e_.y = 0;
		}
		ML::Box2D	hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit)) {
			this->pos.y = preY;
			break;
		}
	}
}

//アニメーション
void BChara::Anim()
{
	for (int x = 0; x < 4; ++x) {
		this->charaImg[x] = ML::Box2D(x * 32, 64, 32, 32);
	}
}

//移動処理
void BChara::CMove(ML::Vec2& est_)
{
	//どの方向でも同じスピードで移動
	this->moveVec = ML::Vec2(cos(this->angle), sin(this->angle)).Normalize() * this->speed;
	est_ = this->moveVec;
}

//弾の移動方向
ML::Vec2 BChara::ShotMoveDir(float ang_)
{
	return ML::Vec2(cos(this->angle), sin(this->angle)).Normalize();
}

//スコアの増減
void BChara::ScorePlus(int point_)
{
	ge->score += point_;
}
void BChara::ScoreMinus(int point_)
{
	ge->score -= point_;
}