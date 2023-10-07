initArena()
{
	arenaInfo[roundMints] = ROUND_TIME;
	arenaInfo[arenaId] = random(MAX_ZONES);

	return true;
}

iniciarServidor()
{
	initArena();

	SetNameTagDrawDistance(40.0);
	SetGameModeText(gamemodeName);

	DisableInteriorEnterExits();
	UsePlayerPedAnims();
	EnableStuntBonusForAll(false);
	SetDisableSyncBugs(true);
	SetTimer("OnScriptUpdate", 1000, true);
	SetWeaponDamage(WEAPON_SNIPER, DAMAGE_TYPE_STATIC, 40.0);
	gmtextdraws();
	createZones();
}

forward OnScriptUpdate();
public OnScriptUpdate()
{
	new strm[256];

	if(arenaInfo[isRunningArena])
	{
		arenaInfo[roundMints]--;

		if(arenaInfo[roundMints] < 0)
		{
			new nextArena = determineArenaID();

			foreach(new p : Player)
			{
				if(User[p][locationId] != i)
					continue;

				format(strm, sizeof(strm), ARENA_ENDED_MSG,
							arenaData[arenaInfo[arenaId]][arenaName], arenaInfo[arenaId],	arenaData[nextArena][arenaName], nextArena);
				SendClientMessage(p, -1, strm);

				ZoneNumberStopFlashForAll(arenaInfo[arenaId]);
			}

			GetRoundRank(i);
			arenaInfo[arenaId] = nextArena;
			PrepareChangeArena(i);
		}
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

}

Respawn(playerid)
{

}