//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Goal.h"
#include  "Task_Player.h"
#include  "Task_Effect01.h"

namespace  Goal
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName1 = "effImg";
		DG::Image_Create(this->imageName1, "./data/image/GoalEff/pipo-mapeffect013a-min.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName1);
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
		this->render2D_Priority[1] = 0.6f;
		this->pos = ML::Vec2(0, 0);
		this->hitBase = ML::Box2D(-8, -8, 16, 16);
		this->hitFlag = false;

		this->effCnt = 0;

		this->Anim();
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
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (this->Check()) {
			this->hitFlag = true;
			ge->clearFlag = true;
		}
		else
		{
			this->hitFlag = false;
		}
		this->effCnt++;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw1 = ML::Box2D(-48, -48, 96, 96);
		draw1.Offset(this->pos);
		ML::Box2D src1 = this->effImg[(this->effCnt / 10) % 5];
		draw1.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName1, draw1, src1);
	}

	bool Object::Check()
	{
		//プレイヤーと当たったら、プレイヤーを動けなくする
		ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
		
		if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName"))
		{
			ML::Box2D	you = player->hitBase.OffsetCopy(player->pos);
			if (you.Hit(me))
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------
	//アニメーション
	void Object::Anim()
	{
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 5; j++) {
				this->effImg[i*j + j] = ML::Box2D(j * 192, i * 192, 192, 192);
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