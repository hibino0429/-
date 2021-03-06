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
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->fontName = "FontH";
		DG::Font_Create(this->fontName, "MS SVbN", 8, 16);
		this->backImg = "scoreBackImg";
		DG::Image_Create(this->backImg, "./data/image/scoreBG.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		DG::Font_Erase(this->fontName);
		DG::Image_Erase(this->backImg);
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		ge->HighScoreLoad("./data/highscore/Ί¬nCXRA.txt");
		this->highScore[0] = ge->highscore;
		ge->HighScoreLoad("./data/highscore/¬nCXRA.txt");
		this->highScore[1] = ge->highscore;
		ge->HighScoreLoad("./data/highscore/γ¬nCXRA.txt");
		this->highScore[2] = ge->highscore;
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
			auto nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		if (in.B1.down) { this->Kill(); }
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		//wi
		ML::Box2D	draw(0, 0, 480, 270);
		ML::Box2D	src(0, 0, 1920, 720);
		DG::Image_Draw(this->res->backImg, draw, src);

		this->Font_Draw("Ί¬", ML::Box2D(100, 50, 200, 50),0);
		this->Font_Draw("¬", ML::Box2D(100, 100, 200, 50),1);
		this->Font_Draw("γ¬", ML::Box2D(100, 150, 200, 50),2);
	}

	//\¦
	void Object::Font_Draw(string stageName_,ML::Box2D draw_,int index_)
	{
		string text = stageName_ + ":" + to_string(this->highScore[index_]);
		DG::Font_Draw("FontH", draw_, text, ML::Color(1, 1, 1, 1));
	}
	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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