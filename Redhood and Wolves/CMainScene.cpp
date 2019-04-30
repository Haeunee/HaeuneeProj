#include "CMainScene.h"
#include "CMy.h"
#include "CShot.h"
#include "CShotLayer.h"
#include "CSkill.h"
#include "CSkillLayer.h"
#include "CEnemy.h"
#include "CEnemyLayer.h"
#include "Cstop.h"
#include "CstageClear.h"
#include "CBGLayer.h"
#include "CItem.h"
#include "CDeadScene.h"
#include "CBoss.h"
#include "CBossShot.h"
#include "CGameSave.h"
#include "CSoundMgr.h"
#include "CEnding.h"

CMainScene::~CMainScene()
{
}

bool CMainScene::init()
{
	srand(time(NULL));
	firstHit = 0;
	coin = 0;
	Rainbow = false;
	sheldTime = 0;
	sheld = false;
	skillTime = 0;
	stage = 1;
	skillEnergy = 0;
	life = 100;
	bossAtkTime = 0;
	shotDelay = 0;
	YbtnClick = false;
	RbtnClick = false;
	GbtnClick = false;
	BbtnClick = false;
	warningTime = 0;
	rainbowAct = false;
	EnergyAct = false;
	warnAct = false;
	start = false;
	sheldAct = false;
	return true;
}

