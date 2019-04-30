#pragma once


enum eMSG //메세지 종류
{
	em_LOGIN = 1,
	em_LOGOUT,
	em_LOGINCHECK,
	em_MATCHREQUEST,
	em_ENTER,
	em_CHARINFO,
	em_MOVE,
	em_ATK,
	em_HIT,
	em_INFO,
	em_ITEMSPAWN,
	em_USEITEM,
	em_ENDITEM,
	em_GETOBJ,
	em_THROWOBJ,
	em_READY,
	em_END,
	em_CHAT,
};

enum eATKTYPE
{
	em_NORMAL = 1,
	em_OBJTHROW,
};

enum eRESULT
{
	em_WIN = 1,
	em_LOSE,
};

struct sFlag //어떤 정보를 보냈는지 구분할 플래그
{
	int flag;
	sFlag(int f = 0) : flag(f) {};
};

struct sLogin : sFlag //로그인
{
	sLogin() :sFlag(flag = eMSG::em_LOGIN) {};
	int loginSucc = 0;
	char nick[30];
};

struct sLogout : sFlag //로그아웃
{
	sLogout() :sFlag(flag = eMSG::em_LOGOUT) {};
};

struct sLoginCheck : sFlag //로그인 되어있는/되어있지 않은 친구 체크
{
	sLoginCheck() :sFlag(flag = eMSG::em_LOGINCHECK) {};
	int loginChk = 0;
	char nick[30];
};

struct sMatchReq : sFlag //친선 매칭 신청
{
	sMatchReq() :sFlag(flag = eMSG::em_MATCHREQUEST) {};
	int matchSucc = 0;
	char recvUserNick[30];
	char sendUserNick[30];
};

struct sGameRoom : sFlag //매칭 정보
{
	sGameRoom() :sFlag(flag = eMSG::em_ENTER) {};
	int userNum = 0; //방에 들어온 유저들을 구분하기 위한 것
};

struct Item
{
	int item[3];
};

struct sCharInfo :sFlag //캐릭터 정보
{
	sCharInfo() :sFlag(flag = eMSG::em_CHARINFO) {};
	int weapon; //무기
	int cloth; //방어구
	int gender, hair, hairColor, face; //캐릭터 외형 정보
	int item[3];
};

struct sMove :sFlag //움직임
{
	sMove() :sFlag(flag = eMSG::em_MOVE) {};
	float x, y, z; //위치 좌표
	float rotX, rotY, rotZ, rotW; //회전 좌표
};

struct sAtk :sFlag //공격
{
	sAtk() :sFlag(flag = eMSG::em_ATK) {};
	int atkNum;
};

struct sHit :sFlag //공격 성공
{
	sHit() :sFlag(flag = eMSG::em_HIT) {};
	int dmgAni = 0;
	int atkType = 0; //일반공격, 물건공격 구분
	int hp = 0;
};

struct sChangeInfo :sFlag //hp정보 전달
{
	sChangeInfo() :sFlag(flag = eMSG::em_INFO) {};
	int hp = 0;
};

struct sItemSpawn :sFlag //던질 아이템 생성 종류
{
	sItemSpawn() :sFlag(flag = eMSG::em_ITEMSPAWN) {};
	int itemKind[10];
};

struct sUseItem :sFlag //아이템 사용
{
	sUseItem() :sFlag(flag = eMSG::em_USEITEM) {};
	int itemNum = 0;
	int hp = 0;
	int speed = 0;
};

struct sEndItem :sFlag //아이템 효과 끝
{
	sEndItem() :sFlag(flag = eMSG::em_ENDITEM) {};
	int itemNum = 0;
	int hp = 0;
	int speed = 0;
};

struct sGetObj :sFlag //물건 줍기
{
	sGetObj() :sFlag(flag = eMSG::em_GETOBJ) {};
	int itemNum = 0;
};

struct sThrowObj :sFlag //물건 던지기
{
	sThrowObj() :sFlag(flag = eMSG::em_THROWOBJ) {};
	float throwPosX, throwPosY, throwPosZ = 0;
};

struct sReady :sFlag //두 유저 모두 준비
{
	sReady() :sFlag(flag = eMSG::em_READY) {};
};

struct sEnd : sFlag //항복 버튼 사용, 죽음
{
	sEnd() : sFlag(flag = eMSG::em_END) {};
	int result = 0;
	char enemyNick[30] = {NULL};
};

struct sChat : sFlag //채팅
{
	sChat() : sFlag(flag = eMSG::em_CHAT) {};
	char chat[50] = {NULL};
};

