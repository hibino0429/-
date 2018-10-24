//-------------------------------------------------------------------
//視覚
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_View.h"
#include  "Task_Player.h"

namespace  View
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "ViewImg";
		DG::Image_Create(this->imageName, "./data/image/HitTest.bmp");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
		this->render2D_Priority[1] = 0.4f;

		this->hitBase		= ML::Box2D(0, -1, 50, 2);	//横長
		this->hitBase1 = ML::Box2D(0, -1, 50, 2);
		this->hitFlag		= false;


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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D	src(0, 0, 100, 100);

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->imageName, this->angle + this->Ang45, ML::Vec2(0, -1));
		DG::Image_Draw(this->res->imageName, draw, src);

		ML::Box2D	draw1 = this->hitBase1.OffsetCopy(this->pos);
		
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->imageName, this->angle - this->Ang45, ML::Vec2(0, -1));
		DG::Image_Draw(this->res->imageName, draw1, src);
	}

	//-------------------------------------------------------------------
	//生成
	void Object::Create(ML::Vec2 pos_,float ang_)
	{
		//キャラの位置に視覚の中心を持ってくる
		this->pos = pos_;
		//キャラの向きアングルと視覚の向きアングルを合わせる
		this->angle = ang_;

		//設定する
		this->Set();
	}
	//設定
	void Object::Set()
	{
		//生成したものを基準に視覚を作る
		this->centerVec			 = ML::Vec2(cos(this->angle), sin(this->angle));
		this->viewVec[ViewLeft]	 = ML::Vec2(cos(this->angle - this->Ang45), sin(this->angle - this->Ang45));
		this->viewVec[ViewRight] = ML::Vec2(cos(this->angle + this->Ang45), sin(this->angle + this->Ang45));
	}
	//外積の計算
	float Object::Cross(ML::Vec2& v1,ML::Vec2& v2)
	{
		return v1.y * v2.x - v1.x * v2.y;
	}
	//内積の計算
	float Object::Product(ML::Vec2& p1,ML::Vec2& p2)
	{
		return p1.x * p2.x + p1.y * p2.y;
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