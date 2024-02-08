initArena()
{
  arenaInfo[roundMints] = ROUND_TIME;
  arenaInfo[arenaId] = Iter_Random(Arenas);
  Iter_Remove(ArenaQueue, arenaInfo[arenaId]);
  printf("arenaInfo[arenaId] %d", arenaInfo[arenaId]);

  return true;
}

iniciarServidor()
{
  SetNameTagDrawDistance(40.0);

  DisableInteriorEnterExits();
  UsePlayerPedAnims();
  EnableStuntBonusForAll(false);
  SetDisableSyncBugs(true);
  SetTimer("OnScriptUpdate", 1000, true);
  SetWeaponDamage(WEAPON_SNIPER, DAMAGE_TYPE_STATIC, 40.0);
  // gmtextdraws();
  createZones();
  initArena();
}

forward OnScriptUpdate();
public OnScriptUpdate()
{
	new strm[512];

	arenaInfo[roundMints]--;

	if(arenaInfo[roundMints] < 0)
	{
		new nextArena = determineArenaID();

		foreach(new i : Player)
		{
			format(strm, sizeof(strm), ARENA_ENDED_MSG,
				arenaData[arenaInfo[arenaId]][arenaName],
				arenaInfo[arenaId],
				arenaData[nextArena][arenaName],
				nextArena
			);

			SendClientMessage(i, -1, strm);
			ZoneNumberStopFlashForAll(arenaInfo[arenaId]);
		}

		arenaInfo[arenaId] = nextArena;
		//	PrepareChangeArena();
	}

	foreach(new i : Player)
	{
		updatePlayerHud(i);

		new drunk2 = GetPlayerDrunkLevel(i);
		if(drunk2 < 100)
		{
			SetPlayerDrunkLevel(i, 2000);
		}
		else
		{
			if(User[i][dLlast] != drunk2)
			{
				new fps = User[i][dLlast] - drunk2;

				if(fps > 0 && fps < 450)
					User[i][FPS] = fps;

				User[i][dLlast] = drunk2;
			}
		}

		if((GetPlayerWeapon(i) == WEAPON_SAWEDOFF || GetPlayerWeapon(i) == WEAPON_ROCKETLAUNCHER || GetPlayerWeapon(i) == WEAPON_MINIGUN) && User[i][isAfk] == false)
		{
			KickEx(i, 300);
		}
	}
	return true;
}

stock PreloadAnims(playerid)
{
	ApplyAnimation(playerid, "ped", "null", 0.0, 0, 0, 0, 0, 0);
	return true;
}

KickEx(playerid, delayms){
	SetTimerEx("KickPlayerEx", delayms, false, "i", playerid);
}

forward KickPlayerEx(playerid);
public KickPlayerEx(playerid){
	Kick(playerid);
}

IsValidPassword(const inputpassword[])
{
	for(new i = 0; inputpassword[i] != EOS; ++i)
	{
		switch(inputpassword[i])
		{
			case '0'..'9', 'A'..'Z', 'a'..'z': continue;
			default: return 0;
		}
	}
	return 1;
}

// podes usar pawn chrono se quiser tbm.
returnDate(timestamp, _form=0 )
{
	new year=1970, day=0, month=0, hourt=0, mins=0, sec=0;

	new days_of_month[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	new names_of_month[12][10] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
	new returnstring[32];

	while(timestamp>31622400){
		timestamp -= 31536000;
		if ( ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0) ) timestamp -= 86400;
		year++;
	}

	if ( ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0) )
		days_of_month[1] = 29;
	else
		days_of_month[1] = 28;


	while(timestamp>86400){
		timestamp -= 86400, day++;
		if(day==days_of_month[month]) day=0, month++;
	}

	while(timestamp>60){
		timestamp -= 60, mins++;
		if( mins == 60) mins=0, hourt++;
	}

	sec=timestamp;

	switch( _form ){
		case 1: format(returnstring, 31, "%02d/%02d/%d %02d:%02d:%02d", day+1, month+1, year, hourt, mins, sec);
		case 2: format(returnstring, 31, "%s %02d, %d, %02d:%02d:%02d", names_of_month[month],day+1,year, hourt, mins, sec);
		case 3: format(returnstring, 31, "%d %c%c%c %d, %02d:%02d", day+1,names_of_month[month][0],names_of_month[month][1],names_of_month[month][2], year,hourt,mins);
		case 4: format(returnstring, 31, "%s %02d, %d", names_of_month[month],day+1,year);
		default: format(returnstring, 31, "%02d/%02d/%d %02d:%02d:%02d", day+1, month+1, year, hourt, mins, sec);
	}

	return returnstring;
}

setAfk(playerid)
{
  SetPlayerVirtualWorld(playerid, 999);
  return 1;
}

stock Respawn(playerid, timestamp)
{
  timestamp = 0;
  if(User[playerid][adminLevel] >= 4)
  {
    SetPlayerPos(playerid, 0, 0, 0);
    SpawnPlayerEx(playerid);
    return true;
  }

  if(User[playerid][timerRespawn] > timestamp)
    SendClientMessage(playerid, -1, "NÃO PODE RESPAWNAR AINDA");

  if(User[playerid][isAfk])
    SendClientMessage(playerid, -1, "VOCÊ ESTÁ AFK");

  new Float:Armour;
  GetPlayerArmour(playerid, Armour);

  if(Armour < 20.0)
    SendClientMessage(playerid, -1, "MENSAGEM CANT RESPAWN");

  SetPlayerPos(playerid, 0, 0, 0);
  SpawnPlayerEx(playerid);

  User[playerid][timerRespawn] = gettime() + 1;

  return true;
}

stock SpawnPlayerEx(playerid)
{
  if(IsPlayerInAnyVehicle(playerid))
    RemovePlayerFromVehicle(playerid);

  SpawnPlayer(playerid);
  return 1;
}

determineArenaID()
{
  new rand;
  if (Iter_IsEmpty(ArenaQueue))
  {
    foreach (new i : Arenas)
    {
      if (arenaInfo[arenaId] != i)
      {
        Iter_Add(ArenaQueue, i);
      }
    }
  }

  rand = Iter_Random(ArenaQueue);
  Iter_Remove(ArenaQueue, rand);

  return rand;
}

updatePlayerHud(playerid)
{
  SetPlayerVirtualWorld(playerid, 999); // workaround
  return 1;
}


SpawnPlayerOnArena(playerid)
{
	if(IsPlayerInAnyVehicle(playerid))
    RemovePlayerFromVehicle(playerid);

	new currentArena = arenaInfo[arenaId];
	new Float:posX, Float:posY, Float:posZ, modelid, i;

	do
	{
		posX = RandomFloat(arenaData[currentArena][arenaPos][0], arenaData[currentArena][arenaPos][2]);
		posY = RandomFloat(arenaData[currentArena][arenaPos][1], arenaData[currentArena][arenaPos][3]);
		modelid = CA_RayCastLine(posX, posY, 700.0, posX, posY, -1000.0, posX, posY, posZ);

		i ++;
	} while ((modelid == WATER_OBJECT || IsModelBuilding(modelid)) && i != 10);

	SetPlayerPos(playerid, posX, posY, posZ + 1.0);
	SetPlayerFacingAngle(playerid, posZ);
	SetCameraBehindPlayer(playerid);

	return true;

}
