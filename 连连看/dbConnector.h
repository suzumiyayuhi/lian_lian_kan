#pragma once
#include <windows.h>
#include<iostream>
#include<string>
#include "mysql.h"
struct DBCONNECTOR
{
	//��Ա����
	bool Open();												//������
	void Close();												//�ر�����
	bool IsLogin(wchar_t *account,wchar_t *password);			//�жϵ����û�������
	bool RunRegister(wchar_t *account, wchar_t *password);		//ע�Ṧ�ܺ���
	/////////////////////////////////////////////
	//��Ա����
	MYSQL			mysql;
	MYSQL_RES		*result;
};