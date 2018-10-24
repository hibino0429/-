//-------------------------------------------------------------------
//ゲーム説明
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Explain.h"
#include  "Task_Title.h"

namespace  Explain
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Explain";
		this->backImageName = "backImg";
		DG::Image_Create(backImageName, "./data/image/EndBG.bmp");
		DG::Font_Create(this->imageName, "MS ゴシック", 8, 12);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(backImageName);
		DG::Font_Erase(this->imageName);
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
		this->render2D_Priority[1] = 0.8f;
		this->pos = ML::Vec2(0, 0);
		this->draw = ML::Box2D(-16, -16, 32, 32);
		this->explain = "";
		
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

		if (in.B1.down)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景
		ML::Box2D	draw(0, 0, 480, 270);
		ML::Box2D	src(0, 0, 512, 512);
		DG::Image_Draw(this->res->backImageName, draw, src);

		ML::Box2D	textBox(0, 0, 480, 270);
		char buf[512];
		sprintf(buf,
			"<ゲーム説明> \n\n"
			"鯉の頂点を目指して、相手を倒しながら、ゴールを目指す\n\n"
			"(プレイヤー)\n"
			"プレイヤーの弾で		[樽]	を破壊できる\n"
			"プレイヤーの弾で		[敵の鯉]	を倒せる\n"
			"[敵]と[樽]と衝突すると、ダメージを受ける\n\n"
			"プレイヤーのライフは　[3]\n"
			"プレイヤーの弾数は　	 [20]\n"
			"黄クリスタルは　	[スピードアップ]\n"
			"赤クリスタルは	[弾数をプラス]\n"
			"青クリスタルは	[ライフ回復]\n");

		DG::Font_Draw(this->res->imageName, textBox, buf, ML::Color(1, 0, 0, 0));

	}

	//------------------------------------------------
	//説明書のロード
	bool Object::Load(string Path)
	{
		ifstream fin(Path);
		if (!fin) { return false; }

		fin >> this->explain;

		fin.close();
		return true;
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