/* ************************************************************************
 *    File:   encounters.c                           Part of LuminariMUD  *
 * Purpose:   Random encounter system for wilderness area                 *
 *  Author:   Gicker                                                      *
 ************************************************************************ */

#include <math.h>

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "db.h"
#include "oasis.h"
#include "screen.h"
#include "interpreter.h"
#include "modify.h"
#include "spells.h"
#include "feats.h"
#include "class.h"
#include "handler.h"
#include "constants.h"
#include "assign_wpn_armor.h"
#include "domains_schools.h"
#include "spell_prep.h"
#include "alchemy.h"
#include "race.h"
#include "encounters.h"
#include "dg_scripts.h"

extern struct room_data *world;
extern struct char_data *character_list;

struct encounter_data encounter_table[NUM_ENCOUNTER_TYPES];

/* function to assign basic attributes to an encounter record */
static void add_encounter(int encounter_record, int encounter_type, int max_level, int encounter_group, const char *object_name, 
                          int load_chance, int min_number, int max_number, int treasure_table, int char_class, int encounter_strength)
{
  encounter_table[encounter_record].encounter_type = encounter_type;
  encounter_table[encounter_record].max_level = max_level;
  encounter_table[encounter_record].encounter_group = encounter_group;
  encounter_table[encounter_record].object_name = object_name;
  encounter_table[encounter_record].load_chance = load_chance;
  encounter_table[encounter_record].min_number = min_number;
  encounter_table[encounter_record].max_number = max_number;
  encounter_table[encounter_record].treasure_table = treasure_table;
  encounter_table[encounter_record].char_class = char_class;
  encounter_table[encounter_record].encounter_strength = encounter_strength;
}

void initialize_encounter_table(void)
{
  int i, j;

  /* initialize the list of encounters */
  for (i = 0; i < NUM_ENCOUNTER_TYPES; i++)
  {
    encounter_table[i].encounter_type = ENCOUNTER_TYPE_NONE;
    encounter_table[i].max_level = 0;
    encounter_table[i].encounter_group = ENCOUNTER_GROUP_TYPE_NONE;
    encounter_table[i].object_name = "Unused Encounter";
    encounter_table[i].load_chance = 0;
    encounter_table[i].min_number = 0;
    encounter_table[i].max_number = 0;
    encounter_table[i].treasure_table = TREASURE_TABLE_NONE;
    encounter_table[i].char_class = CLASS_WARRIOR;
    encounter_table[i].encounter_strength = ENCOUNTER_STRENGTH_NORMAL;
    for (j = 0; j < NUM_ROOM_SECTORS; j++)
      encounter_table[i].sector_types[j] = false;
    
  }
}

void add_encounter_sector(int encounter_record, int sector_type)
{
    encounter_table[encounter_record].sector_types[sector_type] = true;
}

void set_encounter_terrain_any(int encounter_record)
{
  int j;
  for (j = 0; j < NUM_ROOM_SECTORS; j++)
      encounter_table[encounter_record].sector_types[j] = true;
}

void populate_encounter_table(void)
{
    initialize_encounter_table();

    add_encounter(ENCOUNTER_TYPE_GOBLIN_1, ENCOUNTER_CLASS_COMBAT, 40, ENCOUNTER_GROUP_TYPE_GOBLINS, "goblin raider", 100, 1, 3, 
      TREASURE_TABLE_LOW_NORM, CLASS_WARRIOR, ENCOUNTER_STRENGTH_NORMAL);
    set_encounter_terrain_any(ENCOUNTER_TYPE_GOBLIN_1);

    add_encounter(ENCOUNTER_TYPE_GOBLIN_2, ENCOUNTER_CLASS_COMBAT, 40, ENCOUNTER_GROUP_TYPE_GOBLINS, "goblin chieftain", 25, 1, 1, 
      TREASURE_TABLE_LOW_BOSS, CLASS_WARRIOR, ENCOUNTER_STRENGTH_BOSS);
    set_encounter_terrain_any(ENCOUNTER_TYPE_GOBLIN_2);

}

int encounter_chance(struct char_data *ch)
{
    int chance = 5;

    return chance;
}

