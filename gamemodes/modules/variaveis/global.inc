new MySQL:conexaoDB;

enum e_USER
{
    sqlID,
    password[64 + 1],
    registerip[18],
    lastip[18],
    FPS,
    dLlast,
    userName[MAX_PLAYER_NAME + 1],
    playerKills,
    playerDeaths,
    playerExp,
    playerLevel,
    playerSkin,
    adminLevel,
    playerMoney,
    lastLogin,

    bool:isLogged,
    bool:isAfk,
};

new User[MAX_PLAYERS][e_USER];