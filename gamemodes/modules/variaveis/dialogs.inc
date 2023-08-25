forward LOGIN(playerid, dialogid, response, listitem, string:inputtext[]);
public LOGIN(playerid, dialogid, response, listitem, string:inputtext[])
{
	if (response)
	{
		if(strlen(inputtext) != 0)
		{
			new Cache:select, passwordHash[BCRYPT_HASH_LENGTH];

			mysql_format(conexaoDB, queryGlobal, sizeof(queryGlobal), "SELECT `password` FROM `contas` WHERE `username` = '%e' LIMIT 1", User[playerid][userName]);
			select = mysql_query(conexaoDB, queryGlobal);

			new rows = cache_num_rows();
			if(rows)
			{
				cache_get_value_name(0, "password", passwordHash, BCRYPT_HASH_LENGTH);
				bcrypt_verify(playerid, "OnLoginVerify", inputtext, passwordHash);
			}

			cache_delete(select);
			return true;
		}

		showLoginDialog(playerid);
		return true;
	}

	Kick(playerid);
	return true;
}

forward OnLoginVerify(playerid, bool:success);
public OnLoginVerify(playerid, bool:success)
{
 	if(success)
  {
		new rearrangedAdminLevel[25];
		strcopy(rearrangedAdminLevel, adminnames[User[playerid][adminLevel]], sizeof(rearrangedAdminLevel));

		new planeString[250];
		strcopy(planeString, returnDate(User[playerid][lastLogin]-10800), sizeof(planeString));

		va_SendClientMessage(playerid, -1, LOGIN_MSG1, User[playerid][userName]);
		va_SendClientMessage(playerid, -1, LOGIN_MSG2, rearrangedAdminLevel);
		va_SendClientMessage(playerid, -1, LOGIN_MSG3, planeString);

		User[playerid][isLogged] = true;
		SetPlayerColor(playerid, 0x7ea39eFF);
 	}
 	else{
 		showLoginDialog(playerid);
 	}
	return true;
}

forward REGISTER(playerid, dialogid, response, listitem, string:inputtext[]);
public REGISTER(playerid, dialogid, response, listitem, string:inputtext[])
{
	if(response)
	{
		if (IsValidPassword(inputtext))
		{
			bcrypt_hash(playerid, "OnRegisterHash", inputtext, BCRYPT_COST);
			return true;
		}
		showRegistrationDialog(playerid);
		return true;
	}

	Kick(playerid);
	return true;
}

forward OnRegisterHash(playerid, accountname[]);
public OnRegisterHash(playerid, accountname[])
{
	new passwordHash[BCRYPT_HASH_LENGTH];
	bcrypt_get_hash(passwordHash);

	new pip[18];
	GetPlayerIp(playerid, pip, 18);

	mysql_format(conexaoDB, queryGlobal, sizeof(queryGlobal),
		"INSERT INTO `contas`(`username`, `regip`, `password`) VALUES('%e', '%e', '%e')",
		User[playerid][userName], pip, passwordHash);

	mysql_tquery(conexaoDB, queryGlobal , "InsertDataSQL", "i", playerid);

	va_SendClientMessage(playerid, 0x4cbd1cFF, REGISTRO_MSG, User[playerid][userName]);
	va_SendClientMessageToAll(0xC4C4C4FF, "%s has joined the server.", User[playerid][userName]);

	format(User[playerid][lastip], 18, pip);
	User[playerid][isLogged] = true;
	User[playerid][playerMoney] += 5000;
	GivePlayerMoney(playerid, User[playerid][playerMoney]);
	SetPlayerColor(playerid, 0x7ea39eFF);

	return true;
}

forward InsertDataSQL(playerid);
public InsertDataSQL(playerid)
{
    User[playerid][sqlID] = cache_insert_id();
}
