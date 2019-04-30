using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System.Net;
using System.IO;
using System.Text;
using System;
using UnityEngine.UI;

public class EndSceneScript : MonoBehaviour
{
    int result;
    public Text winner;
    public Text loser;
    public Text winningRate;
    WebServerScript server;
    string winning;
    GameObject enemy;
    GameObject player;
    AnimationController playerAni;
    AnimationController enemyAni;

    private void Awake()
    {
        if(GameObject.Find("GameMgr/MSGWin") != null && GameObject.Find("GameMgr/MSGWin").activeSelf)
            GameObject.Find("GameMgr/MSGWin").SetActive(false);
    }

    void Start()
    { //플레이어 캐릭터와 적 캐릭터만 나오게 하기
        PlayerScript playerScript = SocketServer.SingleTonServ().NowPlayerScript();
        player = playerScript.transform.gameObject;
        player.transform.rotation = Quaternion.Euler(0, 180, 0);
        playerAni = player.GetComponent<AnimationController>();
        playerScript.enabled = false;
        playerAni.PlayAtkDmg("Idle");
        player.GetComponentInChildren<Camera>().enabled = false;
        Camera.main.enabled = true;

        EnemyScript enemyScript = SocketServer.SingleTonServ().NowEnemyScript();
        enemy = enemyScript.transform.gameObject;
        enemy.transform.rotation = Quaternion.Euler(0, 180, 0);
        enemyAni = enemy.GetComponent<AnimationController>();
        enemyScript.enabled = false;
        enemy.transform.Find("Canvas").gameObject.SetActive(false);
        enemy.GetComponent<Rigidbody>().isKinematic = false;
        enemyAni.PlayAtkDmg("Idle");

        //승률 표시
        server = GameObject.Find("WebServer").GetComponent<WebServerScript>();
        GameObject.Destroy(GameObject.Find("itemBtnCanvas"));
        result = SocketServer.SingleTonServ().GetResult();

        string userNick = server.nick;
        string enemyNick = SocketServer.SingleTonServ().GetEnemyName();

        //결과에 따라 맞는 위치에 배치하기
        if (result == (int)eRESULT.em_WIN)
        {
            CharSetting(player, enemy, userNick, enemyNick, "win");
            winningRate.rectTransform.anchoredPosition = new Vector2(winner.rectTransform.anchoredPosition.x, winningRate.rectTransform.anchoredPosition.y);
        }
        else if (result == (int)eRESULT.em_LOSE)
        {
            CharSetting(enemy, player, enemyNick, userNick, "lose");
            winningRate.rectTransform.anchoredPosition = new Vector2(loser.rectTransform.anchoredPosition.x, winningRate.rectTransform.anchoredPosition.y);
        }
        
    }

    string ResultSave(string result) //웹서버와 연결, 승률 계산하여 가져오기
    {
        string Url = "BattleEnd";
        StringBuilder sendInfo = new StringBuilder();
        sendInfo.Append("flag=" + result);
        sendInfo.Append("&nick=" + server.nick);
        return server.ConnectServer(Url, sendInfo);
    }    

    IEnumerator DeathAni(int res) //진 캐릭터 죽는 애니메이션 재생하기
    {
        yield return new WaitForSeconds(1.0f);
        if(enemyAni != null && playerAni != null)
        {
            if (result == (int)eRESULT.em_WIN)
                enemyAni.PlayDeath("Death");
            else if (result == (int)eRESULT.em_LOSE)
                playerAni.PlayDeath("Death");
        }
    }

    void CharSetting(GameObject winnerChar, GameObject loserChar, string winNick, string loseNick, string resText)
    { //캐릭터 배치, 승률 표시
        winnerChar.transform.position = new Vector3(-3.5f, 0, 13);
        loserChar.transform.position = new Vector3(4, 0, 13);
        StartCoroutine(DeathAni(result));
        winner.text = winNick;
        loser.text = loseNick;
        winning = ResultSave(resText);
        int win = Mathf.FloorToInt(float.Parse(winning));
        winningRate.text = "승률 : " + win;
    }
}
