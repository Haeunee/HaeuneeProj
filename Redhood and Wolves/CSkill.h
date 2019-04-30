#pragma once
#include "cocos2d.h"
USING_NS_CC;

class CSkill : public Node
{
public:
	CREATE_FUNC(CSkill);
	~CSkill();

	virtual bool init();
	void moveShot();
	Rect getBoundingBox();
	void update(float dt);
};

