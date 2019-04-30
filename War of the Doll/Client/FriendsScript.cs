using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.IO;
using System.Text;
using UnityEngine.UI;
using UnityEngine.EventSystems;

struct sFriendEnter
{
    public string friendName;
    public int enter;
}

public class FriendsScript : MonoBehaviour {
    string nick; //유저 닉네임
    GameObject webServ;
    WebServerScript webScript; 
    public GameObject friendBtnObj;
    public GameObject friendScroll;
    Button friendBtn; //등록된 친구 선택 버튼
    Text friendText; 
    bool friendListOn = false;
    GameObject nowBtnObj;
    GameObject MSGWin;
    public GameObject friendWin;
    public GameObject friendListWin;
    public GameObject searchWin;
    public InputField searchNickInput;
    public GameObject matchReqWin;
    public GameObject requestListWin;
    public GameObject reqScroll;
    public GameObject existImg;
    public GameObject matchingImg;
    public GameObject randBtn;
    public GameObject friendMatchBtn;
    List<GameObject> tmpObj;
    float reqExist = 10;
    string[] savReqList;
    string btnText;
    string url;
    string chkFriendNick;
    string matchReqFriendNick;
    bool matchReqRecv = false;
    int matchSucc;
    bool winActive = false; //friendWin창 활성화 여부 

    string requestName;

    List<sFriendEnter> friendEntList;
    void Start () {
        url = "Friends"; 
        tmpObj = new List<GameObject>();
        webServ = GameObject.Find("WebServer");
        webScript = webServ.GetComponent<WebServerScript>();
        nick = webScript.nick;
        friendBtn = friendBtnObj.GetComponent<Button>();
        friendText = friendBtn.GetComponentInChildren<Text>();

        StringBuilder sendInfo = new StringBuilder();
        sendInfo.Append("flag=reqlist");
        sendInfo.Append("&nick=" + nick);
        string respData = webScript.ConnectServer(url, sendInfo);

        savReqList = respData.Split(',');

        if (respData != "no request") //친구 요청이 있을 경우 표시
        {
            existImg.SetActive(true);
        }
        friendEntList = new List<sFriendEnter>();
        SocketServer.SingleTonServ().GetFriendScript(this);
    }
	
	void Update () {
        if (MSGWin == null && GameObject.Find("GameMgr") != null)
            MSGWin = GameObject.Find("GameMgr").transform.GetChild(4).gameObject;

        if (friendWin.activeSelf && !winActive) //friendWin창이 활성화되어 있는 경우 GameMgr의 Layer 앞으로 보내기 
        {
            winActive = true;
            GameObject gameMgr = GameObject.Find("GameMgr");
            gameMgr.GetComponent<Canvas>().sortingOrder = 3;
            gameMgr.transform.GetChild(0).gameObject.SetActive(false);
            gameMgr.transform.GetChild(1).gameObject.SetActive(false);
        }
        else if(!friendWin.activeSelf && winActive)
        {
            winActive = false;
            GameObject gameMgr = GameObject.Find("GameMgr");
            gameMgr.GetComponent<Canvas>().sortingOrder = 2;
            gameMgr.transform.GetChild(0).gameObject.SetActive(true);
            gameMgr.transform.GetChild(1).gameObject.SetActive(true);
        }

        reqExist += Time.deltaTime;
        if(friendListOn == false && reqExist >= 10) //10초마다 새로운 친구요청이 있는지 확인
        {
            reqExist = 0;
            StringBuilder sendInfo = new StringBuilder();
            sendInfo.Append("flag=reqlist");
            sendInfo.Append("&nick=" + nick);
            string respData = webScript.ConnectServer(url, sendInfo);

            if (respData != "no request")
            {
                string[] nowReqList = respData.Split(',');

                int reqLen = nowReqList.Length;
                int savLen = savReqList.Length;
                if(reqLen==savLen)
                {
                    for (int i = 0; i < reqLen; i++)
                    {
                        if (nowReqList[i] != savReqList[i])
                            existImg.SetActive(true);
                    }
                }
                else
                    existImg.SetActive(true);
            }
        }

        if(matchReqRecv==true)
        { 
            matchReqRecv = false;
            if (matchSucc == 1)
            { //친구에게 신청한 매칭 거절
                string text = "대전신청이 거절되었습니다.";
                MSGWin.SetActive(true);
                MSGWin.GetComponent<PrintMSG>().print(text);
                randBtn.SetActive(true);
                friendMatchBtn.SetActive(true);
                matchingImg.SetActive(false);
            }
            else if (matchSucc == 2)
            { //매칭 신청 도착
                matchReqWin.GetComponentInChildren<Text>().text = "'" + matchReqFriendNick + "'님의 대전신청을 수락하시겠습니까?";
                matchReqWin.SetActive(true);
            }
            else if (matchSucc == 3)
            { //이미 매칭중인 유저
                randBtn.SetActive(true);
                friendMatchBtn.SetActive(true);
                matchingImg.SetActive(false);
                string text = "상대 유저가 이미 매칭중입니다.";
                MSGWin.SetActive(true);
                MSGWin.GetComponent<PrintMSG>().print(text);
            }
        }
    }