int get_exploration_dc(struct char_data *ch)
{
    if (!ch) return 9999;
    
    if (STATE(ch->desc) != CON_PLAYING) return 9999;

    int dc = ((int) GET_LEVEL(ch) / 1.5) + 17;

    return dc;
}

void check_random_encounter(struct char_data *ch)
{

  if (!ch || IN_ROOM(ch) == NOWHERE) return;

  if (dice(1, 100) > encounter_chance(ch))
    return;

  // check if avoiding encounters or not
  
  int i = 0, j = 0, count = 0, roll = 0, group_type = ENCOUNTER_GROUP_TYPE_NONE;
  int groups[NUM_ENCOUNTER_GROUP_TYPES];
  int room_sect = world[IN_ROOM(ch)].sector_type;
  int highest_level = get_max_party_level_same_room(ch);
  //int avg_level = get_avg_party_level_same_room(ch);
  int party_size = get_party_size_same_room(ch);
  int num_mobs = 0, num_mobs_roll = 0;
  struct char_data *mob = NULL;
  char mob_descs[MEDIUM_STRING];

  for (i = 0; i < NUM_ENCOUNTER_GROUP_TYPES; i++)
  {
    for (j = 0; j < NUM_ENCOUNTER_TYPES; j++)
      if (encounter_table[j].encounter_group == i)
        if (encounter_table[j].sector_types[room_sect] == true)
          if (highest_level <= encounter_table[j].max_level)
            groups[i] = true;
        
  }

  for (i = 0; i < NUM_ENCOUNTER_GROUP_TYPES; i++)
    if (groups[i] == true) count++;

  roll = dice(1, count);

  count = 0;

  for (i = 0; i < NUM_ENCOUNTER_GROUP_TYPES; i++)
    if (groups[i] == true)
      if (i == roll)
        group_type = i;

  if (group_type == ENCOUNTER_GROUP_TYPE_NONE)
    return;

  for (j = 0; j < NUM_ENCOUNTER_TYPES; j++) {
    if (encounter_table[j].encounter_group == group_type)
     {
      num_mobs_roll = dice(encounter_table[j].min_number, encounter_table[j].max_number);
      for (i = 0; i < num_mobs_roll && num_mobs < party_size; i++) {
        mob = read_mobile(8100, VIRTUAL);
        if (!mob) return;
        // set descriptions
        mob->player.name = strdup(encounter_table[j].object_name);
        sprintf(mob_descs, "%s %s", AN(encounter_table[j].object_name), encounter_table[j].object_name);
        mob->player.short_descr = strdup(mob_descs);
        sprintf(mob_descs, "%s %s is here.", AN(encounter_table[j].object_name), encounter_table[j].object_name);
        mob->player.long_descr = strdup(mob_descs);
        sprintf(mob_descs, "%s %s is here before you.\r\n", AN(encounter_table[j].object_name), encounter_table[j].object_name);
        mob->player.description = strdup(mob_descs);    
        // set stats
        GET_CLASS(mob) = encounter_table[j].char_class;
        GET_LEVEL(mob) = highest_level;
        autoroll_mob(mob, TRUE, FALSE);
        // set flags
        SET_BIT_AR(MOB_FLAGS(mob), MOB_ENCOUNTER);

        X_LOC(mob) = world[IN_ROOM(ch)].coords[0];
        Y_LOC(mob) = world[IN_ROOM(ch)].coords[1];
        char_to_room(mob, IN_ROOM(ch));
        
        send_to_room(IN_ROOM(ch), "\tY%s has ambushed you!\tn\r\n", CAP(mob->player.short_descr));
        num_mobs++;
      }
    }
  }

  // set up the encounter
  // determine what and how many mobs based on party size and average level
  // create mobs and place in room
  // set treasure type
  // prevent party from exiting the room using normal movement
  // set despawn timer. Timer only goes down if no players are in the room with them

}

bool in_encounter_room(struct char_data *ch)
{
  if (!ch || IN_ROOM(ch) == NOWHERE)
    return false;

  struct char_data *tch = NULL;

  for (tch = world[IN_ROOM(ch)].people; tch; tch = tch->next_in_room)
  {
    if (!IS_NPC(tch)) continue;
    if (MOB_FLAGGED(tch, MOB_ENCOUNTER))
      return true;
  }
  return false;
}