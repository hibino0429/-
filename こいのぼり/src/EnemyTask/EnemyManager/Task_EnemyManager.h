#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//敵の管理
//-------------------------------------------------------------------
#include "../../../GameEngine_Ver3_7.h"

namespace  EnemyManager
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("敵の管理");	//グループ名
	const  string  defName("NoName");	//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		int		count;
		string	stageEnemyFile;

		//敵の情報
		struct EnemyData
		{
			int			point;
			ML::Vec2	pos;
			ML::Vec2	move;
			int			maxShotCnt;
			int			moveSpeed;
			int			scorePoint;
			int			type;
			int			use;
		};

		vector<EnemyData>	enemyTable;

		//敵のデータを読み込む
		void LoadTable();
		//ステージによって敵の読み込むファイルを変える
		void	LoadStageEnemyData(const string& stageName);
	};
}