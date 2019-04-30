#include "CGameSave.h"
#include "CMainScene.h"

CGameSave::~CGameSave()
{
}

bool CGameSave::init()
{
	coin = 0;
	stage = 1;
	path = FileUtils::getInstance()->getWritablePath();
	fileData = path + "RedHoodSave.dat";
	return true;
}


void CGameSave::save(int coin, int stage)
{//데이터(코인, 스테이지) 저장
	saveF = fopen(fileData.c_str(), "w+");
	fprintf(saveF, "%d %d", coin ,stage);
	fclose(saveF);
}

void CGameSave::load()
{//데이터(코인, 스테이지) 가져오기
	saveF = fopen(fileData.c_str(), "r+");
	if (saveF == NULL) {
		return;
	}
	fscanf(saveF, "%d", &coin);
	fscanf(saveF, "%d", &stage);
	fclose(saveF);
}

void CGameSave::SceneChange()
{//시작 시 데이터 넘기기
	auto pMS = CMainScene::create();
	pMS->startGame(coin, stage);
	Director::getInstance()->replaceScene(pMS);
}
