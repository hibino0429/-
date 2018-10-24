#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "BChara.h"


namespace  Player
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�v���C��");	//�O���[�v��
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
		string		imageName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
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
		//�萔
		const int	MaxShotCnt = 20;		//�ő�V���b�g��
		const int	MinShotCnt = 0;			//�ŏ��V���b�g��
		const int	MaxDethCnt = 180;		//�ő厀�S�܂ł̎���
		const int	MaxspeedUpCnt = 300;	//�ő�X�s�[�h�A�b�v����
		const float	NowSpeed = 2.0f;		//�X�s�[�h
		const int	MaxHp = 3;				//�ő�HP
		const int   MaxInviCnt = 180;		//�ő喳�G����


		//��`
		enum State{Normal,Invicible,Dead};	//���ʁA���G�A���S

		//�ϐ�
		string		controllerName;		
		bool        hit;				//����������
		bool		yelcryHit;			//���̃N���X�^���̔���			///
		bool		bluecryHit;
		bool		redcryHit;
		int			dethCnt;			//���S����܂ł̎���
		int			speedUpCnt;			//�X�s�[�h�A�b�v����
		State		state;				//���
		int			inviCnt;			//���G����


		//���\�b�h
		void Init();					//�v���C���[�̏�����
		void Move(ML::Vec2& est_);		//�ړ�����					
		auto ShotCreate();				//�e�̐���
		void ShotAngle(Dir ang_);		//�e�̕���
		bool DethAnim();		//���S�G�t�F�N�g
		void SpeedIncrease();	//�X�s�[�h�A�b�v
		void SpeedManeger();	//�X�s�[�h�Ǘ�
		void HpDown();			//�̗͌��炷
		void HpUp();			//�̗͉�
		void HpManeger();		//�̗͊Ǘ�
		void InviCount();		//���G����

		void ChangeDeth();		//���S��Ԃɂ���
		void ChangeInvi();		//���G��Ԃɂ���
		void ChangeNormal();	//�ӂ���Ԃɂ���

		void ShotPlusCnt();
		void ShotMinusCnt();
		//��
		void ShotAngle2(float ang_);	//�e�̈ړ�������Ԃ�
	};		
}