void CMainScene::update(float dt)
{
	playtime += dt;
	auto pSL = (CShotLayer*)this->getChildByName("SL");
	auto pSkillL = (CSkillLayer*)this->getChildByName("skillL");
	auto pELayer = (CEnemyLayer*)this->getChildByName("EL");
	auto pSheld = (Sprite*)this->getChildByName("sheld");
	auto pMy = (CMy*)this->getChildByName("my");

	if (sheld == true) { //방어막 활성화
		sheldTime += dt;
		pSheld->setOpacity(255);
		if (sheldAct == false) {
			sheldAct = true;
			Animation* sheldAni = Animation::create();
			for (int i = 1; i < 7; i++) {
				sheldAni->addSpriteFrameWithFile(StringUtils::format("sheld0%d.png", i));
			}
			sheldAni->setLoops(-1);
			sheldAni->setDelayPerUnit(0.2f);
			Animate* sheldAnima = Animate::create(sheldAni);
			pSheld->runAction(sheldAnima);
		}
		if (sheldTime >= 5) { //방어막 시간 경과
			pSheld->setOpacity(0);
			pSheld->stopAllActions();
			sheld = false;
			sheldTime = 0;
		}
	}

	if (Rainbow == true) { //무지개 공격 활성화
		skillTime += dt;
		Sprite* Rainbowbg = (Sprite*)this->getChildByName("rainbow");
		Rainbowbg->setOpacity(255);
		if (rainbowAct == false) { //무지개 공격 활성화 중인 것을 알림
			rainbowAct = true;
			Animation* prainbow = Animation::create();
			for (int i = 1; i <= 9; i++) {
				prainbow->addSpriteFrameWithFile(StringUtils::format("ani_rainbow_start_0%d.png", i));
			}
			prainbow->setLoops(1);
			prainbow->setDelayPerUnit(0.1f);
			Animate* pRainbow = Animate::create(prainbow);
			DelayTime* pDelay = DelayTime::create(2);
			Blink* pBlink = Blink::create(2, 10);
			Sequence* pSeq = Sequence::create(pRainbow, pDelay, pBlink, 0);
			Rainbowbg->runAction(pSeq);
		}
		if (skillTime >= 5) {
			rainbowAct = false;
			Rainbowbg->setOpacity(0);
			Rainbow = false;
			skillTime = 0;
		}
	}

	if (playtime >= 40) { 
		if (boss == NULL) { //보스 등장
			warningTime += dt;
			auto pWarning = (Sprite*)this->getChildByName("warning");
			pWarning->setVisible(true);
			if (warnAct == false) { //위험 알림
				CSoundMgr::getInstance()->stopBGM();
				CSoundMgr::getInstance()->PlayEffect("Effect_warning.mp3");
				warnAct = true;
				Animation* pWarnAni = Animation::create();
				pWarnAni->addSpriteFrameWithFileName("boss_warning1.png");
				pWarnAni->addSpriteFrameWithFileName("boss_warning2.png");
				pWarnAni->setDelayPerUnit(0.2);
				pWarnAni->setLoops(-1);
				Animate* pWarnAnima = Animate::create(pWarnAni);
				pWarning->runAction(pWarnAnima);
			}
			if (warningTime >= 3) {
				CSoundMgr::getInstance()->PlayBGM("BGM_bossAttack.mp3");
				pWarning->setVisible(false);
				boss = CBoss::create();
				boss->bossHP = 100 * stage;
				this->addChild(boss, 0, "boss");
				Sprite* hp_Bar_base = Sprite::create("ob_hpBar_brown_boss.png");
				this->addChild(hp_Bar_base);
				hp_Bar_base->setPosition(Vec2(1100, 500));
				Sprite* hp_Bar = Sprite::create("ob_hpBar_red_boss.png");
				ProgressTimer* PT2 = ProgressTimer::create(hp_Bar);
				this->addChild(PT2, 0, "pt3");
				PT2->setPosition(Vec2(1100, 500));
				PT2->setType(ProgressTimer::Type::BAR);
				PT2->setPercentage(100.0f);
				PT2->setMidpoint(Vec2(0.0f, 0.0f));
				PT2->setBarChangeRate(Vec2(1.0f, 0.0f));
				warningTime = 0;
			}
		}
		else {
			bossAtkTime += dt;
		}

		if (bossAtkTime >= 2 + rand() % 10 * 0.1) { //스테이지에 따른 보스 공격
			bossAtkTime = 0;
			if (stage == 1) { 
				boss->bossAttack_move();
			}
			else if (stage == 2) {
				
				int bossatk = rand() % 2;
				switch (bossatk)
				{
				case 0:
					boss->bossAttack_shot();
					break;
				case 1:
					boss->bossAttack_move();
					break;
				default:
					break;
				}
			}
			else if (stage == 3) {
				int bossatk = rand() % 3;
				switch (bossatk)
				{
				case 0:
					boss->bossAttack_momont();
					break;
				case 1:
					boss->bossAttack_move();
					break;
				case 2:
					boss->bossAttack_shot();
					break;
				default:
					break;
				}
			}
		}
	}

	if (boss != NULL) {
		BshotHit(); //보스 존재할 때만 보스와 충돌체크
		attackBoss();
		auto BHpBar = (ProgressTimer*)this->getChildByName("pt3");
		BHpBar->setPercentage(boss->bossHP / stage);
		auto pBoss = (Sprite*)boss->getChildByName("sprboss");
		for (int i = 0; i < 30; i++) {
			auto pBShot = (CBossShot*)boss->getChildByTag(i);
			if (pBShot->getPositionX() < 250) { //보스 샷이 캐릭터 위치에 도달한 경우
				pBShot->stopAllActions();
				pBShot->setPosition(Vec2(3000, -3000));
				if (sheld == false) {
					life -= 20;
				}
			}
		}
		if (pBoss->getPositionX() < 250) { //보스 돌진 공격 때 피격
			boss->hit = true;
			pBoss->stopAllActions();
			pBoss->setPosition(Vec2(1100, 350));
			if (sheld == false) {
				life -= 20;
			}
		}

	}

	if (pELayer != NULL) {
		for (int i = 0; i < 40; i++) { //적이 캐릭터의 포지션에 도달했을 때 캐릭터 피격 처리
			auto pEnemy = (CEnemy*)pELayer->getChildByTag(i);
			auto sEnemy = (Sprite*)pEnemy->getChildByTag(i % 4);
			if (pEnemy->getPositionX() <= pMy->getPositionX() && pEnemy->hit == true) {
				if (sheld == true) {
					pEnemy->setPosition(Vec2(3000, 3000));
					pEnemy->stopAllActions();
					sEnemy->stopAllActions();
				}
				else {
					life -= 10;
					pEnemy->hit = false;
				}
			}
		}
	}

	auto LCoin = (Label*)this->getChildByName("coin");//코인 수 갱신
	LCoin->setString(StringUtils::format("%d", coin));
	if (pELayer != NULL) {//충돌 체크
		attackHit();
	}

	if (itemTime >= 10) {//일정 시간마다 아이템 생성
		CItem* item = CItem::create();
		this->addChild(item, 0, "item");
		itemTime = 0;
	}

	auto pItem = (CItem*)this->getChildByName("item");
	if (pItem != NULL) { //아이템 종류에 따라 효과 적용
		if (pItem->getPositionX() < 200) {
			if (pItem->itemNum == 1) {
				life += 30;
				pItem->removeFromParentAndCleanup(true);
			}
			else if (pItem->itemNum == 2) {
				Rainbow = true;
				pItem->removeFromParentAndCleanup(true);
			}
		}

	}
	//hp바 갱신
	auto HpBar = (ProgressTimer*)this->getChildByName("pt2");
	if (life >= 100) {
		life = 100;
	}
	HpBar->setPercentage(life);
	itemTime += dt;
	shotDelay += dt;
	//버튼에 따라 샷 생성
	if (shotDelay >= 0.5) {
		if (RbtnClick == true || GbtnClick == true || BbtnClick == true || YbtnClick == true) {
			pMy->bowAni();
			if (Rainbow == true) {
				pSkillL->shotMove();
			}
			else if (RbtnClick == true)
				pSL->shotMove(0);
			else if (GbtnClick == true)
				pSL->shotMove(1);
			else if (BbtnClick == true)
				pSL->shotMove(2);
			else if (YbtnClick == true)
				pSL->shotMove(3);

			RbtnClick = false;
			GbtnClick = false;
			BbtnClick = false;
			YbtnClick = false;
			shotDelay = 0;
		}
	}
	//게이지 가득 차서 스킬 사용 가능
	if (skillEnergy >= 100) {
		if (EnergyAct == false) {
			EnergyAct = true;
			auto pGuage_img = Sprite::create("ob_guageBar_effect1.png");
			this->addChild(pGuage_img, 0, "guage_cover");
			pGuage_img->setPosition(Vec2(639, 61));
			pGuage_img->setAnchorPoint(Vec2(0.5, 0));
			Blink* pBlink = Blink::create(2, 5);
			RepeatForever* pRep = RepeatForever::create(pBlink);
			pGuage_img->runAction(pRep);
		}
	}

	sceneChange();
}

