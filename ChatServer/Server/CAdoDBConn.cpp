#include "pch.h"
#include "CAdoDBConn.h"

CAdoDBConn::CAdoDBConn()
{
    CoInitialize(0);
    InitDB("ChatTest", "chat1q2w1", "10.10.10.52", "Chat_DB", "10001");
}


CAdoDBConn::~CAdoDBConn()
{
    m_ConnPtr->Close();
    CoUninitialize();
}

BOOL CAdoDBConn::InitDB(string _id, string _pass, string _DBIp, string _dbName, string _DBPort)
{
    string connStr{ "Provider=SQLOLEDB.1;Data Source=" + _DBIp + "," + _DBPort +";Network Library=DBMSSOCN"
        + ";Initial Catalog=" + _dbName + ";User ID=" + _id +  ";Password=" + _pass };
    
    const char* conn{ connStr.c_str() };

    m_ConnPtr.CreateInstance(__uuidof(Connection));


    m_ConnPtr->ConnectionString = conn;
    try
    {
        m_ConnPtr->Open("", "", "", adConnectUnspecified);

        cout << "DB Conn Succ" << endl;
    }
    catch (_com_error _ex)
    {
        cout << "출처 : " << _ex.Source() << "설명 : " << _ex.Description() << endl;
        return FALSE;
    }

    return TRUE;
}

void CAdoDBConn::AddDBChatLog(int _type, int _sock, char _msg[512])
{
    try
    {
        m_CommPtr = NULL;
        m_CommPtr.CreateInstance(__uuidof(Command));
        m_CommPtr->ActiveConnection = m_ConnPtr;
        m_CommPtr->CommandText = _bstr_t("Add_Chat_Log");

        m_CommPtr->Parameters->Append(m_CommPtr->CreateParameter(_bstr_t("logType"), adInteger, adParamInput,4, _type));
        m_CommPtr->Parameters->Append(m_CommPtr->CreateParameter(_bstr_t("sock"), adInteger, adParamInput, 50, _sock));
        m_CommPtr->Parameters->Append(m_CommPtr->CreateParameter(_bstr_t("str"), adChar, adParamInput, 512, _msg));
        m_CommPtr->Execute(NULL, NULL, adCmdStoredProc);

        cout << "Data Input Succ" << endl;
    }
    catch (_com_error _ex)
    {
        cout << "출처 : " << _ex.Source() << "설명 : " << _ex.Description() << endl;
    }
}
