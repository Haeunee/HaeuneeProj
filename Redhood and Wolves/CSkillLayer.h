#pragma once
#include "cocos2d.h"
USING_NS_CC;

class CSkillLayer : public Layer
{
public:
	CREATE_FUNC(CSkillLayer);
	~CSkillLayer();

	virtual bool init();
	void shotMake();
	void shotMove();
	int LastShot;
};

