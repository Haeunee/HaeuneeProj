using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.IO;
using System.Text;
using UnityEngine.UI;

public class RankingScript : MonoBehaviour
{
    string nick; //유저 닉네임
    GameObject webServ;
    WebServerScript webScript;
    List<GameObject> tmpObj;
    public GameObject rankUser;
    public GameObject rankScroll;
    public GameObject userInfo;
    public GameObject rankingWin;

    private void Start()
    {
        tmpObj = new List<GameObject>();
        webServ = GameObject.Find("WebServer");
        webScript = webServ.GetComponent<WebServerScript>();
        nick = webScript.nick;       
    }

    public void RankBtnClick()
    {
        StringBuilder sendInfo = new StringBuilder();
        sendInfo.Append("flag=reqlist");
        sendInfo.Append("&nick=" + nick);
        string url = "Ranking";
        string respData = webScript.ConnectServer(url, sendInfo);//랭킹 순서대로 서버에서 받아오기
        int y = -25;
        string[] rankList = respData.Split(',');//받아온 스트링 ,기준으로 나누기
        string[] nickList = new string[100];
        string[] scoreList = new string[100];
        int rankLen = rankList.Length;
        string userScore = rankList[0];//리스트 맨처음 나의 점수 가져오기
        int nickNum = 0;
        int scoreNum = 0;
        for (int i=1;i< rankLen;i++) //점수, 닉네임 나눠 리스트에 저장하기
        {
            if(i%2 == 0)
            {
                scoreList[scoreNum] = rankList[i];
                scoreNum++;
            }
            else
            {
                nickList[nickNum] = rankList[i];
                nickNum++;
            }
        }

        int userRank = 0;
        for (int i = 0; i < rankLen/2; i++) //순서대로 출력하기
        {
            if (nickList[i] == nick)
                userRank = i + 1;
            Vector3 chatPos = new Vector3(5, y, 0);
            Quaternion q = new Quaternion(0, 0, 0, 0);
            Text rankText = rankUser.GetComponentInChildren<Text>();
            rankText.text = (i+1) + "          " + nickList[i]+"                    "+scoreList[i];
            GameObject tmp = Instantiate(rankUser, Vector3.zero, q, rankScroll.transform);
            tmpObj.Add(tmp);
            tmp.transform.localPosition = chatPos;
            y -= 20;
        }
        if(userRank==0) //유저 정보 출력하기
            userInfo.GetComponentInChildren<Text>().text = "내 정보\n닉네임 : " + nick + "          순위 : 100위권 밖          점수 : " + userScore;
        else
            userInfo.GetComponentInChildren<Text>().text = "내 정보\n닉네임 : " + nick + "          순위 : " +userRank+ "          점수 : " + userScore;
        rankingWin.SetActive(true);
    }

    public void CloseBtnClick()
    { //랭킹창 닫기
        rankingWin.SetActive(false);
        for (int i = 0; i < tmpObj.Count; i++)
        {
            GameObject.Destroy(tmpObj[i]);
        }
        tmpObj.Clear();
    }

}
