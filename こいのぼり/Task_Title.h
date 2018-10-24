#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  Title
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("タイトル");	//グループ名
	const  string  defName("NoName");		//タスク名
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
		string		fontName;
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
		//---定数
		const int		maxButton = 5;		//ボタンの最大数
		const int		maxSelect = 4;		//選べる最大番号
		const int		minSelect = 0;		//選べる最小番号

		//---変数
		int				logoPosY;
		int				preSelectCnt;		//前の選んでいる番号
		int				nowSelectCnt;		//現在選んでいる番号
		bool			inputDown;		//上下移動したか
		struct Bot {
			ML::Vec2	 pos;
			ML::Box2D	 hitbase;
			string		 name;
			bool		 nowselect;
			bool		 push;
		};
		class Button {
		public:
			string		name;		//ボタンの名前
			bool		select;		//選択されているか
			bool		push;		//押されているか
			ML::Box2D	textBox;	//ボタンの大きさ
			ML::Vec2	pos;		//ボタンの位置
			float		exRate;		//拡大率

		public:
			void Init(Bot& b_);
		};
		Button		button[5];
		//キーの情報
		enum KeyState{Left,Right};
		KeyState	key;

		//ボタンのセレクト処理
		void ButtonSelect();
		//ボタンの決定処理
		void ButtonDecide();
		void ButtonEnter();

		//ステージの名前を返す
		void StageName();
	};
}