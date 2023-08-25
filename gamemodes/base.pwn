#include <a_samp>
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

// modulos
#include "modules/variaveis/defines.inc"
#include "modules/variaveis/messages.inc"
#include "modules/variaveis/global.inc"
#include "modules/variaveis/dialogs.inc"

#include "modules/servidor/db.inc"
#include "modules/servidor/funcoes.inc"

main(){
  printf("base dm v1.0 - by pushline");
}

public OnGameModeInit()
{
  DisableCrashDetectLongCall();
  Command_SetDeniedReturn(true); // Prevenir mensagem "Unknown Command" do ycmd
  mysql_log(ERROR);
  ligarDB();

  return true;
}

public OnGameModeExit()
{
	mysql_close(conexaoDB);
	return true;
}

public OnPlayerConnect(playerid)
{
	if(!IsPlayerNPC(playerid))
	{
		resetPVars(playerid);

		GetPlayerIp(playerid, User[playerid][lastip], 18);
		GetPlayerName(playerid, User[playerid][userName], MAX_PLAYER_NAME + 1);

		PreloadAnims(playerid);
		SetPlayerVirtualWorld(playerid, playerid);
		SetSpawnInfo(playerid, 0, 1, 0, 0, 0, 0, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);

		mysql_format(conexaoDB, queryGlobal, sizeof(queryGlobal),
			"SELECT * FROM `banimentos` WHERE `username` = '%e' OR `ip` = '%e'", User[playerid][userName], User[playerid][lastip]);
		mysql_tquery(conexaoDB, queryGlobal, "VerifyBan", "d", playerid);
	}

	return true;
}

public OnPlayerDisconnect(playerid, reason)
{
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
