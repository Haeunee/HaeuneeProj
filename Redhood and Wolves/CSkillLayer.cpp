#include "CSkillLayer.h"
#include "CSkill.h"


CSkillLayer::~CSkillLayer()
{
}

bool CSkillLayer::init()
{
	LastShot = 0;
	shotMake();
	return true;
}

void CSkillLayer::shotMake()
{ //¼¦ ¸¸µé±â
	for (int i = 0; i < 30; i++) {
		CSkill* pArrow = CSkill::create();
		this->addChild(pArrow, 0, i);
		pArrow->setPosition(Vec2(-3000, -3000));
	}
}

void CSkillLayer::shotMove()
{ //¼¦ ¿òÁ÷ÀÌ±â
	auto pArrow = (CSkill*)this->getChildByTag(LastShot);
	pArrow->setPosition(Vec2(300, 350));
	pArrow->stopAllActions();
	pArrow->moveShot();
	LastShot++;
	if (LastShot >= 30) { 
		LastShot = 0;
	}
}
