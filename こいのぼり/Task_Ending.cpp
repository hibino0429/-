//-------------------------------------------------------------------
//�G���f�B���O
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName =	"EndingLogoImg";
		this->clearImg =	"ClearImg";
		this->badEndImg =	"BadEndImg";
		this->ScoreFont = "ScoreFont";

		DG::Image_Create(this->imageName, "./data/image/EndBG.bmp");
		DG::Image_Create(this->clearImg, "./data/image/gameClear.png");
		DG::Image_Create(this->badEndImg, "./data/image/gameOver.png");

		//���t�H���g
		DG::Font_Create("FontD", "MS �S�V�b�N", 8, 16);
		DG::Font_Create(this->ScoreFont, "MS �S�V�b�N", 32, 64);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->clearImg);
		DG::Image_Erase(this->badEndImg);

		//���t�H���g
		DG::Font_Erase("FontD");
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
		this->logoPosY = 270;
		this->clearState = Non;
		this->oneCnt = 0;
		this->rankName = "";
		this->cnt = 0;

		this->HighScoreUpDate();	//�n�C�X�R�A�X�V
		ge->HighScoreWrite("./data/highscore/" + ge->stageName + "�n�C�X�R�A.txt");

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
			auto  nextTask = Title::Object::Create(true);
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

		if (this->logoPosY == 0) {
			if (in.B1.down) {
				//���g�ɏ��ŗv��
				this->Kill();
			}
		}
		//�����N���� ��
		this->RankUpDate();

		if (this->oneCnt == 0) {
			this->oneCnt++;
			this->ScoreUpdate();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i
		ML::Box2D	endDraw(0, 0, 480, 270);
		ML::Box2D	endSrc(0, 0, 512, 512);
		DG::Image_Draw(this->res->imageName, endDraw, endSrc);

		ML::Box2D  draw(0, 0, 480, 270);
		ML::Box2D  src(0, 0, 240, 135);

		draw.Offset(0, this->logoPosY);
		
		//�N���A�ƃQ�[���I�[�o�[
		if (ge->clearFlag) {
			src = ML::Box2D(0, 0, 480, 270);	
		}
		else if (ge->gameOverFlag) {
			src = ML::Box2D(0, 0, 480, 270);
		}

		//�X�R�A�̕\��
		this->ScoreRender();
		this->HighScoreRender();

		//�����N�̕\��
		this->RankDraw();

	}

	//�X�R�A�̍ŏI�v�Z
	void Object::ScoreUpdate()
	{
		//���ԕ����v���X
		for (this->scoreCnt = 0; this->scoreCnt <= (ge->LimitTime - ge->time); ++this->scoreCnt) {
			ge->score++;
		}
		//���C�t�������� * 0.7f
		
		if (ge->playerLife > 1) {
			ge->score *= (int)(ge->playerLife * 0.7f);
		}
	}
	//---�X�R�A
	void Object::ScoreRender()
	{
		ML::Box2D		textBox(100, 100, 150, 50);
		string	text = "�X�R�A:" + to_string(ge->score);
		DG::Font_Draw("FontD", textBox, text, ML::Color(1, 0, 0, 0));
	}
	//�n�C�X�R�A
	void Object::HighScoreRender()
	{
		ML::Box2D		textBox(100, 150, 150, 50);
		string	text = "�n�C�X�R�A:" + to_string(ge->highscore);
		DG::Font_Draw("FontD", textBox, text, ML::Color(1, 0, 0, 0));
	}
	//�n�C�X�R�A�X�V
	void Object::HighScoreUpDate()
	{
		if (ge->score > ge->highscore)
		{
			ge->highscore = ge->score;
		}
	}

	//�����L���O
	void Object::RankUpDate()
	{
		//����
		if (ge->stageName == "����") {
			this->DownStreamRank();
		}
		else if (ge->stageName == "����") {
			this->MidleStreamRank();
		}
		else if (ge->stageName == "�㗬") {
			this->UpStreamRank();
		}
	}

	void Object::RankDraw()
	{
		ML::Color color;
		
		this->cnt++;
		if (this->rankName == "C��") {
			color = ML::Color(1, 1, 1, 1);
		}
		else if (this->rankName == "B��") {
			color = ML::Color(1, 0.3f, 0.3f, 0.8f);
		}
		else if (this->rankName == "A��") {
			color = ML::Color(1, 0.8f, 0.3f, 0.3f);
		}
		else if (this->rankName == "S��") {
			color = ML::Color((float)sin((this->cnt/20)), 0.8f, 0.8f, 0.3f);
		}
		ML::Box2D	textBox(270, 150, 150, 100);
		DG::Font_Draw(this->res->ScoreFont, textBox, this->rankName,color);
	}

	//�����N���� ��
	void Object::DownStreamRank()
	{
		if (0 <= ge->score && ge->score < 1000)
		{
			//�ꉞ�N���A
			this->rankName = "C��";
		}
		else if (1000 <= ge->score && ge->score < 2000) {
			//����΂낤
			this->rankName = "B��";
		}
		else if (2000 <= ge->score && ge->score < 3000) {
			//����
			this->rankName = "A��";
		}
		else if (3000 <= ge->score && ge->score < 5000) {
			//���Ə���
			this->rankName = "S��";
		}
		else {
			//���_
			this->rankName = "�_";
		}
	}
	void Object::MidleStreamRank()
	{
		if (0 <= ge->score && ge->score < 2000)
		{
			//�ꉞ�N���A
			this->rankName = "C��";
		}
		else if (2000 <= ge->score && ge->score < 5000) {
			//����΂낤
			this->rankName = "B��";
		}
		else if (5000 <= ge->score && ge->score < 7000) {
			//����
			this->rankName = "A��";
		}
		else if (7000 <= ge->score && ge->score < 9000) {
			//���Ə���
			this->rankName = "S��";
		}
		else {
			//���_
			this->rankName = "�_";
		}
	}
	void Object::UpStreamRank()
	{
		if (0 <= ge->score && ge->score < 2000)
		{
			//�ꉞ�N���A
			this->rankName = "C��";
		}
		else if (2000 <= ge->score && ge->score < 4000) {
			//����΂낤
			this->rankName = "B��";
		}
		else if (4000 <= ge->score && ge->score < 6000) {
			//����
			this->rankName = "A��";
		}
		else if (6000 <= ge->score && ge->score < 8000) {
			//���Ə���
			this->rankName = "S��";
		}
		else {
			//���_
			this->rankName = "�_";
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