bool CMainScene::onTouchBegan(Touch * touch, Event * event)
{//가득 찬 게이지 클릭
	auto pGuage_img = (Sprite*)this->getChildByName("guage_cover");
	auto PT = (ProgressTimer*)this->getChildByName("pt");
	Rect rt = PT->getBoundingBox();
	bool b = rt.containsPoint(touch->getLocation());
	if (b == true) {
		if (skillEnergy >= 100) {
			ProgressTo* pto = ProgressTo::create(5, 0.0f);
			PT->runAction(pto);
			sheld = true;
			skillEnergy = 0;
			EnergyAct = false;
			pGuage_img->removeFromParentAndCleanup(true);
		}
		return b;
	}
	else {
		return -1;
	}
}
//누른 버튼 색에 따라 다른 사운드 적용, 샷 생성
void CMainScene::RedBtn(Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	auto pSL = (CShotLayer*)this->getChildByName("SL");
	auto pMy = (CMy*)this->getChildByName("my");
	auto pSkillL = (CSkillLayer*)this->getChildByName("skillL");
	if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		CSoundMgr::getInstance()->PlayEffect("Effect_fire.mp3");
		RbtnClick = true;
	}
}

void CMainScene::BlueBtn(Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	auto pSL = (CShotLayer*)this->getChildByName("SL");
	auto pMy = (CMy*)this->getChildByName("my");
	auto pSkillL = (CSkillLayer*)this->getChildByName("skillL");
	if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		CSoundMgr::getInstance()->PlayEffect("Effect_wind.mp3");
		BbtnClick = true;
	}
}

