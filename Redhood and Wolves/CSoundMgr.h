#pragma once
#include "cocos2d.h"
USING_NS_CC;
#include "AudioEngine.h"
#include <string>
using namespace std;
using namespace experimental;

class CSoundMgr
{

public:
	CSoundMgr();
	~CSoundMgr();



	bool init();
	void PlayBGM(string str);
	void PlayEffect(string str);
	void stopBGM();

	static CSoundMgr* getInstance();

	void changeBGMVol(float f);
	void changeEffVol(float f);

	float getBgmVol();
	float getEffVol();

private:
	int bgmNum;
	float BGMvolume;
	float Effectvolume;

	
};

