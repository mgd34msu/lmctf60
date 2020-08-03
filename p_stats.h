#ifndef _P_STATS_H_
#define _P_STATS_H_

#define STATS_PLAYER_SAMPLE_RATE 20 //how many frames between ping samples, 1 frame = 100 ms

#define STATS_PING_TOTAL		0	// long ping_total; //add to this every time ping is sampled
#define STATS_PING_SAMPLES		1	// long ping_samples; //# of samples(+1 each time you sample ping)
#define STATS_TIME				2	// long time; //time on server in seconds? minutes? 
// seconds would be cool for more precise calculation
#define STATS_SCORE				3	// int score; //the overall score
#define STATS_CAPTURES			4	// int captures; //how many direct flag captures
#define STATS_FRAGS				5	// int frags; //how many direct kills
#define STATS_DEATHS			6	// int deaths; //again, how many times you died
#define STATS_DEFENSE_FLAG		7	// int defense_flag; //defended the flag
#define STATS_DEFENSE_BASE		8	// int defense_base; //defended the base (whether flag present or not)
#define STATS_DEFENSE_CARRIER	9	// int defense_carrier; //defended flag carrier
#define STATS_ASSISTS			10	// int assists; //number of assists
#define STATS_RETURNS			11	// int returns; //returned the flag
#define STATS_OFFENSE_FLAG		12	// int offense_flag; //took the enemy flag
#define STATS_OFFENSE_CARRIER	13	// int offense_carrier; //killed enemy flag carrier
#define STATS_OFFENSE_FLAGLOST	14	// int offense_flaglost; //lost the enemy flag

// BUZZKILL - IMPROVED ANALYTICS - BEGIN
#define STATS_RUNE_STRENGTH		15	// picked up strength rune
#define STATS_RUNE_HASTE		16	// picked up haste rune
#define STATS_RUNE_REGEN		17	// picked up regen rune
#define STATS_RUNE_RESIST		18	// picked up resist rune
#define STATS_ITEM_QUAD			19	// picked up quad
#define STATS_ITEM_SHIELD		21	// picked up power shield (or screen)
#define STATS_ITEM_ARMOR		22	// picked up red armor
#define STATS_ITEM_MEGA			23  // picked up mega health
#define STATS_IS_FC				24	// is the flag carrier
#define STATS_HAS_ST			25	// has the strength rune
#define STATS_HAS_RS			26	// has the resist rune
#define STATS_HAS_HA			27	// has the haste rune
#define STATS_HAS_RG			29	// has the regen rune
#define STATS_RAIL_SHOT			30  // railgun shots fired
#define STATS_RAIL_HIT			31  // railgun hits
#define STATS_RAIL_KILL			32  // railgun kills (not equal to hits due to armor, runes, etc.)
#define STATS_RAIL_ACCURACY		33  // railgun accuracy
#define STATS_DAMAGE_OUT		34  // damage given
#define STATS_DAMAGE_IN			35  // damage received
// BUZZKILL - IMPROVED ANALYTICS - END

// BUZZKILL - SQLITE - START
#define SQL_SHOTS				36 // Not currently implemented
#define SQL_SHOTS_HIT			37 // Not currently implemented
#define SQL_SCORE				38
#define SQL_FRAGS				39
#define SQL_DEATHS				40
#define SQL_DEF_BASE			41
#define SQL_DEF_FLAG			42
#define SQL_DEF_CARRIER			43
#define SQL_FLAG_PICKUPS		44
#define SQL_FLAG_DROPS			45
#define SQL_FLAG_CAPTURES		46
#define SQL_FLAG_KILLS			47
#define SQL_FLAG_RETURNS		48
#define SQL_ASSISTS				49
#define SQL_RAIL_SHOT			50
#define SQL_RAIL_HIT			51
#define SQL_RAIL_KILL			52
#define SQL_RUNE_STRENGTH		53
#define SQL_RUNE_HASTE			54
#define SQL_RUNE_REGEN			55
#define SQL_RUNE_RESIST			56
#define SQL_ITEM_QUAD			57
#define SQL_ITEM_SHIELD			58
#define SQL_ITEM_ARMOR			59
#define SQL_ITEM_MEGA			60
#define SQL_DAMAGE_OUT			61 // Not currently implemented
#define SQL_DAMAGE_IN			62 // Not currently implemented
#define SQL_MVP_OFF				63
#define SQL_MVP_DEF				64
// BUZZKILL - SQLITE - END

#define MAX_PLAYER_STATS		65	

#define LMCTF_DATABASE			"lmctf.db"

typedef struct {
	char name[MAX_INFO_STRING];
	int teamnum;
} stats_client_s;

typedef struct _stats_player {
	qboolean dropped; // whether this player was dropped

	stats_client_s info;

	long stats[MAX_PLAYER_STATS];

	struct _stats_player* pNext;
} stats_player_s;

typedef enum {
	STATS_SUICIDE,
	STATS_FRAG,
	STATS_FC_FRAG,
	STATS_FC_DEFENSE,
	STATS_FLAG_DEFENSE,
	STATS_FLAG_TOUCH,
	STATS_FLAG_RETURN,
	STATS_FLAG_CAPTURE,
	STATS_BASE_DEFENSE
} stats_event_t;

typedef struct {
	stats_client_s killer;
	stats_client_s killee;
	int mod;
	qboolean quad;
} stats_frag_data_s;

typedef struct {
	stats_client_s perp;
} stats_single_data_s;

void stats_add(edict_t* ent, int stat, long amount);
void stats_set(edict_t* ent, int stat, long amount);
long stats_get(edict_t* ent, int stat);
void stats_set_name(edict_t* ent, char* name);
void stats_clear(edict_t* ent);
void Cmd_PlayerStats_f(edict_t* ent);
void stats_log_init();
void stats_log_reset();
// returns pointer to lmctf_player_s struct of a dropped player given playername
stats_player_s* stats_find_dropped_player(char* name);
stats_player_s* stats_new_player(char* name);
void stats_cleanup(); // clean up stats before switching to next level
void Cmd_StatsAll_f(edict_t* ent);

#endif