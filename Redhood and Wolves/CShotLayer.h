#pragma once
#include "cocos2d.h"
USING_NS_CC;

class CShotLayer : public Layer
{
public:
	CREATE_FUNC(CShotLayer);
	~CShotLayer();

	virtual bool init();

	void shotMake();
	void shotMove(int i);

	int LastShot;
};

