#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//���E
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  View
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("���E");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
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
		string			imageName;
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
		enum View
		{
			ViewLeft,
			ViewRight
		};
		const int		Ang30 = 30;
		const int		Ang45 = 45;
		const int		Ang60 = 60;
		ML::Vec2		pos;			//�ʒu
		ML::Vec2		centerVec;		//�O��\���x�N�g��
		ML::Vec2		viewVec[2];		//���o�x�N�g��(���E)
		ML::Box2D		hitBase;		//���E�x�N�g���̑傫��
		float			angle;			//�O��0�ƍl�������̊p�x
		bool			hitFlag;		//���E�x�N�g���ɓ������Ă��邩
		ML::Box2D		hitBase1;
		void	Create(ML::Vec2	pos_,float ang_);	//����
		void	Set();								//�ݒ�
		float	Cross(ML::Vec2& v1,ML::Vec2& v2);
		float	Product(ML::Vec2& p1,ML::Vec2& p2);
		
	};
}