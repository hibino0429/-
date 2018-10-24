//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot01.h"
#include  "Task_Map2D.h"
#include  "Task_Effect05.h"
#include  "Task_Player.h"
#include  "Task_Effect06.h"

namespace  Shot01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Shot01Img";
		DG::Image_Create(this->imageName, "./data/image/shot01.png");
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
		this->render2D_Priority[1] = 0.6f;
		this->hitBase = ML::Box2D(-12, -12, 24, 24);
		//仮----
		if (ge->stageName == "下流") {
			this->speed = 3.0f;
		}
		else if (ge->stageName == "中流") {
			this->speed = 4.0f;
		}
		else if (ge->stageName == "上流") {
			this->speed = 5.0f;
		}

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
		this->moveCnt++;
		if (this->moveCnt >= 30) {
			this->Kill();
			return;
		}
		//移動
		this->pos += this->moveVec;

		this->HitPlayer();
		//移動先で障害物に衝突したら消滅
		//マップが存在するか調べてからアクセス
		if (auto map = ge->GetTask_One_GN<Map2D::Object >("フィールド", "マップ")) {
			ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
			if (map->CheckHit(hit)) {
				this->Kill();

				//エフェクトなどを加える
				auto shoteff = EneShotEff::Object::Create(true);
				shoteff->pos = this->pos;
				return;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw = this->hitBase;
		draw.Offset(this->pos);
		ML::Box2D	src = this->shotImg[(this->animCnt / 10) % 5];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);

	}

	//--------------------------------------------------------------------
	//弾のアニメーション
	void Object::Anim()
	{
		for (int i = 0; i < 6; ++i) {
			for (int j = 0; j < 5; ++j) {
				this->shotImg[i*j + j] = ML::Box2D(j * 192, i * 192, 192, 192);
			}
		}
	}
	//プレイヤーとの当たり判定
	void Object::HitPlayer()
	{
		if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
			ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D	you = player->hitBase.OffsetCopy(player->pos);
			if (you.Hit(me)) {
				player->HpDown();
				this->Kill();
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