    public void FriendBtnClick() //소셜창 열기 
    {
        existImg.SetActive(false);
        if (friendListOn == false)
        {
            friendWin.SetActive(true);
            friendListOn = true;
            FriendList();
        }
    }

    public void FriendList()//친구 리스트 불러오기
    {
        if(tmpObj != null)
        {
            int objLen = tmpObj.Count;
            for (int i = 0; i < objLen; i++)
            {
                GameObject.Destroy(tmpObj[i]);
            }
            tmpObj.Clear();
        }
        int y = -25;

        StringBuilder sendInfo = new StringBuilder();
        sendInfo.Append("flag=list");
        sendInfo.Append("&nick=" + nick);

        string respData = webScript.ConnectServer(url, sendInfo);
        string[] friendList = respData.Split(',');

        int friendLen = friendList.Length;
        for (int i = 0; i < friendLen; i++)
        {
            Vector3 chatPos = new Vector3(5, y, 0);
            Quaternion q = new Quaternion(0, 0, 0, 0);
            friendText.text = friendList[i];
            GameObject tmp = Instantiate(friendBtnObj, Vector3.zero, q, friendScroll.transform);
            tmpObj.Add(tmp);
            tmp.transform.localPosition = chatPos;
            Button tmpBtn = tmp.GetComponent<Button>();
            tmpBtn.onClick.AddListener(FriendNameClick);
            y -= 20;
        }
        
        for (int i = 0; i < friendLen; i++)
        {
            string sendNick = friendList[i];
            sLoginCheck loginChk = new sLoginCheck(sendNick.ToCharArray(), 0);
            SocketServer.SingleTonServ().SendMsg(loginChk);
        }

        StartCoroutine(FriendEntCheckPoint());

        friendEntList.Clear();
        searchWin.SetActive(false);
        friendListWin.SetActive(true);
        requestListWin.SetActive(false);
    }

    public void FriendDelete() //친구 삭제
    {
        if(btnText!="")
        {
            StringBuilder sendInfo = new StringBuilder();
            sendInfo.Append("flag=del");
            sendInfo.Append("&nick=" + nick);
            sendInfo.Append("&friend=" + btnText);
            btnText = "";

            string respData = webScript.ConnectServer(url, sendInfo);
            if (nowBtnObj != null && respData == "succ")
            {
                string text = "친구가 삭제되었습니다.";
                MSGWin.SetActive(true);
                MSGWin.GetComponent<PrintMSG>().print(text);
                MSGWin.transform.GetChild(1).gameObject.SetActive(false);
                nowBtnObj.SetActive(false);
                nowBtnObj = null;
            }
        }
    }

    public void FriendNameClick() //친구 이름 클릭
    {
        nowBtnObj = EventSystem.current.currentSelectedGameObject;
        btnText = nowBtnObj.GetComponentInChildren<Text>().text.ToString();
        if (btnText == "no friend"||btnText=="no request")
        {
            nowBtnObj = null;
            btnText = "";
        }
        else
        {
            int btnArrSize = tmpObj.Count;
            for (int i = 0; i < btnArrSize; i++)
                tmpObj[i].GetComponent<Image>().color = Color.clear;
            nowBtnObj.GetComponent<Image>().color = Color.gray;
        }
    }

