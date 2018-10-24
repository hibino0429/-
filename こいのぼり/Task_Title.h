#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  Title
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�^�C�g��");	//�O���[�v��
	const  string  defName("NoName");		//�^�X�N��
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
		//���L����ϐ��͂����ɒǉ�����
		string		imageName;
		string		fontName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//---�萔
		const int		maxButton = 5;		//�{�^���̍ő吔
		const int		maxSelect = 4;		//�I�ׂ�ő�ԍ�
		const int		minSelect = 0;		//�I�ׂ�ŏ��ԍ�

		//---�ϐ�
		int				logoPosY;
		int				preSelectCnt;		//�O�̑I��ł���ԍ�
		int				nowSelectCnt;		//���ݑI��ł���ԍ�
		bool			inputDown;		//�㉺�ړ�������
		struct Bot {
			ML::Vec2	 pos;
			ML::Box2D	 hitbase;
			string		 name;
			bool		 nowselect;
			bool		 push;
		};
		class Button {
		public:
			string		name;		//�{�^���̖��O
			bool		select;		//�I������Ă��邩
			bool		push;		//������Ă��邩
			ML::Box2D	textBox;	//�{�^���̑傫��
			ML::Vec2	pos;		//�{�^���̈ʒu
			float		exRate;		//�g�嗦

		public:
			void Init(Bot& b_);
		};
		Button		button[5];
		//�L�[�̏��
		enum KeyState{Left,Right};
		KeyState	key;

		//�{�^���̃Z���N�g����
		void ButtonSelect();
		//�{�^���̌��菈��
		void ButtonDecide();
		void ButtonEnter();

		//�X�e�[�W�̖��O��Ԃ�
		void StageName();
	};
}