void CMainScene::GreenBtn(Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	auto pSL = (CShotLayer*)this->getChildByName("SL");
	auto pMy = (CMy*)this->getChildByName("my");
	auto pSkillL = (CSkillLayer*)this->getChildByName("skillL");
	if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		CSoundMgr::getInstance()->PlayEffect("Effect_wind.mp3");
		GbtnClick = true;
	}
}

void CMainScene::YellowBtn(Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	auto pSL = (CShotLayer*)this->getChildByName("SL");
	auto pMy = (CMy*)this->getChildByName("my");
	auto pSkillL = (CSkillLayer*)this->getChildByName("skillL");
	if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		CSoundMgr::getInstance()->PlayEffect("Effect_thunder.mp3");
		YbtnClick = true;
	}
}

void CMainScene::StopBtn(Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{ //모두 정지, 누를 수 없게 함
	cocos2d::ui::Button* stopBtn = (cocos2d::ui::Button*)this->getChildByName("stop");
	cocos2d::ui::Button* RedBtn = (cocos2d::ui::Button*)this->getChildByName("R");
	cocos2d::ui::Button* BlueBtn = (cocos2d::ui::Button*)this->getChildByName("B");
	cocos2d::ui::Button* YellowBtn = (cocos2d::ui::Button*)this->getChildByName("Y");
	cocos2d::ui::Button* GreenBtn = (cocos2d::ui::Button*)this->getChildByName("G");
	if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		Director::getInstance()->pause();
		stopBtn->setTouchEnabled(false);
		RedBtn->setTouchEnabled(false);
		BlueBtn->setTouchEnabled(false);
		YellowBtn->setTouchEnabled(false);
		GreenBtn->setTouchEnabled(false);
		Cstop* stop = Cstop::create();
		this->addChild(stop, 200);
	}
	else {
		return;
	}
}

void CMainScene::attackHit()
{ //적과 유저 공격 충돌 체크
	auto pEL = (CEnemyLayer*)this->getChildByName("EL");
	auto pSL = (CShotLayer*)this->getChildByName("SL");
	auto pSkillL = (CSkillLayer*)this->getChildByName("skillL");
	auto pSG = (ProgressTimer*)this->getChildByName("pt");
	Sprite* sEnemy;
	Rect E_rt;
	CEnemy* hitEnemy;
	CEnemy* pEnemy;

	for (int j = 0; j < 30; j++) {
		firstHit = 0;
		auto pShot = (CShot*)pSL->getChildByTag(j);
		auto pSkill = (CSkill*)pSkillL->getChildByTag(j);
		if (pShot != NULL || pSkill != NULL) {
			Rect S_rt;
			if (Rainbow == true) { //무지개 공격인지 아닌지
				S_rt = pSkill->getBoundingBox();
			}
			else {
				S_rt = pShot->getBoundingBox();
			}
			for (int i = 0; i < 40; i++) { //가장 앞에 있는 늑대 가져오기
				pEnemy = (CEnemy*)pEL->getChildByTag(i);
				hitEnemy = (CEnemy*)pEL->getChildByTag(firstHit);
				if (pEnemy != NULL) {
					pEnemy->NowEnemy = i;
					if (pEnemy->getPositionX() < hitEnemy->getPositionX()
						&&(pEnemy->getPositionX()>350)) {
						firstHit = i;
					}
				}
			}
			hitEnemy = (CEnemy*)pEL->getChildByTag(firstHit);
			E_rt = hitEnemy->getBoundingBox();
			if (Rainbow == true || firstHit % 4 == j % 4) { //적절히 충돌 한 경우
				bool b = S_rt.intersectsRect(E_rt);
				if ((hitEnemy->getPositionX() >= 380 && hitEnemy->getPositionX() <= 450)) {
					if (b == true) { //범위 안에서 충돌했는 지 체크
						sEnemy = (Sprite*)hitEnemy->getChildByTag(firstHit % 4);
						if (sheld == false) {
							skillEnergy += 10;
							pSG->setPercentage(skillEnergy);
						}
						hitAnimation(firstHit);
						coin += 10;
						hitEnemy->setPosition(Vec2(3000, 3000));
						hitEnemy->stopAllActions();
						sEnemy->stopAllActions();
						pShot->setPosition(Vec2(-3000, -3000));
						pShot->stopAllActions();
						pSkill->setPosition(Vec2(-3000, -3000));
						pSkill->stopAllActions();
						return;
					}
				}
			}
		}
	}
}

