//-------------------------------------------------------------------
//二次元配列マップ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Start.h"
#include  "Task_Goal.h"
#include  "Task_Game.h"
#include  "Task_Enemy00.h"
#include  "Task_Enemy04.h"
#include  "Task_Enemy01.h"
#include  "Task_Enemy02.h"
#include  "Task_Enemy03.h"

namespace  Map2D
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		this->render2D_Priority[1] = 0.9f;
		if (auto game = ge->GetTask_One_GN<Game::Object>("本編", "総括")) {
			//this->imageName = "MapChipImg" + to_string(game->stageNum);
			this->imageName = "MapChipImg";
		}
		timeCnt = 0;

		//マップのゼロクリア
		for (int y = 0; y < 50; ++y) {
			for (int x = 0; x < 20; ++x) {
				this->arr[y][x] = -1;
			}
		}
		this->sizeX = 0;
		this->sizeY = 0;
		this->hitBase = ML::Box2D(0, 0, 0, 0);

		//マップチップ情報の初期化
		for (int c = 0; c < 16; ++c) {
			int x = (c % 8);
			int y = (c / 8);
			this->chip[c] = ML::Box2D(x * 32, y * 32, 32, 32);
		}
		this->chip[16] = ML::Box2D(0, 64, 32, 32);
		this->chip[17] = ML::Box2D(0, 96, 32, 32);
		this->chip[18] = ML::Box2D(0, 0, 32, 32);

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		DG::Image_Erase(this->imageName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->RiverAnim();			//川のアニメーション
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (false == this->hitBase.Hit(ge->camera2D)) {
			return;
		}
		//カメラとﾏｯﾌﾟが重なっている範囲だけの矩形を作る
		RECT c = {
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h };
		RECT m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h };
		//2つの矩形の重なっている範囲だけの矩形を求める
		RECT isr;
		isr.left = max(c.left, m.left);
		isr.top = max(c.top, m.top);
		isr.right = min(c.right, m.right);
		isr.bottom = min(c.bottom, m.bottom);

		//ループ範囲を決定
		int sx, sy, ex, ey;
		sx = isr.left / 32;
		sy = isr.top / 32;
		ex = (isr.right - 1) / 32;
		ey = (isr.bottom - 1) / 32;

		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				ML::Box2D	draw(x * 32, y * 32, 32, 32);
				//スクロール対応
				draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
				DG::Image_Draw(this->imageName, draw, this->chip[this->arr[y][x]]);
			}
		}
	}
	//マップファイルの読み込み
	bool  Object::Load(const string& fpath_)
	{
		ifstream fin(fpath_);
		if (!fin) { return false; }

		string chipFileName, chipFilePath;
		fin >> chipFileName;
		chipFilePath = "./data/image/" + chipFileName;
		DG::Image_Create(this->imageName, chipFilePath);

		//マップ配列サイズの読み込み
		fin >> this->sizeX >> this->sizeY;
		this->hitBase = ML::Box2D(0, 0, this->sizeX * 32, this->sizeY * 32);

		//マップ配列データの読み込み
		for (int y = 0; y < this->sizeY; ++y) {
			for (int x = 0; x < this->sizeX; ++x) {
				fin >> this->arr[y][x];
			}
		}
		fin.close();
		return true;
	}
	//-------------------------------------------------------------------
	//川のアニメーション
	void Object::RiverAnim()
	{
		this->timeCnt++;
		if (this->timeCnt <= 32)
		{
			this->chip[17].y -= 1;
		}
		if (this->timeCnt > 32)
		{
			this->chip[17].y = 96;
			this->timeCnt = 0;
		}
	}
	//-------------------------------------------------------------------
	//あたり判定
	bool  Object::CheckHit(const ML::Box2D& h_)
	{
		RECT r = { h_.x,h_.y,h_.x + h_.w,h_.y + h_.h };
		//矩形がマップ外に出ていたら丸め込む
		RECT m = {  this->hitBase.x,
					this->hitBase.y,
					this->hitBase.x + this->hitBase.w,
					this->hitBase.y + this->hitBase.h};

		if (r.left   < m.left)   { r.left = m.left; }
		if (r.top    < m.top)    { r.top = m.top; }
		if (r.right  > m.right)  { r.right = m.right; }
		if (r.bottom > m.bottom) { r.bottom = m.bottom; }

		//ループ範囲調整
		int sx, sy, ex, ey;
		sx = r.left / 32;
		sy = r.top / 32;
		ex = (r.right - 1) / 32;
		ey = (r.bottom - 1) / 32;

		//範囲内の障害物を探す
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				if (9 > this->arr[y][x]) { return true; }
			}
		}
		return false;
	}

	//カメラのスクロール制限
	void Object::AjastCameraPos()
	{
		RECT c = {
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h };
		RECT m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h };

		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top < m.top) { ge->camera2D.y = m.top; }
		if (c.right > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }

	}
	//スタートやごーるなどの生成
	void Object::CreateSGE()
	{
		for (int y = 0; y < this->sizeY; ++y) {
			for (int x = 0; x < this->sizeX; ++x) {
				//敵発見
				if (this->arr[y][x] == 11) {
					//敵を生成
					auto ene = otherKoi::Object::Create(true);
					ene->pos = ML::Vec2((float)x * 32 + 16, (float)y * 32 + 16);
					this->arr[y][x] = 17;
				}
				//スタート地点
				if (this->arr[y][x] == 12) {
					auto start = Start::Object::Create(true);
					start->pos = ML::Vec2((float)x * 32 + 16, (float)y * 32 + 16);
					this->arr[y][x] = 17;
				}
				//ゴール地点
				if (this->arr[y][x] == 13) {
					auto goal = Goal::Object::Create(true);
					goal->pos = ML::Vec2((float)x * 32 + 16, (float)y * 32 + 16);
					this->arr[y][x] = 17;
				}
				//たる地点
				if (this->arr[y][x] == 14) {
					auto bul = Barrel::Object::Create(true);
					bul->pos = ML::Vec2((float)x * 32 + 16, (float)y * 32 + 16);
					this->arr[y][x] = 17;
				}
				//赤クリスタル
				if (this->arr[y][x] == 20) {
					auto redcry = RedCrystal::Object::Create(true);
					redcry->pos = ML::Vec2((float)x * 32 + 16, (float)y * 32 + 16);
					this->arr[y][x] = 17;
				}
				//青クリスタル
				if (this->arr[y][x] == 21) {
					auto bluecry = BlueCrystal::Object::Create(true);
					bluecry->pos = ML::Vec2((float)x * 32 + 16, (float)y * 32 + 16);
					this->arr[y][x] = 17;
				}
				//黄クリスタル
				if (this->arr[y][x] == 22) {
					auto yelcry = YellowCrystal::Object::Create(true);
					yelcry->pos = ML::Vec2((float)x * 32 + 16, (float)y * 32 + 16);
					this->arr[y][x] = 17;
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