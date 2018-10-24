 //-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_Ending.h"
#include  "Task_Map2D.h"
#include  "Task_Player.h"
#include  "Task_Start.h"
#include  "Task_Goal.h"
#include  "Task_Enemy00.h"
#include  "Task_Enemy01.h"
#include  "Task_Enemy02.h"
#include  "Task_Enemy03.h"
#include  "Task_Title.h"
#include  "Task_Enemy04.h"
#include  "Task_View.h"
#include  "Task_Effect00.h"



namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//���t�H���g
		this->fontName = "FontUI";
		DG::Font_Create(this->fontName, "MS �S�V�b�N", 4, 16);

		//������
		DG::Image_Create("GameOverImg", "./data/image/gameOver.png");
		DG::Image_Create("ClearImg", "./data/image/gameClear.png");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		//���t�H���g
		DG::Font_Erase(this->fontName);
		DG::Image_Erase("GameOverImg");
		DG::Image_Erase("ClearImg");
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
		//�Q�[���I�u�W�F�N�g
		ge->camera2D = ML::Box2D(-200, -100, 480, 270);
		ge->time = 0;
		ge->frameTime = 0;
		ge->playerLife = 0;
		ge->clearFlag = false;
		ge->gameOverFlag = false;

		this->endCnt = 0;

		//���\�[�X���풓������
		this->shot00_Resorce = Shot00::Resource::Create();
		this->effect00_Resorce = Effect00::Resource::Create();

		//���^�X�N�̐���
		//�D�揇�ʂ��������ɂ���
		//�}�b�v�̐���
		auto map = Map2D::Object::Create(true);
		map->Load("./data/Stage/" + ge->stageName + ".txt");
		map->CreateSGE();
		
		//�n�C�X�R�A�̓ǂݍ���
		ge->HighScoreLoad("./data/highScore/" + ge->stageName + "�n�C�X�R�A.txt");

		auto player = Player::Object::Create(true);
		if(auto start = ge->GetTask_One_GN<Start::Object>("�X�^�[�g", "NoName") ){
			player->pos = start->pos;
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�t�B�[���h");
		ge->KillAll_G("�v���C��");
		ge->KillAll_G("�Ԃ̃N���X�^��");
		ge->KillAll_G("�̃N���X�^��");
		ge->KillAll_G("���̃N���X�^��");
		ge->KillAll_G("�G�̌�");
		ge->KillAll_G("�M");
		ge->KillAll_G("�X�^�[�g");
		ge->KillAll_G("�S�[��");

		//���\�[�X�̏풓������
		this->shot00_Resorce.reset();
		this->effect00_Resorce.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto  nextTask = Ending::Object::Create(true);
			nextTask->clearState = nextTask->Non;

			if (ge->clearFlag) {
				nextTask->clearState = nextTask->Clear;
			}
			else if (ge->gameOverFlag) {
				nextTask->clearState = nextTask->Badend;
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		
		//�A�N�Z�X������
		ge->qa_Barrels = ge->GetTask_Group_G<BChara>("�M");
		ge->qa_Enemys = ge->GetTask_Group_G<BChara>("�G�̌�");
		ge->qa_Objects = ge->GetTask_Group_G<BObject>("�ԃN���X�^��");

		//��������
		if (!ge->clearFlag && !ge->gameOverFlag) { this->TimeCnt(); }
		
		if (ge->clearFlag || ge->gameOverFlag) {
			this->endCnt++;
			if (this->endCnt > 300) { 
				//�v���C���[�̗̑͂��擾
				if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
					ge->playerLife = player->life;
				}
				this->Kill(); 
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D	textBox(0, 0, 80, 50);
		string	text = "�X�e�[�W:" + ge->stageName;
		DG::Font_Draw(this->res->fontName, textBox, text, ML::Color(1, 1, 1, 1));

		//��������
		this->TimeRender();
		//�X�R�A
		this->ScoreRender();

		this->ScoreComplicate();

		this->PlayerLife();
		this->PlayerShotRemain();
		this->PlayerSpeed();

		//�Q�[���I�[�o�[�\��
		if (ge->gameOverFlag)
		{
			ML::Box2D	draw(120, 65, 240, 150);
			ML::Box2D	src(0, 0, 480, 270);
			DG::Image_Draw("GameOverImg", draw, src);
		}
		//�Q�[���N���A�\��
		if (ge->clearFlag)
		{
			if (endCnt % 8 == 0) {
				auto clearEff1 = Effect00::Object::Create(true);
				clearEff1->pos = ML::Vec2( 170, 130);
				auto clearEff2 = Effect00::Object::Create(true);
				clearEff2->pos = ML::Vec2(420, 130);
			}

			ML::Box2D	draw(120, 65, 240, 150);
			ML::Box2D	src(0, 0, 480, 270);
			DG::Image_Draw("ClearImg", draw, src);
		}
	}

	//�X�R�A�̌v�Z
	void Object::ScoreCalc()
	{
		//�M + �G�̌� + ���� + �e�� + ���C�t
		//�����ł́A�I�u�W�F�N�g�̓��_�����v�Z���Ȃ�
		//�v���C���̃��C�t���擾���čŌ�Ɋ���o��
		if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
			ge->score *= player->life;
		}
	}
	//�X�R�A�̕\��
	void Object::ScoreRender()
	{
		ML::Color	color(1, 1, 1, 1);
		if (ge->score > ge->highscore) { color = ML::Color(1, 0, 1, 0); }
		ML::Box2D	textBox(400, 0, 80, 50);
		string	text = "�X�R�A:" + to_string(ge->score);			//8���\��
		DG::Font_Draw(this->res->fontName, textBox, text, color);
	}
	//���ԃJ�E���g
	void Object::TimeCnt()
	{
		ge->frameTime++;
		if (ge->frameTime >= 60) {
			ge->time++;
			ge->frameTime = 0;
		}
		if (ge->time > ge->LimitTime) {
			ge->time = 0;
			ge->gameOverFlag = true;
		}
	}
	//�������Ԃ̕\��
	void Object::TimeRender()
	{
		ML::Color	color(1, 1, 1, 1);
		if (ge->LimitTime - ge->time <= 10) { color = ML::Color(1, 1, 0, 0); }
		ML::Box2D	textBox(0, 80, 80, 50);
		string	text = "��������:" + to_string(ge->LimitTime - ge->time);
		DG::Font_Draw(this->res->fontName, textBox, text, color);
	}
	//�n�C�X�R�A�̕\��
	void Object::ScoreComplicate()
	{
		ML::Box2D	textBox(400, 80, 80, 50);
		string text = "�n�C�X�R�A:" + to_string(ge->highscore);
		DG::Font_Draw(this->res->fontName, textBox, text, ML::Color(1, 0.9f, 0.7f, 0.5f));
	}

	//�v���C���[�̏�ԕ`��
	void Object::PlayerLife()
	{
		if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
			ML::Color	color(1,1,1,1);
			if (player->life <= 1) { color = ML::Color(1, 1, 0, 0); }
			ML::Box2D	textBox(400, 150, 80, 30);
			string text = "HP : " + to_string(player->life);
			DG::Font_Draw(this->res->fontName, textBox, text, color);
		}
	}
	void Object::PlayerShotRemain()
	{
		if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
			ML::Color	color(1, 1, 1, 1);
			if (player->shotCnt <= 5) { color = ML::Color(1, 1, 0, 0); }
			ML::Box2D	textBox(400, 200, 80, 30);
			string text = "�c��e�� : " + to_string(player->shotCnt);
			DG::Font_Draw(this->res->fontName, textBox, text, color);
		}
	}
	void Object::PlayerSpeed()
	{
		if (auto player = ge->GetTask_One_GN<Player::Object>("�v���C��", "NoName")) {
			ML::Color	color(1, 1, 1, 0);

			if (player->yelcryHit) { 
				ML::Box2D	textBox(400, 230, 80, 30);
				string text = "���X�s�[�h�A�b�v�� ";
				if ((ge->frameCnt / 20) % 3 == 0) {
					DG::Font_Draw(this->res->fontName, textBox, text, color);
				}
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