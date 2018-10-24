#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "BChara.h"


namespace  Player
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤ");	//グループ名
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
		//定数
		const int	MaxShotCnt = 20;		//最大ショット数
		const int	MinShotCnt = 0;			//最小ショット数
		const int	MaxDethCnt = 180;		//最大死亡までの時間
		const int	MaxspeedUpCnt = 300;	//最大スピードアップ時間
		const float	NowSpeed = 2.0f;		//スピード
		const int	MaxHp = 3;				//最大HP
		const int   MaxInviCnt = 180;		//最大無敵時間


		//定義
		enum State{Normal,Invicible,Dead};	//普通、無敵、死亡

		//変数
		string		controllerName;		
		bool        hit;				//当たったか
		bool		yelcryHit;			//黄のクリスタルの判定			///
		bool		bluecryHit;
		bool		redcryHit;
		int			dethCnt;			//死亡するまでの時間
		int			speedUpCnt;			//スピードアップ時間
		State		state;				//状態
		int			inviCnt;			//無敵時間


		//メソッド
		void Init();					//プレイヤーの初期化
		void Move(ML::Vec2& est_);		//移動処理					
		auto ShotCreate();				//弾の生成
		void ShotAngle(Dir ang_);		//弾の方向
		bool DethAnim();		//死亡エフェクト
		void SpeedIncrease();	//スピードアップ
		void SpeedManeger();	//スピード管理
		void HpDown();			//体力減らす
		void HpUp();			//体力回復
		void HpManeger();		//体力管理
		void InviCount();		//無敵処理

		void ChangeDeth();		//死亡状態にする
		void ChangeInvi();		//無敵状態にする
		void ChangeNormal();	//ふつう状態にする

		void ShotPlusCnt();
		void ShotMinusCnt();
		//仮
		void ShotAngle2(float ang_);	//弾の移動方向を返す
	};		
}