void CMainScene::attackBoss()
{ //보스와 유저 공격 충돌 체크
	auto pSL = (CShotLayer*)this->getChildByName("SL");
	auto pSkillL = (CSkillLayer*)this->getChildByName("skillL");

	for (int j = 0; j < 30; j++) {
		auto pShot = (CShot*)pSL->getChildByTag(j);
		auto pSkill = (CSkill*)pSkillL->getChildByTag(j);
		if (pShot != NULL || pSkill != NULL) { //무지개 공격인지 아닌지
			Rect S_rt;
			if (Rainbow == true) {
				S_rt = pSkill->getBoundingBox();
			}
			else {
				S_rt = pShot->getBoundingBox();
			}
			auto pBoss = (Sprite*)boss->getChildByName("sprboss");
			if (pBoss != NULL) {
				Rect E_rt = pBoss->getBoundingBox();
				if (Rainbow == true || boss->bossColor == j % 4) {//충돌했을 때
					bool b = S_rt.intersectsRect(E_rt);
					if (b == true) {
						auto pSG = (ProgressTimer*)this->getChildByName("pt");
						if ((pBoss->getPositionX() >= 200 && pBoss->getPositionX() <= 450)) {
							if (sheld == false) { //범위 안에서 충돌했는 지 체크
								skillEnergy += 10;
								pSG->setPercentage(skillEnergy);
							}
							coin += 10;
							boss->bossHP -= 10;
							boss->hitAni();
							boss->hit = true;
							pBoss->stopAllActions();
							pBoss->setPosition(Vec2(1100, 350));
							pShot->setPosition(Vec2(-3000, -3000));
							pShot->stopAllActions();
							pSkill->setPosition(Vec2(-3000, -3000));
							pSkill->stopAllActions();
						}
						return;
					}
				}
			}
		}
	}
}

