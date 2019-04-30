#pragma once
#include "cocos2d.h"
#include <string>
using namespace std;
USING_NS_CC;

class CMy : public Node
{
public:
	CREATE_FUNC(CMy);
	~CMy();

	virtual bool init();
	void bowAni();
	void walkAni();

	int life;
	int coin;
};

