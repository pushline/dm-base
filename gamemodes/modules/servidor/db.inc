#define MYSQL_HOST "localhost"
#define MYSQL_USER "root"
#define MYSQL_PASS ""
#define MYSQL_DB "dmbase"

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

forward VerifyBan(playerid);
public VerifyBan(playerid)
{
	new rows = cache_num_rows();
	if(rows)
	{
		new adminname[25], usernamee[25], reason[64], unbantimestamp, bantimestamp;
		cache_get_value_int(0, "unbantimestamp", unbantimestamp);
		cache_get_value_int(0, "bantimestamp", bantimestamp);
		cache_get_value_name(0, "admin", adminname, 24);
		cache_get_value_name(0, "username", usernamee, 24);
		cache_get_value_name(0, "reason", reason, 24);

		if(strlen(reason) < 1)
			reason = "Sem razão.";

		if(gettime() >= unbantimestamp)
		{
			new Cache:delete;

			mysql_format(conexaoDB, queryGlobal, sizeof queryGlobal, "DELETE FROM `banimentos` where `username` = '%e' LIMIT 1", usernamee);
			delete = mysql_query(conexaoDB, queryGlobal);
			cache_delete(delete);

			mysql_format(conexaoDB, queryGlobal, sizeof(queryGlobal),
				"SELECT * FROM `contas` WHERE `username` = '%e'", User[playerid][userName]);

			mysql_tquery(conexaoDB, queryGlobal, "OnPlayerJoin", "d", playerid);
		}
		else
		{
			new banTS[50], unbanTS[50];
			strcopy(banTS, returnDate(bantimestamp-10800), sizeof(banTS));
			strcopy(unbanTS, returnDate(unbantimestamp-10800), sizeof(unbanTS));

			new dialogBanned[512];
			format(dialogBanned, sizeof (dialogBanned), BAN_WHOBANNED, usernamee, adminname, reason, banTS, unbanTS);
			SendClientMessage(playerid, -1, BAN_WHOBANNED);
			SendClientMessage(playerid, -1, BAN_BANNED);
			SendClientMessage(playerid, -1, BAN_MISTAKE);
			SendClientMessage(playerid, -1, BAN_DCLINK);

			KickEx(playerid, 2000);
			return true;
		}
	}
	else
	{
		mysql_format(conexaoDB, queryGlobal, sizeof(queryGlobal),
			"SELECT * FROM `contas` WHERE `username` = '%e'", User[playerid][userName]);

		mysql_tquery(conexaoDB, queryGlobal, "OnPlayerJoin", "d", playerid);
	}

	return true;
}

forward OnPlayerJoin(playerid);
public OnPlayerJoin(playerid)
{
	new rows = cache_num_rows();
	if(rows)
	{
		cache_get_value_int(0, "id", User[playerid][sqlID]);
		cache_get_value_name(0, "password", User[playerid][password], BCRYPT_HASH_LENGTH);
		cache_get_value_name(0, "regip", User[playerid][registerip], 18);
		cache_get_value_int(0, "kills", User[playerid][playerKills]);
		cache_get_value_int(0, "deaths", User[playerid][playerDeaths]);
		cache_get_value_int(0, "skin", User[playerid][playerSkin]);
		cache_get_value_int(0, "adminlevel", User[playerid][adminLevel]);
		cache_get_value_int(0, "money", User[playerid][playerMoney]);
		cache_get_value_int(0, "lastlogin", User[playerid][lastLogin]);

		if(User[playerid][lastLogin] == 0)
			User[playerid][lastLogin] = gettime();

		showLoginDialog(playerid);
		va_SendClientMessageToAll(0xC4C4C4FF, "%s has joined the server.", User[playerid][userName]);

		return true;
	}

	showRegistrationDialog(playerid);
	return true;
}

stock showRegistrationDialog(playerid)
{
	Dialog_ShowCallback(playerid, using public REGISTER<iiiis>, DIALOG_STYLE_PASSWORD, "Registro",
  "{FFFFFF}Esta conta não está registrada.\nColoque sua conta abaixo para fazer o registro.", "Login", "Cancel");
}

stock showLoginDialog(playerid)
{
	Dialog_ShowCallback(playerid, using public LOGIN<iiiis>, DIALOG_STYLE_PASSWORD, "Login",
	"{FFFFFF}Sua conta está no nosso banco de dados.\noloque sua conta abaixo para fazer o login.","Login", "Cancel");
}
