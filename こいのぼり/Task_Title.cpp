//-------------------------------------------------------------------
//タイトル画面
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "TitleLogoImg";
		this->fontName = "FontC";
		DG::Image_Create(this->imageName, "./data/image/back.png");
		DG::Font_Create(this->fontName, "MS ゴシック", 8, 16);

		DG::Image_Create("TitleLogo", "./data/image/タイトル.png");
		
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase(this->fontName);

		DG::Image_Erase("TitleLogo");

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
		ge->score = 0;
		ge->highscore = 0;

		this->logoPosY = 270;
		this->preSelectCnt = -1;
		this->nowSelectCnt = 0;
		this->inputDown = false;

		Bot	bot[5] =
		{
			{ML::Vec2(240,150),ML::Box2D(-50,-50,100,100),"ゲーム説明",true,false},
			{ML::Vec2(240,170),ML::Box2D(-50,-50,100,100),"下流",false,false},
			{ML::Vec2(240,190),ML::Box2D(-50,-50,100,100),"中流",false,false},
			{ML::Vec2(240,210),ML::Box2D(-50,-50,100,100),"上流",false,false},
			{ML::Vec2(240,230),ML::Box2D(-50,-50,100,100),"ハイスコア",false,false},
		};
		//ボタンのデータを初期化
		for (int i = 0; i < this->maxButton; ++i)
		{
			this->button[i].Init(bot[i]);
		}
		
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
			//ボタンの名前で、呼び出しを変える
			if (ge->stageName == "ゲーム説明")
			{
				auto nextTask = Explain::Object::Create(true);
			}
			else if (ge->stageName == "ハイスコア")
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
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		this->logoPosY -= 9;
		if (this->logoPosY <= 0) {
			this->logoPosY = 0;
		}
		//--------
		for (int i = 0; i < this->maxButton; ++i) {
			//拡大率の調整
			if (this->button[i].select) { this->button[i].exRate = 1.2f; }
			else { this->button[i].exRate = 1.0f; }
		}
		if (this->logoPosY == 0)
		{
			//ボタンを操作する
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
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw(0, 0, 480, 270);
		ML::Box2D  src(0, 0, 1280, 720);
		DG::Image_Draw(this->res->imageName, draw, src);

		//ボタンの描画
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

		//タイトルロゴ
		ML::Box2D	draw1(0, 0, 480, 270);
		ML::Box2D	src1(0, 0, 480, 270);
		DG::Image_Draw("TitleLogo", draw1, src1);
	}

	//ボタンセレクト処理
	void Object::ButtonSelect()
	{
		auto in = DI::GPad_GetState("P1");
		//上キーが押された
		if (in.LStick.U.down) { this->nowSelectCnt--; this->inputDown = true; }
		if (in.LStick.D.down) { this->nowSelectCnt++; this->inputDown = true; }
		
		//カウントの上限
		if (this->nowSelectCnt > this->maxSelect) { this->nowSelectCnt = this->minSelect; }
		if (this->nowSelectCnt < this->minSelect) { this->nowSelectCnt = this->maxSelect; }

	}
	//ボタン決定
	void Object::ButtonDecide()
	{
		this->button[this->preSelectCnt].select = false;
		this->button[this->nowSelectCnt].select = true;
	}
	//ボタンが押されたら
	void Object::ButtonEnter()
	{
		auto in = DI::GPad_GetState("P1");
		if (this->button[this->nowSelectCnt].select)
		{
			if (in.B1.down)
			{
				this->button[this->nowSelectCnt].push = true;
				this->StageName();		//ステージの名前
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

	//ステージの名前
	void Object::StageName()
	{
		ge->stageName =  this->button[this->nowSelectCnt].name;
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