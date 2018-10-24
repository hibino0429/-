//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_HighScore.h"
#include  "Task_Title.h"

namespace  HighScore
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->fontName = "FontH";
		DG::Font_Create(this->fontName, "MS �S�V�b�N", 8, 16);
		this->backImg = "scoreBackImg";
		DG::Image_Create(this->backImg, "./data/image/scoreBG.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Font_Erase(this->fontName);
		DG::Image_Erase(this->backImg);
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
		ge->HighScoreLoad("./data/highscore/�����n�C�X�R�A.txt");
		this->highScore[0] = ge->highscore;
		ge->HighScoreLoad("./data/highscore/�����n�C�X�R�A.txt");
		this->highScore[1] = ge->highscore;
		ge->HighScoreLoad("./data/highscore/�㗬�n�C�X�R�A.txt");
		this->highScore[2] = ge->highscore;
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

		if (in.B1.down) { this->Kill(); }
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i
		ML::Box2D	draw(0, 0, 480, 270);
		ML::Box2D	src(0, 0, 1920, 720);
		DG::Image_Draw(this->res->backImg, draw, src);

		this->Font_Draw("����", ML::Box2D(100, 50, 200, 50),0);
		this->Font_Draw("����", ML::Box2D(100, 100, 200, 50),1);
		this->Font_Draw("�㗬", ML::Box2D(100, 150, 200, 50),2);
	}

	//�\��
	void Object::Font_Draw(string stageName_,ML::Box2D draw_,int index_)
	{
		string text = stageName_ + ":" + to_string(this->highScore[index_]);
		DG::Font_Draw("FontH", draw_, text, ML::Color(1, 1, 1, 1));
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