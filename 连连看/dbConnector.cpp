#include"dbConnector.h"

#define USER_NAME	"root"
#define USER_PWD	"123456"
#define DB_ACCOUNT	"liandb"

bool DBCONNECTOR::Open()
{

	mysql_library_init(0, NULL, NULL);														//初始化数据库
	mysql_init(&mysql);																		//初始化数据结构
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");									//设置字符集
	mysql_real_connect(&mysql, LOCAL_HOST, USER_NAME, USER_PWD, DB_ACCOUNT, 3306, NULL, 0);	//数据库连接
	return	true;
}

void DBCONNECTOR::Close()
{
	mysql_free_result(result);
	mysql_close(&mysql);
	mysql_server_end();
}

bool DBCONNECTOR::IsLogin(wchar_t *account, wchar_t *password)
{
	bool					flag =	false;
	std::string				sqlstr;
	sqlstr = "SELECT * FROM liandb.user";
	char					*temAccount;
	char					*temPassword;
	std::string				strAccount;
	std::string				strPassword;
	
	int dwNum = WideCharToMultiByte(CP_ACP, NULL, account, -1, NULL, 0, NULL, FALSE);
	temAccount = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, account, -1, temAccount, dwNum, NULL, FALSE);
	strAccount = temAccount;
	delete[]temAccount;

	dwNum = WideCharToMultiByte(CP_ACP, NULL, password, -1, NULL, 0, NULL, FALSE);
	temPassword = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, password, -1, temPassword, dwNum, NULL, FALSE);
	strPassword = temPassword;
	delete[]temPassword;

	if (strAccount.empty() || strPassword.empty())
		return false;

	Open();
	mysql_query(&mysql, sqlstr.c_str());
	result = mysql_store_result(&mysql);

	int rowcount = mysql_num_rows(result);		//行数
	int fieldrow = mysql_num_fields(result);	//字段数
	MYSQL_ROW row;

	row = mysql_fetch_row(result);

	while (row)
	{
		int i = 0;
		std::string tem1 = row[i];
		std::string tem2 = row[i + 1];

		if (tem1 == strAccount&&tem2 == strPassword)
			flag = true;
		row = mysql_fetch_row(result);

	}
	Close();
	return flag;
}

bool DBCONNECTOR::RunRegister(wchar_t *account,wchar_t *password)
{
	Open();
	bool					flag = false;
	std::string				sqlstr;
	char					*temAccount;
	char					*temPassword;
	std::string				strAccount;
	std::string				strPassword;
	
	int dwNum = WideCharToMultiByte(CP_ACP, NULL, account, -1, NULL, 0, NULL, FALSE);
	temAccount = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, account, -1, temAccount, dwNum, NULL, FALSE);
	strAccount = temAccount;
	delete[]temAccount;

	dwNum = WideCharToMultiByte(CP_ACP, NULL, password, -1, NULL, 0, NULL, FALSE);
	temPassword = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, password, -1, temPassword, dwNum, NULL, FALSE);
	strPassword = temPassword;
	delete[]temPassword;

	if (strAccount.empty() || strPassword.empty())
		flag = false;

	sqlstr = "INSERT INTO `liandb`.`user` (`account`, `pwd`) ";
	sqlstr += "VALUES ( '" + strAccount + "', '" + strPassword + "');";
	mysql_query(&mysql, sqlstr.c_str());
	Close();
	flag = true;
	return flag;
}