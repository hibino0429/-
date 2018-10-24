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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->fontName = "FontH";
		DG::Font_Create(this->fontName, "MS ゴシック", 8, 16);
		this->backImg = "scoreBackImg";
		DG::Image_Create(this->backImg, "./data/image/scoreBG.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Font_Erase(this->fontName);
		DG::Image_Erase(this->backImg);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		ge->HighScoreLoad("./data/highscore/下流ハイスコア.txt");
		this->highScore[0] = ge->highscore;
		ge->HighScoreLoad("./data/highscore/中流ハイスコア.txt");
		this->highScore[1] = ge->highscore;
		ge->HighScoreLoad("./data/highscore/上流ハイスコア.txt");
		this->highScore[2] = ge->highscore;
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		if (in.B1.down) { this->Kill(); }
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景
		ML::Box2D	draw(0, 0, 480, 270);
		ML::Box2D	src(0, 0, 1920, 720);
		DG::Image_Draw(this->res->backImg, draw, src);

		this->Font_Draw("下流", ML::Box2D(100, 50, 200, 50),0);
		this->Font_Draw("中流", ML::Box2D(100, 100, 200, 50),1);
		this->Font_Draw("上流", ML::Box2D(100, 150, 200, 50),2);
	}

	//表示
	void Object::Font_Draw(string stageName_,ML::Box2D draw_,int index_)
	{
		string text = stageName_ + ":" + to_string(this->highScore[index_]);
		DG::Font_Draw("FontH", draw_, text, ML::Color(1, 1, 1, 1));
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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