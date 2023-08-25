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

// resetar variaveis
resetPvars(playerid)
{
	User[playerid][sqlID] = 0;
	User[playerid][userName] = '\0';
	User[playerid][password] = '\0';
	User[playerid][registerip] = '\0';
	User[playerid][lastip] = '\0';
	User[playerid][playerKills] = 0;
	User[playerid][playerDeaths] = 0;
	User[playerid][playerMoney] = 0;
	User[playerid][playerSkin] = 0;
	User[playerid][adminLevel] = 0;
	User[playerid][isLogged] = false;
	User[playerid][isAfk] = false;
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
