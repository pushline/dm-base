#include <open.mp>
#undef MAX_PLAYERS
#define MAX_PLAYERS 50 // mude no config.json caso mude aqui

#define YSI_YES_HEAP_MALLOC

// plugins
#include <sscanf2>
#include <a_mysql>
#include <Pawn.RakNet>
#include <crashdetect>
#include <samp_bcrypt>
#include <streamer>

// includes
#include <YSI_Coding\y_va>
#include <YSI_Coding\y_inline>
#include <YSI_Extra\y_inline_timers>
#include <YSI_Visual\y_dialog>
#include <YSI_Visual\y_commands>
#include <YSI_Data\y_iterate>
#include <weapon-config>
#include <sort-inline>
#include <strlib>
#include <a_zone> // https://github.com/pushline/Advanced-Gang-Zones
#include <colandreas>

// modulos
#include "modules/entry.inc"

main()
{
  printf("base dm v1.0 - by pushline");
}

public OnGameModeInit()
{
	DisableCrashDetectLongCall();
	Command_SetDeniedReturn(true); // Prevenir mensagem "Unknown Command" do ycmd
	
	mysql_log(ERROR | WARNING);
	ligarDB();

	SetGameModeText("DM BASE");
	iniciarServidor();

	return true;
}

public OnGameModeExit()
{
	mysql_close(conexaoDB);
	return true;
}

public OnPlayerConnect(playerid)
{
	User[playerid] = User[MAX_PLAYERS];
	User[playerid][time] = 10;
	User[playerid][weather] = 10;

	GetPlayerIp(playerid, User[playerid][lastip], 18);
	GetPlayerName(playerid, User[playerid][userName], MAX_PLAYER_NAME + 1);

	PreloadAnims(playerid);
	SetPlayerVirtualWorld(playerid, playerid);

	SetSpawnInfo(playerid, 0, 1, 0, 0, 0, 0, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);

	mysql_format(conexaoDB, queryGlobal, sizeof(queryGlobal),
		"SELECT * FROM `banimentos` WHERE `username` = '%e' OR `ip` = '%e'", User[playerid][userName], User[playerid][lastip]);
	mysql_tquery(conexaoDB, queryGlobal, "VerifyBan", "d", playerid);

	return true;
}

public OnPlayerDisconnect(playerid, reason)
{
	User[playerid] = User[MAX_PLAYERS];

	new szDisconnectReason[5][] =
	{
		"Timeout/Crash",
		"Quit",
		"Kick/Ban",
		"Custom",
		"Mode End"
	};

	va_SendClientMessageToAll(0xC4C4C4FF, "%s left the server (%s).", User[playerid][userName], szDisconnectReason[reason]);

	return true;
}

public OnPlayerRequestClass(playerid, classid)
{
	return 0;
}

public OnPlayerRequestSpawn(playerid)
{
	return false;
}

public OnPlayerSpawn(playerid)
{
	SetPlayerInterior(playerid, 0);
	SetPlayerWorldBounds(playerid, 20000.0, -20000.0, 20000.0, -20000.0);

	if(User[playerid][isAfk])
		setAfk(playerid);
	else
		SpawnPlayerOnArena(playerid);

	if(GetPVarInt(playerid, "firstSpawn") != 0)
	{
		for (new i; i < MAX_ZONES; i ++)
		{
			ShowZoneForPlayer(playerid, i, 0x85807F65, 0xFFFFFFFF, 0xFFFFFFFF);
		}

		SetPlayerTime(playerid, User[playerid][time], 0);
		SetPlayerWeather(playerid, User[playerid][weather]);

		ZoneNumberFlashForPlayer(playerid, arenaInfo[arenaId], 0xCC0000FF);
		SetPlayerScore(playerid, User[playerid][playerKills]);

		CancelSelectTextDraw(playerid);
		DeletePVar(playerid, "firstSpawn");
	}

	SetPlayerArmour(playerid, 100.0);
	SetPlayerHealth(playerid, 100.0);
	return true;
}
