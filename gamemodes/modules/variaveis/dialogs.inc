forward LOGIN(playerid, dialogid, response, listitem, string:inputtext[]);
public LOGIN(playerid, dialogid, response, listitem, string:inputtext[])
{
	if (response)
	{
		if(strlen(inputtext) != NULL)
		{
			new Cache:select, password[BCRYPT_HASH_LENGTH];

			mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal), "SELECT `password` FROM `contas` WHERE `username` = '%e' LIMIT 1", User[playerid][userName]);
			select = mysql_query(dbHandle, queryGlobal);

			new rows = cache_num_rows();
			if(rows)
			{
				cache_get_value_name(0, "password", password, BCRYPT_HASH_LENGTH);
				bcrypt_verify(playerid, "OnLoginVerify", inputtext, password);
			}

			cache_delete(select);
			return true;
		}

		showAutorizationDialog(playerid);
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
    SetPlayerColor(0x7ea39eFF);
 	}
 	else{
 		showAutorizationDialog(playerid);
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
  new password[BCRYPT_HASH_LENGTH];
  bcrypt_get_hash(password);

	new stringQ[512], pip[18];
	GetPlayerIp(playerid, pip, 18);
	format(User[playerid][user_gpci], 41, ReturnCI(playerid));

	mysql_format(dbHandle, queryGlobal, sizeof(queryGlobal),
		"INSERT INTO `contas`(`username`, `regip`, `password`) VALUES('%e', '%e', '%e')",
		User[playerid][userName], pip, password);

	mysql_tquery(dbHandle, queryGlobal , "InsertDataSQL", "i", playerid);

	va_SendClientMessage(playerid, COLOR_GREEN, REGISTRO_MSG, User[playerid][userName]);
  va_SendClientMessageToAll(0xC4C4C4FF, "%s has joined the server.", User[playerid][userName]);

	format(User[playerid][user_lastip], 18, pip);
	User[playerid][isLogged] = true;
	User[playerid][playerMoney] += 5000;
  GivePlayerMoney(playerid, User[playerid][playerMoney]);
  SetPlayerColor(0x7ea39eFF);

	return true;
}

forward InsertDataSQL(playerid);
public InsertDataSQL(playerid)
{
    User[playerid][user_sqlid] = cache_insert_id();
}
