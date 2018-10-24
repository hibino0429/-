#pragma once
#include "GameEngine_Ver3_7.h"
#include <fstream>
#include <sstream>
#include "BChara.h"
#include "BObject.h"

//�Q�[�����S��Ŏg�p����\���̂Ȃǂ��`����
//-----------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------
namespace  MyPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//----------------------------------------------
	//�J������{�`
	class Camera
	{
		Camera( ){ }
		Camera(
				const ML::Vec3&		tg_,	//	��ʑ̂̈ʒu
				const ML::Vec3&		pos_,	//	�J�����̈ʒu
				const ML::Vec3&		up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
				float				fov_,	//	����p
				float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
				float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
				float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j			
	public:
	//	�r���[���i�����֘A�j
		ML::Vec3 target;			//	��ʑ̂̈ʒu
		ML::Vec3 pos;			//	�J�����̈ʒu
		ML::Vec3 up;				//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
	//	�ˉe���i����͈͊֘A�j
		float fov;					//	����p
		float nearPlane;			//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
		float forePlane;			//	��N���b�v���ʁi��������͉f��Ȃ��j
		float aspect;				//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j
	//	�s����
		ML::Mat4x4  matView, matProj;
		~Camera( );
		typedef shared_ptr<Camera> SP;
	//	�J�����𐶐�����
		static SP Create(
				const ML::Vec3&		tg_,	//	��ʑ̂̈ʒu
				const ML::Vec3&		pos_,	//	�J�����̈ʒu
				const ML::Vec3&		up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
				float				fov_,	//	����p
				float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
				float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
				float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j	
	//	�J�����̐ݒ�
		void UpDate( );
	};
	//----------------------------------------------
	class MyGameEngine : public GameEngine
	{
	public:
		MyGameEngine( );
	//�Q�[���G���W���ɒǉ��������̂̏������ƊJ��
		bool Initialize(HWND wnd_);
		~MyGameEngine( );
	//�Q�[���G���W���ɒǉ��������̂̃X�e�b�v����
		void UpDate( );

	//3DPG1�Ή��ɂ��ǉ�
		//2D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set2DRenderState(DWORD l_);
		//3D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set3DRenderState(DWORD l_);

		//�J�����̌X�����x���ɑ΂���p�x�Ŏ擾����
		//X���{������0�x�Ƃ����J�����̌�����Ԃ��B
		float GetCameraDirectionY_XPlus0(UINT cn_);


	//�Q�[���G���W���ɒǉ����������͉̂��ɉ�����
	//----------------------------------------------
		MyPG::Camera::SP		camera[4];		//	�J����

		ML::Box2D				camera2D;		//2D�X�N���[���p
		//�G�̌��o���������Ȃ�����
		shared_ptr<vector<BChara::SP>>		qa_Enemys;
		shared_ptr<vector<BChara::SP>>		qa_Barrels;
		shared_ptr<vector<BObject::SP>>		qa_Objects;

		//�萔
		const int				LimitTime = 100;//��������
		//�Q�[���p�̕ϐ�
		int						score;			//�X�R�A
		int						highscore;		//�n�C�X�R�A
		int						time;			//����
		int						frameTime;		//�t���[������
		int						playerLife;		//�v���C���[�̗̑�
		bool					clearFlag;		//�N���A�t���O
		bool					gameOverFlag;	//�Q�[���I�[�o�[�t���O
		string					stageName;		//�X�e�[�W��

		//Load�EWrite
		bool HighScoreLoad(const string& text_);
		bool HighScoreWrite(const string& path_);
	//----------------------------------------------
	};
}
extern MyPG::MyGameEngine* ge;


