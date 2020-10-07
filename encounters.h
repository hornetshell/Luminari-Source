
#define ENCOUNTER_CLASS_NONE               0
#define ENCOUNTER_CLASS_COMBAT             1

#define NUM_ENCOUNTER_CLASSES              2

#define ENCOUNTER_TYPE_NONE                 0
#define ENCOUNTER_TYPE_GOBLIN_1             1
#define ENCOUNTER_TYPE_GOBLIN_2             2
#define ENCOUNTER_TYPE_KING_CRAB_1          3
#define ENCOUNTER_TYPE_KOBOLD_1             4
#define ENCOUNTER_TYPE_GIANT_RAT_1          5
#define ENCOUNTER_TYPE_SEWER_CENTIPEDE      6
#define ENCOUNTER_TYPE_SEA_WASP_JELLYFISH   7
#define ENCOUNTER_TYPE_MANDRILL             8
#define ENCOUNTER_TYPE_MUCKDWELLER          9
#define ENCOUNTER_TYPE_BARRACUDA            10
#define ENCOUNTER_TYPE_FIRE_BEETLE          11
#define ENCOUNTER_TYPE_DROW_WARRIOR         12
#define ENCOUNTER_TYPE_DROW_PRIESTESS       13
#define ENCOUNTER_TYPE_DROW_WIZARD          14
#define ENCOUNTER_TYPE_DUERGAR_WARRIOR      15
#define ENCOUNTER_TYPE_HAWK                 16
#define ENCOUNTER_TYPE_MERFOLK_HUNTER       17
#define ENCOUNTER_TYPE_ORCISH_WARRIOR       18
#define ENCOUNTER_TYPE_OWL                  19
#define ENCOUNTER_TYPE_RATFOLK_WANDERER     20
#define ENCOUNTER_TYPE_SEAL                 21
#define ENCOUNTER_TYPE_WANDERING_SKELETON   22
#define ENCOUNTER_TYPE_SPRITE               23
#define ENCOUNTER_TYPE_GIANT_MOSQUITO       24
#define ENCOUNTER_TYPE_ANTELOPE             25
#define ENCOUNTER_TYPE_BABOON               26
#define ENCOUNTER_TYPE_BADGER               27
#define ENCOUNTER_TYPE_DOLPHIN              28

#define NUM_ENCOUNTER_TYPES                 29

#define ENCOUNTER_GROUP_TYPE_NONE           0
#define ENCOUNTER_GROUP_TYPE_GOBLINS        1
#define ENCOUNTER_GROUP_TYPE_KING_CRABS     2
#define ENCOUNTER_GROUP_TYPE_KOBOLDS        3
#define ENCOUNTER_GROUP_TYPE_GIANT_RATS     4
#define ENCOUNTER_GROUP_TYPE_CENTIPEDES     5
#define ENCOUNTER_GROUP_TYPE_SEA_WASP_JELLYFISH 6
#define ENCOUNTER_GROUP_TYPE_MANDRILL       7
#define ENCOUNTER_GROUP_TYPE_MUCKDWELLER    8
#define ENCOUNTER_GROUP_TYPE_BARRACUDA      9
#define ENCOUNTER_GROUP_TYPE_FIRE_BEETLE    10
#define ENCOUNTER_GROUP_TYPE_DROW_PATROL    11
#define ENCOUNTER_GROUP_TYPE_DUERGAR_PATROL 12
#define ENCOUNTER_GROUP_TYPE_HAWK           13
#define ENCOUNTER_GROUP_TYPE_MERFOLK        14
#define ENCOUNTER_GROUP_TYPE_ORC_PATROL     15
#define ENCOUNTER_GROUP_TYPE_OWL            16
#define ENCOUNTER_GROUP_TYPE_RATFOLK        17
#define ENCOUNTER_GROUP_TYPE_SEAL           18
#define ENCOUNTER_GROUP_TYPE_WANDERING_SKELETON 19
#define ENCOUNTER_GROUP_TYPE_SPRITE         20
#define ENCOUNTER_GROUP_TYPE_GIANT_MOSQUITO 21
#define ENCOUNTER_GROUP_TYPE_ANTELOPE       22
#define ENCOUNTER_GROUP_TYPE_BABOON         23
#define ENCOUNTER_GROUP_TYPE_BADGER         24
#define ENCOUNTER_GROUP_TYPE_DOLPHIN        25

#define NUM_ENCOUNTER_GROUP_TYPES           26

#define ENCOUNTER_STRENGTH_NORMAL          0
#define ENCOUNTER_STRENGTH_BOSS            1

#define NUM_ENCOUNTER_STRENGTHS            2

#define TREASURE_TABLE_NONE                0
#define TREASURE_TABLE_LOW_NORM            1
#define TREASURE_TABLE_LOW_BOSS            2
#define TREASURE_TABLE_LOW_MID_NORM        3
#define TREASURE_TABLE_LOW_MID_BOSS        4
#define TREASURE_TABLE_MID_NORM            5
#define TREASURE_TABLE_MID_BOSS            6
#define TREASURE_TABLE_MID_HIGH_NORM       7
#define TREASURE_TABLE_MID_HIGH_BOSS       8
#define TREASURE_TABLE_HIGH_NORM           9
#define TREASURE_TABLE_HIGH_BOSS           10
#define TREASURE_TABLE_EPIC_LOW_NORM       11
#define TREASURE_TABLE_EPIC_LOW_BOSS       12
#define TREASURE_TABLE_EPIC_MID_NORM       13
#define TREASURE_TABLE_EPIC_MID_BOSS       14
#define TREASURE_TABLE_EPIC_HIGH_NORM      15
#define TREASURE_TABLE_EPIC_HIGH_BOSS      16

#define NUM_TREASURE_TABLES                17

#define ENCOUNTER_MOB_VNUM                 8100

struct encounter_data {

  int encounter_type;
  int min_level;
  int max_level;
  int sector_types[NUM_ROOM_SECTORS];
  int encounter_group;
  const char * object_name;
  int load_chance;
  int min_number;
  int max_number;
  int treasure_table;
  int char_class;
  int encounter_strength;
  int alignment;
  int race_type;
  int subrace[3];
  int size;
};

extern struct encounter_data encounter_table[NUM_ENCOUNTER_TYPES];

int get_exploration_dc(struct char_data *ch);
bool in_encounter_room(struct char_data *ch);
void check_random_encounter(struct char_data *ch);
int encounter_chance(struct char_data *ch);
void populate_encounter_table(void);
void add_encounter_record(int encounter_record, int encounter_type, int min_level, int max_level, int encounter_group, const char *object_name, 
    int load_chance, int min_number, int max_number, int treasure_table, int char_class, int encounter_strength,
    int alignment, int race_type, int subrace1, int subrace2, int subrace3, int size);