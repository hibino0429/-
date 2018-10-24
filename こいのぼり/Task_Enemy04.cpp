//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy04.h"
#include  "Task_Shot00.h"
#include  "Task_Player.h"
#include  "Task_Map2D.h"
#include  "Task_View.h"
#include  "Task_Shot01.h"
#include  "Task_Effect07.h"

namespace  otherKoi
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "KoiImg";
		DG::Image_Create(this->imageName, "./data/image/koi1.png"); 
		DG::Font_Create("FontE", "MS �S�V�b�N", 8, 8);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase("FontE");
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
		this->render2D_Priority[1] = 0.4f;
		this->dire = Dir::Down;
		this->state = State::Normal;
		this->animCnt = 0;
		this->timeCnt = 0;
		this->searchAngle =  ML::ToRadian(120/2);
		this->searchDist = 200.0f;
		this->hitBase = ML::Box2D(-16, -16, 32, 32);
		this->moveSpeed = 0.5f;
		this->angle = ML::ToRadian(90);		//����	
		this->rotRow = ML::ToRadian(4);			//����
		this->shotCnt = 0;
		this->point = 300;		//�|�C���g
		this->maxShotTimeCnt = 180;

		this->Load(ge->stageName);
		this->Anim();
		//���^�X�N�̐���

		return true;
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

		this->timeCnt++;

		this->ViewChangeState();
		this->PlayerHit();
		if (this->hitFlag)
		{
			this->ScorePlus(this->point);
			auto deadEff = CharaDead::Object::Create(true);
			deadEff->pos = this->pos;
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw(-16, -16, 32, 32);
		draw.Offset(this->pos);
		ML::Box2D	src1(0, 64, 32, 32);
		ML::Box2D	src = this->charaImg[(this->animCnt / 10) % 4];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->imageName, this->angle, ML::Vec2(draw.w / 2.0f, draw.h / 2.0f));
		DG::Image_Draw(this->res->imageName, draw, src);

	}

	//��ԑJ��
	void Object::StateChange()
	{
		//��ԑJ��
		switch (this->state) {
		case State::Normal:
			if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
				if (this->SearchRotation(player->pos))
				{
					this->state = State::Attack;
				}
				else {
					this->state = State::Normal;
				}
			}
			break;
		case State::Attack:
			//�v���C���[�����o��
			if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
				if (this->SearchRotation(player->pos))
				{
					this->moveVec = ML::Vec2(cos(this->angle), sin(this->angle))*this->moveSpeed;
				}
				else
				{
					this->moveVec = ML::Vec2(0, 0);
					this->state = State::Normal;
				}
			}
			break;
		case State::Dead:
			break;
		}
	}
	//�s������
	void Object::AI()
	{
		switch (this->state) {
		case State::Normal:
			this->Move();
			break;
		case State::Attack:
			this->ShotAtk();
			break;
		case State::Dead:
			this->Deth();
			break;
		}
		//this->StateChange();	//��ԑJ��
	}
	//�ړ�����
	void Object::Move()
	{
		//�㉺���E�Ƀ����_���Ɉړ�������
		int move = rand() % 4;
		ML::Vec2		est(0, 0);
		switch (move) {
		case Dir::Left:
			this->angle = ML::ToRadian(180);
			est = ML::Vec2(-10,0);
			break;
		case Dir::Right:
			this->angle = ML::ToRadian(0);
			est = ML::Vec2(10, 0);
			break;
		case Dir::Up:
			this->angle = ML::ToRadian(270);
			est = ML::Vec2(0, -10);
			break;
		case Dir::Down:
			this->angle = ML::ToRadian(90);
			est = ML::Vec2(0, 10);
			break;
		}
		this->CheckMove(est);
	}
	//�U������
	void Object::ShotAtk()
	{
		auto shot = Shot01::Object::Create(true);
		shot->moveVec = this->ShotMoveDir(this->angle);
	}
	//���S����
	void Object::Deth()
	{
		this->Kill();
	}
	//�v���C���[�Ƃ̓����蔻��
	bool Object::PlayerHit()
	{
		if (this->state == State::Dead) { return false; }
		ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
		if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
			ML::Box2D	you = player->hitBase.OffsetCopy(player->pos);
			if (this->HitCheck(me, you)) {
				//�v���C���[�̓_���[�W���󂯂�
				if (player->state == Player::Object::State::Invicible) { return false; }
				if (ge->score > 0) { this->ScoreMinus(this->point / 3); }
				player->HpDown();
				return true;
			}
		}
		return false;
	}
	//�����蔻��
	bool Object::HitCheck(ML::Box2D& me_,ML::Box2D& you_)
	{
		if (you_.Hit(me_)) { return true; }
		return false;
	}

	//���o�͈̔�
	bool Object::SearchRotation(const ML::Vec2& tg_)
	{
		ML::Vec2	toTarget = tg_ - this->pos;		//�G�܂ł̋���
		//����p�͈͂ɂ���^�[�Q�b�g��Ώۂɂ���
		float dist = toTarget.Length();
		if (dist < this->searchDist) {
			ML::Vec2	toTargetN = toTarget.Normalize();
			ML::Vec2	moveVecN = ML::Vec2(cos(this->angle), sin(this->angle));
			//�O�ςō��E�ǂ��炩�𔻒�
			float dot = moveVecN.x * toTargetN.x + moveVecN.y * toTargetN.y;
			if (dot > cos(this->searchAngle)) {
				if (dot > cos(this->rotRow)) {
					//this->angle1 = atan2(toTargetN.y,toTargetN.x);
				}
				else {
					float cr = moveVecN.x * toTargetN.y - moveVecN.y * toTargetN.x;
					//���E�ɍ��킹�Đ��񂷂�
					if (cr >= 0) { this->angle += this->rotRow; }
					else { this->angle -= this->rotRow; }
				}
				return true;
			}
		}
		return false;
	}

	//�e�����܂ł̎���
	bool Object::ShotTimeCnt()
	{
		this->shotCnt++;
		if (this->shotCnt >= this->maxShotTimeCnt) 
		{
			this->shotCnt = 0;
			return true;
		}
		return false;
	}
	//�e��������
	void Object::Shot()
	{
		if (!this->ShotTimeCnt()) { return; }
		auto shot = Shot01::Object::Create(true);
		shot->pos = this->pos;
		shot->moveVec = ML::Vec2(cos(this->angle), sin(this->angle)) * shot->speed;
	}
	//���o�ɂ���Ă̏�ԑJ��
	//�G��������O�́A����낫���
	//�G����������A�ǔ�	//����ɒe��5�b��1�񔭎˂���
	void Object::ViewChangeState()
	{
		if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
			if (this->SearchRotation(player->pos))
			{
				this->moveVec = ML::Vec2(cos(this->angle), sin(this->angle))*this->moveSpeed;
				
				this->Shot();
			}
			else
			{
				this->moveVec = ML::Vec2(0, 0);
			}
			ML::Vec2	est = this->moveVec;
			this->CheckMove(est);
		}
	}

	//���[�h
	void Object::Load(string stageName_)
	{
		if (stageName_ == "����") {
			this->EneInfo("./data/EneCarp/EneInfo01.txt");
		}
		else if (stageName_ == "����") {
			this->EneInfo("./data/EneCarp/EneInfo02.txt");
		}
		else if (stageName_ == "�㗬") {
			this->EneInfo("./data/EneCarp/EneInfo03.txt");
		}
	}
	bool Object::EneInfo(string path_)
	{
		ifstream	fin(path_);
		if (!fin) { return false; }

		fin >> this->moveSpeed;
		
		fin.close();
		return true;
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
	Object::Object() {	}
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