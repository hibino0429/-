//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot00.h"
#include  "Task_Map2D.h"
#include  "Task_Effect00.h"
#include  "Task_Effect01.h"
#include  "Task_Enemy04.h"
#include  "Task_Enemy00.h"
#include  "Task_Effect07.h"

namespace  Shot00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "Shot00Img";
		DG::Image_Create(this->imageName, "./data/image/ShotEff/shot00.png");
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
		this->speed = 5.0f;
		this->hitBase = ML::Box2D(-10, -10, 20, 20);

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
		//限界時間を超えたら消滅
		if (this->moveCnt >= 30) {
			this->Kill();
			return;
		}
		//移動
		this->pos += this->moveVec;

		//移動先で障害物に衝突したら消滅
		//マップが存在するか調べてからアクセス
		if (auto map = ge->GetTask_One_GN<Map2D::Object >("フィールド", "マップ")) {
			ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
			if (map->CheckHit(hit)) {
				this->Kill();

				//エフェクトなどを加える
				for (int c = 0; c < 4; ++c) {
					auto eff1 = WaterEffect::Object::Create(true);
					eff1->pos = this->pos;
				}
				return;
			}
		}
		//敵との当たり判定
		if (this->HitEnemyCarp() || this->HitEnemyBarrel())
		{
			auto waterEff = WaterEffect::Object::Create(true);
			waterEff->pos = this->pos;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D	src = this->shotImg[(this->animCnt / 10) % 5];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}

	//--------------------------------------------------------------------
	//弾のアニメーション
	void Object::Anim()
	{
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 10; ++j) {
				this->shotImg[i*j + j] = ML::Box2D(j * 128, i * 128, 256, 256);
			}
		}
	}

	//敵の鯉と衝突
	//攻撃した側が処理//ぷれいやーの弾が処理
	//プレイヤーの弾との当たり判定
	bool Object::HitEnemyBarrel()
	{
		ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
		for (auto it = ge->qa_Barrels->begin();
			it != ge->qa_Barrels->end();
			++it)
		{
			if ((*it)->CheckState() == BTask::eKill) { continue; }
			ML::Box2D	you = (*it)->hitBase.OffsetCopy((*it)->pos);
			if (you.Hit(me)) {
				(*it)->hitFlag = true;
				this->Kill();
				return true;
			}
		}
		return false;
	}

	//すべてのキャラに対して当たり判定
	bool Object::HitEnemyCarp()
	{
		ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
		for (auto it = ge->qa_Enemys->begin();
			it != ge->qa_Enemys->end();
			++it)
		{
			if ((*it)->CheckState() == BTask::eKill) { continue; }
			ML::Box2D	you = (*it)->hitBase.OffsetCopy((*it)->pos);
			if (you.Hit(me)) {
				(*it)->hitFlag = true;
				this->Kill();
				return true;
			}
		}
		return false;
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
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