void CMainScene::BshotHit()
{//보스의 샷과 유저의 공격 충돌체크
	auto pSL = (CShotLayer*)this->getChildByName("SL");
	auto pSkillL = (CSkillLayer*)this->getChildByName("skillL");

	for (int j = 0; j < 30; j++) {
		auto pShot = (CShot*)pSL->getChildByTag(j);
		auto pSkill = (CSkill*)pSkillL->getChildByTag(j);
		if (pShot != NULL || pSkill != NULL) {
			Rect S_rt;
			if (Rainbow == true) {
				S_rt = pSkill->getBoundingBox();
			}
			else {
				S_rt = pShot->getBoundingBox();
			}
			for (int i = 0; i < 30; i++) {
				auto pBShot = (CBossShot*)boss->getChildByTag(i);
				auto pBoss = (Sprite*)boss->getChildByName("sprboss");
				auto sBShot = (Sprite*)pBShot->getChildByTag(1);
				if (pBShot != NULL) {
					Rect E_rt = pBShot->getBoundingBox();
					if (Rainbow == true || i % 4 == j % 4) { //충돌한 경우
						bool b = S_rt.intersectsRect(E_rt);
						if (b == true) {
							auto pSG = (ProgressTimer*)this->getChildByName("pt");
							if ((pBShot->getPositionX() >= 200 && pBShot->getPositionX() <= 450)) {
								if (sheld == false) {
									skillEnergy += 10;
									pSG->setPercentage(skillEnergy);
								}
								coin += 10;
								MoveTo* pMTo = MoveTo::create(0.3f, pBoss->getPosition());
								Place* pPla = Place::create(Vec2(3000, -3000));
								Sequence* pSeq = Sequence::create(pMTo, pPla, 0);
								boss->hitAni();
								pBShot->stopAllActions();
								pBShot->runAction(pSeq);
								sBShot->setFlippedX(true);
								boss->bossHP -= 10;

								pShot->setPosition(Vec2(-3000, -3000));
								pShot->stopAllActions();
								pSkill->setPosition(Vec2(-3000, -3000));
								pSkill->stopAllActions();
							}
							return;
						}
					}
				}
			}
		}
	}
}

void CMainScene::sceneChange()
{ //게임의 결과가 결정되었을 때
	if (boss != NULL) {
		if (boss->bossHP <= 0) {
			if (stage == 1) {
				CstageClear* pSC = CstageClear::create();
				pSC->save(coin, stage, playtime);
				Director::getInstance()->replaceScene(pSC);
			}
			else if (stage == 2) {
				CstageClear* pSC = CstageClear::create();
				pSC->save(coin, stage, playtime);
				Director::getInstance()->replaceScene(pSC);
			}
			else if (stage == 3) {
				CEnding* pEnd = CEnding::create(); 
				pEnd->coin = coin;
				Director::getInstance()->replaceScene(pEnd);
			}
		}
	}
	if (life <= 0) {
		CDeadScene* pDS = CDeadScene::create();
		CGameSave* save = (CGameSave*)this->getChildByName("save");
		Director::getInstance()->replaceScene(pDS);
	}
}

