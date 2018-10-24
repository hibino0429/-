//-------------------------------------------------------
//�I�u�W�F�N�g�ėp�X�[�p�[�N���X
//-------------------------------------------------------
#include "BObject.h"
#include "MyPG.h"
#include "Task_Player.h"
//------------------------------------------------------------------
//�A�j���[�V����
void	BObject::Anim()
{
	for (int y = 0; y < 6; ++y) {
		for (int x = 0; x < 5; ++x) {
			this->crystalImg[x*y + y] = ML::Box2D(x * 192, y * 192, 192, 192);
		}
	}
}
//�v���C���[�Ƃ̂����蔻��
bool	BObject::HitPlayer()
{
	ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
	if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName"))
	{
		ML::Box2D	you = player->hitBase.OffsetCopy(player->pos);
		if (you.Hit(me))
		{
			return true;
		}
	}
	return false;
}

//�X�R�A����
void BObject::ScorePlus(int point_)
{
	ge->score += point_;
}
