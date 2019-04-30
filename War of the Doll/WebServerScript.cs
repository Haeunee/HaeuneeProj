using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.IO;
using System.Text;

public class WebServerScript : MonoBehaviour {
    public string nick;

    void Start () {
        DontDestroyOnLoad(this);
        Screen.SetResolution(Screen.width, (Screen.width / 16) * 9, false);
        SocketServer.SingleTonServ();      
    }

    private void Update()
    {//비동기 소켓서버 데이터 기다리기
        SocketServer.SingleTonServ().WaitRecieve();
    }

    public string ConnectServer(string Url,StringBuilder Info) //웹서버와 연결
    {
        StringBuilder sendInfo = Info;
        string url = "http://192.168.0.61:10000/" + Url;
        HttpWebRequest httpWebRequest = (HttpWebRequest)WebRequest.Create(url);
        byte[] byteArr = UTF8Encoding.UTF8.GetBytes(sendInfo.ToString());
        httpWebRequest.ContentType = "application/x-www-form-urlencoded";
        httpWebRequest.Method = "POST";
        httpWebRequest.ContentLength = byteArr.Length;

        Stream stream = httpWebRequest.GetRequestStream();
        if (stream == null)
            Debug.Log("stream is null");
        stream.Write(byteArr, 0, byteArr.Length);

        HttpWebResponse httpWebResponse = (HttpWebResponse)httpWebRequest.GetResponse();
        Stream result = httpWebResponse.GetResponseStream();
        StreamReader readerResult = new StreamReader(result, Encoding.Default);

        return readerResult.ReadToEnd();
    }
    
}
