//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_Explain.h"
#include  "Task_HighScore.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "TitleLogoImg";
		this->fontName = "FontC";
		DG::Image_Create(this->imageName, "./data/image/back.png");
		DG::Font_Create(this->fontName, "MS �S�V�b�N", 8, 16);

		DG::Image_Create("TitleLogo", "./data/image/�^�C�g��.png");
		
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase(this->fontName);

		DG::Image_Erase("TitleLogo");

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
		ge->score = 0;
		ge->highscore = 0;

		this->logoPosY = 270;
		this->preSelectCnt = -1;
		this->nowSelectCnt = 0;
		this->inputDown = false;

		Bot	bot[5] =
		{
			{ML::Vec2(240,150),ML::Box2D(-50,-50,100,100),"�Q�[������",true,false},
			{ML::Vec2(240,170),ML::Box2D(-50,-50,100,100),"����",false,false},
			{ML::Vec2(240,190),ML::Box2D(-50,-50,100,100),"����",false,false},
			{ML::Vec2(240,210),ML::Box2D(-50,-50,100,100),"�㗬",false,false},
			{ML::Vec2(240,230),ML::Box2D(-50,-50,100,100),"�n�C�X�R�A",false,false},
		};
		//�{�^���̃f�[�^��������
		for (int i = 0; i < this->maxButton; ++i)
		{
			this->button[i].Init(bot[i]);
		}
		
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
			//�{�^���̖��O�ŁA�Ăяo����ς���
			if (ge->stageName == "�Q�[������")
			{
				auto nextTask = Explain::Object::Create(true);
			}
			else if (ge->stageName == "�n�C�X�R�A")
			{
				auto nextTask = HighScore::Object::Create(true);
			}
			else 
			{
				auto nextTask = Game::Object::Create(true);
			}
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		this->logoPosY -= 9;
		if (this->logoPosY <= 0) {
			this->logoPosY = 0;
		}
		//--------
		for (int i = 0; i < this->maxButton; ++i) {
			//�g�嗦�̒���
			if (this->button[i].select) { this->button[i].exRate = 1.2f; }
			else { this->button[i].exRate = 1.0f; }
		}
		if (this->logoPosY == 0)
		{
			//�{�^���𑀍삷��
			this->ButtonSelect();
			if (this->inputDown) {
				this->ButtonDecide();
				this->preSelectCnt = this->nowSelectCnt;
				this->inputDown = false;
			}
			this->ButtonEnter();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw(0, 0, 480, 270);
		ML::Box2D  src(0, 0, 1280, 720);
		DG::Image_Draw(this->res->imageName, draw, src);

		//�{�^���̕`��
		ML::Color color;
		if (this->button[preSelectCnt].select) {
			color = ML::Color(1.0f, 0.0f, 1.0f, 0.5f);
		}
		else {
			color = ML::Color(1, 1, 1, 1);
		}
		
		for (int cnt = 0; cnt < this->maxButton; ++cnt) {
			ML::Box2D	textBox((int)(-50 * this->button[cnt].exRate), (int)(this->button[cnt].exRate), (int)(100 * this->button[cnt].exRate), (int)(100 * this->button[cnt].exRate));
			textBox.Offset(this->button[cnt].pos);
			DG::Font_Draw(this->res->fontName, textBox, this->button[cnt].name,color);
		}

		//�^�C�g�����S
		ML::Box2D	draw1(0, 0, 480, 270);
		ML::Box2D	src1(0, 0, 480, 270);
		DG::Image_Draw("TitleLogo", draw1, src1);
	}

	//�{�^���Z���N�g����
	void Object::ButtonSelect()
	{
		auto in = DI::GPad_GetState("P1");
		//��L�[�������ꂽ
		if (in.LStick.U.down) { this->nowSelectCnt--; this->inputDown = true; }
		if (in.LStick.D.down) { this->nowSelectCnt++; this->inputDown = true; }
		
		//�J�E���g�̏��
		if (this->nowSelectCnt > this->maxSelect) { this->nowSelectCnt = this->minSelect; }
		if (this->nowSelectCnt < this->minSelect) { this->nowSelectCnt = this->maxSelect; }

	}
	//�{�^������
	void Object::ButtonDecide()
	{
		this->button[this->preSelectCnt].select = false;
		this->button[this->nowSelectCnt].select = true;
	}
	//�{�^���������ꂽ��
	void Object::ButtonEnter()
	{
		auto in = DI::GPad_GetState("P1");
		if (this->button[this->nowSelectCnt].select)
		{
			if (in.B1.down)
			{
				this->button[this->nowSelectCnt].push = true;
				this->StageName();		//�X�e�[�W�̖��O
				this->Kill();
			}
		}
	}
	void Object::Button::Init(Bot& b_)
	{
		this->pos = b_.pos;
		this->name = b_.name;
		this->select = b_.nowselect;
		this->push = b_.push;
		this->textBox = b_.hitbase;
	}

	//�X�e�[�W�̖��O
	void Object::StageName()
	{
		ge->stageName =  this->button[this->nowSelectCnt].name;
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