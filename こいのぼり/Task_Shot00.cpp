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
	//\[XÌú»
	bool  Resource::Initialize()
	{
		this->imageName = "Shot00Img";
		DG::Image_Create(this->imageName, "./data/image/ShotEff/shot00.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XÌðú
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//uú»v^XN¶¬ÉPñ¾¯s¤
	bool  Object::Initialize()
	{
		//X[p[NXú»
		__super::Initialize(defGroupName, defName, true);
		//\[XNX¶¬or\[X¤L
		this->res = Resource::Create();

		//f[^ú»
		this->render2D_Priority[1] = 0.6f;
		this->speed = 5.0f;
		this->hitBase = ML::Box2D(-10, -10, 20, 20);

		this->Anim();
		//^XNÌ¶¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uI¹v^XNÁÅÉPñ¾¯s¤
	bool  Object::Finalize()
	{
		//f[^^XNðú


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ø«p¬^XNÌ¶¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[És¤
	void  Object::UpDate()
	{
		this->moveCnt++;
		//ÀEÔð´¦½çÁÅ
		if (this->moveCnt >= 30) {
			this->Kill();
			return;
		}
		//Ú®
		this->pos += this->moveVec;

		//Ú®æÅáQ¨ÉÕËµ½çÁÅ
		//}bvª¶Ý·é©²×Ä©çANZX
		if (auto map = ge->GetTask_One_GN<Map2D::Object >("tB[h", "}bv")) {
			ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
			if (map->CheckHit(hit)) {
				this->Kill();

				//GtFNgÈÇðÁ¦é
				for (int c = 0; c < 4; ++c) {
					auto eff1 = WaterEffect::Object::Create(true);
					eff1->pos = this->pos;
				}
				return;
			}
		}
		//GÆÌ½è»è
		if (this->HitEnemyCarp() || this->HitEnemyBarrel())
		{
			auto waterEff = WaterEffect::Object::Create(true);
			waterEff->pos = this->pos;
		}
	}
	//-------------------------------------------------------------------
	//uQc`ævPt[És¤
	void  Object::Render2D_AF()
	{
		ML::Box2D	draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D	src = this->shotImg[(this->animCnt / 10) % 5];

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}

	//--------------------------------------------------------------------
	//eÌAj[V
	void Object::Anim()
	{
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 10; ++j) {
				this->shotImg[i*j + j] = ML::Box2D(j * 128, i * 128, 256, 256);
			}
		}
	}

	//GÌïÆÕË
	//Uµ½¤ª//Õê¢â[Ìeª
	//vC[ÌeÆÌ½è»è
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

	//·×ÄÌLÉÎµÄ½è»è
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
	//
	//ÈºÍî{IÉÏXsvÈ\bh
	//
	//-------------------------------------------------------------------
	//^XN¶¬û
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWÉo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYÉ¸sµ½çKill
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
	//\[XNXÌ¶¬
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