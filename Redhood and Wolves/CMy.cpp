#include "CMy.h"



CMy::~CMy()
{
}

bool CMy::init()
{
	life = 10;
	coin = 0;

	Sprite* pMy = Sprite::create("redridinghood1.png");
	this->addChild(pMy,0,"my");
	
	walkAni();
	return true;
}

void CMy::bowAni()
{//공격 애니메이션
	auto pMy = (Sprite*)this->getChildByName("my");
	Animation* pshot = Animation::create();
	for (int i = 1; i <= 7; i++) {
		string anima = StringUtils::format("redridingatk%d.png", i);
		pshot->addSpriteFrameWithFileName(anima);
	}
	pshot->setLoops(1);
	pshot->setDelayPerUnit(0.08f);
	Animate* pshoot = Animate::create(pshot);
	CCCallFunc* pCall = CCCallFunc::create(CC_CALLBACK_0(CMy::walkAni, this));
	Sequence* pSeq = Sequence::create(pshoot, pCall, 0);
	pMy->stopAllActions();
	pMy->runAction(pSeq);
}

void CMy::walkAni()
{//걷는 애니메이션
	auto pMy = (Sprite*)this->getChildByName("my");
	Animation* pWalking = Animation::create();

	for (int i = 1; i <= 4; i++) {
		string anima = StringUtils::format("redridinghood%d.png", i);
		pWalking->addSpriteFrameWithFileName(anima);
	}
	pWalking->setLoops(-1);
	pWalking->setDelayPerUnit(0.2f);

	Animate* pWalk = Animate::create(pWalking);

	pMy->runAction(pWalk);
}
