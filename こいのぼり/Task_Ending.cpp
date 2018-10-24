//-------------------------------------------------------------------
//エンディング
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName =	"EndingLogoImg";
		this->clearImg =	"ClearImg";
		this->badEndImg =	"BadEndImg";
		this->ScoreFont = "ScoreFont";

		DG::Image_Create(this->imageName, "./data/image/EndBG.bmp");
		DG::Image_Create(this->clearImg, "./data/image/gameClear.png");
		DG::Image_Create(this->badEndImg, "./data/image/gameOver.png");

		//仮フォント
		DG::Font_Create("FontD", "MS ゴシック", 8, 16);
		DG::Font_Create(this->ScoreFont, "MS ゴシック", 32, 64);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase(this->clearImg);
		DG::Image_Erase(this->badEndImg);

		//仮フォント
		DG::Font_Erase("FontD");
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
		this->logoPosY = 270;
		this->clearState = Non;
		this->oneCnt = 0;
		this->rankName = "";
		this->cnt = 0;

		this->HighScoreUpDate();	//ハイスコア更新
		ge->HighScoreWrite("./data/highscore/" + ge->stageName + "ハイスコア.txt");

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
			auto  nextTask = Title::Object::Create(true);
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

		if (this->logoPosY == 0) {
			if (in.B1.down) {
				//自身に消滅要請
				this->Kill();
			}
		}
		//ランク処理 仮
		this->RankUpDate();

		if (this->oneCnt == 0) {
			this->oneCnt++;
			this->ScoreUpdate();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景
		ML::Box2D	endDraw(0, 0, 480, 270);
		ML::Box2D	endSrc(0, 0, 512, 512);
		DG::Image_Draw(this->res->imageName, endDraw, endSrc);

		ML::Box2D  draw(0, 0, 480, 270);
		ML::Box2D  src(0, 0, 240, 135);

		draw.Offset(0, this->logoPosY);
		
		//クリアとゲームオーバー
		if (ge->clearFlag) {
			src = ML::Box2D(0, 0, 480, 270);	
		}
		else if (ge->gameOverFlag) {
			src = ML::Box2D(0, 0, 480, 270);
		}

		//スコアの表示
		this->ScoreRender();
		this->HighScoreRender();

		//ランクの表示
		this->RankDraw();

	}

	//スコアの最終計算
	void Object::ScoreUpdate()
	{
		//時間分をプラス
		for (this->scoreCnt = 0; this->scoreCnt <= (ge->LimitTime - ge->time); ++this->scoreCnt) {
			ge->score++;
		}
		//ライフをかける * 0.7f
		
		if (ge->playerLife > 1) {
			ge->score *= (int)(ge->playerLife * 0.7f);
		}
	}
	//---スコア
	void Object::ScoreRender()
	{
		ML::Box2D		textBox(100, 100, 150, 50);
		string	text = "スコア:" + to_string(ge->score);
		DG::Font_Draw("FontD", textBox, text, ML::Color(1, 0, 0, 0));
	}
	//ハイスコア
	void Object::HighScoreRender()
	{
		ML::Box2D		textBox(100, 150, 150, 50);
		string	text = "ハイスコア:" + to_string(ge->highscore);
		DG::Font_Draw("FontD", textBox, text, ML::Color(1, 0, 0, 0));
	}
	//ハイスコア更新
	void Object::HighScoreUpDate()
	{
		if (ge->score > ge->highscore)
		{
			ge->highscore = ge->score;
		}
	}

	//ランキング
	void Object::RankUpDate()
	{
		//下流
		if (ge->stageName == "下流") {
			this->DownStreamRank();
		}
		else if (ge->stageName == "中流") {
			this->MidleStreamRank();
		}
		else if (ge->stageName == "上流") {
			this->UpStreamRank();
		}
	}

	void Object::RankDraw()
	{
		ML::Color color;
		
		this->cnt++;
		if (this->rankName == "C級") {
			color = ML::Color(1, 1, 1, 1);
		}
		else if (this->rankName == "B級") {
			color = ML::Color(1, 0.3f, 0.3f, 0.8f);
		}
		else if (this->rankName == "A級") {
			color = ML::Color(1, 0.8f, 0.3f, 0.3f);
		}
		else if (this->rankName == "S級") {
			color = ML::Color((float)sin((this->cnt/20)), 0.8f, 0.8f, 0.3f);
		}
		ML::Box2D	textBox(270, 150, 150, 100);
		DG::Font_Draw(this->res->ScoreFont, textBox, this->rankName,color);
	}

	//ランク処理 仮
	void Object::DownStreamRank()
	{
		if (0 <= ge->score && ge->score < 1000)
		{
			//一応クリア
			this->rankName = "C級";
		}
		else if (1000 <= ge->score && ge->score < 2000) {
			//がんばろう
			this->rankName = "B級";
		}
		else if (2000 <= ge->score && ge->score < 3000) {
			//中堅
			this->rankName = "A級";
		}
		else if (3000 <= ge->score && ge->score < 5000) {
			//あと少し
			this->rankName = "S級";
		}
		else {
			//満点
			this->rankName = "神";
		}
	}
	void Object::MidleStreamRank()
	{
		if (0 <= ge->score && ge->score < 2000)
		{
			//一応クリア
			this->rankName = "C級";
		}
		else if (2000 <= ge->score && ge->score < 5000) {
			//がんばろう
			this->rankName = "B級";
		}
		else if (5000 <= ge->score && ge->score < 7000) {
			//中堅
			this->rankName = "A級";
		}
		else if (7000 <= ge->score && ge->score < 9000) {
			//あと少し
			this->rankName = "S級";
		}
		else {
			//満点
			this->rankName = "神";
		}
	}
	void Object::UpStreamRank()
	{
		if (0 <= ge->score && ge->score < 2000)
		{
			//一応クリア
			this->rankName = "C級";
		}
		else if (2000 <= ge->score && ge->score < 4000) {
			//がんばろう
			this->rankName = "B級";
		}
		else if (4000 <= ge->score && ge->score < 6000) {
			//中堅
			this->rankName = "A級";
		}
		else if (6000 <= ge->score && ge->score < 8000) {
			//あと少し
			this->rankName = "S級";
		}
		else {
			//満点
			this->rankName = "神";
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