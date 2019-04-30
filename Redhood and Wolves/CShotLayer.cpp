#include "CShotLayer.h"
#include "CShot.h"
#include "CMy.h"

CShotLayer::~CShotLayer()
{
}

bool CShotLayer::init()
{
	LastShot = 0;
	shotMake();
	return true;
}

void CShotLayer::shotMake()
{//���̾� �ȿ� �� ����
	for (int i = 0; i < 30; i++) {
		CShot* pArrow = CShot::create();
		pArrow->makeShot(i);
		this->addChild(pArrow, 0, i);
		pArrow->setPosition(Vec2(-3000, -3000));
	}
}

void CShotLayer::shotMove(int i)
{//�˸��� ���� ���� ������
	while (LastShot % 4 != i) {
		LastShot++;
		if (LastShot >= 30) {
			LastShot = 0;
		}
	}	
	auto pArrow = (CShot*)this->getChildByTag(LastShot);
	pArrow->setPosition(Vec2(300, 300));
	pArrow->stopAllActions();
	pArrow->moveShot();
}
