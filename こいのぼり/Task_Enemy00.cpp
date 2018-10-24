//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy00.h"
#include  "Task_Shot00.h"
#include  "Task_Effect01.h"
#include  "Task_Player.h"
#include  "Task_Effect08.h"

namespace  Barrel
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "BarrelImg";
		DG::Image_Create(this->imageName, "./data/image/Taru.png");
		DG::Image_Create("BreakImg", "./data/image/tarubreak.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Image_Erase("BreakImg");
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
		this->timeCnt = 0;
		this->hitBase = ML::Box2D(-12, -12, 24, 24);
		this->state = Flow;
		this->point = 100;
		
		
		this->FlowAnim();
		this->DestructAnim();
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
		this->animCnt++;
		//樽とプレイヤーが衝突したら、樽は爆発するエフェクトを出す
		if (this->state != State::Destruct)
		{
			if (this->PlayerHit())
			{
				this->ScoreMinus(this->point / 3);
				//爆発エフェクト
				auto explode = Explosion::Object::Create(true);
				explode->pos = this->pos;
			}
		}
		if (this->state == State::Destruct)
		{
			this->timeCnt++;
			if (this->timeCnt > 20) {
				this->ScorePlus(this->point);
				this->Kill();
			}
		}
		if (this->hitFlag)
		{
			this->animCnt = 0;
			this->ToDestruct();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		switch (this->state) {
		case State::Flow:
			this->FlowBarrelDraw();
			break;
		case State::Destruct:
			this->DestructBarrelDraw();
			break;
		}
	}

	//-------------------------------------------------------------------
	//樽が流れるアニメーション
	void Object::FlowAnim()
	{
		for (int i = 0; i < 2; ++i) {
			this->BarrelImg[i] = ML::Box2D(i * 85, 0, 85, 64);
		}
	}
	//樽が壊れるアニメーション
	void Object::DestructAnim()
	{
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 4; ++j) {
				this->DestructImg[i * j + j] = ML::Box2D(j * 85, i * 64, 85, 64);
			}
		}
	}
	//プレイヤーとの衝突
	bool Object::PlayerHit()
	{
		ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
		if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
			if (player->state == Player::Object::State::Invicible) { return false; }
			ML::Box2D	you = player->hitBase.OffsetCopy(player->pos);
			if (you.Hit(me)) {
				player->HpDown();
				this->Kill();
				return true;
			}
		}
		return false;
	}
	
	//樽のアニメーション描画
	void Object::FlowBarrelDraw()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src = this->BarrelImg[(this->animCnt / 20) % 2];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	void Object::DestructBarrelDraw()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src = this->DestructImg[(this->animCnt / 20) % 3];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw("BreakImg", draw, src);
	}
	
	//破壊状態へ
	void Object::ToDestruct()
	{
		this->state = State::Destruct;
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