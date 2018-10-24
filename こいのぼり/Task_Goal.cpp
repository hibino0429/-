//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Goal.h"
#include  "Task_Player.h"
#include  "Task_Effect01.h"

namespace  Goal
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName1 = "effImg";
		DG::Image_Create(this->imageName1, "./data/image/GoalEff/pipo-mapeffect013a-min.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName1);
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
		this->pos = ML::Vec2(0, 0);
		this->hitBase = ML::Box2D(-8, -8, 16, 16);
		this->hitFlag = false;

		this->effCnt = 0;

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
		if (this->Check()) {
			this->hitFlag = true;
			ge->clearFlag = true;
		}
		else
		{
			this->hitFlag = false;
		}
		this->effCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw1 = ML::Box2D(-48, -48, 96, 96);
		draw1.Offset(this->pos);
		ML::Box2D src1 = this->effImg[(this->effCnt / 10) % 5];
		draw1.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName1, draw1, src1);
	}

	bool Object::Check()
	{
		//�v���C���[�Ɠ���������A�v���C���[�𓮂��Ȃ�����
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
	//------------------------------------------------------------------
	//�A�j���[�V����
	void Object::Anim()
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 5; j++) {
				this->effImg[i*j + j] = ML::Box2D(j * 192, i * 192, 192, 192);
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