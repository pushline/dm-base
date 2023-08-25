new MySQL:conexaoDB;
new queryGlobal[2000];

enum e_USER
{
    sqlID,
    userName[MAX_PLAYER_NAME + 1],
    password[64 + 1],
    registerip[18],
    lastip[18],
    FPS,
    dLlast,
    playerKills,
    playerDeaths,
    playerSkin,
    adminLevel,
    playerMoney,
    lastLogin,

    bool:isLogged,
    bool:isAfk,
};

new User[MAX_PLAYERS][e_USER];

new adminnames[5][] =
{
    "{7ea39e}Player Normal",
	"{5b8012}Ajudante",
	"{5a24ab}Moderador",
	"{b55f09}Administrador",
	"{0517e6}Fundador"
};
