#include "CSoundMgr.h"

static CSoundMgr* soundMgr = nullptr;

CSoundMgr::CSoundMgr()
{
}

CSoundMgr::~CSoundMgr()
{
}

bool CSoundMgr::init()
{
	bgmNum = -1;
	BGMvolume = 1.0f;
	Effectvolume = 1.0f;
	//미리 소리 파일 불러오기
	AudioEngine::preload("BGM_title.mp3");
	AudioEngine::preload("BGM_main.mp3");
	AudioEngine::preload("BGM_ending.mp3");
	AudioEngine::preload("BGM_bossAttack.mp3");
	AudioEngine::preload("BGM_story.mp3");
	AudioEngine::preload("Effect_fire.mp3");
	AudioEngine::preload("Effect_thunder.mp3");
	AudioEngine::preload("Effect_warning.mp3");
	AudioEngine::preload("Effect_wind.mp3");

	return true;
}

void CSoundMgr::PlayBGM(string str)
{//배경음악 재생
	AudioEngine::stop(bgmNum);
	bgmNum = AudioEngine::play2d(str, true, BGMvolume);
}

void CSoundMgr::PlayEffect(string str)
{//효과음 재생
	AudioEngine::play2d(str, false, Effectvolume);
}


void CSoundMgr::stopBGM()
{//배경음악 끄기
	AudioEngine::stop(bgmNum);
}

CSoundMgr * CSoundMgr::getInstance()
{//싱클톤 생성
	if (soundMgr == nullptr) {
		soundMgr = new CSoundMgr;
		soundMgr->init();
	}
	return soundMgr;
}

void CSoundMgr::changeBGMVol(float f)
{//배경음 볼륨조절
	BGMvolume = f;
	AudioEngine::setVolume(bgmNum, BGMvolume);
}

void CSoundMgr::changeEffVol(float f)
{//효과음 볼륨 조절
	Effectvolume = f;
}

float CSoundMgr::getBgmVol()
{
	return BGMvolume;
}

float CSoundMgr::getEffVol()
{
	return Effectvolume;
}
