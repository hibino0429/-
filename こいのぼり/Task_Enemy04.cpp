//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy04.h"
#include  "Task_Shot00.h"
#include  "Task_Player.h"
#include  "Task_Map2D.h"
#include  "Task_View.h"
#include  "Task_Shot01.h"
#include  "Task_Effect07.h"

namespace  otherKoi
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "KoiImg";
		DG::Image_Create(this->imageName, "./data/image/koi1.png"); 
		DG::Font_Create("FontE", "MS ゴシック", 8, 8);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase("FontE");
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
		this->dire = Dir::Down;
		this->state = State::Normal;
		this->animCnt = 0;
		this->timeCnt = 0;
		this->searchAngle =  ML::ToRadian(120/2);
		this->searchDist = 200.0f;
		this->hitBase = ML::Box2D(-16, -16, 32, 32);
		this->moveSpeed = 0.5f;
		this->angle = ML::ToRadian(90);		//向き	
		this->rotRow = ML::ToRadian(4);			//旋回
		this->shotCnt = 0;
		this->point = 300;		//ポイント
		this->maxShotTimeCnt = 180;

		this->Load(ge->stageName);
		this->Anim();
		//★タスクの生成

		return true;
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

		this->timeCnt++;

		this->ViewChangeState();
		this->PlayerHit();
		if (this->hitFlag)
		{
			this->ScorePlus(this->point);
			auto deadEff = CharaDead::Object::Create(true);
			deadEff->pos = this->pos;
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw(-16, -16, 32, 32);
		draw.Offset(this->pos);
		ML::Box2D	src1(0, 64, 32, 32);
		ML::Box2D	src = this->charaImg[(this->animCnt / 10) % 4];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->imageName, this->angle, ML::Vec2(draw.w / 2.0f, draw.h / 2.0f));
		DG::Image_Draw(this->res->imageName, draw, src);

	}

	//状態遷移
	void Object::StateChange()
	{
		//状態遷移
		switch (this->state) {
		case State::Normal:
			if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
				if (this->SearchRotation(player->pos))
				{
					this->state = State::Attack;
				}
				else {
					this->state = State::Normal;
				}
			}
			break;
		case State::Attack:
			//プレイヤーが視覚内
			if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
				if (this->SearchRotation(player->pos))
				{
					this->moveVec = ML::Vec2(cos(this->angle), sin(this->angle))*this->moveSpeed;
				}
				else
				{
					this->moveVec = ML::Vec2(0, 0);
					this->state = State::Normal;
				}
			}
			break;
		case State::Dead:
			break;
		}
	}
	//行動処理
	void Object::AI()
	{
		switch (this->state) {
		case State::Normal:
			this->Move();
			break;
		case State::Attack:
			this->ShotAtk();
			break;
		case State::Dead:
			this->Deth();
			break;
		}
		//this->StateChange();	//状態遷移
	}
	//移動処理
	void Object::Move()
	{
		//上下左右にランダムに移動させる
		int move = rand() % 4;
		ML::Vec2		est(0, 0);
		switch (move) {
		case Dir::Left:
			this->angle = ML::ToRadian(180);
			est = ML::Vec2(-10,0);
			break;
		case Dir::Right:
			this->angle = ML::ToRadian(0);
			est = ML::Vec2(10, 0);
			break;
		case Dir::Up:
			this->angle = ML::ToRadian(270);
			est = ML::Vec2(0, -10);
			break;
		case Dir::Down:
			this->angle = ML::ToRadian(90);
			est = ML::Vec2(0, 10);
			break;
		}
		this->CheckMove(est);
	}
	//攻撃処理
	void Object::ShotAtk()
	{
		auto shot = Shot01::Object::Create(true);
		shot->moveVec = this->ShotMoveDir(this->angle);
	}
	//死亡処理
	void Object::Deth()
	{
		this->Kill();
	}
	//プレイヤーとの当たり判定
	bool Object::PlayerHit()
	{
		if (this->state == State::Dead) { return false; }
		ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
		if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
			ML::Box2D	you = player->hitBase.OffsetCopy(player->pos);
			if (this->HitCheck(me, you)) {
				//プレイヤーはダメージを受ける
				if (player->state == Player::Object::State::Invicible) { return false; }
				if (ge->score > 0) { this->ScoreMinus(this->point / 3); }
				player->HpDown();
				return true;
			}
		}
		return false;
	}
	//当たり判定
	bool Object::HitCheck(ML::Box2D& me_,ML::Box2D& you_)
	{
		if (you_.Hit(me_)) { return true; }
		return false;
	}

	//視覚の範囲
	bool Object::SearchRotation(const ML::Vec2& tg_)
	{
		ML::Vec2	toTarget = tg_ - this->pos;		//敵までの距離
		//視野角範囲にいるターゲットを対象にする
		float dist = toTarget.Length();
		if (dist < this->searchDist) {
			ML::Vec2	toTargetN = toTarget.Normalize();
			ML::Vec2	moveVecN = ML::Vec2(cos(this->angle), sin(this->angle));
			//外積で左右どちらかを判定
			float dot = moveVecN.x * toTargetN.x + moveVecN.y * toTargetN.y;
			if (dot > cos(this->searchAngle)) {
				if (dot > cos(this->rotRow)) {
					//this->angle1 = atan2(toTargetN.y,toTargetN.x);
				}
				else {
					float cr = moveVecN.x * toTargetN.y - moveVecN.y * toTargetN.x;
					//左右に合わせて旋回する
					if (cr >= 0) { this->angle += this->rotRow; }
					else { this->angle -= this->rotRow; }
				}
				return true;
			}
		}
		return false;
	}

	//弾を撃つまでの時間
	bool Object::ShotTimeCnt()
	{
		this->shotCnt++;
		if (this->shotCnt >= this->maxShotTimeCnt) 
		{
			this->shotCnt = 0;
			return true;
		}
		return false;
	}
	//弾を撃つ処理
	void Object::Shot()
	{
		if (!this->ShotTimeCnt()) { return; }
		auto shot = Shot01::Object::Create(true);
		shot->pos = this->pos;
		shot->moveVec = ML::Vec2(cos(this->angle), sin(this->angle)) * shot->speed;
	}
	//視覚によっての状態遷移
	//敵を見つける前は、きょろきょろ
	//敵を見つけたら、追尾	//さらに弾を5秒に1回発射する
	void Object::ViewChangeState()
	{
		if (auto player = ge->GetTask_One_GN<Player::Object>("プレイヤ", "NoName")) {
			if (this->SearchRotation(player->pos))
			{
				this->moveVec = ML::Vec2(cos(this->angle), sin(this->angle))*this->moveSpeed;
				
				this->Shot();
			}
			else
			{
				this->moveVec = ML::Vec2(0, 0);
			}
			ML::Vec2	est = this->moveVec;
			this->CheckMove(est);
		}
	}

	//ロード
	void Object::Load(string stageName_)
	{
		if (stageName_ == "下流") {
			this->EneInfo("./data/EneCarp/EneInfo01.txt");
		}
		else if (stageName_ == "中流") {
			this->EneInfo("./data/EneCarp/EneInfo02.txt");
		}
		else if (stageName_ == "上流") {
			this->EneInfo("./data/EneCarp/EneInfo03.txt");
		}
	}
	bool Object::EneInfo(string path_)
	{
		ifstream	fin(path_);
		if (!fin) { return false; }

		fin >> this->moveSpeed;
		
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