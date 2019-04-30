using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Net;
using System.IO;
using System.Text;
using UnityEngine.SceneManagement;
using UnityEngine.EventSystems;

//웹서버 사용, 유저 로그인, 회원가입 등 
public class UserScript : MonoBehaviour
{
    public InputField nickInput; 
    public InputField passInput; 
    public GameObject MSGWin; //메세지 전달 창
    public GameObject SearchWin; //닉네임, 비밀번호 찾기 창 
    GameObject server; 
    WebServerScript web; 
    public string nick;
    bool loginResult = false;
    int loginSucc = 0;

    public GameObject signUpWin;
    public InputField signUpNick;
    public InputField signUpPass;
    public InputField signUpPhone;

    int tuto = -1;
    public GameObject tutorialWin; //튜토리얼 진행여부 묻는 화면 
    public GameObject YesBtn;
    public GameObject NoBtn;
    public GameObject block;

    private void Start()
    {
        SocketServer.SingleTonServ().GetUserScript(this);
        server = GameObject.Find("WebServer");
        web = server.GetComponent<WebServerScript>();
        passInput.contentType = InputField.ContentType.Password; //비밀번호 창에 별표 띄우기
        signUpPass.contentType = InputField.ContentType.Password;
    }

    private void Update()
    {
        if (loginResult) //중복체크 결과에 따른 반응 
        {
            loginResult = false;
            if (loginSucc == 0) //로그인 성공
            {
                web.nick = nick;
                GameObject.Find("GameMgr").gameObject.transform.GetChild(3).GetChild(5).gameObject.SetActive(true);

                if (tuto == 0)
                    tutorialWin.SetActive(true);
                if (tuto == 1)
                    loading.LoadScene("WaitScene");
            }
            else if (loginSucc == 1) //실패
            {
                string text = "이미 로그인 중입니다.";
                MSGWin.SetActive(true);
                MSGWin.GetComponent<PrintMSG>().print(text);
            }
        }

        if (tutorialWin.activeSelf == true) //튜토리얼 실행 여부 창 출력
        {
            if (EventSystem.current.currentSelectedGameObject == YesBtn)
            {
                block.SetActive(false);
                StringBuilder sendInfo = new StringBuilder();
                sendInfo.Append("nick=" + nick);
                sendInfo.Append("&tuto=" + tuto);
                web.ConnectServer("Tuto", sendInfo);
                loading.LoadScene("TutorialScene");
            }
            else if (EventSystem.current.currentSelectedGameObject == NoBtn)
            {
                block.SetActive(false);
                StringBuilder sendInfo = new StringBuilder();
                sendInfo.Append("nick=" + nick);
                sendInfo.Append("&tuto=" + tuto);
                web.ConnectServer("Tuto", sendInfo);
                loading.LoadScene("WaitScene");
            }
        }
    }

    public void SignUp() //회원가입
    {
        signUpWin.SetActive(true);
    }

    public void SignUpClick()
    { //회원가입 버튼 클릭
        string phoneNum = signUpPhone.text;
        int phoneLen = phoneNum.Length;
        StringBuilder sendInfo = new StringBuilder();
        sendInfo.Append("nick=" + signUpNick.text);
        sendInfo.Append("&password=" + signUpPass.text);
        sendInfo.Append("&phone=" + phoneNum);
        signUpNick.text = "";
        signUpPass.text = "";
        signUpPhone.text = "";
        string text = "";

        if (phoneLen<10)
        { //전화번호가 너무 짧을 때
            text = "올바른 전화번호를 입력해주세요.";
            MSGWin.SetActive(true);
            MSGWin.GetComponent<PrintMSG>().print(text);
        }
        else
        {
            for(int i=0;i<phoneLen; i++)
            { //전화번호가 숫자가 아닌 경우
                if(phoneNum[i] =='0'|| phoneNum[i] == '1'|| phoneNum[i] == '2'|| phoneNum[i] == '3'|| phoneNum[i] == '4'
                    || phoneNum[i] == '5'|| phoneNum[i] == '6'|| phoneNum[i] == '7'|| phoneNum[i] == '8' || phoneNum[i] == '9')
                    continue;
                else
                {
                    text = "올바른 전화번호를 입력해주세요.";
                    MSGWin.SetActive(true);
                    MSGWin.GetComponent<PrintMSG>().print(text);
                    return;
                }
            }

            string respJson = web.ConnectServer("SignUp", sendInfo);
            Debug.Log(respJson);
          
            if (respJson.Equals("succ"))
                text = "회원가입 되었습니다.";
            else if (respJson.Equals("overlap"))
                text = "이미 등록된 닉네임입니다.";
            else if (respJson.Equals("phone overlap"))
                text = "이미 입력된 전화번호입니다. ";
            else if (respJson.Equals("fail"))
                text = "회원정보를 다시 확인해주세요.";
            MSGWin.SetActive(true);
            MSGWin.GetComponent<PrintMSG>().print(text);
            signUpWin.SetActive(false);
        }
    }

    public void CloseBtn()
    {
        GameObject nowBtnObj = EventSystem.current.currentSelectedGameObject;
        nowBtnObj.transform.parent.gameObject.SetActive(false);
    }

    public void SearchWinOpen()
    {
        SearchWin.SetActive(true);
    }

    public void Login() //로그인
    {
        string respJson = ConnectServer("SignIn");
        Debug.Log(respJson);
        string[] loginResult = respJson.Split(',');
        if (loginResult[0].Equals("login")) //존재하는 아이디와 패스워드일 때
        {
            sLogin login = new sLogin(nick.ToCharArray(), 1);
            tuto = int.Parse(loginResult[1]);
            SocketServer.SingleTonServ().SendMsg(login); //소켓 서버에서 이미 로그인 된 아이디는 아닌지 확인
        }
        else if (respJson.Equals("fail"))
        {
            string text = "회원정보를 다시 확인해주세요.";
            MSGWin.SetActive(true);
            MSGWin.GetComponent<PrintMSG>().print(text);
        }
    }

    string ConnectServer(string Url) //서버에 정보 보내기
    {
        nick = nickInput.text;
        bool nickOk = nick.Contains(",") || nick.Contains("&") || nick.Contains("^") || nick.Contains(" ") || nick.Contains("!");
        string password = passInput.text;
        if (nick != "" && password != "" && nickOk == false)
        {
            nickInput.text = "";
            passInput.text = "";

            StringBuilder sendInfo = new StringBuilder();
            sendInfo.Append("nick=" + nick);
            sendInfo.Append("&password=" + password);

            return web.ConnectServer(Url, sendInfo);
        }
        else
            return "fail";
    }

    public void LoginResult(char[] loginNick, int succ) //소켓서버에서 로그인 중복체크 결과
    {
        nick = "";
        int i = 0;
        while (loginNick[i] != '\0')
        {
            nick += loginNick[i];
            i++;
        }
        loginSucc = succ; //중복이 아니라면 0 중복이면 1
        loginResult = true;
    }
}