    public void SearchBtnClick() //친구 찾기 버튼 클릭
    {
        string searchName = searchNickInput.text.ToString();
        searchNickInput.text = "";
        StringBuilder sendInfo = new StringBuilder();
        sendInfo.Append("flag=search");
        sendInfo.Append("&nick=" + nick);
        sendInfo.Append("&search=" + searchName);
        string respData = webScript.ConnectServer(url, sendInfo);
        string text = "";

        if(respData == "fail")
            text = "존재하지 않는 유저입니다.";
        else
        {
            text = "이름 : " + searchName + "\n점수 : " + respData;
            requestName = searchName;
        }
        MSGWin.SetActive(true);
        MSGWin.GetComponent<PrintMSG>().print(text);
        MSGWin.transform.GetChild(1).gameObject.SetActive(false);
    }

    public void FriendSearch() //친구 신청하는 창 켜기
    {
        searchWin.SetActive(true);
        friendListWin.SetActive(false);
        requestListWin.SetActive(false);
        if (tmpObj != null)
        {
            int objLen = tmpObj.Count;
            for (int i = 0; i < objLen; i++)
            {
                GameObject.Destroy(tmpObj[i]);
            }
            tmpObj.Clear();
        }
    }

    public void FriendRequest() //친구 요청 창 켜기
    {
        if (tmpObj != null)
        {
            int objLen = tmpObj.Count;
            for (int i = 0; i < objLen; i++)
            {
                GameObject.Destroy(tmpObj[i]);
            }
            tmpObj.Clear();
        }
        int y = -25;
        StringBuilder sendInfo = new StringBuilder();
        sendInfo.Append("flag=reqlist");
        sendInfo.Append("&nick=" + nick);
        string respData = webScript.ConnectServer(url, sendInfo);

        string[] reqList = respData.Split(',');
        savReqList = reqList;

        int reqLen = reqList.Length;
        for (int i = 0; i < reqLen; i++)
        {
            Vector3 chatPos = new Vector3(5, y, 0);
            Quaternion q = new Quaternion(0, 0, 0, 0);
            friendText.text = reqList[i];
            GameObject tmp = Instantiate(friendBtnObj, Vector3.zero, q, reqScroll.transform);
            tmpObj.Add(tmp);
            tmp.transform.localPosition = chatPos;
            Button tmpBtn = tmp.GetComponent<Button>();
            tmpBtn.onClick.AddListener(FriendNameClick);
            y -= 20;
            Debug.Log(reqList[i]);
        }
        
        requestListWin.SetActive(true);
        searchWin.SetActive(false);
        friendListWin.SetActive(false);
    }

    public void FriendAccCheck(char[] friendNick, int chk)
    { //접속한 친구 체크
        sFriendEnter friendEnter = new sFriendEnter();
        friendEnter.friendName = "";
        int i = 0;
        while (friendNick[i] != '\0')
        {
            friendEnter.friendName += friendNick[i];
            i++;
        }
        friendEnter.enter = chk;
        friendEntList.Add(friendEnter);
    }

    IEnumerator FriendEntCheckPoint()
    { //접속한 친구 표시하기
        yield return new WaitForSeconds(0.5f);
        int friendLen = friendEntList.Count;
        for (int i = 0; i < friendLen; i++)
        {
            sFriendEnter friendEnter = friendEntList[i];
            int len = tmpObj.Count;
            for (int j = 0; j < len; j++)
            {
                string name = tmpObj[j].GetComponentInChildren<Text>().text.ToString();
                if (name.Equals(friendEnter.friendName))
                {
                    if (friendEnter.enter == 0)
                        tmpObj[j].transform.Find("FriendAccPoint").gameObject.SetActive(true);
                    break;
                }
            }
        }
    }

