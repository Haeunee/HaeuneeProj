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
{//������(����, ��������) ����
	saveF = fopen(fileData.c_str(), "w+");
	fprintf(saveF, "%d %d", coin ,stage);
	fclose(saveF);
}

void CGameSave::load()
{//������(����, ��������) ��������
	saveF = fopen(fileData.c_str(), "r+");
	if (saveF == NULL) {
		return;
	}
	fscanf(saveF, "%d", &coin);
	fscanf(saveF, "%d", &stage);
	fclose(saveF);
}

void CGameSave::SceneChange()
{//���� �� ������ �ѱ��
	auto pMS = CMainScene::create();
	pMS->startGame(coin, stage);
	Director::getInstance()->replaceScene(pMS);
}
