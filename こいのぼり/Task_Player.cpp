//-------------------------------------------------------------------
//プレイヤー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Map2D.h"
#include  "Task_Effect00.h"
#include  "Task_Shot00.h"
#include  "Task_Shot01.h"
#include  "Task_Goal.h"
#include  "Task_Effect01.h"
#include  "Task_Effect05.h"
#include  "Task_Effect07.h"
#include  "Task_Effect09.h"
#include  "Task_Enemy01.h"
#include  "Task_Enemy02.h"
#include  "Task_Enemy03.h"
#include  "Task_Enemy04.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/image/koi.png");
		DG::Font_Create("FontA", "MS ゴシック", 4, 8);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase("FontA");
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
		this->render2D_Priority[1] = 0.5f;
		this->Init();		//初期化
		this->Anim();		//アニメーション

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
		if (ge->gameOverFlag || ge->clearFlag) { return; }
		//移動
		ML::Vec2	est(0, 0);
		this->Move(est);
		this->CheckMove(est);	//移動範囲チェック

		//スピード管理
		this->SpeedManeger();
		this->HpManeger();		//HP管理

		//体力管理
		if (this->life <= 0) { this->state = State::Dead; ge->gameOverFlag = true; }

		switch (this->state) {
		case State::Normal:		//普通の状態
			//当たり判定
			break;
		case State::Invicible:	//無敵の状態
			this->InviCount();	//無敵のカウントし、3秒たったら、Normalへ
			break;
		case State::Dead:
			//死亡エフェクト
			this->dethCnt++;
			auto ex = CharaDead::Object::Create(true);
			ex->pos = this->pos;
			this->Kill();
		}
		//ゴールとの当たり判定(本来ゴールに任せる
		//仮処理(hit)
		if (auto go = ge->GetTask_One_GN<Goal::Object>("Goal", "NoName"))
		{
			if (go->Check()) { this->hit = true; }
		}
		//カメラ位置を調整
		{
			//プレイヤを画面のどこにおくか
			int px = ge->camera2D.w / 2;
			int py = ge->camera2D.h * 3 / 4;
			//プレイヤを画面中央に置いた時のカメラの左上座標を求める
			int cpx = int(this->pos.x) - px;
			int cpy = int(this->pos.y) - py;
			//カメラの座標を更新
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
		}
		//マップのカメラ移動範囲
		if (auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ")) {
			map->AjastCameraPos();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw = ML::Box2D(-16,-16,32,32);
		draw.Offset(this->pos);
		ML::Box2D src = this->charaImg[(this->animCnt / 10) % 4];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->imageName, this->angle, ML::Vec2(16, 16));
		switch (this->state) {
		case State::Normal:
			DG::Image_Draw(this->res->imageName, draw, src);
			break;
		case State::Invicible:
			if (this->animCnt % 2 == 0) {
				DG::Image_Draw(this->res->imageName, draw, src);
			}
			break;
		case State::Dead:
			auto deth = CharaDead::Object::Create(true);
			deth->pos = this->pos;
		}
	}

	//---------------------------------------------------
	//初期化
	void Object::Init()
	{
		//プレイヤー情報
		this->controllerName = "P1";
		this->hit = false;
		this->yelcryHit = false;
		this->dethCnt = 0;
		this->speedUpCnt = 0;
		this->state = Normal;
		this->inviCnt = 0;
		//継承情報(プレイヤーのみにカスタマイズ)
		this->hitBase = ML::Box2D(-8, -12, 16,24);
		this->shotCnt = this->MaxShotCnt;
		this->life = 3;
		this->angle = ML::ToRadian(270);
		this->speed = 2.0f;
	}
	//----------------------------------------------------------
	//移動処理
	void Object::Move(ML::Vec2& est_)
	{
		auto in = DI::GPad_GetState(this->controllerName);
		//方向とスピードで移動
		if (in.LStick.L.on) { this->angle = ML::ToRadian(180); est_ = ShotMoveDir(this->angle) * this->speed; }
		if (in.LStick.R.on) { this->angle = ML::ToRadian(0); est_ = ShotMoveDir(this->angle) * this->speed; }
		if (in.LStick.U.on) { this->angle = ML::ToRadian(270); est_ = ShotMoveDir(this->angle) * this->speed;}
		if (in.LStick.D.on) { this->angle = ML::ToRadian(90); est_ = ShotMoveDir(this->angle) * this->speed;}
		//弾を撃つ
		if (in.B1.down)
		{
			if (this->shotCnt <= this->MinShotCnt) { return; }
			this->ShotMinusCnt();
			this->ShotAngle2(this->angle);	//弾を生成し、向きを合わせる
		}
	}
	//--------------------------------------------------------------
	//弾の生成・初期位置
	auto Object::ShotCreate()
	{
		auto shot = Shot00::Object::Create(true);
		shot->pos = this->pos;
		return shot;
	}
	//----------------------------------------------------------
	//プレイヤーの向きによって弾の方向を変える
	void Object::ShotAngle(Dir ang_)
	{
		auto shot = ShotCreate();
		switch (ang_) {
		case Left:	shot->moveVec = ML::Vec2(-8, 0);	break;
		case Right:	shot->moveVec = ML::Vec2(8, 0);		break;
		case Up:	shot->moveVec = ML::Vec2(0, -8);	break;
		case Down:	shot->moveVec = ML::Vec2(0, 8);		break;
		}
	}
	//プレイヤーの向きによって弾の方向を変える
	void Object::ShotAngle2(float ang_)
	{
		auto shot = ShotCreate();
		shot->moveVec = this->ShotMoveDir(this->angle) * shot->speed;	//3.0fは弾のスピード(変数で用意する)
	}
	//死亡エフェクト
	bool Object::DethAnim()
	{
		this->dethCnt++;
		if (this->dethCnt > 5 * 60)
		{
			//死亡エフェクト
			auto deth = CharaDead::Object::Create(true);
			deth->pos.x = this->pos.x;
			deth->pos.y = this->pos.y - 20;
			return true;
		}
		return false;
	}
	//----------------------------------------------------------
	//弾数の変動
	void Object::ShotPlusCnt()
	{
		this->shotCnt += 3;
	}
	void Object::ShotMinusCnt()
	{
		this->shotCnt--;
	}
	//---------------------------------------------------------
	//スピード
	void Object::SpeedIncrease()
	{
		this->speed = this->NowSpeed * 1.3f;
	}
	void Object::SpeedManeger()
	{
		if (this->yelcryHit)
		{
			this->speedUpCnt++;
			this->SpeedIncrease();
			//プレイヤーにエフェクトを作る
			if (this->speedUpCnt == 1) {
				//プレイヤーにエフェクトを作る
				auto speedUp = SpeedUp::Object::Create(true);
				speedUp->pos = this->pos;
			}
		}
		else
		{
			this->speed = this->NowSpeed;
		}
		if (this->speedUpCnt >= this->MaxspeedUpCnt) { 
			this->speedUpCnt = 0;
			this->yelcryHit = false; 
		}
	}
	//------------------------------------------------------
	//体力管理
	void Object::HpManeger()
	{
		if (this->state != State::Normal) { return; }

		if (this->life <= 0)
		{
			//死亡する
			this->state = State::Dead;
		}
	}

	//ダメージ処理
	void Object::HpDown()
	{
		if (this->state == State::Invicible) { return; }
		//無敵時間
		this->state = State::Invicible;
		if (this->inviCnt == 0 && this->state == State::Invicible) {
			this->life--;
		}
		this->inviCnt++;
		if (this->inviCnt > this->MaxInviCnt) {
			this->inviCnt = 0; 
			this->state = State::Normal;
		}
	}
	//HP回復
	void Object::HpUp()
	{
		if (this->life < this->MaxHp) {
			this->life++;
		}
	}

	//----------------------------------------------
	//無敵カウント
	void Object::InviCount()
	{
		this->inviCnt++;
		if (this->inviCnt >= this->MaxInviCnt)
		{
			this->inviCnt = 0;
			this->ChangeNormal();
		}
	}
	//--------------------------------------------------
	//状態変化
	void Object::ChangeNormal()
	{
		this->state = State::Normal;
	}
	void Object::ChangeInvi()
	{
		this->state = State::Invicible;
	}
	void Object::ChangeDeth()
	{
		this->state = State::Dead;
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
	Object::Object()
	{	}
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