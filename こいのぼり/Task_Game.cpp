 //-------------------------------------------------------------------
//ゲーム本編
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//仮フォント
		this->fontName = "FontUI";
		DG::Font_Create(this->fontName, "MS ゴシック", 4, 16);

		//仮処理
		DG::Image_Create("GameOverImg", "./data/image/gameOver.png");
		DG::Image_Create("ClearImg", "./data/image/gameClear.png");

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		//仮フォント
		DG::Font_Erase(this->fontName);
		DG::Image_Erase("GameOverImg");
		DG::Image_Erase("ClearImg");
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
		//ゲームオブジェクト
		ge->camera2D = ML::Box2D(-200, -100, 480, 270);
		ge->time = 0;
		ge->frameTime = 0;
		ge->playerLife = 0;
		ge->clearFlag = false;
		ge->gameOverFlag = false;

		this->endCnt = 0;

		//リソースを常駐させる
		this->shot00_Resorce = Shot00::Resource::Create();
		this->effect00_Resorce = Effect00::Resource::Create();

		//★タスクの生成
		//優先順位が高い順にする
		//マップの生成
		auto map = Map2D::Object::Create(true);
		map->Load("./data/Stage/" + ge->stageName + ".txt");
		map->CreateSGE();
		
		//ハイスコアの読み込み
		ge->HighScoreLoad("./data/highScore/" + ge->stageName + "ハイスコア.txt");

		auto player = Player::Object::Create(true);
		if(auto start = ge->GetTask_One_GN<Start::Object>("スタート", "NoName") ){
			player->pos = start->pos;
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("フィールド");
		ge->KillAll_G("プレイヤ");
		ge->KillAll_G("赤のクリスタル");
		ge->KillAll_G("青のクリスタル");
		ge->KillAll_G("黄のクリスタル");
		ge->KillAll_G("敵の鯉");
		ge->KillAll_G("樽");
		ge->KillAll_G("スタート");
		ge->KillAll_G("ゴール");

		//リソースの常駐を解除
		this->shot00_Resorce.reset();
		this->effect00_Resorce.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
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
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		
		//アクセス効率化
		ge->qa_Barrels = ge->GetTask_Group_G<BChara>("樽");
		ge->qa_Enemys = ge->GetTask_Group_G<BChara>("敵の鯉");
		ge->qa_Objects = ge->GetTask_Group_G<BObject>("赤クリスタル");

		//制限時間
		if (!ge->clearFlag && !ge->gameOverFlag) { this->TimeCnt(); }
		
		if (ge->clearFlag || ge->gameOverFlag) {
			this->endCnt++;
			if (this->endCnt > 300) { 
				//プレイヤーの体力を取得
				if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
					ge->playerLife = player->life;
				}
				this->Kill(); 
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D	textBox(0, 0, 80, 50);
		string	text = "ステージ:" + ge->stageName;
		DG::Font_Draw(this->res->fontName, textBox, text, ML::Color(1, 1, 1, 1));

		//制限時間
		this->TimeRender();
		//スコア
		this->ScoreRender();

		this->ScoreComplicate();

		this->PlayerLife();
		this->PlayerShotRemain();
		this->PlayerSpeed();

		//ゲームオーバー表示
		if (ge->gameOverFlag)
		{
			ML::Box2D	draw(120, 65, 240, 150);
			ML::Box2D	src(0, 0, 480, 270);
			DG::Image_Draw("GameOverImg", draw, src);
		}
		//ゲームクリア表示
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

	//スコアの計算
	void Object::ScoreCalc()
	{
		//樽 + 敵の鯉 + 時間 + 弾数 + ライフ
		//ここでは、オブジェクトの得点しか計算しない
		//プレイヤのライフを取得して最後に割り出す
		if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
			ge->score *= player->life;
		}
	}
	//スコアの表示
	void Object::ScoreRender()
	{
		ML::Color	color(1, 1, 1, 1);
		if (ge->score > ge->highscore) { color = ML::Color(1, 0, 1, 0); }
		ML::Box2D	textBox(400, 0, 80, 50);
		string	text = "スコア:" + to_string(ge->score);			//8桁表示
		DG::Font_Draw(this->res->fontName, textBox, text, color);
	}
	//時間カウント
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
	//制限時間の表示
	void Object::TimeRender()
	{
		ML::Color	color(1, 1, 1, 1);
		if (ge->LimitTime - ge->time <= 10) { color = ML::Color(1, 1, 0, 0); }
		ML::Box2D	textBox(0, 80, 80, 50);
		string	text = "制限時間:" + to_string(ge->LimitTime - ge->time);
		DG::Font_Draw(this->res->fontName, textBox, text, color);
	}
	//ハイスコアの表示
	void Object::ScoreComplicate()
	{
		ML::Box2D	textBox(400, 80, 80, 50);
		string text = "ハイスコア:" + to_string(ge->highscore);
		DG::Font_Draw(this->res->fontName, textBox, text, ML::Color(1, 0.9f, 0.7f, 0.5f));
	}

	//プレイヤーの状態描画
	void Object::PlayerLife()
	{
		if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
			ML::Color	color(1,1,1,1);
			if (player->life <= 1) { color = ML::Color(1, 1, 0, 0); }
			ML::Box2D	textBox(400, 150, 80, 30);
			string text = "HP : " + to_string(player->life);
			DG::Font_Draw(this->res->fontName, textBox, text, color);
		}
	}
	void Object::PlayerShotRemain()
	{
		if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
			ML::Color	color(1, 1, 1, 1);
			if (player->shotCnt <= 5) { color = ML::Color(1, 1, 0, 0); }
			ML::Box2D	textBox(400, 200, 80, 30);
			string text = "残り弾数 : " + to_string(player->shotCnt);
			DG::Font_Draw(this->res->fontName, textBox, text, color);
		}
	}
	void Object::PlayerSpeed()
	{
		if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
			ML::Color	color(1, 1, 1, 0);

			if (player->yelcryHit) { 
				ML::Box2D	textBox(400, 230, 80, 30);
				string text = "☆スピードアップ☆ ";
				if ((ge->frameCnt / 20) % 3 == 0) {
					DG::Font_Draw(this->res->fontName, textBox, text, color);
				}
			}
		}
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