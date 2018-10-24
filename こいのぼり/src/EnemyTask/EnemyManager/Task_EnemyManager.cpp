//-------------------------------------------------------------------
//敵の管理
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_EnemyManager.h"

//敵のinclude
#include  "../../../Task_Enemy00.h"
#include  "../../../Task_Enemy01.h"
#include  "../../../Task_Enemy02.h"
#include  "../../../Task_Enemy03.h"
#include  "../../../Task_Enemy04.h"

namespace  EnemyManager
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
		LoadTable();

		this->count = 0;
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
		this->count++;
		for (auto& t : this->enemyTable) {
			if (t.point <= this->count) {
				if (t.use == 0) {
					t.use = 1;
					//敵の出現
					if (t.type == 1)
					{
						//敵の種類1の生成
						auto ene1 = Barrel::Object::Create(true);
						ene1->pos.x = t.pos.x;
						ene1->pos.y = t.pos.y;
						ene1->angle = ML::ToRadian(90);
						ene1->moveVec.x = cos(ene1->angle) * t.move.x;
						ene1->moveVec.y = sin(ene1->angle) * t.move.y;
						ene1->point = t.scorePoint;
					}
					else if (t.type == 2)
					{
						//敵の種類2の生成
						auto ene2 = otherKoi::Object::Create(true);
						ene2->pos.x = t.pos.x;
						ene2->pos.y = t.pos.y;
						ene2->moveVec.x = cos(ene2->angle) * t.move.x;
						ene2->moveVec.y = sin(ene2->angle) * t.move.y;
						ene2->point = t.scorePoint;
						ene2->Load("./data/stageData/enemy1.txt");
						//MessageBox(NULL, TEXT("敵2出現"), TEXT("type1"), MB_OK);
					}
					else if (t.type == 3)
					{
						//敵の種類3の生成
						auto ene3 = RedCrystal::Object::Create(true);
						ene3->pos.x = t.pos.x;
						ene3->pos.y = t.pos.y;
					}
					else if (t.type == 4)
					{
						//敵の種類3の生成
						auto ene4 = BlueCrystal::Object::Create(true);
						ene4->pos.x = t.pos.x;
						ene4->pos.y = t.pos.y;
					}
					else if (t.type == 5)
					{
						//敵の種類4の生成
						auto ene5 = YellowCrystal::Object::Create(true);
						ene5->pos.x = t.pos.x;
						ene5->pos.y = t.pos.y;
					}
					else if (t.type == 6)
					{
				
					}
					else if (t.type == 7)
					{
					
					}
					else if (t.type == 8)
					{
				
					}
					
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}


	//読み込みの処理を改造
	istream& operator>>(
		istream& i, Object::EnemyData& data)
	{
		i >> data.point >>
			data.pos.x >>
			data.pos.y >>
			data.move.x >>
			data.move.y >>
			data.type;
		return i;
	}

	//敵のデータの読み込み
	void	Object::LoadTable()
	{
		int c = 0;
		ifstream	fin("./data/EnemyAppear/apper.txt");

		if (!fin)
		{
			MessageBox(NULL, TEXT("読み込み不可"), TEXT("残念"), MB_OK);
			return;
		}
		if (fin.is_open()) {
			while (!fin.eof()) {
				Object::EnemyData	note;
				fin >> note;
				note.use = 0;
				enemyTable.push_back(note);
			}
		}
		fin.close();
	}

	//ステージによって敵の読み込むファイルを変える
	void	Object::LoadStageEnemyData(const string& stageName)
	{
		this->stageEnemyFile = "./data/EnemyAppear/";
		if (stageName == "下流")
		{
			this->stageEnemyFile += + "appearStage1.txt";
		}
		else if (stageName == "中流")
		{
			this->stageEnemyFile += "appearStage1.txt";
		}
		else if (stageName == "上流")
		{
			this->stageEnemyFile += "appearStage2.txt";
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