#pragma once
#include "cocos2d.h"
#include <string>
using namespace std;
USING_NS_CC;

class CGameSave :public Node
{
public:
	CREATE_FUNC(CGameSave);
	~CGameSave();

	virtual bool init();

	FILE* saveF;

	void save(int coin, int stage);

	void load();
	void SceneChange();
	int coin;
	int stage;
	string path;
	string fileData;
};
