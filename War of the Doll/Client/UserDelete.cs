using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.IO;
using System.Text;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class UserDelete : MonoBehaviour
{
    WebServerScript web;
    GameObject webObj;
    public GameObject userDelWin;
    public InputField passInput;
    public GameObject MSGWin;
    bool delete = false;

    void Start()
    {
        webObj = GameObject.Find("WebServer");
        web = webObj.GetComponent<WebServerScript>();
        passInput.contentType = InputField.ContentType.Password;
    }
    
    void Update()
    {
        if(MSGWin.activeSelf)
        {
            if (delete)
            {
                loading.LoadScene("LoginScene");
                GameObject.Destroy(webObj);
                GameObject.Destroy(GameObject.Find("GameMgr2"));
            }
        }
    }

    public void UserDelBtn()
    {
        userDelWin.SetActive(true);
    }

    public void DelBtnClick()
    {
        string delPass = passInput.text;
        passInput.text = "";

        StringBuilder sendInfo = new StringBuilder();
        sendInfo.Append("nick=" + web.nick);
        sendInfo.Append("&password=" + delPass);

        string url = "UserDel";
        string respData = web.ConnectServer(url, sendInfo);
        string text = "";
        if (respData == "succ")
        {
            text = "탈퇴되었습니다.";
            delete = true;
        }
        else
        {
            text = "회원정보를 다시 확인해주세요.";
            delete = false;
        }
        MSGWin.SetActive(true);
        MSGWin.GetComponent<PrintMSG>().print(text);
        MSGWin.transform.GetChild(1).gameObject.SetActive(false);
    }

    public void CancelBtnClick()
    {
        passInput.text = "";
        userDelWin.SetActive(false);
    }
}
