using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System;
using System.Runtime.InteropServices;
using UnityEngine.SceneManagement;

//게임안에서 사용할 소켓서버

public class RecvData //받은 데이터와 소켓을 저장하는 클래스
{
    public byte[] buffer = new byte[100];
    public Socket socket;
}

public class SocketServer {
    private static SocketServer socketServer = null;

    public static SocketServer SingleTonServ()
    {
        if(socketServer==null)
        {
            socketServer = new SocketServer();
            socketServer.MakeServer();
        }
        return socketServer;
    }

    private static ManualResetEvent conn = new ManualResetEvent(false); //커넥트 스레드
    private static ManualResetEvent recv = new ManualResetEvent(false); //리시브 스레드
    private static Socket sock; //소켓
    static EnemyScript eScript;
    static PlayerScript pScript;
    static GameEnterScript gScript; 
    static SpawnScript sScript;
    static UserScript userScript;
    RecvData rd = new RecvData();
    static sGameRoom room;
    static int enterNum;
    static int gameResult;
    static char[] enemyNick;
    static FriendsScript fScript;
    IntPtr ptr;
    static ChatScript chatScript;

    private void MakeServer()
    {
        sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp); //소켓생성
        IPEndPoint ep = new IPEndPoint(IPAddress.Parse("192.168.0.61"), 10001); //연결할 서버 정보
        sock.Connect(ep); //커넥트
        rd.socket = sock;
    }

    private static void ReceiveCallBack(IAsyncResult ar) //서버에게 신호를 받았을 때
    {
        RecvData recvData = (RecvData)ar.AsyncState; //리시브된 데이터
        int receive = recvData.socket.EndReceive(ar); //리시브된 정보가 있는지 체크
        if (receive <= 0) //서버 종료
        {
            recvData.socket.Close(); //소켓을 닫음
            Application.Quit();
            return;
        }
        IntPtr buff = Marshal.AllocHGlobal(recvData.buffer.Length); //받은 byte 데이터를 struct로 변환
        Marshal.Copy(recvData.buffer, 0, buff, recvData.buffer.Length);
        Type type = typeof(sGameRoom);
        room = (sGameRoom)Marshal.PtrToStructure(buff, type);

        if(room.flag==(int)eMSG.em_LOGIN) //로그인
        {
            Type m_type = typeof(sLogin);
            sLogin loginInfo = (sLogin)Marshal.PtrToStructure(buff, m_type);
            userScript.LoginResult(loginInfo.nick, loginInfo.loginSucc);
        }
        else if(room.flag==(int)eMSG.em_LOGINCHECK) //친구 로그인 체크
        {
            Type m_type = typeof(sLoginCheck);
            sLoginCheck loginChk = (sLoginCheck)Marshal.PtrToStructure(buff, m_type);
            fScript.FriendAccCheck(loginChk.nick, loginChk.loginChk);
        }
        else if (room.flag == (int)eMSG.em_MATCHREQUEST) //친선매칭 신청
        {
            Type m_type = typeof(sMatchReq);
            sMatchReq matchReq = (sMatchReq)Marshal.PtrToStructure(buff, m_type);
            fScript.MatchReqResult(matchReq.sendUserNick, matchReq.matchSucc);
        }
        else if (room.flag == (int)eMSG.em_ENTER) //매칭 버튼을 눌렀다는 정보
        {
            if(room.userNum==0) //상대가 없음
                gScript.Matching();
            else //상대가 접속
            {
                enterNum = room.userNum;
                gScript.MatchSucc();
            }
        }
        else if(room.flag==(int)eMSG.em_CHARINFO) //아이템 선택이 끝났다는 정보
        {
            Type m_type = typeof(sCharInfo);
            sCharInfo charInfo = (sCharInfo)Marshal.PtrToStructure(buff, m_type);
            if (enterNum == 1) //내가 먼저 입장
                sScript.SpawnInfo("Spawn1", "Spawn2", charInfo);
            else if (enterNum == 2) //상대가 먼저 입장
                sScript.SpawnInfo("Spawn2", "Spawn1", charInfo);
        }
        else if (room.flag == (int)eMSG.em_READY) //두 유저 모두 준비되었다는 정보
        {
            sScript.SpawnReady();
        }
        else if (eScript != null && room.flag == (int)eMSG.em_MOVE) //적의 좌표, 회전 정보
        {
            Type m_type = typeof(sMove);
            sMove move = (sMove)Marshal.PtrToStructure(buff, m_type);
            Vector3 enemyPos = new Vector3(move.x, move.y, move.z);
            Quaternion enemyRot = new Quaternion(move.rotX, move.rotY, move.rotZ, move.rotW);
            eScript.EnemyMove(enemyPos, enemyRot);
        }
        else if(room.flag==(int)eMSG.em_ATK) //적이 공격했다는 정보
        {
            Type m_type = typeof(sAtk);
            sAtk atk = (sAtk)Marshal.PtrToStructure(buff, m_type);
            eScript.EnemyAtk(atk.atkAni);
        }
        else if(room.flag==(int)eMSG.em_HIT) //내가 공격을 성공함
        {
            Type m_type = typeof(sHit);
            sHit hit = (sHit)Marshal.PtrToStructure(buff, m_type);
            pScript.ChangePlayerHp(hit.hp);
            pScript.PlayerDamage(hit);
        }
        else if (room.flag == (int)eMSG.em_INFO) //적의 hp정보가 변한경우
        {
            Type m_type = typeof(sChangeInfo);
            sChangeInfo hpInfo = (sChangeInfo)Marshal.PtrToStructure(buff, m_type);
            eScript.ChangeEnemyHp(hpInfo.hp);
        }
        else if(room.flag == (int)eMSG.em_ITEMSPAWN) //던지기 아이템 두 유저 동일한 위치로 화면에 출력
        {
            Type m_type = typeof(sItemSpawn);
            sItemSpawn itemSpawn = (sItemSpawn)Marshal.PtrToStructure(buff, m_type);
            pScript.passOnItemSpawnInfo(itemSpawn.itemKind);
            pScript.GetAtkMgrFromServer(true);
        }
        else if (room.flag == (int)eMSG.em_USEITEM) //아이템 사용
        {
            Type m_type = typeof(sUseItem);
            sUseItem useItem = (sUseItem)Marshal.PtrToStructure(buff, m_type);
            pScript.ChangeItemImg(useItem.itemNum, true);
            pScript.ChangePlayerHp(useItem.hp);
            pScript.ChangePlayerSpeed(useItem.speed);
        }
        else if (room.flag == (int)eMSG.em_ENDITEM) //아이템 시간 끝
        {
            Type m_type = typeof(sEndItem);
            sEndItem endItem = (sEndItem)Marshal.PtrToStructure(buff, m_type);
            pScript.ChangeItemImg(endItem.itemNum, false);
            pScript.ChangePlayerSpeed(endItem.speed);
        }
        else if (room.flag == (int)eMSG.em_GETOBJ) //던질 물건 잡기
        {
            Type m_type = typeof(sGetObj);
            sGetObj getObj = (sGetObj)Marshal.PtrToStructure(buff, m_type);
            Debug.Log("from server get succ");
            eScript.GetThrowObj(getObj.itemNum);
        }
        else if (room.flag == (int)eMSG.em_THROWOBJ) //물건 던지기
        {
            eScript.ThrowObj();
            Debug.Log("from server throw succ");
        }
        else if (room.flag == (int)eMSG.em_END) //게임이 종료되었을 경우
        {
            Type m_type = typeof(sEnd);
            sEnd esc = (sEnd)Marshal.PtrToStructure(buff, m_type);
            gameResult = esc.result;
            enemyNick = esc.enemyNick;
            if(pScript==null) //player가 강종했을 경우 
                sScript.GameEndTrue();
            else //상대편(enemy)가 강종했을 경우 
                pScript.GameEndTrue();
        }
        else if (room.flag == (int)eMSG.em_CHAT) //채팅
        {
            Type m_type = typeof(sChat);
            sChat chat = (sChat)Marshal.PtrToStructure(buff, m_type);
            string nowChat = new string(chat.chat);
            chatScript.ChatRecv(nowChat);
        }
    }

    public void GetEnterScript(GameEnterScript enter)
    {
        gScript = enter;
    }

    public void GetSpawnScript(SpawnScript spawn)
    {
        sScript = spawn;
    }

    public void GetFriendScript(FriendsScript friends)
    {
        fScript = friends;
    }

    public void GetCharScripts(PlayerScript player, EnemyScript enemy)
    {
        eScript = enemy;
        pScript = player;
    }

    public PlayerScript NowPlayerScript()
    {
        return pScript;
    }

    public EnemyScript NowEnemyScript()
    {
        return eScript;
    }

    public void GetUserScript(UserScript user)
    {
        userScript = user;
    }

    public void GetChatScript(ChatScript chat)
    {
        chatScript = chat;
    }

    public void WaitRecieve()
    {
        sock.BeginReceive(rd.buffer, 0, rd.buffer.Length, SocketFlags.None, new AsyncCallback(ReceiveCallBack), rd);
        //리시브 기다리기
        recv.Set();
    }

    public void SendMsg(object obj)//메세지 보내기 함수
    {
        int byteSize = Marshal.SizeOf(obj); //구조체를 바이트로 변환
        byte[] byteData = new byte[byteSize];
        ptr = Marshal.AllocHGlobal(byteSize);
        Marshal.Copy(byteData,0, ptr, byteSize);
        Marshal.StructureToPtr(obj, ptr, false);
        Marshal.Copy(ptr, byteData, 0, byteSize);
        Marshal.FreeHGlobal(ptr);

        sock.Send(byteData);
    }

    public int GetResult()
    {
        return gameResult;
    }

    public string GetEnemyName() //EndScene에서 상대편 닉네임 출력
    {
        return string.Concat(enemyNick);
    }
}
