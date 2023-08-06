#define MYSQL_HOST "localhost"
#define MYSQL_USER "root"
#define MYSQL_PASS ""
#define MYSQL_DB "bancodedados"

ligarDB()
{
	conexaoDB = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB);

	if(conexaoDB == MYSQL_INVALID_HANDLE || mysql_errno() != 0)
	{
		print("MySQL falhou.");
		SendRconCommand("exit");
		return true;
	}

	print("MySQL conectou.");
	return true;
}