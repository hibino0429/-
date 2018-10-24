//-------------------------------------------------------------------
//�v���C���[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Map2D.h"
#include  "Task_Effect00.h"
#include  "Task_Shot00.h"
#include  "Task_Shot01.h"
#include  "Task_Goal.h"
#include  "Task_Effect01.h"
#include  "Task_Effect05.h"
#include  "Task_Effect07.h"
#include  "Task_Effect09.h"
#include  "Task_Enemy01.h"
#include  "Task_Enemy02.h"
#include  "Task_Enemy03.h"
#include  "Task_Enemy04.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/image/koi.png");
		DG::Font_Create("FontA", "MS �S�V�b�N", 4, 8);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase("FontA");
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.5f;
		this->Init();		//������
		this->Anim();		//�A�j���[�V����

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->animCnt++;
		if (ge->gameOverFlag || ge->clearFlag) { return; }
		//�ړ�
		ML::Vec2	est(0, 0);
		this->Move(est);
		this->CheckMove(est);	//�ړ��͈̓`�F�b�N

		//�X�s�[�h�Ǘ�
		this->SpeedManeger();
		this->HpManeger();		//HP�Ǘ�

		//�̗͊Ǘ�
		if (this->life <= 0) { this->state = State::Dead; ge->gameOverFlag = true; }

		switch (this->state) {
		case State::Normal:		//���ʂ̏��
			//�����蔻��
			break;
		case State::Invicible:	//���G�̏��
			this->InviCount();	//���G�̃J�E���g���A3�b��������ANormal��
			break;
		case State::Dead:
			//���S�G�t�F�N�g
			this->dethCnt++;
			auto ex = CharaDead::Object::Create(true);
			ex->pos = this->pos;
			this->Kill();
		}
		//�S�[���Ƃ̓����蔻��(�{���S�[���ɔC����
		//������(hit)
		if (auto go = ge->GetTask_One_GN<Goal::Object>("Goal", "NoName"))
		{
			if (go->Check()) { this->hit = true; }
		}
		//�J�����ʒu�𒲐�
		{
			//�v���C������ʂ̂ǂ��ɂ�����
			int px = ge->camera2D.w / 2;
			int py = ge->camera2D.h * 3 / 4;
			//�v���C������ʒ����ɒu�������̃J�����̍�����W�����߂�
			int cpx = int(this->pos.x) - px;
			int cpy = int(this->pos.y) - py;
			//�J�����̍��W���X�V
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
		}
		//�}�b�v�̃J�����ړ��͈�
		if (auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v")) {
			map->AjastCameraPos();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw = ML::Box2D(-16,-16,32,32);
		draw.Offset(this->pos);
		ML::Box2D src = this->charaImg[(this->animCnt / 10) % 4];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->imageName, this->angle, ML::Vec2(16, 16));
		switch (this->state) {
		case State::Normal:
			DG::Image_Draw(this->res->imageName, draw, src);
			break;
		case State::Invicible:
			if (this->animCnt % 2 == 0) {
				DG::Image_Draw(this->res->imageName, draw, src);
			}
			break;
		case State::Dead:
			auto deth = CharaDead::Object::Create(true);
			deth->pos = this->pos;
		}
	}

	//---------------------------------------------------
	//������
	void Object::Init()
	{
		//�v���C���[���
		this->controllerName = "P1";
		this->hit = false;
		this->yelcryHit = false;
		this->dethCnt = 0;
		this->speedUpCnt = 0;
		this->state = Normal;
		this->inviCnt = 0;
		//�p�����(�v���C���[�݂̂ɃJ�X�^�}�C�Y)
		this->hitBase = ML::Box2D(-8, -12, 16,24);
		this->shotCnt = this->MaxShotCnt;
		this->life = 3;
		this->angle = ML::ToRadian(270);
		this->speed = 2.0f;
	}
	//----------------------------------------------------------
	//�ړ�����
	void Object::Move(ML::Vec2& est_)
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//�����ƃX�s�[�h�ňړ�
		if (in.LStick.L.on) { this->angle = ML::ToRadian(180); est_ = ShotMoveDir(this->angle) * this->speed; }
		if (in.LStick.R.on) { this->angle = ML::ToRadian(0); est_ = ShotMoveDir(this->angle) * this->speed; }
		if (in.LStick.U.on) { this->angle = ML::ToRadian(270); est_ = ShotMoveDir(this->angle) * this->speed;}
		if (in.LStick.D.on) { this->angle = ML::ToRadian(90); est_ = ShotMoveDir(this->angle) * this->speed;}
		//�e������
		if (in.B1.down)
		{
			if (this->shotCnt <= this->MinShotCnt) { return; }
			this->ShotMinusCnt();
			this->ShotAngle2(this->angle);	//�e�𐶐����A���������킹��
		}
	}
	//--------------------------------------------------------------
	//�e�̐����E�����ʒu
	auto Object::ShotCreate()
	{
		auto shot = Shot00::Object::Create(true);
		shot->pos = this->pos;
		return shot;
	}
	//----------------------------------------------------------
	//�v���C���[�̌����ɂ���Ēe�̕�����ς���
	void Object::ShotAngle(Dir ang_)
	{
		auto shot = ShotCreate();
		switch (ang_) {
		case Left:	shot->moveVec = ML::Vec2(-8, 0);	break;
		case Right:	shot->moveVec = ML::Vec2(8, 0);		break;
		case Up:	shot->moveVec = ML::Vec2(0, -8);	break;
		case Down:	shot->moveVec = ML::Vec2(0, 8);		break;
		}
	}
	//�v���C���[�̌����ɂ���Ēe�̕�����ς���
	void Object::ShotAngle2(float ang_)
	{
		auto shot = ShotCreate();
		shot->moveVec = this->ShotMoveDir(this->angle) * shot->speed;	//3.0f�͒e�̃X�s�[�h(�ϐ��ŗp�ӂ���)
	}
	//���S�G�t�F�N�g
	bool Object::DethAnim()
	{
		this->dethCnt++;
		if (this->dethCnt > 5 * 60)
		{
			//���S�G�t�F�N�g
			auto deth = CharaDead::Object::Create(true);
			deth->pos.x = this->pos.x;
			deth->pos.y = this->pos.y - 20;
			return true;
		}
		return false;
	}
	//----------------------------------------------------------
	//�e���̕ϓ�
	void Object::ShotPlusCnt()
	{
		this->shotCnt += 3;
	}
	void Object::ShotMinusCnt()
	{
		this->shotCnt--;
	}
	//---------------------------------------------------------
	//�X�s�[�h
	void Object::SpeedIncrease()
	{
		this->speed = this->NowSpeed * 1.3f;
	}
	void Object::SpeedManeger()
	{
		if (this->yelcryHit)
		{
			this->speedUpCnt++;
			this->SpeedIncrease();
			//�v���C���[�ɃG�t�F�N�g�����
			if (this->speedUpCnt == 1) {
				//�v���C���[�ɃG�t�F�N�g�����
				auto speedUp = SpeedUp::Object::Create(true);
				speedUp->pos = this->pos;
			}
		}
		else
		{
			this->speed = this->NowSpeed;
		}
		if (this->speedUpCnt >= this->MaxspeedUpCnt) { 
			this->speedUpCnt = 0;
			this->yelcryHit = false; 
		}
	}
	//------------------------------------------------------
	//�̗͊Ǘ�
	void Object::HpManeger()
	{
		if (this->state != State::Normal) { return; }

		if (this->life <= 0)
		{
			//���S����
			this->state = State::Dead;
		}
	}

	//�_���[�W����
	void Object::HpDown()
	{
		if (this->state == State::Invicible) { return; }
		//���G����
		this->state = State::Invicible;
		if (this->inviCnt == 0 && this->state == State::Invicible) {
			this->life--;
		}
		this->inviCnt++;
		if (this->inviCnt > this->MaxInviCnt) {
			this->inviCnt = 0; 
			this->state = State::Normal;
		}
	}
	//HP��
	void Object::HpUp()
	{
		if (this->life < this->MaxHp) {
			this->life++;
		}
	}

	//----------------------------------------------
	//���G�J�E���g
	void Object::InviCount()
	{
		this->inviCnt++;
		if (this->inviCnt >= this->MaxInviCnt)
		{
			this->inviCnt = 0;
			this->ChangeNormal();
		}
	}
	//--------------------------------------------------
	//��ԕω�
	void Object::ChangeNormal()
	{
		this->state = State::Normal;
	}
	void Object::ChangeInvi()
	{
		this->state = State::Invicible;
	}
	void Object::ChangeDeth()
	{
		this->state = State::Dead;
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object()
	{	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}