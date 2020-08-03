#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include "g_local.h"
#include "g_ctffunc.h"

int callback(void*, int, char**, char**);

int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    NotUsed = 0;

    for (int i = 0; i < argc; i++)
    {
        gi.dprintf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    gi.dprintf("----------------------------------------\n");
    return 0;
}

void Cmd_SQLiteShowAllData_f()
{
    char* teststr = NULL;

    sqlite3* db;
    char* err_msg = 0;

    int rc = sqlite3_open(LMCTF_DATABASE, &db);

    if (rc != SQLITE_OK) {

        teststr = sqlite3_errmsg(db);
        gi.dprintf("Cannot open database: %s\n", teststr);
        sqlite3_close(db);

    }

    char* sql = "SELECT * FROM players";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK) {

        gi.dprintf("Failed to select data\n");
        gi.dprintf("SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

    }

    sqlite3_close(db);

}

void Cmd_SQLiteCreateDB_f()
{
    char* teststr = NULL;

    sqlite3* db;
    char* err_msg = 0;

    int rc = sqlite3_open(LMCTF_DATABASE, &db);

    if (rc != SQLITE_OK) {

        teststr = sqlite3_errmsg(db);
        gi.dprintf("Cannot open database: %s\n", teststr);
        sqlite3_close(db);

    }

    char* sql = "DROP TABLE IF EXISTS players;"
        "CREATE TABLE players(player_id INTEGER PRIMARY KEY AUTOINCREMENT, player_name TEXT, shots INTEGER, shots_hit INTEGER, score INTEGER, frags INTEGER, deaths INTEGER, def_base INTEGER, def_flag INTEGER, def_carrier INTEGER, flag_pickups INTEGER, flag_drops INTEGER, flag_captures INTEGER, flag_kills INTEGER, flag_returns INTEGER, assists INTEGER, rail_shot INTEGER, rail_hit INTEGER, rail_kill INTEGER, rune_strength INTEGER, rune_haste INTEGER, rune_resist INTEGER, rune_regen INTEGER, item_quad INTEGER, item_shield INTEGER, item_armor INTEGER, item_mega INTEGER, damage_out INTEGER, damage_in INTEGER, mvp_off INTEGER, mvp_def INTEGER);"
        "INSERT INTO players VALUES(1,'SQLite_Test',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {

        gi.dprintf("SQL error : % s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

    }

    sqlite3_close(db);

}

void SQL_Init_Player(stats_player_s* p_player)
{
    char* playername = p_player->info.name;
    char* strbuff[MAX_MSGLEN];
    char* sql[MAX_MSGLEN];
    sqlite3* db;
    char* err_msg = 0;
    sqlite3_stmt* res;

    int rc = sqlite3_open(LMCTF_DATABASE, &db);

    if (rc != SQLITE_OK)
    {
        gi.dprintf("SQL error : % s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }

    sprintf(strbuff, "SELECT player_id FROM players WHERE player_name = \'%s'\;", playername);
    strcpy(sql, strbuff);

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    qboolean sqlresults = false;

    int step = sqlite3_step(res);

    if (step == SQLITE_ROW)
    {
        sqlresults = true;
        sqlite3_finalize(res);
    }
    if (!sqlresults)
    {
        gi.dprintf("\nCreating Player: %s\n", playername);

        sqlite3_finalize(res);

        sprintf(strbuff, "INSERT INTO players VALUES(NULL,\'%s\',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);", playername);
        strcpy(sql, strbuff);

        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

        if (rc != SQLITE_OK)
        {
            gi.dprintf("SQL error : % s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
        }
    }

    sqlite3_close(db);
}

void SQL_Update_Player(stats_player_s* p_player)
{
    char* err_msg = 0;
    char* playername;
    char* sql[MAX_MSGLEN];
    char* strbuff[MAX_MSGLEN];
    sqlite3* db;

    int rc = sqlite3_open(LMCTF_DATABASE, &db);

    playername = p_player->info.name;

    sprintf(strbuff, "UPDATE players SET shots = shots + %i, shots_hit = shots_hit + %i, score = score + %i, frags = frags + %i, deaths = deaths + %i, def_base = def_base + %i, def_flag = def_flag + %i, def_carrier = def_carrier + %i, flag_pickups = flag_pickups + %i, flag_drops = flag_drops + %i, flag_captures = flag_captures + %i, flag_kills = flag_kills + %i, flag_returns = flag_returns + %i, assists = assists + %i, rail_shot = rail_shot + %i, rail_hit = rail_hit + %i, rail_kill = rail_kill + %i, rune_strength = rune_strength + %i, rune_haste = rune_haste + %i, rune_resist = rune_resist + %i, rune_regen = rune_regen + %i, item_quad = item_quad + %i, item_shield = item_shield + %i, item_armor = item_armor + %i, item_mega = item_mega + %i, damage_out = damage_out + %i, damage_in = damage_in + %i, mvp_off = mvp_off + %i, mvp_def = mvp_def + %i WHERE player_name = \'%s\';",
        p_player->stats[SQL_SHOTS],
        p_player->stats[SQL_SHOTS_HIT],
        p_player->stats[SQL_SCORE],
        p_player->stats[SQL_FRAGS],
        p_player->stats[SQL_DEATHS],
        p_player->stats[SQL_DEF_BASE],
        p_player->stats[SQL_DEF_FLAG],
        p_player->stats[SQL_DEF_CARRIER],
        p_player->stats[SQL_FLAG_PICKUPS],
        p_player->stats[SQL_FLAG_DROPS],
        p_player->stats[SQL_FLAG_CAPTURES],
        p_player->stats[SQL_FLAG_KILLS],
        p_player->stats[SQL_FLAG_RETURNS],
        p_player->stats[SQL_ASSISTS],
        p_player->stats[SQL_RAIL_SHOT],
        p_player->stats[SQL_RAIL_HIT],
        p_player->stats[SQL_RAIL_KILL],
        p_player->stats[SQL_RUNE_STRENGTH],
        p_player->stats[SQL_RUNE_HASTE],
        p_player->stats[SQL_RUNE_RESIST],
        p_player->stats[SQL_RUNE_REGEN],
        p_player->stats[SQL_ITEM_QUAD],
        p_player->stats[SQL_ITEM_SHIELD],
        p_player->stats[SQL_ITEM_ARMOR],
        p_player->stats[SQL_ITEM_MEGA],
        p_player->stats[SQL_DAMAGE_OUT],
        p_player->stats[SQL_DAMAGE_IN],
        p_player->stats[SQL_MVP_OFF],
        p_player->stats[SQL_MVP_DEF],
        playername);

    strcpy(sql, strbuff);
    //gi.dprintf("%s", sql); // Uncomment to show SQL being sent (outputs to server only)
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        gi.dprintf("SQL error : % s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
    sqlite3_close(db);
}

void SQL_Stats_Reset(edict_t* ent)
{
    for (int i = SQL_SHOTS; i < MAX_PLAYER_STATS; i++)
    {
        stats_set(ent, i, 0);
    }
}

void SQL_Stats_Update(edict_t* ent)
{
    SQL_Update_Player(ent->client->p_stats_player);
    SQL_Stats_Reset(ent);
}