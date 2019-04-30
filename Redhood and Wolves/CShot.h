#pragma once
#include "cocos2d.h"
USING_NS_CC;

class CShot : public Node
{
public:
	CREATE_FUNC(CShot);
	~CShot();

	virtual bool init();
	void update(float dt);

	void makeShot(int i);
	void moveShot();
	int NowShot;

	Rect getBoundingBox();
};

