#include "CSkill.h"
#include "CMy.h"

CSkill::~CSkill()
{
}

bool CSkill::init()
{
	Sprite* pArrow = Sprite::create("Rainbowatk1.png");
	this->addChild(pArrow, 0, 0);

	this->scheduleUpdate();
	return true;
}

void CSkill::moveShot()
{ //무지개 공격 애니메이션, 움직임
	auto pShot = (Sprite*)this->getChildByTag(0);
	Animation* pAni = Animation::create();
	for (int i = 1; i < 5; i++) {
		pAni->addSpriteFrameWithFile(StringUtils::format("Rainbowatk%d.png", i));
	}
	pAni->setDelayPerUnit(0.06f);
	pAni->setLoops(1);
	Animate* pAnima = Animate::create(pAni);
	pShot->runAction(pAnima);
	MoveBy* pMBy = MoveBy::create(0.3f, Vec2(200, 0));
	Place* pPla = Place::create(Vec2(-3000, -3000));
	Sequence* pSeq = Sequence::create(pMBy, pPla, 0);
	this->runAction(pSeq);
}

Rect CSkill::getBoundingBox()
{//스킬 충돌박스
	auto pShot = (Sprite*)this->getChildByTag(0);
	Rect rt = pShot->getBoundingBox(); 

	rt.origin.x = this->getPositionX() - pShot->getContentSize().width * pShot->getAnchorPoint().x;
	rt.origin.y = this->getPositionY() - pShot->getContentSize().height * pShot->getAnchorPoint().y;

	return rt;
}

void CSkill::update(float dt)
{
	if (this->getPositionX() <= 0) {
		auto pShot = (Sprite*)this->getChildByTag(0);
		pShot->stopAllActions();
	}
}
