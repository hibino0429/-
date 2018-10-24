#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�񎟌��z��}�b�v
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  Map2D
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�t�B�[���h");	//�O���[�v��
	const  string  defName("�}�b�v");			//�^�X�N��
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
		string		imageName;
		int			arr[50][20];
		int			sizeY, sizeX;
		ML::Box2D	hitBase;
		ML::Box2D	chip[19];
		int			timeCnt;

		bool Load(const string& fpath_);	//�}�b�v�ǂݍ���
		bool CheckHit(const ML::Box2D& hit_);	//�����蔻��
		void AjastCameraPos();			//�J�����̃X�N���[������
		void RiverAnim();				//��̃A�j���[�V����
		void CreateSGE();			//�X�^�[�g��S�[����G�̔z�u
	};
}