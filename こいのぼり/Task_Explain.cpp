//-------------------------------------------------------------------
//�Q�[������
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Explain.h"
#include  "Task_Title.h"

namespace  Explain
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Explain";
		this->backImageName = "backImg";
		DG::Image_Create(backImageName, "./data/image/EndBG.bmp");
		DG::Font_Create(this->imageName, "MS �S�V�b�N", 8, 12);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(backImageName);
		DG::Font_Erase(this->imageName);
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
		this->render2D_Priority[1] = 0.8f;
		this->pos = ML::Vec2(0, 0);
		this->draw = ML::Box2D(-16, -16, 32, 32);
		this->explain = "";
		
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
			auto nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		if (in.B1.down)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i
		ML::Box2D	draw(0, 0, 480, 270);
		ML::Box2D	src(0, 0, 512, 512);
		DG::Image_Draw(this->res->backImageName, draw, src);

		ML::Box2D	textBox(0, 0, 480, 270);
		char buf[512];
		sprintf(buf,
			"<�Q�[������> \n\n"
			"��̒��_��ڎw���āA�����|���Ȃ���A�S�[����ڎw��\n\n"
			"(�v���C���[)\n"
			"�v���C���[�̒e��		[�M]	��j��ł���\n"
			"�v���C���[�̒e��		[�G�̌�]	��|����\n"
			"[�G]��[�M]�ƏՓ˂���ƁA�_���[�W���󂯂�\n\n"
			"�v���C���[�̃��C�t�́@[3]\n"
			"�v���C���[�̒e���́@	 [20]\n"
			"���N���X�^���́@	[�X�s�[�h�A�b�v]\n"
			"�ԃN���X�^����	[�e�����v���X]\n"
			"�N���X�^����	[���C�t��]\n");

		DG::Font_Draw(this->res->imageName, textBox, buf, ML::Color(1, 0, 0, 0));

	}

	//------------------------------------------------
	//�������̃��[�h
	bool Object::Load(string Path)
	{
		ifstream fin(Path);
		if (!fin) { return false; }

		fin >> this->explain;

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