//-------------------------------------------------------------------
//���o
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_View.h"
#include  "Task_Player.h"

namespace  View
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "ViewImg";
		DG::Image_Create(this->imageName, "./data/image/HitTest.bmp");
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
		this->render2D_Priority[1] = 0.4f;

		this->hitBase		= ML::Box2D(0, -1, 50, 2);	//����
		this->hitBase1 = ML::Box2D(0, -1, 50, 2);
		this->hitFlag		= false;


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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D	src(0, 0, 100, 100);

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->imageName, this->angle + this->Ang45, ML::Vec2(0, -1));
		DG::Image_Draw(this->res->imageName, draw, src);

		ML::Box2D	draw1 = this->hitBase1.OffsetCopy(this->pos);
		
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->imageName, this->angle - this->Ang45, ML::Vec2(0, -1));
		DG::Image_Draw(this->res->imageName, draw1, src);
	}

	//-------------------------------------------------------------------
	//����
	void Object::Create(ML::Vec2 pos_,float ang_)
	{
		//�L�����̈ʒu�Ɏ��o�̒��S�������Ă���
		this->pos = pos_;
		//�L�����̌����A���O���Ǝ��o�̌����A���O�������킹��
		this->angle = ang_;

		//�ݒ肷��
		this->Set();
	}
	//�ݒ�
	void Object::Set()
	{
		//�����������̂���Ɏ��o�����
		this->centerVec			 = ML::Vec2(cos(this->angle), sin(this->angle));
		this->viewVec[ViewLeft]	 = ML::Vec2(cos(this->angle - this->Ang45), sin(this->angle - this->Ang45));
		this->viewVec[ViewRight] = ML::Vec2(cos(this->angle + this->Ang45), sin(this->angle + this->Ang45));
	}
	//�O�ς̌v�Z
	float Object::Cross(ML::Vec2& v1,ML::Vec2& v2)
	{
		return v1.y * v2.x - v1.x * v2.y;
	}
	//���ς̌v�Z
	float Object::Product(ML::Vec2& p1,ML::Vec2& p2)
	{
		return p1.x * p2.x + p1.y * p2.y;
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