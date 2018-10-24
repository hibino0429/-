//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot00.h"
#include  "Task_Map2D.h"
#include  "Task_Effect00.h"
#include  "Task_Effect01.h"
#include  "Task_Enemy04.h"
#include  "Task_Enemy00.h"
#include  "Task_Effect07.h"

namespace  Shot00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Shot00Img";
		DG::Image_Create(this->imageName, "./data/image/ShotEff/shot00.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
		this->render2D_Priority[1] = 0.6f;
		this->speed = 5.0f;
		this->hitBase = ML::Box2D(-10, -10, 20, 20);

		this->Anim();
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
		this->moveCnt++;
		//���E���Ԃ𒴂��������
		if (this->moveCnt >= 30) {
			this->Kill();
			return;
		}
		//�ړ�
		this->pos += this->moveVec;

		//�ړ���ŏ�Q���ɏՓ˂��������
		//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
		if (auto map = ge->GetTask_One_GN<Map2D::Object >("�t�B�[���h", "�}�b�v")) {
			ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
			if (map->CheckHit(hit)) {
				this->Kill();

				//�G�t�F�N�g�Ȃǂ�������
				for (int c = 0; c < 4; ++c) {
					auto eff1 = WaterEffect::Object::Create(true);
					eff1->pos = this->pos;
				}
				return;
			}
		}
		//�G�Ƃ̓����蔻��
		if (this->HitEnemyCarp() || this->HitEnemyBarrel())
		{
			auto waterEff = WaterEffect::Object::Create(true);
			waterEff->pos = this->pos;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D	src = this->shotImg[(this->animCnt / 10) % 5];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}

	//--------------------------------------------------------------------
	//�e�̃A�j���[�V����
	void Object::Anim()
	{
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 10; ++j) {
				this->shotImg[i*j + j] = ML::Box2D(j * 128, i * 128, 256, 256);
			}
		}
	}

	//�G�̌�ƏՓ�
	//�U��������������//�Ղꂢ��[�̒e������
	//�v���C���[�̒e�Ƃ̓����蔻��
	bool Object::HitEnemyBarrel()
	{
		ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
		for (auto it = ge->qa_Barrels->begin();
			it != ge->qa_Barrels->end();
			++it)
		{
			if ((*it)->CheckState() == BTask::eKill) { continue; }
			ML::Box2D	you = (*it)->hitBase.OffsetCopy((*it)->pos);
			if (you.Hit(me)) {
				(*it)->hitFlag = true;
				this->Kill();
				return true;
			}
		}
		return false;
	}

	//���ׂẴL�����ɑ΂��ē����蔻��
	bool Object::HitEnemyCarp()
	{
		ML::Box2D	me = this->hitBase.OffsetCopy(this->pos);
		for (auto it = ge->qa_Enemys->begin();
			it != ge->qa_Enemys->end();
			++it)
		{
			if ((*it)->CheckState() == BTask::eKill) { continue; }
			ML::Box2D	you = (*it)->hitBase.OffsetCopy((*it)->pos);
			if (you.Hit(me)) {
				(*it)->hitFlag = true;
				this->Kill();
				return true;
			}
		}
		return false;
	}
	//����������������������������������������������������������������������������������
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