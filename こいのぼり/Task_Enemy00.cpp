//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy00.h"
#include  "Task_Shot00.h"
#include  "Task_Effect01.h"
#include  "Task_Player.h"
#include  "Task_Effect08.h"

namespace  Barrel
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "BarrelImg";
		DG::Image_Create(this->imageName, "./data/image/Taru.png");
		DG::Image_Create("BreakImg", "./data/image/tarubreak.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase("BreakImg");
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
		this->render2D_Priority[1] = 0.6f;
		this->timeCnt = 0;
		this->hitBase = ML::Box2D(-12, -12, 24, 24);
		this->state = Flow;
		this->point = 100;
		
		
		this->FlowAnim();
		this->DestructAnim();
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
		//�M�ƃv���C���[���Փ˂�����A�M�͔�������G�t�F�N�g���o��
		if (this->state != State::Destruct)
		{
			if (this->PlayerHit())
			{
				this->ScoreMinus(this->point / 3);
				//�����G�t�F�N�g
				auto explode = Explosion::Object::Create(true);
				explode->pos = this->pos;
			}
		}
		if (this->state == State::Destruct)
		{
			this->timeCnt++;
			if (this->timeCnt > 20) {
				this->ScorePlus(this->point);
				this->Kill();
			}
		}
		if (this->hitFlag)
		{
			this->animCnt = 0;
			this->ToDestruct();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		switch (this->state) {
		case State::Flow:
			this->FlowBarrelDraw();
			break;
		case State::Destruct:
			this->DestructBarrelDraw();
			break;
		}
	}

	//-------------------------------------------------------------------
	//�M�������A�j���[�V����
	void Object::FlowAnim()
	{
		for (int i = 0; i < 2; ++i) {
			this->BarrelImg[i] = ML::Box2D(i * 85, 0, 85, 64);
		}
	}
	//�M������A�j���[�V����
	void Object::DestructAnim()
	{
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 4; ++j) {
				this->DestructImg[i * j + j] = ML::Box2D(j * 85, i * 64, 85, 64);
			}
		}
	}
	//�v���C���[�Ƃ̏Փ�
	bool Object::PlayerHit()
	{
		ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
		if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
			if (player->state == Player::Object::State::Invicible) { return false; }
			ML::Box2D	you = player->hitBase.OffsetCopy(player->pos);
			if (you.Hit(me)) {
				player->HpDown();
				this->Kill();
				return true;
			}
		}
		return false;
	}
	
	//�M�̃A�j���[�V�����`��
	void Object::FlowBarrelDraw()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src = this->BarrelImg[(this->animCnt / 20) % 2];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	void Object::DestructBarrelDraw()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src = this->DestructImg[(this->animCnt / 20) % 3];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw("BreakImg", draw, src);
	}
	
	//�j���Ԃ�
	void Object::ToDestruct()
	{
		this->state = State::Destruct;
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