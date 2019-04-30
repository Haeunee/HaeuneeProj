#include "CstageClear.h"
#include "CMainScene.h"
#include "CGameSave.h"
#include "CTitleScene.h"
#include "CSoundMgr.h"
#pragma execution_character_set("utf-8")

CstageClear::~CstageClear()
{
}

bool CstageClear::init()
{
	coin = 0;
	stage = 0;
	textTime = 0;
	add = 330;
	CSoundMgr::getInstance()->PlayBGM("BGM_story.mp3");
	Sprite* bg = Sprite::create("clear_bg.png");
	this->addChild(bg);
	bg->setAnchorPoint(Vec2::ZERO);
	bg->setPosition(Vec2(60, 0));

	cocos2d::ui::Button* startBtn = cocos2d::ui::Button::create("btn_next_stage.png");
	this->addChild(startBtn);
	startBtn->addTouchEventListener(CC_CALLBACK_2(CstageClear::startBtn, this));
	startBtn->setPosition(Vec2(1150, 300));

	cocos2d::ui::Button* titleBtn = cocos2d::ui::Button::create("btn_back_title.png");
	this->addChild(titleBtn);
	titleBtn->addTouchEventListener(CC_CALLBACK_2(CstageClear::titleBtn, this));
	titleBtn->setPosition(Vec2(130, 300));

	Sprite* timeIcon = Sprite::create("obj_time.png");
	this->addChild(timeIcon);
	timeIcon->setPosition(Vec2(530,380));

	Sprite* coinIcon = Sprite::create("obj_coin.png");
	this->addChild(coinIcon);
	coinIcon->setPosition(Vec2(700, 380));
	 
	Label* pClear = Label::createWithTTF("클리어", "BM-HANNA.ttf", 100);
	this->addChild(pClear);
	pClear->setPosition(Vec2(640, 520));

	Label* lNext = Label::createWithTTF("다음 스테이지로", "BM-HANNA.ttf", 30);
	this->addChild(lNext);
	lNext->setPosition(Vec2(1150, 210));

	Label* ltitle = Label::createWithTTF("타이틀로", "BM-HANNA.ttf", 30);
	this->addChild(ltitle);
	ltitle->setPosition(Vec2(130, 210));

	return true;
}

void CstageClear::startBtn(Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{//다음 스테이지로
	if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		CGameSave* pGS = CGameSave::create();
		pGS->load();
		pGS->SceneChange();
	}
	else {
		return;
	}
}

void CstageClear::titleBtn(Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{//메인으로 돌아가기
	if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		CTitleScene* pTitle = CTitleScene::create();
		CGameSave* pGS = CGameSave::create();
		pGS->save(coin, stage + 1);
		pTitle->BGMsound();
		Director::getInstance()->replaceScene(pTitle);
	}
	else {
		return;
	}
}

void CstageClear::save(int c, int s, float time)
{	//저장하기
	coin = c;
	stage = s;
	CGameSave* pGS = CGameSave::create();
	pGS->save(coin, stage + 1);
	//시간, 코인, 스테이지 표시
	float min = time / 60;
	float sec = fmodf(time, 60);
	Label* ltime = Label::createWithTTF(StringUtils::format("%0.0f:%0.0f", min, sec), "BM-HANNA.ttf", 50);
	this->addChild(ltime);
	ltime->setPosition(Vec2(600, 380));

	Label* lcoin = Label::createWithTTF(StringUtils::format("%d", coin), "BM-HANNA.ttf", 50);
	this->addChild(lcoin);
	lcoin->setPosition(Vec2(770, 380));

	Label* pstage = Label::createWithTTF(StringUtils::format("스테이지%d", stage), "BM-HANNA.ttf", 30);
	this->addChild(pstage);
	pstage->setPosition(Vec2(640, 450));
	//스테이지에 따른 스토리 가져오기
	if (stage == 1) {
		str = FileUtils::getInstance()->getStringFromFile("stageclear_story_01.txt");	
	}
	else {
		str = FileUtils::getInstance()->getStringFromFile("stageclear_story_02.txt");
	}
	this->scheduleUpdate();
}

void CstageClear::update(float dt)
{
	textTime += dt;
	string str2 = "";
	//시간마다 1줄씩 가져오기
	if (textTime >= 1) {
		textTime = 0;
		if (str != "") {
			for (int i = 0; i < str.find("\n"); i++) {
				str2 += StringUtils::format("%c", str.at(i));
			}
			str.erase(0, str.find("\n")+1);
			Label* pstory = Label::createWithTTF(str2, "BM-HANNA.ttf", 30);
			this->addChild(pstory);
			add -= 35;
			pstory->setPosition(Vec2(640, add));
			pstory->setAnchorPoint(Vec2(0.5, 0));
			pstory->setAlignment(CCTextAlignment::CENTER);
			str2 = "";
		}
	}
}
