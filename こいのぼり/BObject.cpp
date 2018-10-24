//-------------------------------------------------------
//オブジェクト汎用スーパークラス
//-------------------------------------------------------
#include "BObject.h"
#include "MyPG.h"
#include "Task_Player.h"
//------------------------------------------------------------------
//アニメーション
void	BObject::Anim()
{
	for (int y = 0; y < 6; ++y) {
		for (int x = 0; x < 5; ++x) {
			this->crystalImg[x*y + y] = ML::Box2D(x * 192, y * 192, 192, 192);
		}
	}
}
//プレイヤーとのあたり判定
bool	BObject::HitPlayer()
{
	ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
	if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName"))
	{
		ML::Box2D	you = player->hitBase.OffsetCopy(player->pos);
		if (you.Hit(me))
		{
			return true;
		}
	}
	return false;
}

//スコア増減
void BObject::ScorePlus(int point_)
{
	ge->score += point_;
}
