#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CBoss.h"
USING_NS_CC;

class CMainScene : public Scene
{
public:
	CREATE_FUNC(CMainScene);
	~CMainScene();

	virtual bool init();
	void update(float dt);
	CBoss* boss;

	bool onTouchBegan(Touch* touch, Event* event);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	int skillEnergy;
	float sheldTime;
	float skillTime;
	bool sheld;
	bool Rainbow;
	float playtime;

	void RedBtn(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void BlueBtn(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void GreenBtn(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void YellowBtn(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void StopBtn(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void attackHit();
	void attackBoss();
	void BshotHit();

	int stage;
	int coin;
	int life;
	float bossAtkTime;

	void sceneChange();

	float itemTime;
	float shotDelay;
	bool RbtnClick;
	bool GbtnClick;
	bool BbtnClick;
	bool YbtnClick;

	void startGame(int,int);
	float warningTime;

	int firstHit;
	bool rainbowAct;
	bool EnergyAct;
	bool warnAct;
	bool sheldAct;

	bool start;
	void hitAnimation(int i);

};

