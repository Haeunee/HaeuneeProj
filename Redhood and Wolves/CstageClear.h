#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
using namespace std;
USING_NS_CC;


class CstageClear : public Scene
{
public:
	CREATE_FUNC(CstageClear);
	~CstageClear();
	virtual bool init();
	void startBtn(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void titleBtn(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void save(int, int, float);
	void update(float dt);

	float textTime;
	string str;
	int add;
	int coin;
	int stage;
};