void CMainScene::startGame(int c,int s)
{
	coin = c;
	stage = s;
	CSoundMgr::getInstance()->PlayBGM("BGM_main.mp3");
	playtime = 0;
	//배경
	CBGLayer* pBg = CBGLayer::create();
	pBg->stage = stage;
	pBg->makeBG();
	this->addChild(pBg);
	//유저 캐릭터
	CMy* pMy = CMy::create();
	this->addChild(pMy, 0, "my");
	pMy->setPosition(Vec2(200, 300));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(CMainScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	//유저 공격
	CShotLayer* pSL = CShotLayer::create();
	this->addChild(pSL, 0, "SL");
	//방어막
	Sprite* pSheld = Sprite::create("sheld01.png");
	this->addChild(pSheld, 0, "sheld");
	pSheld->setOpacity(0);
	pSheld->setPosition(pMy->getPosition());
	//공격 성공 존
	Sprite* pZone = Sprite::create("zone1.png");
	this->addChild(pZone, 0, "zone");
	pZone->setPosition(Vec2(380, 200));

	Animation* zoneAni = Animation::create();
	zoneAni->addSpriteFrameWithFileName("zone1.png");
	zoneAni->addSpriteFrameWithFileName("zone2.png");
	zoneAni->setDelayPerUnit(0.2f);
	zoneAni->setLoops(-1);
	Animate* zoneAnima = Animate::create(zoneAni);
	pZone->runAction(zoneAnima);

	Sprite* pZoneArr = Sprite::create("zone_arrow.png");
	this->addChild(pZoneArr, 0, "zonearr");
	pZoneArr->setPosition(Vec2(380, 300));
	MoveBy* pMBy = MoveBy::create(0.8f, Vec2(0, -20));
	MoveBy* pMBy2 = MoveBy::create(0.8f, Vec2(0, 20));
	Sequence* pSeq = Sequence::create(pMBy, pMBy2, 0);
	RepeatForever* pRep = RepeatForever::create(pSeq);
	pZoneArr->runAction(pRep);
	//무지개 스킬
	CSkillLayer* pSkillL = CSkillLayer::create();
	this->addChild(pSkillL, 0, "skillL");
	//적
	CEnemyLayer* penemylayer = CEnemyLayer::create();
	penemylayer->makeEnemy(stage);
	this->addChild(penemylayer, 0, "EL");

	// 공격 버튼 생성
	cocos2d::ui::Button* redBtn = cocos2d::ui::Button::create("btn_red.png");
	cocos2d::ui::Button* blueBtn = cocos2d::ui::Button::create("btn_blue.png");
	cocos2d::ui::Button* yellowBtn = cocos2d::ui::Button::create("btn_yellow.png");
	cocos2d::ui::Button* greenBtn = cocos2d::ui::Button::create("btn_green.png");
	this->addChild(redBtn, 10, "R");
	this->addChild(blueBtn, 10, "B");
	this->addChild(yellowBtn, 10, "Y");
	this->addChild(greenBtn, 10, "G");

	redBtn->setPosition(Vec2(10, 50));
	blueBtn->setPosition(Vec2(1260, 50));
	yellowBtn->setPosition(Vec2(220, 10));
	greenBtn->setPosition(Vec2(1050, 10));

	redBtn->setAnchorPoint(Vec2::ZERO);
	blueBtn->setAnchorPoint(Vec2(1, 0));
	yellowBtn->setAnchorPoint(Vec2::ZERO);
	greenBtn->setAnchorPoint(Vec2(1, 0));

	redBtn->addTouchEventListener(CC_CALLBACK_2(CMainScene::RedBtn, this));
	blueBtn->addTouchEventListener(CC_CALLBACK_2(CMainScene::BlueBtn, this));
	yellowBtn->addTouchEventListener(CC_CALLBACK_2(CMainScene::YellowBtn, this));
	greenBtn->addTouchEventListener(CC_CALLBACK_2(CMainScene::GreenBtn, this));

	//gauge 생성 
	Sprite* gauge_base = Sprite::create("ob_guageBar_black.png");
	this->addChild(gauge_base);
	gauge_base->setPosition(Vec2(640, 100));
	Sprite* gauge = Sprite::create("ob_guageBar_gradation.png");
	ProgressTimer* PT = ProgressTimer::create(gauge);
	this->addChild(PT);
	PT->setName("pt");
	PT->setPosition(Vec2(640, 100));
	PT->setType(ProgressTimer::Type::BAR);
	PT->setPercentage(0.0f);
	Sprite* gauge_cover = Sprite::create("ob_guageBar_white.png");
	gauge_cover->setAnchorPoint(Vec2(0.5, 0));
	this->addChild(gauge_cover);
	gauge_cover->setPosition(Vec2(640, 63));
	PT->setMidpoint(Vec2(0.0f, 0.0f));
	PT->setBarChangeRate(Vec2(1.0f, 0.0f));
	//공격성공할 때마다 10.0f씩 올리기 

	//stop 버튼 생성 
	cocos2d::ui::Button* stopBtn = cocos2d::ui::Button::create("btn_stop.png");
	this->addChild(stopBtn);
	stopBtn->setName("stop");
	stopBtn->setAnchorPoint(Vec2(1, 1));
	stopBtn->setPosition(Vec2(1200, 700));

	stopBtn->addTouchEventListener(CC_CALLBACK_2(CMainScene::StopBtn, this));

	Sprite* pCoin = Sprite::create("ob_coin.png");
	this->addChild(pCoin, 0, "coins");
	pCoin->setPosition(Vec2(550, 670));

	//코인 획득 Label 생성
	Label* L_coin = Label::create(CCString::createWithFormat("%0.2d", coin)->getCString(), "BM-HANNA.ttf", 50);
	this->addChild(L_coin, 0, "coin");
	L_coin->setPosition(Vec2(640, 670));

	//Hp_Bar 생성
	Sprite* hp_Bar_base = Sprite::create("ob_hpBar_brown.png");
	this->addChild(hp_Bar_base, 0, "hp_base");
	hp_Bar_base->setPosition(Vec2(200, 400));
	Sprite* hp_Bar = Sprite::create("ob_hpBar_red.png");
	ProgressTimer* PT2 = ProgressTimer::create(hp_Bar);
	this->addChild(PT2, 0, "pt2");
	PT2->setPosition(Vec2(200, 400));
	PT2->setType(ProgressTimer::Type::BAR);
	PT2->setPercentage(100.0f);
	PT2->setMidpoint(Vec2(0.0f, 0.0f));
	PT2->setBarChangeRate(Vec2(1.0f, 0.0f));
	
	Sprite* pWarning = Sprite::create("boss_warning1.png");
	this->addChild(pWarning, 100, "warning");
	pWarning->setAnchorPoint(Vec2::ZERO);
	pWarning->setVisible(false);

	CGameSave* gamesave = CGameSave::create();
	this->addChild(gamesave, 0, "save");

	Sprite* Rainbowbg = Sprite::create("ani_rainbow_start_09.png");
	this->addChild(Rainbowbg, 5, "rainbow");
	Rainbowbg->setAnchorPoint(Vec2::ZERO);
	Rainbowbg->setOpacity(0);
	Animation* prainbow = Animation::create();
	for (int i = 1; i <= 9; i++) {
		prainbow->addSpriteFrameWithFile(StringUtils::format("ani_rainbow_start_0%d.png", i));
	}
	prainbow->setLoops(1);
	prainbow->setDelayPerUnit(0.01f);
	Animate* pRainbow = Animate::create(prainbow);
	Rainbowbg->runAction(pRainbow);

	this->scheduleUpdate();
}

void CMainScene::hitAnimation(int i) //맞을때 애니
{
	CEnemyLayer* pEL = (CEnemyLayer*)this->getChildByName("EL");
	CEnemy* penemy = (CEnemy*)pEL->getChildByTag(i);

	Sprite* sprHit = Sprite::create("green (1).png");
	this->addChild(sprHit);
	Animation* ani_hit = Animation::create();
	if (i % 4 == 1)
	{
		for (int i = 1; i <= 5; i++)//  애니 5 프레임으로 수정
		{
			ani_hit->addSpriteFrameWithFile(StringUtils::format("green (%d).png", i));
		}

	}
	else if (i % 4 == 2)
	{
		for (int i = 1; i <= 5; i++)//  애니 5 프레임으로 수정
		{
			ani_hit->addSpriteFrameWithFile(StringUtils::format("blue (%d).png", i));
		}

	}
	else if (i % 4 == 3)
	{
		for (int i = 1; i <= 5; i++)//  애니 5 프레임으로 수정
		{
			ani_hit->addSpriteFrameWithFile(StringUtils::format("yellow (%d).png", i));
		}

	}
	else if (i % 4 == 0)
	{
		for (int i = 1; i <= 5; i++)//  애니 5 프레임으로 수정
		{
			ani_hit->addSpriteFrameWithFile(StringUtils::format("red (%d).png", i));
		}

	}
	sprHit->setPosition(penemy->getPosition());
	ani_hit->setDelayPerUnit(0.1f);
	ani_hit->setLoops(1);
	Animate* mate_hit = Animate::create(ani_hit);
	RemoveSelf* self = RemoveSelf::create();
	Sequence* seq = Sequence::create(mate_hit, self, NULL);
	sprHit->runAction(seq);
}