//-------------------------------------------------------------------
//�G�̊Ǘ�
//-------------------------------------------------------------------
#include  "../../../MyPG.h"
#include  "Task_EnemyManager.h"

//�G��include
#include  "../../../Task_Enemy00.h"
#include  "../../../Task_Enemy01.h"
#include  "../../../Task_Enemy02.h"
#include  "../../../Task_Enemy03.h"
#include  "../../../Task_Enemy04.h"

namespace  EnemyManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		LoadTable();

		this->count = 0;
		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->count++;
		for (auto& t : this->enemyTable) {
			if (t.point <= this->count) {
				if (t.use == 0) {
					t.use = 1;
					//�G�̏o��
					if (t.type == 1)
					{
						//�G�̎��1�̐���
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
						//�G�̎��2�̐���
						auto ene2 = otherKoi::Object::Create(true);
						ene2->pos.x = t.pos.x;
						ene2->pos.y = t.pos.y;
						ene2->moveVec.x = cos(ene2->angle) * t.move.x;
						ene2->moveVec.y = sin(ene2->angle) * t.move.y;
						ene2->point = t.scorePoint;
						ene2->Load("./data/stageData/enemy1.txt");
						//MessageBox(NULL, TEXT("�G2�o��"), TEXT("type1"), MB_OK);
					}
					else if (t.type == 3)
					{
						//�G�̎��3�̐���
						auto ene3 = RedCrystal::Object::Create(true);
						ene3->pos.x = t.pos.x;
						ene3->pos.y = t.pos.y;
					}
					else if (t.type == 4)
					{
						//�G�̎��3�̐���
						auto ene4 = BlueCrystal::Object::Create(true);
						ene4->pos.x = t.pos.x;
						ene4->pos.y = t.pos.y;
					}
					else if (t.type == 5)
					{
						//�G�̎��4�̐���
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
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}


	//�ǂݍ��݂̏���������
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

	//�G�̃f�[�^�̓ǂݍ���
	void	Object::LoadTable()
	{
		int c = 0;
		ifstream	fin("./data/EnemyAppear/apper.txt");

		if (!fin)
		{
			MessageBox(NULL, TEXT("�ǂݍ��ݕs��"), TEXT("�c�O"), MB_OK);
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

	//�X�e�[�W�ɂ���ēG�̓ǂݍ��ރt�@�C����ς���
	void	Object::LoadStageEnemyData(const string& stageName)
	{
		this->stageEnemyFile = "./data/EnemyAppear/";
		if (stageName == "����")
		{
			this->stageEnemyFile += + "appearStage1.txt";
		}
		else if (stageName == "����")
		{
			this->stageEnemyFile += "appearStage1.txt";
		}
		else if (stageName == "�㗬")
		{
			this->stageEnemyFile += "appearStage2.txt";
		}
	}

	
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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