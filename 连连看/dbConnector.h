#pragma once
#include <windows.h>
#include<iostream>
#include<string>
#include "mysql.h"
struct DBCONNECTOR
{
	//成员函数
	bool Open();												//打开连接
	void Close();												//关闭连接
	bool IsLogin(wchar_t *account,wchar_t *password);			//判断登入用户名密码
	bool RunRegister(wchar_t *account, wchar_t *password);		//注册功能函数
	/////////////////////////////////////////////
	//成员变量
	MYSQL			mysql;
	MYSQL_RES		*result;
};