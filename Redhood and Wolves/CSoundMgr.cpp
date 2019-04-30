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
	//�̸� �Ҹ� ���� �ҷ�����
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
{//������� ���
	AudioEngine::stop(bgmNum);
	bgmNum = AudioEngine::play2d(str, true, BGMvolume);
}

void CSoundMgr::PlayEffect(string str)
{//ȿ���� ���
	AudioEngine::play2d(str, false, Effectvolume);
}


void CSoundMgr::stopBGM()
{//������� ����
	AudioEngine::stop(bgmNum);
}

CSoundMgr * CSoundMgr::getInstance()
{//��Ŭ�� ����
	if (soundMgr == nullptr) {
		soundMgr = new CSoundMgr;
		soundMgr->init();
	}
	return soundMgr;
}

void CSoundMgr::changeBGMVol(float f)
{//����� ��������
	BGMvolume = f;
	AudioEngine::setVolume(bgmNum, BGMvolume);
}

void CSoundMgr::changeEffVol(float f)
{//ȿ���� ���� ����
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
