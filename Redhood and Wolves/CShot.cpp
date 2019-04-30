#include "CShot.h"
#include "CMy.h"


CShot::~CShot()
{
}

bool CShot::init()
{
	NowShot = 0;
	return true;
}

void CShot::update(float dt)
{
}

void CShot::makeShot(int i)
{//색 별로 샷 생성
	NowShot = i;
	if (i % 4 == 0) {
		Sprite* pArrow = Sprite::create("redatk.png");
		this->addChild(pArrow,0,0);
	}
	else if (i % 4 == 1) {
		Sprite* pArrow = Sprite::create("greenatk.png");
		this->addChild(pArrow, 0, 1);
	}
	else if (i % 4 == 2) {
		Sprite* pArrow = Sprite::create("blueatk.png");
		this->addChild(pArrow, 0, 2);
	}
	else if (i % 4 == 3) {
		Sprite* pArrow = Sprite::create("yellowatk.png");
		this->addChild(pArrow, 0, 3);
	}
}

void CShot::moveShot()
{ //샷 움직임
	MoveBy* pMBy = MoveBy::create(0.3f, Vec2(200,0));
	Place* pPla = Place::create(Vec2(-3000, -3000));
	Sequence* pSeq = Sequence::create(pMBy,pPla,0);
	this->runAction(pSeq);
}

Rect CShot::getBoundingBox()
{ //샷 충돌 박스 가져오기
	auto pShot = (Sprite*)this->getChildByTag(NowShot % 4);
	Rect rt = pShot->getBoundingBox();

	rt.origin.x = this->getPositionX() - pShot->getContentSize().width * pShot->getAnchorPoint().x;
	rt.origin.y = this->getPositionY() - pShot->getContentSize().height * pShot->getAnchorPoint().y;

	return rt;
}
