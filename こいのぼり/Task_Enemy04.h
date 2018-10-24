#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ほかの鯉
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "BChara.h"

namespace  otherKoi
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("敵の鯉");	//グループ名
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
		string		imageName;
		string		viewImg;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
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
		enum State {
			Normal,
			Attack,
			Dead
		};
		int				maxShotTimeCnt;
		State			state;
		int				timeCnt;
		//視覚のための用意
		float			searchAngle;
		float			searchDist;
		float			moveSpeed;
		float			rotRow;		//旋回能力

		void Move();	//移動
		void ShotAtk();	//攻撃
		void Deth();	//死亡
		void AI();		//行動処理
		void StateChange();	//状態遷移
		bool PlayerHit();		//プレイヤーとの当たり判定
		bool HitCheck(ML::Box2D& me_,ML::Box2D& you_);		//当たり判定
		//視覚の範囲
		bool	SearchRotation(const ML::Vec2& tg_);
		void	ViewChangeState();		//敵の行動を視覚で変える

		//処理の分割
		bool	ShotTimeCnt();		//弾を撃つまでの時間
		void	Shot();				//弾を撃つ処理

		//仮
		void Load(string stageName_);
		bool EneInfo(string path_);
	};
}