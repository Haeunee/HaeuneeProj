using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

//적, 캐릭터를 스폰시키는 스크립트
public class SpawnScript : MonoBehaviour {
    public GameObject player;
    public GameObject enemy;
    string playerPname;
    string enemyPname;
    bool spawnPoss = false;
    public GameObject nowEnemy;
    public sCharInfo enemyInfo;
    GameObject nowPlayer;
    GameEnterScript enter;
    bool gameEnd = false;

    void Start () {
        enter = GetComponent<GameEnterScript>();
        SocketServer.SingleTonServ().GetSpawnScript(this);
    }

    private void Update()
    {
        if(spawnPoss==true) //스폰될 정보를 받았을 때
        {
            StartCoroutine(ItemDelay()); //두 유저가 모두 정보를 주고 받을 때까지 기다리는 코루틴
            spawnPoss = false;
        }
        if (gameEnd == true) //상대 유저의 접속 종료
        {
            GameObject enemyOutWin = GameObject.Find("Canvas").transform.Find("EnemyOut").gameObject;
            enemyOutWin.SetActive(true);
            changeScene();
        }
    }

    void Spawn()
    {//적과 내 캐릭터 스폰시키기
        Quaternion q = new Quaternion(0, 0, 0, 0);
        GameObject playerP = GameObject.Find(playerPname);
        GameObject enemyP = GameObject.Find(enemyPname);

        nowPlayer = Instantiate(player, playerP.transform.position, q, playerP.transform);
        nowEnemy = Instantiate(enemy, enemyP.transform.position, q, enemyP.transform);
        
        Custom(nowPlayer, enter.savCharInfo);
        Custom(nowEnemy, enemyInfo);

        Camera playerCam = nowPlayer.GetComponentInChildren<Camera>();
        nowEnemy.GetComponentInChildren<Canvas>().worldCamera = playerCam; //enemyHp바를 내 캐릭터 카메라 화면에 출력 
        playerCam.enabled = true;
        GameObject img = GameObject.Find("Canvas").transform.GetChild(2).gameObject;
        img.SetActive(false);
        SocketServer.SingleTonServ().SendMsg(new sReady((int)eMSG.em_READY));
        SocketServer.SingleTonServ().GetCharScripts(nowPlayer.GetComponent<PlayerScript>(), nowEnemy.GetComponent<EnemyScript>());
    }

    public void SpawnInfo(string playerParent, string enemyParent, sCharInfo charInfo)
    { //스폰될 정보를 받아오는 함수
        playerPname = playerParent;
        enemyPname = enemyParent;
        enemyInfo = charInfo;
    }

    public void SpawnReady()
    {
        spawnPoss = true;
    }

    void Custom(GameObject nowObj, sCharInfo charInfo) //정보에 따라 캐릭터를 커스텀하는 함수
    {
        HeroCustomize custom = nowObj.GetComponent<HeroCustomize>();
        AnimationController aniMgr = nowObj.GetComponent<AnimationController>();
        string genderStr = "";
        if (charInfo.gender == 1)
            genderStr = "Male";
        else if (charInfo.gender == 2)
            genderStr = "Female";
        custom.Gender = genderStr;
        custom.IndexWeapon.CurrentType = charInfo.weapon;
        custom.IndexWeapon.CurrentIndex = 5;
        custom.IndexSuit.CurrentIndex = charInfo.armor;
        custom.IndexHair.CurrentIndex = charInfo.hair;
        custom.IndexColorHair.CurrentIndex = charInfo.hairColor;
        custom.IndexFace.CurrentIndex = charInfo.face;
        custom.UpdateVisual();
        custom.UpdateWeapon();
        aniMgr.weaponIndex = charInfo.weapon; //무기에 따른 애니메이션도 설정
    }
    

    public void GameEndTrue()
    {
        gameEnd = true;
    }

    void changeScene() // Scene 전환 
    {
        loading.LoadScene("WaitScene");
        gameEnd = false;
    }

    IEnumerator ItemDelay() 
    {
        yield return new WaitForSeconds(1.0f);
        BgmController sound = GameObject.Find("GameMgr").GetComponent<BgmController>();
        sound.ChangeBgm();
        loading.LoadScene("GameScene");
        while (true)
        {
            yield return new WaitForSeconds(0.2f);
            if(SceneManager.GetActiveScene().name == "GameScene")
            {
                Spawn();
                break;
            }
        }
    }
}