    public void RequestBtnClick() //친구 신청하기 버튼 클릭, 신청 전송
    {
        if(requestName!="")
        {
            StringBuilder sendInfo = new StringBuilder();
            sendInfo.Append("flag=request");
            sendInfo.Append("&nick=" + nick);
            sendInfo.Append("&request=" + requestName);
            requestName = "";
            string respData = webScript.ConnectServer(url, sendInfo);
            string text = "" ;
            if (respData == "already")
                text = "이미 등록된 친구입니다.";
            else if (respData == "succ")
                text = "친구신청되었습니다.";

            MSGWin.SetActive(true);
            MSGWin.GetComponent<PrintMSG>().print(text);
            MSGWin.transform.GetChild(1).gameObject.SetActive(false);
        }
    }

    public void AcceptBtnClick() //친구 신청 수락
    {
        if (btnText != "")
        {
            StringBuilder sendInfo = new StringBuilder();
            sendInfo.Append("flag=accept");
            sendInfo.Append("&nick=" + nick);
            sendInfo.Append("&accept=" + btnText);
            btnText = "";
            string respData = webScript.ConnectServer(url, sendInfo);

            if (nowBtnObj != null && respData == "succ")
            {
                int savLen = savReqList.Length;
                for (int i = 0; i < savLen; i++)
                {
                    if (savReqList[i] == btnText)
                    {
                        while (i + 1 < savLen)
                        {
                            savReqList[i] = savReqList[i + 1];
                            i++;
                        }
                    }
                }
                string text = "친구신청이 완료되었습니다.";
                MSGWin.SetActive(true);
                MSGWin.GetComponent<PrintMSG>().print(text);
                MSGWin.transform.GetChild(1).gameObject.SetActive(false);
                nowBtnObj.SetActive(false);
                nowBtnObj = null;
            }
        }
    }

    public void RefuseBtnClick() //친구 신청 거절
    {
        if (btnText != "")
        {
            StringBuilder sendInfo = new StringBuilder();
            sendInfo.Append("flag=refuse");
            sendInfo.Append("&nick=" + nick);
            sendInfo.Append("&refuse=" + btnText);
            btnText = "";
            string respData = webScript.ConnectServer(url, sendInfo);

            if (nowBtnObj != null && respData == "succ")
            {
                int savLen = savReqList.Length;
                for (int i = 0; i < savLen; i++)
                {
                    if (savReqList[i] == btnText)
                    {
                        while (i + 1 < savLen)
                        {
                            savReqList[i] = savReqList[i + 1];
                            i++;
                        }
                    }
                }
                string text = "친구신청이 거절되었습니다.";
                MSGWin.SetActive(true);
                MSGWin.GetComponent<PrintMSG>().print(text);
                nowBtnObj.SetActive(false);
                nowBtnObj = null;
            }
        }
    }

    public void FriendMatchReq() //친선매칭 신청
    {
        if (btnText != "")
        {
            randBtn.SetActive(false);
            friendMatchBtn.SetActive(false);
            matchingImg.SetActive(true);
            sMatchReq matchReq = new sMatchReq(nick.ToCharArray(),btnText.ToCharArray(), 2);
            SocketServer.SingleTonServ().SendMsg(matchReq);
        }
    }

    public void MatchReqResult(char[] enemyNick, int succ) //친선매칭 신청 결과
    {
        matchReqFriendNick = "";
        int i = 0;
        while (enemyNick[i] != '\0')
        {
            matchReqFriendNick += enemyNick[i];
            i++;
        }
        matchSucc = succ;
        matchReqRecv = true;
    }

    public void MatchAcceptBtnClick() //친선매칭 수락
    {
        sMatchReq matchReq = new sMatchReq(nick.ToCharArray(), matchReqFriendNick.ToCharArray(), 0);
        SocketServer.SingleTonServ().SendMsg(matchReq);
    }

    public void MatchRefuseBtnClick() //친선매칭 거절
    {
        sMatchReq matchReq = new sMatchReq(nick.ToCharArray(), matchReqFriendNick.ToCharArray(), 1);
        SocketServer.SingleTonServ().SendMsg(matchReq);
        matchReqWin.SetActive(false);
        string text = "매칭이 취소되었습니다.";
        MSGWin.SetActive(true);
        MSGWin.GetComponent<PrintMSG>().print(text);
    }

    public void CloseBtnClick() //소셜창 닫기
    {
        friendWin.SetActive(false);
        friendListOn = false;
    }

}
