#pragma once

#include "ChatServDefine.h"
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "ADOEOF")

class CAdoDBConn
{
    _ConnectionPtr m_ConnPtr;
    _CommandPtr m_CommPtr;

public:
    CAdoDBConn();
    ~CAdoDBConn();
    BOOL InitDB(string _id, string _pass, string _DBIp, string _dbName, string _DBPort);
    void AddDBChatLog(int _type, int _sock, char _msg[512]);
};

