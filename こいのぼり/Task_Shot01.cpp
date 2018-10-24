//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot01.h"
#include  "Task_Map2D.h"
#include  "Task_Effect05.h"
#include  "Task_Player.h"
#include  "Task_Effect06.h"

namespace  Shot01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Shot01Img";
		DG::Image_Create(this->imageName, "./data/image/shot01.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
		this->hitBase = ML::Box2D(-12, -12, 24, 24);
		//��----
		if (ge->stageName == "����") {
			this->speed = 3.0f;
		}
		else if (ge->stageName == "����") {
			this->speed = 4.0f;
		}
		else if (ge->stageName == "�㗬") {
			this->speed = 5.0f;
		}

		this->Anim();
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
		this->moveCnt++;
		if (this->moveCnt >= 30) {
			this->Kill();
			return;
		}
		//�ړ�
		this->pos += this->moveVec;

		this->HitPlayer();
		//�ړ���ŏ�Q���ɏՓ˂��������
		//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
		if (auto map = ge->GetTask_One_GN<Map2D::Object >("�t�B�[���h", "�}�b�v")) {
			ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
			if (map->CheckHit(hit)) {
				this->Kill();

				//�G�t�F�N�g�Ȃǂ�������
				auto shoteff = EneShotEff::Object::Create(true);
				shoteff->pos = this->pos;
				return;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw = this->hitBase;
		draw.Offset(this->pos);
		ML::Box2D	src = this->shotImg[(this->animCnt / 10) % 5];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);

	}

	//--------------------------------------------------------------------
	//�e�̃A�j���[�V����
	void Object::Anim()
	{
		for (int i = 0; i < 6; ++i) {
			for (int j = 0; j < 5; ++j) {
				this->shotImg[i*j + j] = ML::Box2D(j * 192, i * 192, 192, 192);
			}
		}
	}
	//�v���C���[�Ƃ̓����蔻��
	void Object::HitPlayer()
	{
		if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
			ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D	you = player->hitBase.OffsetCopy(player->pos);
			if (you.Hit(me)) {
				player->HpDown();
				this->Kill();
			}
		}
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