/* Use
 *
 * gcc -c fight_lst.cpp -o fight_lst.o
 * objcopy -O binary fight_lst.o FIGHT.LST
 *
 * to create the file FIGHT.LST which can be packed into SCHICK.DAT.
 */

#include "v302de.h"
#include "common.h"
#include "datseg.h"

struct fight_lst {
	int16_t number_of_entries;
	struct fight fights[FIGHT_ID__END];
};

struct fight_lst fight_data = {
	FIGHT_ID__END, // number
	{
		{ // FIGHT_ID_ZUFALL1_LAND == 0
			"ZUFALL1_LAND",
			0, // intro_seen
			SCENARIO_ID_ZUFALL1_LAND,
			{
				{ MONSTER_ID_OGER, 10, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_1, 5, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 8, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_1, 12, 14, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_HARPYIE, 22, 9, FIG_VIEWDIR_LEFT, 3 } // enemy 4
			},
			{
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 11, 5, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 4, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 10, 4, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 7, 1, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 16, 5, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 11, 0, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_KURZSCHWERT, // loot 0
				ITEM_ID_SCHILD, // loot 1
				ITEM_ID_KRAFTGUERTEL // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_LAND_FIGHT1_1 == 1
			"LAND_FIGHT1.1",
			0, // intro_seen
			SCENARIO_ID_LAND_ZUFALL1,
			{
				{ MONSTER_ID_ORK__LVL_1, 6, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_1, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 7, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_OGER, 6, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_OGER, 14, 1, FIG_VIEWDIR_UP, 0 }, // enemy 4
				{ MONSTER_ID_ORK__LVL_1, 13, 6, FIG_VIEWDIR_LEFT, 0 }, // enemy 5
				{ MONSTER_ID_ORK__LVL_1, 14, 5, FIG_VIEWDIR_LEFT, 0 }, // enemy 6
				{ MONSTER_ID_ORK__LVL_1, 11, 5, FIG_VIEWDIR_LEFT, 0 }, // enemy 7
				{ MONSTER_ID_GOBLIN__LVL_1, 13, 4, FIG_VIEWDIR_LEFT, 0 }, // enemy 8
				{ MONSTER_ID_WALDSCHRAT, 4, 7, FIG_VIEWDIR_RIGHT, 2 } // enemy 9
			},
			{
				{ 6, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 10, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 8, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 6, 6, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_SILBERSCHMUCK, // loot 4
				ITEM_ID_WURFMESSER, // loot 5
				ITEM_ID_SILBERSCHMUCK // loot 6
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_ZUFALL2_LAND == 2
			"ZUFALL2_LAND",
			0, // intro_seen
			SCENARIO_ID_LAND_ZUFALL1,
			{
				{ MONSTER_ID_ORK__LVL_1, 6, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_1, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 7, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_OGER, 6, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_OGER, 14, 1, FIG_VIEWDIR_UP, 0 }, // enemy 4
				{ MONSTER_ID_ORK__LVL_1, 13, 6, FIG_VIEWDIR_LEFT, 0 }, // enemy 5
				{ MONSTER_ID_ORK__LVL_1, 14, 5, FIG_VIEWDIR_LEFT, 0 }, // enemy 6
				{ MONSTER_ID_ORK__LVL_1, 11, 5, FIG_VIEWDIR_LEFT, 0 }, // enemy 7
				{ MONSTER_ID_GOBLIN__LVL_1, 13, 4, FIG_VIEWDIR_LEFT, 0 }, // enemy 8
				{ MONSTER_ID_WALDSCHRAT, 4, 7, FIG_VIEWDIR_RIGHT, 2 } // enemy 9
			},
			{
				{ 6, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 10, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 8, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 6, 6, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_SILBERSCHMUCK, // loot 4
				ITEM_ID_SILBERSCHMUCK, // loot 5
				ITEM_ID_WURFMESSER, // loot 6
				ITEM_ID_WURFMESSER // loot 7
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_ZUFALL3_LAND == 3
			"ZUFALL3_LAND",
			0, // intro_seen
			SCENARIO_ID_LAND_ZUFALL1,
			{
				{ MONSTER_ID_ORK__LVL_1, 7, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_STREUNER__LVL_1, 8, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 8, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_GOBLIN__LVL_1, 9, 12, FIG_VIEWDIR_DOWN, 0 } // enemy 3
			},
			{
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 5, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 4, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 10, 4, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 9, 2, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 5, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 7, 2, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_HEILTRANK, // loot 0
				ITEM_ID_KURZSCHWERT, // loot 1
				ITEM_ID_LEDERHARNISCH, // loot 2
				ITEM_ID_SAEBEL, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_SAEBEL, // loot 6
				ITEM_ID_SAEBEL // loot 7
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_ZUFALL4_LAND == 4
			"ZUFALL4_LAND",
			0, // intro_seen
			SCENARIO_ID_LAND_ZUFALL1,
			{
				{ MONSTER_ID_ORK__LVL_1, 9, 0, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_1, 6, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 7, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_1, 9, 12, FIG_VIEWDIR_DOWN, 0 } // enemy 3
			},
			{
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 11, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 12, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 13, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 13, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 13, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 14, 5, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SAEBEL, // loot 2
				ITEM_ID_ZAUBERTRANK // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP3 == 5
			"SHIP3",
			0, // intro_seen
			SCENARIO_ID_SHIP3,
			{
				{ MONSTER_ID_ZOMBIE, 6, 5, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 9, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 10, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 13, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 14, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 15, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 17, 10, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP4 == 6
			"SHIP4",
			0, // intro_seen
			SCENARIO_ID_SHIP4,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 5, 13, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 6, 10, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 6, 8, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 4, 9, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 3, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 4, 7, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP5 == 7
			"SHIP5",
			0, // intro_seen
			SCENARIO_ID_SHIP5,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 13, 7, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 6, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 5, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 4, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 3, 10, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP6 == 8
			"SHIP6",
			0, // intro_seen
			SCENARIO_ID_SHIP6,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 18, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_1, 19, 10, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 14, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 12, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 15, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 11, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 14, 16, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 12, 16, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 10, 17, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_SAEBEL // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP8 == 9
			"SHIP8",
			0, // intro_seen
			SCENARIO_ID_SHIP8,
			{
				{ MONSTER_ID_ZOMBIE, 8, 9, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 7, 7, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 5, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 5, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 4, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 4, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 3, 7, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP9 == 10
			"SHIP9",
			0, // intro_seen
			SCENARIO_ID_SHIP9,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 8, 10, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 9, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 8, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 8, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 9, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 10, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 15, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 15, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP12 == 11
			"SHIP12",
			0, // intro_seen
			SCENARIO_ID_SHIP12,
			{
				{ MONSTER_ID_SKELETT__LVL_2, 10, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_2, 11, 8, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 5, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 4, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 4, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 3, 5, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 4, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 3, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 2, 4, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_KETTENHEMD, // loot 1
				ITEM_ID_SCHWERT, // loot 2
				ITEM_ID_KETTENHEMD // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP14 == 12
			"SHIP14",
			0, // intro_seen
			SCENARIO_ID_SHIP14,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 6, 9, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 8, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 8, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 7, 7, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 6, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 7, 6, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 8, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 9, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP15 == 13
			"SHIP15",
			0, // intro_seen
			SCENARIO_ID_SHIP15,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 11, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 7, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 7, 6, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 4, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 6, 5, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP17 == 14
			"SHIP17",
			0, // intro_seen
			SCENARIO_ID_SHIP17,
			{
				{ MONSTER_ID_ZOMBIE, 10, 13, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_MUMIE__LVL_1, 8, 8, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 7, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 5, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 4, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 4, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 3, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 2, 11, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP18 == 15
			"SHIP18",
			0, // intro_seen
			SCENARIO_ID_SHIP18,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 8, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_NONE, 8, 6, FIG_VIEWDIR_RIGHT, 0 } // enemy 1
			},
			{
				{ 14, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 15, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 16, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 16, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 17, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 18, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 19, 8, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP19 == 16
			"SHIP19",
			0, // intro_seen
			SCENARIO_ID_SHIP19,
			{
				{ MONSTER_ID_ZOMBIE, 6, 7, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 6, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 8, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 7, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 8, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 9, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 6, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 9, 9, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP21 == 17
			"SHIP21",
			0, // intro_seen
			SCENARIO_ID_SHIP19,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 9, 7, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 7, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 8, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 9, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 6, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 6, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 8, 10, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP22 == 18
			"SHIP22",
			0, // intro_seen
			SCENARIO_ID_SHIP22,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 9, 7, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 8, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 5, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 3, 6, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP23A == 19
			"SHIP23A",
			0, // intro_seen
			SCENARIO_ID_SHIP23A,
			{
				{ MONSTER_ID_ZOMBIE, 18, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 19, 7, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 5, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 4, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 3, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 3, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 2, 6, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 4, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 2, 5, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP23B == 20
			"SHIP23B",
			0, // intro_seen
			SCENARIO_ID_SHIP23B,
			{
				{ MONSTER_ID_ZOMBIE, 8, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 19, 8, FIG_VIEWDIR_LEFT, 1 } // enemy 1
			},
			{
				{ 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 3, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 4, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 2, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 1, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 3, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 2, 10, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP24 == 21
			"SHIP24",
			0, // intro_seen
			SCENARIO_ID_SHIP24,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 12, 15, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_1, 10, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 13, 13, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_ZOMBIE, 3, 4, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_MUMIE__LVL_2, 12, 5, FIG_VIEWDIR_LEFT, 0 } // enemy 4
			},
			{
				{ 7, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 5, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 9, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 6, 2, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_AMULETT__FEUERBANN // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP10 == 22
			"SHIP10",
			0, // intro_seen
			SCENARIO_ID_SHIP10,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 10, 3, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 11, 4, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 2, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 4, 13, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 6, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 5, 15, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 1, 16, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 10, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 9, 12, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F061_2 == 23
			"F0612",
			0, // intro_seen
			SCENARIO_ID_F061_2,
			{
				{ MONSTER_ID_GRIMWOLF__LVL_1, 5, 21, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_GRIMWOLF__LVL_1, 3, 22, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 9, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 11, 9, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 7, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 3, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 5, 9, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 6, 6, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 1, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F061_3 == 24
			"F0613",
			0, // intro_seen
			SCENARIO_ID_F061_3,
			{
				{ MONSTER_ID_GRIMWOLF__LVL_1, 9, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_GRIMWOLF__LVL_1, 11, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_GRIMWOLF__LVL_1, 10, 12, FIG_VIEWDIR_DOWN, 0 } // enemy 2
			},
			{
				{ 6, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 8, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 5, 7, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 2, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 2, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 4, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 4, 10, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F061_4B == 25
			"F0614B",
			0, // intro_seen
			SCENARIO_ID_F046_1B,
			{
				{ MONSTER_ID_GRIMWOLF__LVL_1, 2, 3, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_GRIMWOLF__LVL_1, 3, 5, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_GRIMWOLF__LVL_3, 3, 8, FIG_VIEWDIR_DOWN, 0 } // enemy 2
			},
			{
				{ 14, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 18, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 11, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 8, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 12, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 14, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 13, 6, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F061_4A == 26
			"F0614A",
			0, // intro_seen
			SCENARIO_ID_F061_4B,
			{
				{ MONSTER_ID_GRIMWOLF__LVL_1, 2, 3, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_GRIMWOLF__LVL_1, 3, 5, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_NONE, 3, 8, FIG_VIEWDIR_DOWN, 0 } // enemy 2
			},
			{
				{ 14, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 18, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 11, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 8, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 12, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 14, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 13, 6, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F061_5 == 27
			"F0615",
			0, // intro_seen
			SCENARIO_ID_F061_5,
			{
				{ MONSTER_ID_GRIMWOLF__LVL_1, 3, 3, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_GRIMWOLF__LVL_1, 3, 4, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_GRIMWOLF__LVL_1, 4, 5, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_GRIMWOLF__LVL_1, 6, 3, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_GRIMWOLF__LVL_1, 3, 6, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_GRIMWOLF__LVL_1, 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 5
				{ MONSTER_ID_GRIMWOLF__LVL_3, 11, 11, FIG_VIEWDIR_LEFT, 10 }, // enemy 6
				{ MONSTER_ID_GRIMWOLF__LVL_3, 10, 12, FIG_VIEWDIR_LEFT, 10 }, // enemy 7
				{ MONSTER_ID_GRIMWOLF__LVL_1, 12, 9, FIG_VIEWDIR_DOWN, 10 }, // enemy 8
				{ MONSTER_ID_GRIMWOLF__LVL_1, 11, 10, FIG_VIEWDIR_LEFT, 10 } // enemy 9
			},
			{
				{ 8, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 7, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 9, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 7, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 10, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 8, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 9, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F061_6A == 28
			"F0616",
			0, // intro_seen
			SCENARIO_ID_F061_6,
			{
				{ MONSTER_ID_GRIMWOLF__LVL_3, 4, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_GRIMWOLF__LVL_3, 8, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_GRIMWOLF__LVL_3, 9, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_GRIMWOLF__LVL_3, 6, 9, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_GRIMWOLF__LVL_1, 10, 4, FIG_VIEWDIR_LEFT, 15 }, // enemy 4
				{ MONSTER_ID_GRIMWOLF__LVL_1, 12, 5, FIG_VIEWDIR_LEFT, 15 }, // enemy 5
				{ MONSTER_ID_GRIMWOLF__LVL_1, 10, 7, FIG_VIEWDIR_UP, 15 }, // enemy 6
				{ MONSTER_ID_GRIMWOLF__LVL_1, 12, 8, FIG_VIEWDIR_UP, 15 }, // enemy 7
				{ MONSTER_ID_GRIMWOLF__LVL_1, 15, 10, FIG_VIEWDIR_LEFT, 15 }, // enemy 8
				{ MONSTER_ID_GRIMWOLF__LVL_1, 14, 9, FIG_VIEWDIR_LEFT, 15 } // enemy 9
			},
			{
				{ 6, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 7, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 7, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 11, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 2, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 10, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F108_1 == 29
			"F1081",
			0, // intro_seen
			SCENARIO_ID_F108_1,
			{
				{ MONSTER_ID_ORK__LVL_3, 1, 5, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_3, 2, 4, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 7, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 8, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 11, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 11, 8, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_STREITKOLBEN, // loot 0
				ITEM_ID_STREITKOLBEN, // loot 1
				ITEM_ID_SCHILD, // loot 2
				ITEM_ID_SCHILD, // loot 3
				ITEM_ID_LEDERHELM, // loot 4
				ITEM_ID_LEDERHELM // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F108_2 == 30
			"F1082",
			0, // intro_seen
			SCENARIO_ID_F108_2,
			{
				{ MONSTER_ID_ORK__LVL_2, 6, 16, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_2, 8, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_2, 4, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_2, 6, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_ORK__LVL_3, 5, 9, FIG_VIEWDIR_DOWN, 0 } // enemy 4
			},
			{
				{ 5, 5, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 5, 4, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 2, 4, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 6, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 3, 3, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 6, 1, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHILD, // loot 0
				ITEM_ID_STREITKOLBEN, // loot 1
				ITEM_ID_EISENHELM, // loot 2
				ITEM_ID_SCHWERT, // loot 3
				ITEM_ID_SCHWERT, // loot 4
				ITEM_ID_SCHWERT, // loot 5
				ITEM_ID_SCHWERT // loot 6
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F108_3A == 31
			"F1083A",
			0, // intro_seen
			SCENARIO_ID_F108_3,
			{
				{ MONSTER_ID_ORK__LVL_2, 15, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_2, 12, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_2, 12, 13, FIG_VIEWDIR_DOWN, 0 } // enemy 2
			},
			{
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 9, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 7, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 9, 3, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_SCHWERT // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F108_3B == 32
			"F1083B",
			0, // intro_seen
			SCENARIO_ID_F108_3,
			{
				{ MONSTER_ID_ORK__LVL_2, 12, 16, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_2, 11, 14, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_2, 12, 14, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_2, 10, 13, FIG_VIEWDIR_LEFT, 0 } // enemy 3
			},
			{
				{ 8, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 8, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 17, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 4, 16, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 2, 17, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 8, 14, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 1, 15, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_SCHWERT, // loot 2
				ITEM_ID_SCHWERT // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F108_3C == 33
			"F1083C",
			0, // intro_seen
			SCENARIO_ID_F108_3,
			{
				{ MONSTER_ID_ORK__LVL_2, 12, 6, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_2, 14, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_2, 15, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_2, 16, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_NONE, 16, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 4
				{ MONSTER_ID_NONE, 16, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 5
				{ MONSTER_ID_NONE, 20, 8, FIG_VIEWDIR_LEFT, 15 }, // enemy 6
				{ MONSTER_ID_NONE, 20, 8, FIG_VIEWDIR_LEFT, 15 } // enemy 7
			},
			{
				{ 6, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 8, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 3, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 5, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 4, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 6, 3, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 6, 7, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_SCHWERT, // loot 2
				ITEM_ID_SCHWERT, // loot 3
				ITEM_ID_SCHWERT // loot 4
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F108_4 == 34
			"F1084",
			0, // intro_seen
			SCENARIO_ID_F108_4,
			{
				{ MONSTER_ID_ORK__LVL_2, 7, 8, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_2, 9, 9, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_2, 7, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_2, 9, 18, FIG_VIEWDIR_LEFT, 0 } // enemy 3
			},
			{
				{ 5, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 7, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 4, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 4, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 4, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 3, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 1, 13, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_SCHWERT, // loot 2
				ITEM_ID_SCHWERT // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F108_6 == 35
			"F1086",
			0, // intro_seen
			SCENARIO_ID_F108_6,
			{
				{ MONSTER_ID_ORK__LVL_2, 10, 5, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 6, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 4, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 7, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 4, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 1, 9, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 3, 17, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 0, 15, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F108_9 == 36
			"F1089",
			0, // intro_seen
			SCENARIO_ID_F108_9,
			{
				{ MONSTER_ID_ORK__LVL_2, 9, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_2, 10, 13, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_2, 7, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_OGER, 6, 7, FIG_VIEWDIR_RIGHT, 0 } // enemy 3
			},
			{
				{ 11, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 12, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 11, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 13, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 14, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 16, 4, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_STREITKOLBEN, // loot 1
				ITEM_ID_STREITKOLBEN, // loot 2
				ITEM_ID_EISENHELM, // loot 3
				ITEM_ID_EISENHELM, // loot 4
				ITEM_ID_EISENHELM, // loot 5
				ITEM_ID_STREITKOLBEN, // loot 6
				ITEM_ID_SCHILD, // loot 7
				ITEM_ID_SCHILD, // loot 8
				ITEM_ID_SCHILD, // loot 9
				ITEM_ID_BRONZESCHLUESSEL // loot 10
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F108_7 == 37
			"F1087",
			0, // intro_seen
			SCENARIO_ID_F108_7,
			{
				{ MONSTER_ID_ORK__LVL_2, 7, 13, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_2, 6, 15, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 5, 18, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 6, 19, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 7, 19, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 4, 20, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 6, 20, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 6, 21, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 5, 22, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_05_4 == 38
			"F05105.4",
			0, // intro_seen
			SCENARIO_ID_F051_05_4,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 7, 9, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 10, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 11, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 10, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 11, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 11, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 11, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_05_3 == 39
			"F05105.3",
			0, // intro_seen
			SCENARIO_ID_F051_5_3,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 12, 10, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 8, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 6, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 9, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 8, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 10, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 7, 11, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 8, 12, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_KRISTALL // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_05_2 == 40
			"F05105.2",
			0, // intro_seen
			SCENARIO_ID_F051_5_2,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 11, 12, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 8, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 7, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 8, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 9, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 7, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 9, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 8, 12, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_KRISTALL // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_05_1 == 41
			"F05105.1",
			0, // intro_seen
			SCENARIO_ID_F051_5_1,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 11, 11, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 8, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 7, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 9, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 7, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 7, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 9, 11, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 7, 13, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_KRISTALL // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_07 == 42
			"F05107",
			0, // intro_seen
			SCENARIO_ID_F051_07,
			{
				{ MONSTER_ID_KULTIST__LVL_3__1, 9, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_3__1, 8, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_KULTIST__LVL_3__1, 9, 10, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_KULTIST__LVL_3__1, 10, 11, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_PRIESTER__LVL_3, 11, 13, FIG_VIEWDIR_LEFT, 0 } // enemy 4
			},
			{
				{ 4, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 3, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 3, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 3, 13, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 2, 14, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 2, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 1, 11, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_VULKANGLASDOLCH, // loot 4
				ITEM_ID_ARAXGIFT, // loot 5
				ITEM_ID_ARAXGIFT, // loot 6
				ITEM_ID_ARAXGIFT, // loot 7
				ITEM_ID_ARAXGIFT // loot 8
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_09 == 43
			"F05109",
			0, // intro_seen
			SCENARIO_ID_F051_09,
			{
				{ MONSTER_ID_KULTIST__LVL_3__1, 9, 7, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 5, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 2, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 4, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 2, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 3, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 3, 12, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_ARAXGIFT // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_13 == 44
			"F05113",
			0, // intro_seen
			SCENARIO_ID_F051_13,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 6, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 11, 12, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_KULTIST__LVL_3__1, 5, 5, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_KULTIST__LVL_3__1, 11, 4, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 11, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 10, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 11, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 12, 10, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 13, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 14, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_ARAXGIFT, // loot 2
				ITEM_ID_ARAXGIFT // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F108_15 == 45
			"F10815",
			0, // intro_seen
			SCENARIO_ID_F108_15,
			{
				{ MONSTER_ID_WALDSPINNE, 3, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_WALDSPINNE, 10, 8, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_WALDSPINNE, 3, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 8, 4, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 4, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 6, 3, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 8, 3, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 10, 1, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 8, 2, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_15 == 46
			"F05115",
			0, // intro_seen
			SCENARIO_ID_F051_15,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 3, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 10, 8, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 3, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 8, 4, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 4, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 6, 3, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 8, 3, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 10, 1, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 8, 2, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_16 == 47
			"F05116",
			0, // intro_seen
			SCENARIO_ID_F051_16,
			{
				{ MONSTER_ID_WALDSPINNE, 6, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_WALDSPINNE, 7, 17, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_WALDSPINNE, 8, 16, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_GROSSE_SCHROETER, 10, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_GROSSE_SCHROETER, 2, 19, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_GROSSE_SCHROETER, 12, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 5
				{ MONSTER_ID_WALDSCHRAT, 7, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 6
				{ MONSTER_ID_HESHTHOT__LVL_1, 2, 10, FIG_VIEWDIR_RIGHT, 0 } // enemy 7
			},
			{
				{ 6, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 7, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 11, 8, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 6, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 6, 3, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_16_1 == 48
			"F05116.1",
			0, // intro_seen
			SCENARIO_ID_F051_16_1,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 10, 4, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 2, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_HOHEPRIESTER, 0, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 11, 13, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 2, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_HOEHLENSPINNE__LVL_3, 12, 3, FIG_VIEWDIR_LEFT, 0 }, // enemy 5
				{ MONSTER_ID_KULTIST__LVL_3__1, 14, 12, FIG_VIEWDIR_LEFT, 0 }, // enemy 6
				{ MONSTER_ID_KULTIST__LVL_3__1, 13, 11, FIG_VIEWDIR_LEFT, 0 }, // enemy 7
				{ MONSTER_ID_PRIESTER__LVL_3, 14, 4, FIG_VIEWDIR_LEFT, 0 } // enemy 8
			},
			{
				{ 6, 13, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 7, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 4, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 4, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 6, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 5, 11, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_VULKANGLASDOLCH, // loot 2
				ITEM_ID_ARAXGIFT, // loot 3
				ITEM_ID_ARAXGIFT // loot 4
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_03 == 49
			"F05103",
			0, // intro_seen
			SCENARIO_ID_F051_2,
			{
				{ MONSTER_ID_KULTIST__LVL_3__1, 9, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_3__1, 10, 4, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_NONE, 11, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_NONE, 11, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_NONE, 6, 12, FIG_VIEWDIR_LEFT, 0 } // enemy 4
			},
			{
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 4, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 5, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 4, 6, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_ARAXGIFT, // loot 0
				ITEM_ID_ARAXGIFT, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_DOLCH // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_04 == 50
			"F05104",
			0, // intro_seen
			SCENARIO_ID_F051_4,
			{
				{ MONSTER_ID_PRIESTER__LVL_3, 10, 9, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 7, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 8, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 9, 6, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 6, 6, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_VULKANGLASDOLCH // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_17 == 51
			"F05117",
			0, // intro_seen
			SCENARIO_ID_F051_17,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 5, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 6, 13, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 7, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 5, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 4, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 6, 8, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 3, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_18 == 52
			"F05118",
			0, // intro_seen
			SCENARIO_ID_F051_18,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 8, 15, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 4, 6, FIG_VIEWDIR_UP, 0 } // enemy 2
			},
			{
				{ 4, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 5, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 6, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 4, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 2, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 4, 16, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_19 == 53
			"F05119",
			0, // intro_seen
			SCENARIO_ID_F051_19,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 8, 15, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 13, 17, FIG_VIEWDIR_UP, 1 } // enemy 1
			},
			{
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 8, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 10, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 8, 7, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 9, 2, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_20_2 == 54
			"F05120.2",
			0, // intro_seen
			SCENARIO_ID_F051_20_2,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 2, 5, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 5, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 8, 9, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 12, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 9, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 7, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 12, 5, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_18_3 == 55
			"F05118.3",
			0, // intro_seen
			SCENARIO_ID_F051_18_3,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 10, 8, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 9, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 8, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 7, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 8, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 6, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 5, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 6, 3, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_01 == 56
			"F04601",
			0, // intro_seen
			SCENARIO_ID_F046_01,
			{
				{ MONSTER_ID_ZOMBIE, 2, 15, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 3, 17, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 4, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 6, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 7, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 9, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 2, 8, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 10, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 7, 5, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_04 == 57
			"F04604",
			0, // intro_seen
			SCENARIO_ID_F046_04,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 4, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_1, 15, 5, FIG_VIEWDIR_LEFT, 5 }, // enemy 1
				{ MONSTER_ID_SKELETT__LVL_1, 15, 3, FIG_VIEWDIR_LEFT, 5 } // enemy 2
			},
			{
				{ 4, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 5, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 4, 8, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 6, 8, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 6, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 8, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 8, 8, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_06 == 58
			"F04606",
			0, // intro_seen
			SCENARIO_ID_F046_06,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 7, 8, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 6, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 7, 5, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 6, 3, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 8, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 8, 1, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_07 == 59
			"F04607",
			0, // intro_seen
			SCENARIO_ID_F046_07,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 13, 9, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 10, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 9, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 7, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 6, 9, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_10 == 60
			"F04610",
			0, // intro_seen
			SCENARIO_ID_F046_10,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 7, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_MUMIE__LVL_1, 11, 12, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 6, 10, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 10, 10, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 11, 10, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 8, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SILBERSCHMUCK, // loot 0
				ITEM_ID_SILBERSCHMUCK, // loot 1
				ITEM_ID_SILBERSCHMUCK, // loot 2
				ITEM_ID_SILBERSCHMUCK__MAGIC, // loot 3
				ITEM_ID_SILBERSTREITKOLBEN // loot 4
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_12 == 61
			"F04612",
			0, // intro_seen
			SCENARIO_ID_F046_12,
			{
				{ MONSTER_ID_SKELETT__LVL_2, 8, 17, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 4, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 10, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 10, 9, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 7, 2, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 5, 8, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 8, 6, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_13 == 62
			"F04613",
			0, // intro_seen
			SCENARIO_ID_F046_13,
			{
				{ MONSTER_ID_ZOMBIE, 6, 22, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 8, 20, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 6, 20, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_ZOMBIE, 8, 19, FIG_VIEWDIR_DOWN, 0 } // enemy 3
			},
			{
				{ 6, 14, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 13, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 4, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 11, 13, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 2, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 3, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 7, 10, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_15 == 63
			"F04615",
			0, // intro_seen
			SCENARIO_ID_F046_15,
			{
				{ MONSTER_ID_ZOMBIE, 2, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 6, 9, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 13, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 14, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 15, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 16, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 16, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 16, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 18, 8, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_16 == 64
			"F04616",
			0, // intro_seen
			SCENARIO_ID_F046_16,
			{
				{ MONSTER_ID_ZOMBIE, 6, 9, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 10, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 12, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 12, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 13, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 13, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 15, 8, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_18 == 65
			"F04618",
			0, // intro_seen
			SCENARIO_ID_F046_18,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 7, 6, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 4, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 4, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 3, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 2, 13, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 2, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_22 == 66
			"F04622",
			0, // intro_seen
			SCENARIO_ID_F046_18,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 9, 6, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 9, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 11, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 11, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 12, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 13, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 14, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_24 == 67
			"F04624",
			0, // intro_seen
			SCENARIO_ID_F046_24,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 12, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_1, 12, 4, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 10, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 11, 9, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 11, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 11, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 12, 9, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 13, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 14, 8, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_25 == 68
			"F04625",
			0, // intro_seen
			SCENARIO_ID_F046_25,
			{
				{ MONSTER_ID_MAGIER__LVL_3, 2, 16, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 7, 11, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_ZOMBIE, 3, 4, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_ZOMBIE, 4, 3, FIG_VIEWDIR_LEFT, 0 }, // enemy 4
				{ MONSTER_ID_MUMIE__LVL_1, 12, 18, FIG_VIEWDIR_LEFT, 0 } // enemy 5
			},
			{
				{ 10, 15, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 18, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 8, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 7, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 8, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 9, 12, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_TOTENKOPFGUERTEL // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_26 == 69
			"F04626",
			0, // intro_seen
			SCENARIO_ID_F046_26,
			{
				{ MONSTER_ID_SKELETT__LVL_2, 9, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_2, 5, 8, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_SKELETT__LVL_2, 5, 13, FIG_VIEWDIR_RIGHT, 0 } // enemy 2
			},
			{
				{ 11, 14, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 13, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 7, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 6, 7, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 11, 12, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 7, 4, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_SCHWERT // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_26_5 == 70
			"F04626.5",
			0, // intro_seen
			SCENARIO_ID_F046_26_5,
			{
				{ MONSTER_ID_SKELETT__LVL_2, 9, 9, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 12, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 13, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 14, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 13, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 14, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 16, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 13, 14, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_27 == 71
			"F04627\0005", // sanitized: "F04627"
			0, // intro_seen
			SCENARIO_ID_F046_27,
			{
				{ MONSTER_ID_ZOMBIE, 1, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 2, 8, FIG_VIEWDIR_RIGHT, 0 } // enemy 1
			},
			{
				{ 13, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 14, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 15, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 16, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 16, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 15, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 17, 6, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_28 == 72
			"F04628",
			0, // intro_seen
			SCENARIO_ID_F046_28,
			{
				{ MONSTER_ID_FOLTERKNECHT, 13, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 8, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 8, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 7, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 4, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 5, 5, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_TOTENKOPFGUERTEL, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_PEITSCHE, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_HEILTRANK, // loot 4
				ITEM_ID_BIER, // loot 5
				ITEM_ID_GOLDSCHMUCK // loot 6
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046_31 == 73
			"F04631",
			0, // intro_seen
			SCENARIO_ID_F046_31,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 7, 8, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_MUMIE__LVL_1, 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_MUMIE__LVL_1, 12, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_MAGIER__LVL_3, 8, 5, FIG_VIEWDIR_UP, 0 }, // enemy 3
				{ MONSTER_ID_MAGIER__LVL_3, 11, 6, FIG_VIEWDIR_UP, 0 } // enemy 4
			},
			{
				{ 9, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 8, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 10, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 7, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 10, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 8, 10, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_TOTENKOPFGUERTEL, // loot 0
				ITEM_ID_TOTENKOPFGUERTEL // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F076_04 == 74
			"F07604",
			0, // intro_seen
			SCENARIO_ID_F076_04,
			{
				{ MONSTER_ID_GOBLIN__LVL_1, 6, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_GOBLIN__LVL_1, 7, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_GOBLIN__LVL_1, 8, 11, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_GOBLIN__LVL_1, 7, 4, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 9, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 6, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 4, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 4, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 4, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 2, 8, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_SAEBEL, // loot 2
				ITEM_ID_SAEBEL // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F076_06 == 75
			"F07606",
			0, // intro_seen
			SCENARIO_ID_F076_06,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 7, 8, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 7, 6, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 6, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 4, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 6, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 6, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 5, 5, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F076_07 == 76
			"F07607",
			0, // intro_seen
			SCENARIO_ID_F076_07,
			{
				{ MONSTER_ID_GOBLIN__LVL_2, 9, 13, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_GOBLIN__LVL_2, 11, 12, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_GOBLIN__LVL_2, 10, 11, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_GOBLIN__LVL_2, 4, 4, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 5, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 9, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 6, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 5, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 4, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 5, 13, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 2, 8, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_LEDERZEUG, // loot 0
				ITEM_ID_LEDERZEUG, // loot 1
				ITEM_ID_LEDERZEUG, // loot 2
				ITEM_ID_LEDERZEUG, // loot 3
				ITEM_ID_SAEBEL, // loot 4
				ITEM_ID_SAEBEL, // loot 5
				ITEM_ID_SAEBEL, // loot 6
				ITEM_ID_SAEBEL // loot 7
			},
			0, 15, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F076_10 == 77
			"F07610",
			0, // intro_seen
			SCENARIO_ID_F076_10,
			{
				{ MONSTER_ID_GROSSE_SCHROETER, 8, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_GROSSE_SCHROETER, 9, 14, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_GROSSE_SCHROETER, 8, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_GROSSE_SCHROETER, 10, 11, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_GROSSE_SCHROETER, 11, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 4
				{ MONSTER_ID_GROSSE_SCHROETER, 11, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 5
				{ MONSTER_ID_GROSSE_SCHROETER, 8, 5, FIG_VIEWDIR_UP, 0 }, // enemy 6
				{ MONSTER_ID_GROSSE_SCHROETER, 10, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 7
				{ MONSTER_ID_GROSSE_SCHROETER, 10, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 8
				{ MONSTER_ID_GROSSE_SCHROETER, 11, 5, FIG_VIEWDIR_LEFT, 0 } // enemy 9
			},
			{
				{ 8, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 8, 9, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 8, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 7, 9, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 7, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F076_11 == 78
			"F07611",
			0, // intro_seen
			SCENARIO_ID_F076_11,
			{
				{ MONSTER_ID_GOBLIN__LVL_1, 4, 14, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_GOBLIN__LVL_1, 5, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_GOBLIN__LVL_1, 7, 15, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_GOBLIN__LVL_1, 10, 8, FIG_VIEWDIR_DOWN, 0 } // enemy 3
			},
			{
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 11, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 12, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 13, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 13, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 15, 6, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_SCHWERT, // loot 2
				ITEM_ID_SCHWERT // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F076_13 == 79
			"F07613",
			0, // intro_seen
			SCENARIO_ID_F076_13,
			{
				{ MONSTER_ID_GOBLIN__LVL_2, 8, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_GOBLIN__LVL_2, 10, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_GOBLIN__LVL_1, 10, 12, FIG_VIEWDIR_DOWN, 0 } // enemy 2
			},
			{
				{ 8, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 7, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 11, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 9, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 10, 5, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_SAEBEL, // loot 2
				ITEM_ID_HELM__CURSED, // loot 3
				ITEM_ID_HELM__CURSED, // loot 4
				ITEM_ID_HELM__CURSED // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F094_02 == 80
			"F09402",
			0, // intro_seen
			SCENARIO_ID_F094_02,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 6, 4, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_2, 4, 5, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_2, 5, 4, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_2, 3, 9, FIG_VIEWDIR_RIGHT, 15 }, // enemy 3
				{ MONSTER_ID_PIRAT__LVL_2, 3, 7, FIG_VIEWDIR_RIGHT, 15 } // enemy 4
			},
			{
				{ 6, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 8, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 5, 11, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 6, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 9, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 10, 12, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_ENTERMESSER, // loot 0
				ITEM_ID_ENTERMESSER, // loot 1
				ITEM_ID_ENTERMESSER, // loot 2
				ITEM_ID_LEDERZEUG, // loot 3
				ITEM_ID_LEDERZEUG, // loot 4
				ITEM_ID_LEDERZEUG, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_AMULETT__ORDINARY, // loot 9
				ITEM_ID_SCHNAPSFLASCHE // loot 10
			},
			0, 30, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F094_05 == 81
			"F09405",
			0, // intro_seen
			SCENARIO_ID_F094_05,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 7, 3, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_2, 7, 5, FIG_VIEWDIR_RIGHT, 0 } // enemy 1
			},
			{
				{ 9, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 9, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 9, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 8, 2, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 9, 3, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 10, 2, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 9, 1, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_ENTERMESSER, // loot 2
				ITEM_ID_ENTERMESSER, // loot 3
				ITEM_ID_LEDERZEUG, // loot 4
				ITEM_ID_LEDERZEUG // loot 5
			},
			21, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F094_10 == 82
			"F09410",
			0, // intro_seen
			SCENARIO_ID_F094_10,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 11, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_2, 11, 10, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 8, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 7, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 7, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 5, 12, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 6, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 3, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 3, 10, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_ENTERMESSER, // loot 0
				ITEM_ID_ENTERMESSER, // loot 1
				ITEM_ID_LEDERZEUG, // loot 2
				ITEM_ID_LEDERZEUG // loot 3
			},
			0, 26, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F094_13 == 83
			"F09413",
			0, // intro_seen
			SCENARIO_ID_F094_13,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 8, 14, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_2, 6, 14, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 10, 10, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 11, 9, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 13, 9, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 13, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 13, 4, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_ENTERMESSER, // loot 0
				ITEM_ID_LEDERZEUG, // loot 1
				ITEM_ID_LEDERZEUG, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_ENTERMESSER // loot 5
			},
			0, 26, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F094_17 == 84
			"F09417",
			0, // intro_seen
			SCENARIO_ID_F094_17,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 4, 8, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_2, 4, 11, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 2, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 2, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 2, 13, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 1, 14, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 1, 13, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 1, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 0, 12, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_ENTERMESSER, // loot 2
				ITEM_ID_ENTERMESSER, // loot 3
				ITEM_ID_LEDERZEUG, // loot 4
				ITEM_ID_LEDERZEUG // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F094_19 == 85
			"F09419",
			0, // intro_seen
			SCENARIO_ID_F094_19,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 0, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_2, 1, 7, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_2, 15, 9, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_2, 16, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_NONE, 16, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 4
				{ MONSTER_ID_NONE, 16, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 5
			},
			{
				{ 10, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 9, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 9, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 8, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 8, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 7, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 6, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_ENTERMESSER, // loot 2
				ITEM_ID_ENTERMESSER, // loot 3
				ITEM_ID_LEDERZEUG, // loot 4
				ITEM_ID_LEDERZEUG, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_LEDERZEUG, // loot 8
				ITEM_ID_LEDERZEUG, // loot 9
				ITEM_ID_ENTERMESSER, // loot 10
				ITEM_ID_ENTERMESSER // loot 11
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F094_22 == 86
			"F09422",
			0, // intro_seen
			SCENARIO_ID_F094_22,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 1, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ESTORIK, 6, 4, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 11, 9, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 6, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 7, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 8, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 5, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 7, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 5, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 6, 13, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SCHNEIDZAHN, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_ORKNASE, // loot 3
				ITEM_ID_LEDERZEUG, // loot 4
				ITEM_ID_LEDERZEUG, // loot 5
				ITEM_ID_SCHNEIDZAHN, // loot 6
				ITEM_ID_SCHNEIDZAHN // loot 7
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F100_01 == 87
			"F10001",
			0, // intro_seen
			SCENARIO_ID_F100_01,
			{
				{ MONSTER_ID_ORK__LVL_2, 9, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_2, 10, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_2, 7, 21, FIG_VIEWDIR_DOWN, 8 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_2, 12, 18, FIG_VIEWDIR_DOWN, 8 }, // enemy 3
				{ MONSTER_ID_ORK__LVL_2, 9, 21, FIG_VIEWDIR_DOWN, 16 }, // enemy 4
				{ MONSTER_ID_ORK__LVL_2, 10, 20, FIG_VIEWDIR_DOWN, 16 }, // enemy 5
				{ MONSTER_ID_ORK__LVL_2, 15, 15, FIG_VIEWDIR_LEFT, 24 }, // enemy 6
				{ MONSTER_ID_ORK__LVL_2, 4, 22, FIG_VIEWDIR_DOWN, 24 }, // enemy 7
				{ MONSTER_ID_ORK__LVL_2, 15, 13, FIG_VIEWDIR_LEFT, 32 }, // enemy 8
				{ MONSTER_ID_ORK__LVL_2, 6, 22, FIG_VIEWDIR_DOWN, 27 } // enemy 9
			},
			{
				{ 4, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 5, 14, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 2, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 3, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 1, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 1, 20, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_SCHWERT, // loot 2
				ITEM_ID_SCHWERT, // loot 3
				ITEM_ID_SCHWERT, // loot 4
				ITEM_ID_SCHWERT, // loot 5
				ITEM_ID_SCHWERT, // loot 6
				ITEM_ID_SCHWERT, // loot 7
				ITEM_ID_SCHWERT, // loot 8
				ITEM_ID_SCHWERT, // loot 9
				ITEM_ID_BEUTEL // loot 10
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F100_03 == 88
			"F10003",
			0, // intro_seen
			SCENARIO_ID_F100_03,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 14, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_1, 15, 5, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_SKELETT__LVL_1, 13, 6, FIG_VIEWDIR_UP, 0 } // enemy 2
			},
			{
				{ 9, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 10, 16, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 11, 14, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 10, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 9, 13, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 8, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 12, 13, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F100_05 == 89
			"F10005",
			0, // intro_seen
			SCENARIO_ID_F100_05,
			{
				{ MONSTER_ID_ZAUBERER__LVL_3, 3, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_3, 2, 9, FIG_VIEWDIR_RIGHT, 0 } // enemy 1
			},
			{
				{ 6, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 7, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 8, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 10, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 12, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 9, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 10, 12, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_STARKER_HEILTRANK, // loot 0
				ITEM_ID_KETTENHEMD, // loot 1
				ITEM_ID_KHUNCHOMER, // loot 2
				ITEM_ID_STARKER_HEILTRANK, // loot 3
				ITEM_ID_ZAUBERTRANK_STARK // loot 4
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F100_12 == 90
			"F10012",
			0, // intro_seen
			SCENARIO_ID_F100_12,
			{
				{ MONSTER_ID_ORK__LVL_3, 3, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZAUBERER__LVL_3, 16, 12, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 5, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 6, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 7, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 6, 9, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 6, 8, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 9, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_KETTENHEMD, // loot 0
				ITEM_ID_KHUNCHOMER, // loot 1
				ITEM_ID_STARKER_HEILTRANK, // loot 2
				ITEM_ID_ZAUBERTRANK_STARK // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F100_13 == 91
			"F10013",
			0, // intro_seen
			SCENARIO_ID_F100_13,
			{
				{ MONSTER_ID_ZAUBERER__LVL_6, 13, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_NONE, 13, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_NONE, 13, 11, FIG_VIEWDIR_DOWN, 0 } // enemy 2
			},
			{
				{ 8, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 5, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 4, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 3, 13, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 3, 11, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_KUKRIS_MENGBILAR, // loot 0
				ITEM_ID_AMULETT__MR_BONUS // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_03 == 92
			"F12603",
			0, // intro_seen
			SCENARIO_ID_F126_03,
			{
				{ MONSTER_ID_KULTIST__LVL_2, 16, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 4, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 4, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 1, 7, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 3, 6, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 2, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 1, 9, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_07 == 93
			"F12607",
			0, // intro_seen
			SCENARIO_ID_F126_07,
			{
				{ MONSTER_ID_KULTIST__LVL_2, 2, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_2, 12, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 8, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 6, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 3, 4, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 4, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 6, 6, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_08 == 94
			"F12608",
			0, // intro_seen
			SCENARIO_ID_F126_08,
			{
				{ MONSTER_ID_KULTIST__LVL_3__2, 9, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_2, 10, 1, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_KULTIST__LVL_2, 9, 1, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_KULTIST__LVL_2, 12, 2, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_KULTIST__LVL_4, 10, 11, FIG_VIEWDIR_LEFT, 0 }, // enemy 4
				{ MONSTER_ID_KULTIST__LVL_3__2, 8, 20, FIG_VIEWDIR_DOWN, 0 } // enemy 5
			},
			{
				{ 8, 14, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 13, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 14, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 9, 10, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 9, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 8, 9, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 7, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_LEDERZEUG // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_09 == 95
			"F12609",
			0, // intro_seen
			SCENARIO_ID_F126_09,
			{
				{ MONSTER_ID_KULTIST__LVL_3__2, 8, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 8, 7, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 8, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 6, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 5, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 6, 7, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_11 == 96
			"F12611",
			0, // intro_seen
			SCENARIO_ID_F126_11,
			{
				{ MONSTER_ID_KULTIST__LVL_3__2, 9, 6, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 5, 20, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 5, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 7, 20, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 6, 13, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 19, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 8, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 9, 15, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_12 == 97
			"F12612",
			0, // intro_seen
			SCENARIO_ID_F046_01,
			{
				{ MONSTER_ID_KULTIST__LVL_2, 2, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_2, 5, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_KULTIST__LVL_2, 4, 14, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_KULTIST__LVL_3__2, 6, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_KULTIST__LVL_3__2, 9, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 4
			},
			{
				{ 9, 6, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 7, 5, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 4, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 6, 3, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_13 == 98
			"F12613",
			0, // intro_seen
			SCENARIO_ID_F126_13,
			{
				{ MONSTER_ID_KULTIST__LVL_3__2, 9, 8, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 9, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 8, 14, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 9, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 9, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 8, 18, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 8, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 7, 17, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_17 == 99
			"F12617",
			0, // intro_seen
			SCENARIO_ID_F126_17,
			{
				{ MONSTER_ID_KULTIST__LVL_4, 4, 4, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_4, 3, 3, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 4, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 5, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 6, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 3, 14, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 5, 14, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 3, 17, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 4, 19, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_LEDERZEUG, // loot 2
				ITEM_ID_LEDERZEUG // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_18 == 100
			"F12618",
			0, // intro_seen
			SCENARIO_ID_F126_18,
			{
				{ MONSTER_ID_KULTIST__LVL_2, 6, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_2, 12, 13, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_KULTIST__LVL_2, 8, 4, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_KULTIST__LVL_4, 10, 5, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 9, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 8, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 10, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 8, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 10, 13, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 8, 13, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 10, 12, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_LEDERZEUG // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_20 == 101
			"F12620",
			0, // intro_seen
			SCENARIO_ID_F128_20,
			{
				{ MONSTER_ID_PRIESTER__LVL_2, 3, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_4, 8, 10, FIG_VIEWDIR_RIGHT, 0 } // enemy 1
			},
			{
				{ 14, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 15, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 15, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 16, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 16, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 17, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 17, 12, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_LEDERZEUG, // loot 1
				ITEM_ID_VULKANGLASDOLCH // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_22 == 102
			"F12622",
			0, // intro_seen
			SCENARIO_ID_F126_22,
			{
				{ MONSTER_ID_HESHTHOT__LVL_1, 7, 10, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 7, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 10, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 10, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 12, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 11, 3, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_23 == 103
			"F12623",
			0, // intro_seen
			SCENARIO_ID_F126_23,
			{
				{ MONSTER_ID_KULTIST__LVL_3__2, 9, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_3__2, 19, 9, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 3, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 7, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 6, 9, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 5, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 1, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 4, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_25 == 104
			"F12625",
			0, // intro_seen
			SCENARIO_ID_F126_25,
			{
				{ MONSTER_ID_KULTIST__LVL_4, 8, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_4, 12, 11, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 6, 5, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 7, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 4, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 4, 2, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_LEDERZEUG, // loot 2
				ITEM_ID_LEDERZEUG // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_27 == 105
			"F12627",
			0, // intro_seen
			SCENARIO_ID_F126_27,
			{
				{ MONSTER_ID_KULTIST__LVL_4, 8, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_PRIESTER__LVL_2, 7, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_KULTIST__LVL_2, 15, 6, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_KULTIST__LVL_2, 15, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_KULTIST__LVL_4, 8, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_PRIESTER__LVL_2, 6, 8, FIG_VIEWDIR_RIGHT, 0 } // enemy 5
			},
			{
				{ 11, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 12, 12, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 13, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 12, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 15, 11, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 15, 9, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_LEDERZEUG, // loot 2
				ITEM_ID_LEDERZEUG, // loot 3
				ITEM_ID_VULKANGLASDOLCH, // loot 4
				ITEM_ID_VULKANGLASDOLCH // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F126_28 == 106
			"F12628",
			0, // intro_seen
			SCENARIO_ID_F126_28,
			{
				{ MONSTER_ID_KULTIST__LVL_2, 6, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_HOHEPRIESTER, 3, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_NONE, 10, 5, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_NONE, 10, 5, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 7, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 8, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 8, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 9, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 10, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 11, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_LEDERZEUG, // loot 1
				ITEM_ID_KUKRISDOLCH, // loot 2
				ITEM_ID_KRISTALLKUGEL, // loot 3
				ITEM_ID_GOLDSCHMUCK, // loot 4
				ITEM_ID_ROTER_SCHLUESSEL // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_05 == 107
			"F12905",
			0, // intro_seen
			SCENARIO_ID_F129_05,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 8, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_2, 10, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_2, 9, 5, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 6, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 5, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 4, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 5, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 3, 6, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 3, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 2, 5, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_LEDERZEUG, // loot 0
				ITEM_ID_LEDERZEUG, // loot 1
				ITEM_ID_LEDERZEUG, // loot 2
				ITEM_ID_ENTERMESSER, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_08 == 108
			"F12908",
			0, // intro_seen
			SCENARIO_ID_F129_08,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 8, 4, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 7, 16, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 6, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 5, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 3, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 3, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 2, 15, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_LEDERZEUG, // loot 1
				ITEM_ID_ENTERMESSER // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_09 == 109
			"F12909",
			0, // intro_seen
			SCENARIO_ID_F129_09,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 8, 4, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 7, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 7, 17, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 10, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 10, 16, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 10, 17, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 11, 16, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 11, 18, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_LEDERZEUG, // loot 1
				ITEM_ID_ENTERMESSER // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_18 == 110
			"F12918",
			0, // intro_seen
			SCENARIO_ID_F129_18,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 5, 4, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 9, 9, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 12, 5, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 8, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 9, 7, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 10, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 8, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 9, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 9, 3, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_LEDERZEUG, // loot 0
				ITEM_ID_LEDERZEUG, // loot 1
				ITEM_ID_LEDERZEUG, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_SKRAJA, // loot 4
				ITEM_ID_SKRAJA, // loot 5
				ITEM_ID_SCHNEIDZAHN, // loot 6
				ITEM_ID_SCHNEIDZAHN, // loot 7
				ITEM_ID_SCHNEIDZAHN // loot 8
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_21 == 111
			"F12921",
			0, // intro_seen
			SCENARIO_ID_F129_21,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 3, 4, FIG_VIEWDIR_UP, 1 }, // enemy 0
				{ MONSTER_ID_MUMIE__LVL_1, 13, 4, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_MUMIE__LVL_1, 13, 14, FIG_VIEWDIR_DOWN, 1 }, // enemy 2
				{ MONSTER_ID_MUMIE__LVL_1, 3, 14, FIG_VIEWDIR_DOWN, 0 } // enemy 3
			},
			{
				{ 3, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 5, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 9, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 12, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 13, 10, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 14, 8, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_23 == 112
			"F12923",
			0, // intro_seen
			SCENARIO_ID_F129_23,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 2, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_1, 4, 6, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 14, 13, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 14, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 15, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 13, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 14, 14, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 12, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 14, 16, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_24 == 113
			"F12924",
			0, // intro_seen
			SCENARIO_ID_F129_24,
			{
				{ MONSTER_ID_ZOMBIE, 3, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 5, 4, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 12, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 7, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 7, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 9, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 8, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 7, 10, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_25 == 114
			"F12925",
			0, // intro_seen
			SCENARIO_ID_F129_25,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 7, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 7, 6, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 6, 5, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 8, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 7, 3, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 9, 3, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_WUNDERKUR // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_27 == 115
			"F12927",
			0, // intro_seen
			SCENARIO_ID_F129_27,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 6, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_MUMIE__LVL_1, 9, 4, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 10, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 8, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 9, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 10, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 8, 12, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 9, 14, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 13, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_28 == 116
			"F12928",
			0, // intro_seen
			SCENARIO_ID_F129_28,
			{
				{ MONSTER_ID_ZOMBIE, 7, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 9, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 6, 7, FIG_VIEWDIR_RIGHT, 0 } // enemy 2
			},
			{
				{ 8, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 8, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 8, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 10, 7, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 9, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 10, 7, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 5, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_RING__GREEN // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_29 == 117
			"F12929",
			0, // intro_seen
			SCENARIO_ID_F129_29,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 11, 9, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 10, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 9, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_3, 7, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_PIRATENBOSS, 8, 9, FIG_VIEWDIR_DOWN, 0 } // enemy 4
			},
			{
				{ 8, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 7, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 7, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 11, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 6, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 9, 5, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_PLATINSCHLUESSEL, // loot 0
				ITEM_ID_LEDERZEUG, // loot 1
				ITEM_ID_LEDERZEUG, // loot 2
				ITEM_ID_LEDERZEUG, // loot 3
				ITEM_ID_SCHNEIDZAHN, // loot 4
				ITEM_ID_SCHNEIDZAHN, // loot 5
				ITEM_ID_SKRAJA, // loot 6
				ITEM_ID_SKRAJA, // loot 7
				ITEM_ID_SKRAJA, // loot 8
				ITEM_ID_SCHNEIDZAHN // loot 9
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_30 == 118
			"F12930",
			0, // intro_seen
			SCENARIO_ID_F129_30,
			{
				{ MONSTER_ID_HOEHLENDRACHE, 3, 11, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 9, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 11, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 12, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 13, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 14, 5, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_HEILTRANK, // loot 0
				ITEM_ID_HEILTRANK, // loot 1
				ITEM_ID_ZAUBERTRANK, // loot 2
				ITEM_ID_WURFDOLCH__MAGIC // loot 3
			},
			200, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_01A == 119
			"F13101A",
			0, // intro_seen
			SCENARIO_ID_F131_01A,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 10, 14, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 7, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 9, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 7, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 8, 4, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_01B == 120
			"F13101B",
			0, // intro_seen
			SCENARIO_ID_F131_01A,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 10, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 1, 9, FIG_VIEWDIR_RIGHT, 0 } // enemy 1
			},
			{
				{ 7, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 9, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 7, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 8, 4, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_SILBERSCHMUCK, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_SILBERSCHMUCK, // loot 4
				ITEM_ID_SAEBEL // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_04 == 121
			"F13104",
			0, // intro_seen
			SCENARIO_ID_F131_04,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 9, 16, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 9, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 9, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 10, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 10, 3, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_05 == 122
			"F13105",
			0, // intro_seen
			SCENARIO_ID_F131_05,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 8, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 10, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 11, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 11, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 12, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 12, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_06 == 123
			"F13106",
			0, // intro_seen
			SCENARIO_ID_F131_06,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 9, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 10, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 11, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 12, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 12, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 12, 6, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_SILBERSCHMUCK, // loot 4
				ITEM_ID_SILBERSCHMUCK // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_07 == 124
			"F13107",
			0, // intro_seen
			SCENARIO_ID_F131_05,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 10, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 10, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 11, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 11, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 12, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 12, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_08 == 125
			"F13108",
			0, // intro_seen
			SCENARIO_ID_F131_05,
			{
				{ MONSTER_ID_THORWALER__LVL_1, 7, 7, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 9, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 10, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 11, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 12, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 12, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_10 == 126
			"F13110",
			0, // intro_seen
			SCENARIO_ID_F131_10,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 14, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_5, 13, 15, FIG_VIEWDIR_DOWN, 0 } // enemy 2
			},
			{
				{ 10, 11, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 11, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 8, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 11, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 9, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 8, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 12, 7, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_SILBERSCHMUCK, // loot 4
				ITEM_ID_SILBERSCHMUCK, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_SILBERSCHMUCK, // loot 7
				ITEM_ID_SKRAJA, // loot 8
				ITEM_ID_EISENSCHLUESSEL__1 // loot 9
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_11_1 == 127
			"F13111.1",
			0, // intro_seen
			SCENARIO_ID_F131_11_1,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 9, 18, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 9, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 7, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 11, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 9, 3, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 4, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_14A == 128
			"F13114A",
			0, // intro_seen
			SCENARIO_ID_F131_14A,
			{
				{ MONSTER_ID_PIRAT__LVL_5, 2, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_5, 16, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_5, 3, 5, FIG_VIEWDIR_UP, 0 } // enemy 2
			},
			{
				{ 11, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 12, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 13, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 14, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 12, 7, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SILBERSCHMUCK, // loot 0
				ITEM_ID_ENTERMESSER, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_EISENSCHLUESSEL__2 // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_14B == 129
			"F13114B",
			0, // intro_seen
			SCENARIO_ID_F131_14A,
			{
				{ MONSTER_ID_PIRAT__LVL_5, 2, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_5, 14, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_5, 4, 8, FIG_VIEWDIR_RIGHT, 0 } // enemy 2
			},
			{
				{ 11, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 12, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 13, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 14, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 12, 7, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SILBERSCHMUCK, // loot 0
				ITEM_ID_ENTERMESSER, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SILBERSCHMUCK, // loot 3
				ITEM_ID_SILBERSCHMUCK, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_SKRAJA, // loot 7
				ITEM_ID_SKRAJA, // loot 8
				ITEM_ID_EISENSCHLUESSEL__2 // loot 9
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_16 == 130
			"F13116",
			0, // intro_seen
			SCENARIO_ID_F131_16,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 11, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 6, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 4, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 3, 8, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 3, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 4, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 3, 6, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DFIN12 == 131
			"DFIN12",
			0, // intro_seen
			SCENARIO_ID_DFIN12,
			{
				{ MONSTER_ID_ZOMBIE, 2, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 3, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 16, 7, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 8, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 9, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 11, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 13, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 9, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 11, 6, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DFIN16 == 132
			"DFIN16",
			0, // intro_seen
			SCENARIO_ID_DFIN16,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 15, 14, FIG_VIEWDIR_LEFT, 1 }, // enemy 0
				{ MONSTER_ID_MUMIE__LVL_1, 15, 16, FIG_VIEWDIR_LEFT, 1 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 13, 16, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 11, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 10, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 11, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 12, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 8, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 10, 18, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 9, 18, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DFIN18A == 133
			"DFIN18A",
			0, // intro_seen
			SCENARIO_ID_DFIN18,
			{
				{ MONSTER_ID_ZOMBIE, 3, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 4, 11, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 6, 8, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_ZOMBIE, 7, 9, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 6, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 4, 18, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 19, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 22, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 4, 20, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 2, 18, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 2, 20, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DFIN18B == 134
			"DFIN18B",
			0, // intro_seen
			SCENARIO_ID_DFIN18,
			{
				{ MONSTER_ID_ZOMBIE, 3, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 4, 11, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 6, 8, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_ZOMBIE, 7, 9, FIG_VIEWDIR_UP, 0 }, // enemy 3
				{ MONSTER_ID_MUMIE__LVL_1, 12, 5, FIG_VIEWDIR_UP, 0 }, // enemy 4
				{ MONSTER_ID_MUMIE__LVL_1, 14, 4, FIG_VIEWDIR_UP, 0 }, // enemy 5
				{ MONSTER_ID_ZOMBIE, 15, 6, FIG_VIEWDIR_LEFT, 0 } // enemy 6
			},
			{
				{ 6, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 4, 18, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 19, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 22, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 4, 20, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 2, 18, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 2, 20, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DFIN26 == 135
			"DFIN26",
			0, // intro_seen
			SCENARIO_ID_DFIN26,
			{
				{ MONSTER_ID_ZOMBIE, 10, 4, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 11, 3, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 3, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 3, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 3, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 1, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 1, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 1, 11, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DFIN27A == 136
			"DFIN27A",
			0, // intro_seen
			SCENARIO_ID_DFIN27,
			{
				{ MONSTER_ID_ZOMBIE, 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 15, 10, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 8, 17, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 3, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 4, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 2, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 4, 18, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 3, 18, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 2, 18, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 3, 19, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DFIN27B == 137
			"DFIN27B",
			0, // intro_seen
			SCENARIO_ID_DFIN27,
			{
				{ MONSTER_ID_ZOMBIE, 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 15, 10, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 8, 17, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_ZOMBIE, 10, 6, FIG_VIEWDIR_UP, 0 }, // enemy 3
				{ MONSTER_ID_ZOMBIE, 12, 7, FIG_VIEWDIR_LEFT, 0 } // enemy 4
			},
			{
				{ 3, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 4, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 2, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 4, 18, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 3, 18, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 2, 18, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 3, 19, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DFIN28 == 138
			"DFIN28",
			0, // intro_seen
			SCENARIO_ID_DFIN28,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 6, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_1, 8, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 10, 4, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_ZOMBIE, 7, 6, FIG_VIEWDIR_UP, 0 }, // enemy 3
				{ MONSTER_ID_ZOMBIE, 5, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_ZOMBIE, 12, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 5
				{ MONSTER_ID_ZOMBIE, 3, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 6
				{ MONSTER_ID_ZOMBIE, 14, 9, FIG_VIEWDIR_LEFT, 0 }, // enemy 7
				{ MONSTER_ID_ZOMBIE, 15, 11, FIG_VIEWDIR_LEFT, 0 }, // enemy 8
				{ MONSTER_ID_ZOMBIE, 14, 14, FIG_VIEWDIR_LEFT, 0 } // enemy 9
			},
			{
				{ 7, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 8, 13, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 13, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 11, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 8, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 9, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 10, 11, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DFIN30 == 139
			"DFIN30",
			0, // intro_seen
			SCENARIO_ID_DFIN30,
			{
				{ MONSTER_ID_MUMIE__LVL_1, 8, 6, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_MUMIE__LVL_1, 10, 3, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 8, 2, FIG_VIEWDIR_UP, 0 } // enemy 2
			},
			{
				{ 8, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 9, 16, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 8, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 11, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 9, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 8, 19, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 10, 19, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DPRE10_1 == 140
			"DPRE10.1",
			0, // intro_seen
			SCENARIO_ID_DPRE10_1,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 9, 6, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_1, 7, 6, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_SKELETT__LVL_1, 11, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_SKELETT__LVL_1, 8, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_SKELETT__LVL_1, 9, 16, FIG_VIEWDIR_DOWN, 0 } // enemy 4
			},
			{
				{ 7, 12, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 6, 13, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 5, 13, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 4, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 3, 12, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_KRIEGSBEIL, // loot 0
				ITEM_ID_KRIEGSBEIL, // loot 1
				ITEM_ID_HACKE, // loot 2
				ITEM_ID_HACKE, // loot 3
				ITEM_ID_HACKE // loot 4
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DOBE07 == 141
			"DOBE07",
			0, // intro_seen
			SCENARIO_ID_DOBE07,
			{
				{ MONSTER_ID_ZWERG, 6, 8, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 3, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 4, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 9, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 7, 2, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 8, 1, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 9, 1, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_KRIEGSBEIL__SPECIAL // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DOBE09 == 142
			"DOBE09",
			0, // intro_seen
			SCENARIO_ID_DOBE09,
			{
				{ MONSTER_ID_ZWERG, 10, 8, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 12, 6, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 13, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 14, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 14, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 15, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 15, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 15, 8, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_KRIEGSBEIL__SPECIAL // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DOBE11 == 143
			"DOBE11",
			0, // intro_seen
			SCENARIO_ID_DOBE11,
			{
				{ MONSTER_ID_ZWERG, 8, 11, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_ZWERG, 9, 1, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 10, 14, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 9, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 11, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 7, 16, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 9, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 13, 16, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 11, 17, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_KRIEGSBEIL__SPECIAL, // loot 0
				ITEM_ID_KRIEGSBEIL // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DOBE20 == 144
			"DOBE20",
			0, // intro_seen
			SCENARIO_ID_DOBE20,
			{
				{ MONSTER_ID_ZWERG, 4, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZWERG, 1, 14, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_ZWERG, 9, 7, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_ZWERG, 7, 7, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 9, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 7, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 10, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 11, 18, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 8, 18, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 12, 19, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 9, 19, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_KRIEGSBEIL__SPECIAL, // loot 0
				ITEM_ID_KRIEGSBEIL__SPECIAL, // loot 1
				ITEM_ID_KRIEGSBEIL, // loot 2
				ITEM_ID_KRIEGSBEIL // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DOBE19 == 145
			"DOBE19",
			0, // intro_seen
			SCENARIO_ID_DOBE19,
			{
				{ MONSTER_ID_ZWERGENVETERAN, 3, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZWERGENVETERAN, 7, 3, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_ZWERGENVETERAN, 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_ZWERGENVETERAN, 3, 9, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 7, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 6, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 4, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 8, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 8, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 6, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 5, 12, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_KRIEGSBEIL, // loot 0
				ITEM_ID_KRIEGSBEIL, // loot 1
				ITEM_ID_KRIEGSBEIL, // loot 2
				ITEM_ID_KRIEGSBEIL // loot 3
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DOBE22 == 146
			"DOBE22",
			0, // intro_seen
			SCENARIO_ID_DOBE22,
			{
				{ MONSTER_ID_LINDWURM, 9, 3, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 7, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 8, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 9, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 10, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 8, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 7, 10, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO03 == 147
			"DTHO03",
			0, // intro_seen
			SCENARIO_ID_DTHO03,
			{
				{ MONSTER_ID_RAEUBER__LVL_2, 9, 8, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_2, 8, 5, FIG_VIEWDIR_RIGHT, 0 } // enemy 1
			},
			{
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 11, 4, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 12, 4, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 13, 3, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 11, 3, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 12, 2, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_RAPIER // loot 3
			},
			0, 15, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO05 == 148
			"DTHO05",
			0, // intro_seen
			SCENARIO_ID_DTHO05,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 9, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_2, 5, 8, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 4, 5, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 5, 4, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 6, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 3, 4, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 4, 3, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 5, 2, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 3, 2, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_BIER // loot 4
			},
			0, 22, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO06 == 149
			"DTHO06",
			0, // intro_seen
			SCENARIO_ID_DTHO06,
			{
				{ MONSTER_ID_RAEUBER__LVL_2, 8, 5, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_2, 11, 12, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 8, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 8, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 7, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 6, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 5, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 5, 11, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_RAPIER // loot 3
			},
			0, 13, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO09 == 150
			"DTHO09",
			0, // intro_seen
			SCENARIO_ID_DTHO09,
			{
				{ MONSTER_ID_RAEUBER__LVL_2, 9, 12, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 9, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 5, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 10, 4, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 3, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 10, 3, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 9, 2, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_DOLCH // loot 1
			},
			0, 8, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO10 == 151
			"DTHO10",
			0, // intro_seen
			SCENARIO_ID_DTHO10,
			{
				{ MONSTER_ID_PIRAT__LVL_1, 14, 11, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 7, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 4, 13, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 1, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 3, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 1, 8, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO13 == 152
			"DTHO13",
			0, // intro_seen
			SCENARIO_ID_DTHO13,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 9, 14, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 8, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 11, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 9, 6, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 8, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 9, 5, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_BIER // loot 2
			},
			0, 19, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO15 == 153
			"DTHO15",
			0, // intro_seen
			SCENARIO_ID_DTHO15,
			{
				{ MONSTER_ID_RAEUBER__LVL_2, 9, 16, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 9, 12, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 8, 13, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 6, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 5, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 5, 11, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_RAPIER // loot 1
			},
			0, 5, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO16 == 154
			"DTHO16",
			0, // intro_seen
			SCENARIO_ID_DTHO16,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 15, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 12, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 11, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 11, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 8, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 9, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 9, 6, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_BIER // loot 2
			},
			0, 11, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO18 == 155
			"DTHO18",
			0, // intro_seen
			SCENARIO_ID_DTHO18,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 11, 6, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 6, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 7, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 5, 5, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 3, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 4, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 4, 3, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_BIER, // loot 1
				ITEM_ID_SKRAJA // loot 2
			},
			0, 14, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO19 == 156
			"DTHO19",
			0, // intro_seen
			SCENARIO_ID_DTHO19,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 11, 6, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 7, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 7, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 5, 3, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 4, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 4, 2, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SILBERSCHMUCK, // loot 1
				ITEM_ID_ENTERMESSER // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO20 == 157
			"DTHO20",
			0, // intro_seen
			SCENARIO_ID_DTHO20A,
			{
				{ MONSTER_ID_PIRAT__LVL_1, 12, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 5, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 4, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 3, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 2, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 1, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 2, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 1, 9, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO20_1 == 158
			"DTHO20.1",
			0, // intro_seen
			SCENARIO_ID_F076_06,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 8, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 9, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 9, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 10, 5, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 11, 6, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SILBERSCHMUCK, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_ENTERMESSER // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO21A == 159
			"DTHO21A",
			0, // intro_seen
			SCENARIO_ID_DTHO21,
			{
				{ MONSTER_ID_RAEUBER__LVL_2, 6, 18, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_2, 7, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_RAEUBER__LVL_2, 9, 18, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_RAEUBER__LVL_3, 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 6, FIG_VIEWDIR_UP, 0 }, // enemy 4
				{ MONSTER_ID_RAEUBER__LVL_6, 15, 12, FIG_VIEWDIR_LEFT, 0 } // enemy 5
			},
			{
				{ 9, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 12, 5, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 9, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 13, 4, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 10, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 2, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_RAPIER, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_SKRAJA, // loot 6
				ITEM_ID_SKRAJA, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_BIER, // loot 10
				ITEM_ID_BIER, // loot 11
				ITEM_ID_ORKNASE, // loot 12
				ITEM_ID_SAEBEL, // loot 13
				ITEM_ID_DOLCH, // loot 14
				ITEM_ID_GOLDSCHMUCK, // loot 15
				ITEM_ID_WEINFLASCHE, // loot 16
				ITEM_ID_SCHLUESSEL__YELLOW // loot 17
			},
			0, 51, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO21B == 160
			"DTHO21B",
			0, // intro_seen
			SCENARIO_ID_DTHO21A,
			{
				{ MONSTER_ID_RAEUBER__LVL_2, 6, 18, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_2, 7, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_RAEUBER__LVL_2, 9, 18, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_RAEUBER__LVL_3, 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 6, FIG_VIEWDIR_UP, 0 }, // enemy 4
				{ MONSTER_ID_RAEUBER__LVL_6, 15, 12, FIG_VIEWDIR_LEFT, 0 }, // enemy 5
				{ MONSTER_ID_RAEUBER__LVL_3, 5, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 6
				{ MONSTER_ID_RAEUBER__LVL_2, 6, 16, FIG_VIEWDIR_RIGHT, 0 } // enemy 7
			},
			{
				{ 9, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 12, 5, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 9, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 13, 4, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 10, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 2, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_RAPIER, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_SKRAJA, // loot 6
				ITEM_ID_SKRAJA, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_BIER, // loot 10
				ITEM_ID_BIER, // loot 11
				ITEM_ID_ORKNASE, // loot 12
				ITEM_ID_SAEBEL, // loot 13
				ITEM_ID_DOLCH, // loot 14
				ITEM_ID_GOLDSCHMUCK, // loot 15
				ITEM_ID_WEINFLASCHE, // loot 16
				ITEM_ID_SCHLUESSEL__YELLOW, // loot 17
				ITEM_ID_RAPIER, // loot 18
				ITEM_ID_DOLCH, // loot 19
				ITEM_ID_DOLCH, // loot 20
				ITEM_ID_BIER // loot 21
			},
			0, 63, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO23 == 161
			"DTHO23",
			0, // intro_seen
			SCENARIO_ID_DTHO23,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 9, 12, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 9, 6, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 10, 4, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 9, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 3, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 11, 2, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 9, 2, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SILBERSCHMUCK, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_ENTERMESSER // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO25 == 162
			"DTHO25",
			0, // intro_seen
			SCENARIO_ID_DTHO25,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 4, 6, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 8, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 9, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 9, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 10, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 10, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 12, 8, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_BIER, // loot 1
				ITEM_ID_SKRAJA // loot 2
			},
			0, 19, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO27 == 163
			"DTHO27",
			0, // intro_seen
			SCENARIO_ID_DTHO27,
			{
				{ MONSTER_ID_RAEUBER__LVL_2, 10, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_2, 5, 4, FIG_VIEWDIR_UP, 0 } // enemy 1
			},
			{
				{ 7, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 4, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 3, 7, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_RAPIER // loot 3
			},
			0, 16, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO43 == 164
			"DTHO43",
			0, // intro_seen
			SCENARIO_ID_DTHO43,
			{
				{ MONSTER_ID_SKELETT__LVL_2, 9, 13, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 4, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 3, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 5, 7, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 5, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 6, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 3, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 5, 3, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_KETTENHEMD, // loot 0
				ITEM_ID_SCHWERT // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO48_1 == 165
			"DTHO48.1",
			0, // intro_seen
			SCENARIO_ID_DTHO48_1,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 12, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 0
			},
			{
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 9, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 9, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 8, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 6, 7, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO49 == 166
			"DTHO49",
			0, // intro_seen
			SCENARIO_ID_DTHO49,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 7, 15, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_1, 9, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 7, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 10, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 10, 3, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 11, 2, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO50 == 167
			"DTHO50",
			0, // intro_seen
			SCENARIO_ID_DTHO50,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 9, 5, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 9, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 10, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 8, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 9, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 8, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 9, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 9, 13, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO53 == 168
			"DTHO53",
			0, // intro_seen
			SCENARIO_ID_DTHO53,
			{
				{ MONSTER_ID_SKELETT__LVL_2, 9, 16, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 9, 11, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 10, 10, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 8, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 10, 7, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO55 == 169
			"DTHO55",
			0, // intro_seen
			SCENARIO_ID_DTHO55,
			{
				{ MONSTER_ID_SKELETT__LVL_2, 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_2, 8, 11, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 8, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 7, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 8, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 10, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 9, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 10, 7, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO56 == 170
			"DTHO56",
			0, // intro_seen
			SCENARIO_ID_DTHO56,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 11, 9, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 14, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 15, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 15, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 16, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 16, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 16, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 18, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO57 == 171
			"DTHO57\0001", // sanitized: "DTHO57"
			0, // intro_seen
			SCENARIO_ID_DTHO57,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 12, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_2, 1, 6, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_SKELETT__LVL_2, 7, 3, FIG_VIEWDIR_LEFT, 1 } // enemy 2
			},
			{
				{ 3, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 4, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 2, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 3, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 2, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 2, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 0, 14, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO57_1 == 172
			"DTHO57.1",
			0, // intro_seen
			SCENARIO_ID_DTHO57,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 14, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_2, 2, 7, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_SKELETT__LVL_2, 7, 5, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 3, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 4, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 2, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 3, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 2, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 2, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 0, 14, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_SCHWERT // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO58 == 173
			"DTHO58",
			0, // intro_seen
			SCENARIO_ID_DTHO58,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 5, 13, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 4, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 6, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 3, 7, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 2, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 4, 7, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 4, 6, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 3, 5, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO59 == 174
			"DTHO59",
			0, // intro_seen
			SCENARIO_ID_DTHO59,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 9, 15, FIG_VIEWDIR_DOWN, 1 }, // enemy 0
				{ MONSTER_ID_SKELETT__LVL_1, 1, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_SKELETT__LVL_1, 14, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_SKELETT__LVL_2, 8, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 3
			},
			{
				{ 7, 6, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 4, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 8, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 6, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 2, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 8, 2, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SCHWERT // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO60 == 175
			"DTHO60",
			0, // intro_seen
			SCENARIO_ID_DTHO60,
			{
				{ MONSTER_ID_SKELETT__LVL_1, 5, 11, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 13, 14, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 12, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 10, 16, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 13, 16, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 14, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 13, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 14, 16, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO61 == 176
			"DTHO61",
			0, // intro_seen
			SCENARIO_ID_DTHO61,
			{
				{ MONSTER_ID_HESHTHOT__LVL_2, 11, 13, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 4, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 4, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 3, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 2, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 3, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 1, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 2, 4, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F031 == 177
			"F031",
			0, // intro_seen
			SCENARIO_ID_F031,
			{
				{ MONSTER_ID_ORK__LVL_2, 5, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_2, 6, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_2, 8, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_2, 9, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_ORK__LVL_3, 5, 4, FIG_VIEWDIR_UP, 0 }, // enemy 4
				{ MONSTER_ID_ORK__LVL_2, 7, 4, FIG_VIEWDIR_UP, 0 }, // enemy 5
				{ MONSTER_ID_ORK__LVL_3, 3, 3, FIG_VIEWDIR_UP, 0 } // enemy 6
			},
			{
				{ 7, 11, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 5, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 10, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 6, 10, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 5, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 8, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_SAEBEL, // loot 5
				ITEM_ID_SAEBEL, // loot 6
				ITEM_ID_SAEBEL, // loot 7
				ITEM_ID_SAEBEL, // loot 8
				ITEM_ID_SAEBEL, // loot 9
				ITEM_ID_KRIEGSBEIL, // loot 10
				ITEM_ID_KRIEGSBEIL, // loot 11
				ITEM_ID_DOLCH, // loot 12
				ITEM_ID_DOLCH, // loot 13
				ITEM_ID_DECKE, // loot 14
				ITEM_ID_DECKE, // loot 15
				ITEM_ID_KURZBOGEN, // loot 16
				ITEM_ID_KURZBOGEN, // loot 17
				ITEM_ID_PFEIL, // loot 18
				ITEM_ID_PFEIL // loot 19
			},
			0, 40, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F035 == 178
			"F035",
			0, // intro_seen
			SCENARIO_ID_F035,
			{
				{ MONSTER_ID_ORK__LVL_3, 7, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_1, 8, 9, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 9, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_1, 10, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_ORK__LVL_1, 8, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_ORK__LVL_1, 7, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 5
				{ MONSTER_ID_ORK__LVL_1, 8, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 6
				{ MONSTER_ID_ORK__LVL_1, 6, 13, FIG_VIEWDIR_RIGHT, 0 } // enemy 7
			},
			{
				{ 9, 6, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 10, 5, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 12, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 11, 3, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 12, 4, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_SAEBEL, // loot 2
				ITEM_ID_SAEBEL, // loot 3
				ITEM_ID_SAEBEL, // loot 4
				ITEM_ID_SAEBEL, // loot 5
				ITEM_ID_SAEBEL, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_DOLCH, // loot 12
				ITEM_ID_KURZBOGEN, // loot 13
				ITEM_ID_KURZBOGEN, // loot 14
				ITEM_ID_KURZBOGEN, // loot 15
				ITEM_ID_KURZBOGEN, // loot 16
				ITEM_ID_PFEIL, // loot 17
				ITEM_ID_PFEIL, // loot 18
				ITEM_ID_PFEIL, // loot 19
				ITEM_ID_PFEIL, // loot 20
				ITEM_ID_DOLCH, // loot 21
				ITEM_ID_ZWEIHAENDER, // loot 22
				ITEM_ID_KURZSCHWERT, // loot 23
				ITEM_ID_DECKE, // loot 24
				ITEM_ID_AMULETT__MR_BONUS // loot 25
			},
			0, 12, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F046 == 179
			"F046",
			0, // intro_seen
			SCENARIO_ID_DOBE20,
			{
				{ MONSTER_ID_ZOMBIE, 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 8, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_ZOMBIE, 11, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_ZOMBIE, 14, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_ZOMBIE, 8, 19, FIG_VIEWDIR_RIGHT, 0 } // enemy 5
			},
			{
				{ 10, 17, FIG_VIEWDIR_UP, 1 }, // hero 0
				{ 12, 10, FIG_VIEWDIR_RIGHT, 1 }, // hero 1
				{ 9, 9, FIG_VIEWDIR_UP, 1 }, // hero 2
				{ 10, 7, FIG_VIEWDIR_RIGHT, 1 }, // hero 3
				{ 12, 6, FIG_VIEWDIR_RIGHT, 1 }, // hero 4
				{ 13, 8, FIG_VIEWDIR_RIGHT, 1 }, // hero 5
				{ 8, 6, FIG_VIEWDIR_DOWN, 1 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F064 == 180
			"F064",
			0, // intro_seen
			SCENARIO_ID_F064,
			{
				{ MONSTER_ID_GORAH, 8, 20, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_GRIMWOLF__LVL_1, 6, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_GRIMWOLF__LVL_1, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_GRIMWOLF__LVL_1, 5, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_GRIMWOLF__LVL_1, 9, 18, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_GRIMWOLF__LVL_1, 9, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 5
				{ MONSTER_ID_GRIMWOLF__LVL_1, 8, 14, FIG_VIEWDIR_RIGHT, 0 }, // enemy 6
				{ MONSTER_ID_GRIMWOLF__LVL_1, 10, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 7
				{ MONSTER_ID_GRIMWOLF__LVL_1, 6, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 8
				{ MONSTER_ID_GRIMWOLF__LVL_1, 9, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 9
				{ MONSTER_ID_GRIMWOLF__LVL_1, 13, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 10
				{ MONSTER_ID_HARPYIE, 11, 3, FIG_VIEWDIR_UP, 5 }, // enemy 11
				{ MONSTER_ID_HARPYIE, 7, 5, FIG_VIEWDIR_RIGHT, 5 }, // enemy 12
				{ MONSTER_ID_WALDLOEWE, 15, 11, FIG_VIEWDIR_LEFT, 12 }, // enemy 13
				{ MONSTER_ID_HESHTHOT__LVL_1, 5, 9, FIG_VIEWDIR_RIGHT, 20 } // enemy 14
			},
			{
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 11, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 9, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 11, 7, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F066 == 181
			"F066",
			0, // intro_seen
			SCENARIO_ID_F066,
			{
				{ MONSTER_ID_STEPPENHUND, 9, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_STEPPENHUND, 10, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_STEPPENHUND, 7, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_STEPPENHUND, 11, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_STEPPENHUND, 5, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_STEPPENHUND, 7, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 5
				{ MONSTER_ID_STEPPENHUND, 9, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 6
				{ MONSTER_ID_STEPPENHUND, 13, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 7
				{ MONSTER_ID_STEPPENHUND, 13, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 8
				{ MONSTER_ID_STEPPENHUND, 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 9
				{ MONSTER_ID_STEPPENHUND, 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 10
				{ MONSTER_ID_STEPPENHUND, 16, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 11
				{ MONSTER_ID_STEPPENHUND, 19, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 12
				{ MONSTER_ID_NONE, 6, 12, FIG_VIEWDIR_RIGHT, 0 } // enemy 13
			},
			{
				{ 11, 3, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 3, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 13, 2, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 11, 1, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 1, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 14, 1, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 15, 1, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F074 == 182
			"F074",
			0, // intro_seen
			SCENARIO_ID_F074,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 5, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_3, 7, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_RAEUBER__LVL_3, 9, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_RAEUBER__LVL_3, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_RAEUBER__LVL_6, 12, 5, FIG_VIEWDIR_UP, 0 } // enemy 4
			},
			{
				{ 8, 11, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 12, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 10, 10, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 12, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 11, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 12, 9, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_RAPIER, // loot 4
				ITEM_ID_RAPIER, // loot 5
				ITEM_ID_RAPIER, // loot 6
				ITEM_ID_RAPIER, // loot 7
				ITEM_ID_RAPIER, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_ARMBRUST, // loot 10
				ITEM_ID_BOLZEN, // loot 11
				ITEM_ID_KUKRISDOLCH // loot 12
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F075_A == 183
			"F075A",
			0, // intro_seen
			SCENARIO_ID_F075,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 8, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_RAEUBER__LVL_3, 5, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_RAEUBER__LVL_3, 9, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 6, FIG_VIEWDIR_UP, 0 }, // enemy 4
				{ MONSTER_ID_RAEUBER__LVL_8, 10, 8, FIG_VIEWDIR_RIGHT, 0 } // enemy 5
			},
			{
				{ 19, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 19, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 20, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 20, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 20, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 21, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 21, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_ORKNASE // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F075_B == 184
			"F075B",
			0, // intro_seen
			SCENARIO_ID_F075,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 8, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_RAEUBER__LVL_3, 5, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_RAEUBER__LVL_3, 9, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 6, FIG_VIEWDIR_UP, 0 }, // enemy 4
				{ MONSTER_ID_RAEUBER__LVL_8, 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 5
				{ MONSTER_ID_RAEUBER__LVL_3, 4, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 6
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 7
				{ MONSTER_ID_RAEUBER__LVL_3, 7, 5, FIG_VIEWDIR_UP, 0 }, // enemy 8
				{ MONSTER_ID_RAEUBER__LVL_3, 9, 5, FIG_VIEWDIR_UP, 0 }, // enemy 9
				{ MONSTER_ID_RAEUBER__LVL_3, 10, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 10
				{ MONSTER_ID_RAEUBER__LVL_5, 9, 10, FIG_VIEWDIR_RIGHT, 0 } // enemy 11
			},
			{
				{ 19, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 19, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 20, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 20, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 20, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 21, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 21, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_ORKNASE // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F075_C == 185
			"F075C",
			0, // intro_seen
			SCENARIO_ID_F075,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 8, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_RAEUBER__LVL_3, 5, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_RAEUBER__LVL_8, 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_RAEUBER__LVL_3, 7, 5, FIG_VIEWDIR_UP, 0 }, // enemy 5
				{ MONSTER_ID_RAEUBER__LVL_3, 9, 5, FIG_VIEWDIR_UP, 0 }, // enemy 6
				{ MONSTER_ID_RAEUBER__LVL_5, 9, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 7
				{ MONSTER_ID_RAEUBER__LVL_5, 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // enemy 8
				{ MONSTER_ID_RAEUBER__LVL_3, 4, 7, FIG_VIEWDIR_RIGHT, 0 }, // enemy 9
				{ MONSTER_ID_RAEUBER__LVL_5, 8, 3, FIG_VIEWDIR_UP, 0 }, // enemy 10
				{ MONSTER_ID_RAEUBER__LVL_5, 5, 4, FIG_VIEWDIR_UP, 0 }, // enemy 11
				{ MONSTER_ID_RAEUBER__LVL_3, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 12
				{ MONSTER_ID_NONE, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 13
				{ MONSTER_ID_NONE, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 14
				{ MONSTER_ID_NONE, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 15
				{ MONSTER_ID_NONE, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 16
				{ MONSTER_ID_NONE, 7, 16, FIG_VIEWDIR_DOWN, 0 } // enemy 17
			},
			{
				{ 19, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 19, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 20, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 20, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 20, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 21, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 21, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_ORKNASE // loot 0
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F077 == 186
			"F077",
			0, // intro_seen
			SCENARIO_ID_F077,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 8, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_RAEUBER__LVL_3, 3, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_RAEUBER__LVL_3, 5, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_RAEUBER__LVL_5, 7, 11, FIG_VIEWDIR_RIGHT, 0 } // enemy 5
			},
			{
				{ 11, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 12, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 12, 13, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 13, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 13, 14, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 14, 14, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 14, 13, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_RAPIER, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_RAPIER, // loot 3
				ITEM_ID_RAPIER, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_WURFMESSER, // loot 11
				ITEM_ID_WURFMESSER, // loot 12
				ITEM_ID_WURFMESSER, // loot 13
				ITEM_ID_WURFMESSER, // loot 14
				ITEM_ID_WURFMESSER, // loot 15
				ITEM_ID_WURFMESSER, // loot 16
				ITEM_ID_ARMBRUST, // loot 17
				ITEM_ID_BOLZEN, // loot 18
				ITEM_ID_WURFMESSER, // loot 19
				ITEM_ID_RAPIER // loot 20
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F080 == 187
			"F080",
			0, // intro_seen
			SCENARIO_ID_F080,
			{
				{ MONSTER_ID_LINDWURM, 5, 5, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 15, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 15, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 16, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 16, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 15, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 17, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 16, 11, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F084 == 188
			"F084",
			0, // intro_seen
			SCENARIO_ID_F084,
			{
				{ MONSTER_ID_OGER, 8, 5, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_2, 9, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_2, 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_2, 6, 4, FIG_VIEWDIR_UP, 0 }, // enemy 3
				{ MONSTER_ID_ORK__LVL_2, 9, 3, FIG_VIEWDIR_UP, 0 } // enemy 4
			},
			{
				{ 7, 16, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 4, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 5, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 9, 16, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 5, 18, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 6, 19, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 7, 20, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_SAEBEL, // loot 2
				ITEM_ID_SAEBEL, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_SCHNAPSFLASCHE, // loot 8
				ITEM_ID_SCHNAPSFLASCHE, // loot 9
				ITEM_ID_SCHNAPSFLASCHE, // loot 10
				ITEM_ID_SCHNAPSFLASCHE, // loot 11
				ITEM_ID_DECKE, // loot 12
				ITEM_ID_DECKE, // loot 13
				ITEM_ID_DECKE, // loot 14
				ITEM_ID_DECKE // loot 15
			},
			0, 10, 32 // ducats, silver, heller
		},
		{ // FIGHT_ID_F099 == 189
			"F099",
			0, // intro_seen
			SCENARIO_ID_F099,
			{
				{ MONSTER_ID_HARPYIE, 9, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_HARPYIE, 10, 4, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_HARPYIE, 13, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_HARPYIE, 8, 6, FIG_VIEWDIR_RIGHT, 0 } // enemy 3
			},
			{
				{ 10, 11, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 12, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 12, 8, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 11, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 10, 6, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F101 == 190
			"F101",
			0, // intro_seen
			SCENARIO_ID_F101,
			{
				{ MONSTER_ID_HARPYIE, 10, 9, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_HARPYIE, 7, 9, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_HARPYIE, 10, 4, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_HARPYIE, 4, 7, FIG_VIEWDIR_RIGHT, 0 } // enemy 3
			},
			{
				{ 9, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 7, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 8, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 7, 4, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 5, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 5, 3, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F122 == 191
			"F122",
			0, // intro_seen
			SCENARIO_ID_F122,
			{
				{ MONSTER_ID_GRIMWOLF__LVL_1, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_GRIMWOLF__LVL_1, 9, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_GRIMWOLF__LVL_1, 4, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_GRIMWOLF__LVL_1, 11, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_GRIMWOLF__LVL_1, 6, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_GRIMWOLF__LVL_1, 8, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 5
				{ MONSTER_ID_GRIMWOLF__LVL_1, 7, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 6
				{ MONSTER_ID_GRIMWOLF__LVL_1, 9, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 7
				{ MONSTER_ID_GRIMWOLF__LVL_1, 12, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 8
				{ MONSTER_ID_GRIMWOLF__LVL_1, 3, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 9
				{ MONSTER_ID_GRIMWOLF__LVL_1, 14, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 10
				{ MONSTER_ID_GRIMWOLF__LVL_1, 10, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 11
				{ MONSTER_ID_GRIMWOLF__LVL_1, 4, 15, FIG_VIEWDIR_DOWN, 0 } // enemy 12
			},
			{
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 4, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 6, 4, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 3, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 7, 2, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 5, 3, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F144 == 192
			"F144",
			0, // intro_seen
			SCENARIO_ID_F144,
			{
				{ MONSTER_ID_ORKCHAMPION, 8, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_3, 8, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_3, 10, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_3, 6, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_ORK__LVL_3, 12, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_ORK__LVL_3, 4, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 5
				{ MONSTER_ID_ORK__LVL_3, 2, 15, FIG_VIEWDIR_RIGHT, 0 }, // enemy 6
				{ MONSTER_ID_ORK__LVL_3, 2, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 7
				{ MONSTER_ID_ORK__LVL_3, 2, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 8
				{ MONSTER_ID_ORK__LVL_3, 14, 15, FIG_VIEWDIR_LEFT, 0 }, // enemy 9
				{ MONSTER_ID_ORK__LVL_3, 14, 12, FIG_VIEWDIR_LEFT, 0 }, // enemy 10
				{ MONSTER_ID_ORK__LVL_3, 14, 14, FIG_VIEWDIR_LEFT, 0 }, // enemy 11
				{ MONSTER_ID_ORK__LVL_3, 14, 9, FIG_VIEWDIR_LEFT, 0 }, // enemy 12
				{ MONSTER_ID_ORK__LVL_3, 2, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 13
				{ MONSTER_ID_ORK__LVL_3, 14, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 14
				{ MONSTER_ID_ORK__LVL_3, 2, 7, FIG_VIEWDIR_RIGHT, 0 } // enemy 15
			},
			{
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 12, 1, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 14, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 5, 1, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 2, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 8, 1, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 3, 1, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP1A == 193
			"DASP1A",
			0, // intro_seen
			SCENARIO_ID_CITY2,
			{
				{ MONSTER_ID_PIRAT__LVL_5, 8, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_5, 9, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_5, 10, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_5, 11, 7, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 12, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 13, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 13, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 13, 13, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 14, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 14, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 15, 12, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_SCHNAPSFLASCHE, // loot 12
				ITEM_ID_SCHNAPSFLASCHE, // loot 13
				ITEM_ID_SCHNAPSFLASCHE, // loot 14
				ITEM_ID_SCHNAPSFLASCHE // loot 15
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP1B == 194
			"DASP1B",
			0, // intro_seen
			SCENARIO_ID_CITY3,
			{
				{ MONSTER_ID_PIRAT__LVL_5, 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_5, 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_5, 8, 6, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_10, 8, 12, FIG_VIEWDIR_DOWN, 0 } // enemy 3
			},
			{
				{ 9, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 9, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 10, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 11, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 11, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 12, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 13, 11, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_ENTERMESSER, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_SCHNAPSFLASCHE, // loot 9
				ITEM_ID_SCHNAPSFLASCHE, // loot 10
				ITEM_ID_SCHNAPSFLASCHE, // loot 11
				ITEM_ID_STREITAXT, // loot 12
				ITEM_ID_SAEBEL, // loot 13
				ITEM_ID_DOLCH, // loot 14
				ITEM_ID_GOLDSCHMUCK // loot 15
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP2 == 195
			"DASP2",
			0, // intro_seen
			SCENARIO_ID_CITY4,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 10, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 8, 14, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 13, 14, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_3, 8, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_NONE, 8, 14, FIG_VIEWDIR_RIGHT, 0 } // enemy 4
			},
			{
				{ 11, 10, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 12, 9, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 9, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 13, 8, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 11, 8, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 11, 7, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_SCHNAPSFLASCHE, // loot 12
				ITEM_ID_SCHNAPSFLASCHE, // loot 13
				ITEM_ID_SCHNAPSFLASCHE, // loot 14
				ITEM_ID_SCHNAPSFLASCHE // loot 15
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP3 == 196
			"DASP3",
			0, // intro_seen
			SCENARIO_ID_CITY4,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 5, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 10, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 8, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_3, 7, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_NONE, 5, 12, FIG_VIEWDIR_RIGHT, 0 } // enemy 4
			},
			{
				{ 11, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 9, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 10, 7, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 12, 7, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 13, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 13, 7, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_SCHNAPSFLASCHE, // loot 12
				ITEM_ID_SCHNAPSFLASCHE, // loot 13
				ITEM_ID_SCHNAPSFLASCHE, // loot 14
				ITEM_ID_SCHNAPSFLASCHE // loot 15
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP4 == 197
			"DASP4",
			0, // intro_seen
			SCENARIO_ID_CITY3,
			{
				{ MONSTER_ID_PIRAT__LVL_5, 4, 5, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 8, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 9, 3, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 11, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 9, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 9, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_ENTERMESSER, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_SCHNAPSFLASCHE, // loot 3
				ITEM_ID_SILBERSCHMUCK // loot 4
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP5 == 198
			"DASP5",
			0, // intro_seen
			SCENARIO_ID_CITY3,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 2, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 1, 4, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 5, 14, FIG_VIEWDIR_RIGHT, 10 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_3, 1, 10, FIG_VIEWDIR_RIGHT, 10 }, // enemy 3
				{ MONSTER_ID_PIRAT__LVL_3, 1, 12, FIG_VIEWDIR_DOWN, 10 }, // enemy 4
				{ MONSTER_ID_PIRAT__LVL_3, 12, 14, FIG_VIEWDIR_LEFT, 10 } // enemy 5
			},
			{
				{ 6, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 5, 2, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 6, 3, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 6, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 7, 4, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 7, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 9, 5, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_SCHNAPSFLASCHE, // loot 12
				ITEM_ID_SCHNAPSFLASCHE, // loot 13
				ITEM_ID_SCHNAPSFLASCHE, // loot 14
				ITEM_ID_SCHNAPSFLASCHE, // loot 15
				ITEM_ID_SCHNAPSFLASCHE, // loot 16
				ITEM_ID_SCHNAPSFLASCHE // loot 17
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP6A == 199
			"DASP6A",
			0, // intro_seen
			SCENARIO_ID_CITY4,
			{
				{ MONSTER_ID_PIRAT__LVL_5, 11, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_5, 10, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 10, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 11, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 12, 7, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 13, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 12, 5, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 14, 6, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_ENTERMESSER, // loot 2
				ITEM_ID_ENTERMESSER, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_SCHNAPSFLASCHE, // loot 6
				ITEM_ID_SCHNAPSFLASCHE, // loot 7
				ITEM_ID_SILBERSCHMUCK, // loot 8
				ITEM_ID_SILBERSCHMUCK // loot 9
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP6B == 200
			"DASP6B",
			0, // intro_seen
			SCENARIO_ID_CITY4,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 9, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 10, 7, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 12, 7, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_3, 14, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 3
			},
			{
				{ 11, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 9, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 10, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 8, 13, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 7, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 6, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 5, 12, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_SCHNAPSFLASCHE, // loot 12
				ITEM_ID_SCHNAPSFLASCHE, // loot 13
				ITEM_ID_SCHNAPSFLASCHE, // loot 14
				ITEM_ID_SCHNAPSFLASCHE, // loot 15
				ITEM_ID_SKRAJA // loot 16
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP7 == 201
			"DASP7",
			0, // intro_seen
			SCENARIO_ID_CITY3,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 7, 16, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 8, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 8, 17, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_5, 8, 10, FIG_VIEWDIR_UP, 0 }, // enemy 3
				{ MONSTER_ID_STREUNER__LVL_4, 10, 11, FIG_VIEWDIR_UP, 0 } // enemy 4
			},
			{
				{ 7, 18, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 5, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 13, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 11, 17, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 11, 14, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 12, 16, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 4, 15, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_SCHNAPSFLASCHE, // loot 12
				ITEM_ID_SCHNAPSFLASCHE, // loot 13
				ITEM_ID_SCHNAPSFLASCHE, // loot 14
				ITEM_ID_SCHNAPSFLASCHE, // loot 15
				ITEM_ID_SILBERSCHMUCK, // loot 16
				ITEM_ID_RAPIER, // loot 17
				ITEM_ID_DOLCH, // loot 18
				ITEM_ID_WEINFLASCHE, // loot 19
				ITEM_ID_GOLDSCHMUCK // loot 20
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP8 == 202
			"DASP8",
			0, // intro_seen
			SCENARIO_ID_CITY1,
			{
				{ MONSTER_ID_STREUNER__LVL_4, 5, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_STREUNER__LVL_4, 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_STREUNER__LVL_4, 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_STREUNER__LVL_4, 5, 5, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 10, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 13, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 13, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 9, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 13, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 11, 6, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 13, 6, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_RAPIER, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_WEINFLASCHE, // loot 6
				ITEM_ID_WEINFLASCHE, // loot 7
				ITEM_ID_WEINFLASCHE // loot 8
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP9 == 203
			"DASP9",
			0, // intro_seen
			SCENARIO_ID_CITY4,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 6, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 7, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 7, 14, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_5, 9, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_STREUNER__LVL_4, 11, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_STREUNER__LVL_4, 13, 17, FIG_VIEWDIR_LEFT, 0 }, // enemy 5
				{ MONSTER_ID_STREUNER__LVL_4, 14, 15, FIG_VIEWDIR_LEFT, 0 }, // enemy 6
				{ MONSTER_ID_STREUNER__LVL_4, 8, 16, FIG_VIEWDIR_LEFT, 0 }, // enemy 7
				{ MONSTER_ID_PIRAT__LVL_5, 10, 16, FIG_VIEWDIR_UP, 0 }, // enemy 8
				{ MONSTER_ID_PIRAT__LVL_5, 12, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 9
				{ MONSTER_ID_PIRAT__LVL_5, 9, 15, FIG_VIEWDIR_LEFT, 0 }, // enemy 10
				{ MONSTER_ID_PIRAT__LVL_5, 10, 17, FIG_VIEWDIR_RIGHT, 0 } // enemy 11
			},
			{
				{ 9, 11, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 11, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 11, 12, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 12, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 11, 10, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 12, 10, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 10, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_ENTERMESSER, // loot 2
				ITEM_ID_ENTERMESSER, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_SCHNAPSFLASCHE, // loot 8
				ITEM_ID_SCHNAPSFLASCHE, // loot 9
				ITEM_ID_SILBERSCHMUCK, // loot 10
				ITEM_ID_SILBERSCHMUCK, // loot 11
				ITEM_ID_SILBERSCHMUCK, // loot 12
				ITEM_ID_SKRAJA, // loot 13
				ITEM_ID_ENTERMESSER, // loot 14
				ITEM_ID_DOLCH, // loot 15
				ITEM_ID_STREITAXT, // loot 16
				ITEM_ID_STREITAXT, // loot 17
				ITEM_ID_STREITAXT, // loot 18
				ITEM_ID_SAEBEL, // loot 19
				ITEM_ID_SAEBEL, // loot 20
				ITEM_ID_SAEBEL, // loot 21
				ITEM_ID_DOLCH, // loot 22
				ITEM_ID_DOLCH, // loot 23
				ITEM_ID_GOLDSCHMUCK, // loot 24
				ITEM_ID_GOLDSCHMUCK, // loot 25
				ITEM_ID_GOLDSCHMUCK, // loot 26
				ITEM_ID_GOLDSCHMUCK // loot 27
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP10 == 204
			"DASP10",
			0, // intro_seen
			SCENARIO_ID_CITY1,
			{
				{ MONSTER_ID_PIRAT__LVL_5, 11, 3, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_STREUNER__LVL_4, 13, 9, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_STREUNER__LVL_4, 13, 12, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_STREUNER__LVL_4, 13, 6, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_STREUNER__LVL_4, 11, 7, FIG_VIEWDIR_UP, 0 } // enemy 4
			},
			{
				{ 10, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 10, 13, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 9, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 10, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 9, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 8, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 8, 8, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_RAPIER, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_RAPIER, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_WEINFLASCHE, // loot 8
				ITEM_ID_WEINFLASCHE, // loot 9
				ITEM_ID_WEINFLASCHE, // loot 10
				ITEM_ID_WEINFLASCHE, // loot 11
				ITEM_ID_DOLCH, // loot 12
				ITEM_ID_SKRAJA, // loot 13
				ITEM_ID_ENTERMESSER, // loot 14
				ITEM_ID_DOLCH, // loot 15
				ITEM_ID_SCHNAPSFLASCHE, // loot 16
				ITEM_ID_SILBERSCHMUCK, // loot 17
				ITEM_ID_GOLDSCHMUCK // loot 18
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP11 == 205
			"DASP11",
			0, // intro_seen
			SCENARIO_ID_CITY2,
			{
				{ MONSTER_ID_STREUNER__LVL_4, 13, 11, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_5, 1, 7, FIG_VIEWDIR_RIGHT, 0 } // enemy 1
			},
			{
				{ 4, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 4, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 5, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 5, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 4, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 3, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 6, 8, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_ENTERMESSER, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_SCHNAPSFLASCHE, // loot 3
				ITEM_ID_RAPIER, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_WEINFLASCHE, // loot 6
				ITEM_ID_GOLDSCHMUCK // loot 7
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP12A == 206
			"DASP12A",
			0, // intro_seen
			SCENARIO_ID_CITY3,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 11, 5, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 8, 6, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 8, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_3, 4, 10, FIG_VIEWDIR_RIGHT, 0 } // enemy 3
			},
			{
				{ 11, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 12, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 12, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 13, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 14, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 14, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 14, 10, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_SCHNAPSFLASCHE, // loot 12
				ITEM_ID_SCHNAPSFLASCHE, // loot 13
				ITEM_ID_SCHNAPSFLASCHE, // loot 14
				ITEM_ID_SCHNAPSFLASCHE // loot 15
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP12B == 207
			"DASP12B",
			0, // intro_seen
			SCENARIO_ID_CITY3,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 11, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 13, 11, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 13, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_3, 14, 9, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_STREUNER__LVL_4, 10, 11, FIG_VIEWDIR_LEFT, 0 } // enemy 4
			},
			{
				{ 8, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 8, 7, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 14, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 11, 5, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 4, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 13, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 11, 13, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_SCHNAPSFLASCHE, // loot 12
				ITEM_ID_SCHNAPSFLASCHE, // loot 13
				ITEM_ID_SCHNAPSFLASCHE, // loot 14
				ITEM_ID_SCHNAPSFLASCHE, // loot 15
				ITEM_ID_SCHNAPSFLASCHE, // loot 16
				ITEM_ID_RAPIER, // loot 17
				ITEM_ID_DOLCH, // loot 18
				ITEM_ID_WEINFLASCHE // loot 19
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP13 == 208
			"DASP13",
			0, // intro_seen
			SCENARIO_ID_CITY2,
			{
				{ MONSTER_ID_PIRAT__LVL_5, 9, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_5, 8, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_5, 2, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_5, 7, 4, FIG_VIEWDIR_UP, 0 } // enemy 3
			},
			{
				{ 11, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 12, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 12, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 12, 12, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 13, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 13, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 14, 11, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_SCHNAPSFLASCHE, // loot 12
				ITEM_ID_SCHNAPSFLASCHE, // loot 13
				ITEM_ID_SCHNAPSFLASCHE, // loot 14
				ITEM_ID_SCHNAPSFLASCHE, // loot 15
				ITEM_ID_SILBERSCHMUCK, // loot 16
				ITEM_ID_SILBERSCHMUCK, // loot 17
				ITEM_ID_SILBERSCHMUCK, // loot 18
				ITEM_ID_SILBERSCHMUCK // loot 19
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP14 == 209
			"DASP14",
			0, // intro_seen
			SCENARIO_ID_CITY1,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 12, 4, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 10, 3, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 12, 2, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_5, 12, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_PIRAT__LVL_5, 10, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_PIRAT__LVL_5, 12, 11, FIG_VIEWDIR_LEFT, 0 } // enemy 5
			},
			{
				{ 10, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 10, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 9, 10, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 10, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 9, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 8, 6, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_SKRAJA, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_ENTERMESSER, // loot 8
				ITEM_ID_ENTERMESSER, // loot 9
				ITEM_ID_ENTERMESSER, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_DOLCH, // loot 12
				ITEM_ID_DOLCH, // loot 13
				ITEM_ID_DOLCH, // loot 14
				ITEM_ID_DOLCH, // loot 15
				ITEM_ID_DOLCH, // loot 16
				ITEM_ID_SCHNAPSFLASCHE, // loot 17
				ITEM_ID_SCHNAPSFLASCHE, // loot 18
				ITEM_ID_SCHNAPSFLASCHE, // loot 19
				ITEM_ID_SCHNAPSFLASCHE, // loot 20
				ITEM_ID_SCHNAPSFLASCHE, // loot 21
				ITEM_ID_SCHNAPSFLASCHE, // loot 22
				ITEM_ID_SILBERSCHMUCK, // loot 23
				ITEM_ID_SILBERSCHMUCK, // loot 24
				ITEM_ID_SILBERSCHMUCK, // loot 25
				ITEM_ID_SKRAJA // loot 26
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP16 == 210
			"DASP16",
			0, // intro_seen
			SCENARIO_ID_CITY2,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 2, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 4, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_3, 5, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_5, 7, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_STREUNER__LVL_4, 7, 6, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_PIRAT__LVL_5, 8, 10, FIG_VIEWDIR_DOWN, 0 }, // enemy 5
				{ MONSTER_ID_PIRAT__LVL_3, 7, 11, FIG_VIEWDIR_DOWN, 0 } // enemy 6
			},
			{
				{ 2, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 3, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 4, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 5, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 5, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 6, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 3, 5, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_SKRAJA, // loot 4
				ITEM_ID_SKRAJA, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_ENTERMESSER, // loot 7
				ITEM_ID_ENTERMESSER, // loot 8
				ITEM_ID_ENTERMESSER, // loot 9
				ITEM_ID_ENTERMESSER, // loot 10
				ITEM_ID_ENTERMESSER, // loot 11
				ITEM_ID_DOLCH, // loot 12
				ITEM_ID_DOLCH, // loot 13
				ITEM_ID_DOLCH, // loot 14
				ITEM_ID_DOLCH, // loot 15
				ITEM_ID_DOLCH, // loot 16
				ITEM_ID_DOLCH, // loot 17
				ITEM_ID_DOLCH, // loot 18
				ITEM_ID_SCHNAPSFLASCHE, // loot 19
				ITEM_ID_SCHNAPSFLASCHE, // loot 20
				ITEM_ID_SCHNAPSFLASCHE, // loot 21
				ITEM_ID_SILBERSCHMUCK, // loot 22
				ITEM_ID_SILBERSCHMUCK, // loot 23
				ITEM_ID_SILBERSCHMUCK, // loot 24
				ITEM_ID_SILBERSCHMUCK, // loot 25
				ITEM_ID_RAPIER, // loot 26
				ITEM_ID_WEINFLASCHE // loot 27
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP17 == 211
			"DASP17",
			0, // intro_seen
			SCENARIO_ID_CITY2,
			{
				{ MONSTER_ID_STREUNER__LVL_4, 3, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_STREUNER__LVL_4, 14, 9, FIG_VIEWDIR_LEFT, 5 }, // enemy 1
				{ MONSTER_ID_STREUNER__LVL_4, 13, 10, FIG_VIEWDIR_LEFT, 5 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_5, 14, 11, FIG_VIEWDIR_LEFT, 15 }, // enemy 3
				{ MONSTER_ID_PIRAT__LVL_5, 15, 12, FIG_VIEWDIR_UP, 15 }, // enemy 4
				{ MONSTER_ID_PIRAT__LVL_5, 15, 10, FIG_VIEWDIR_LEFT, 15 }, // enemy 5
				{ MONSTER_ID_PIRAT__LVL_5, 14, 8, FIG_VIEWDIR_LEFT, 15 } // enemy 6
			},
			{
				{ 7, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 7, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 6, 9, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 5, 8, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 6, 13, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 8, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 5, 6, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_WEINFLASCHE, // loot 2
				ITEM_ID_RAPIER, // loot 3
				ITEM_ID_RAPIER, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_WEINFLASCHE, // loot 7
				ITEM_ID_WEINFLASCHE, // loot 8
				ITEM_ID_SKRAJA, // loot 9
				ITEM_ID_SKRAJA, // loot 10
				ITEM_ID_ENTERMESSER, // loot 11
				ITEM_ID_ENTERMESSER, // loot 12
				ITEM_ID_ENTERMESSER, // loot 13
				ITEM_ID_ENTERMESSER, // loot 14
				ITEM_ID_SKRAJA, // loot 15
				ITEM_ID_SKRAJA, // loot 16
				ITEM_ID_DOLCH, // loot 17
				ITEM_ID_DOLCH, // loot 18
				ITEM_ID_DOLCH, // loot 19
				ITEM_ID_DOLCH, // loot 20
				ITEM_ID_SILBERSCHMUCK, // loot 21
				ITEM_ID_SILBERSCHMUCK, // loot 22
				ITEM_ID_SILBERSCHMUCK, // loot 23
				ITEM_ID_SILBERSCHMUCK, // loot 24
				ITEM_ID_SCHNAPSFLASCHE, // loot 25
				ITEM_ID_SCHNAPSFLASCHE, // loot 26
				ITEM_ID_SCHNAPSFLASCHE, // loot 27
				ITEM_ID_SCHNAPSFLASCHE // loot 28
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DASP18 == 212
			"DASP18",
			0, // intro_seen
			SCENARIO_ID_CITY3,
			{
				{ MONSTER_ID_STREUNER__LVL_4, 2, 5, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_STREUNER__LVL_4, 4, 4, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_STREUNER__LVL_4, 7, 3, FIG_VIEWDIR_UP, 0 }, // enemy 2
				{ MONSTER_ID_STREUNER__LVL_4, 11, 3, FIG_VIEWDIR_UP, 0 }, // enemy 3
				{ MONSTER_ID_STREUNER__LVL_4, 13, 4, FIG_VIEWDIR_LEFT, 0 }, // enemy 4
				{ MONSTER_ID_STREUNER__LVL_4, 3, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 5
				{ MONSTER_ID_PIRAT__LVL_3, 13, 10, FIG_VIEWDIR_LEFT, 10 }, // enemy 6
				{ MONSTER_ID_PIRAT__LVL_3, 13, 9, FIG_VIEWDIR_LEFT, 10 }, // enemy 7
				{ MONSTER_ID_PIRAT__LVL_3, 15, 10, FIG_VIEWDIR_LEFT, 10 }, // enemy 8
				{ MONSTER_ID_PIRAT__LVL_3, 14, 9, FIG_VIEWDIR_LEFT, 10 } // enemy 9
			},
			{
				{ 7, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 8, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 9, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 10, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 11, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 12, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 8, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_RAPIER, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_RAPIER, // loot 3
				ITEM_ID_RAPIER, // loot 4
				ITEM_ID_RAPIER, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_DOLCH, // loot 10
				ITEM_ID_DOLCH, // loot 11
				ITEM_ID_DOLCH, // loot 12
				ITEM_ID_WEINFLASCHE, // loot 13
				ITEM_ID_WEINFLASCHE, // loot 14
				ITEM_ID_WEINFLASCHE, // loot 15
				ITEM_ID_WEINFLASCHE, // loot 16
				ITEM_ID_WEINFLASCHE, // loot 17
				ITEM_ID_SKRAJA, // loot 18
				ITEM_ID_SKRAJA, // loot 19
				ITEM_ID_ENTERMESSER, // loot 20
				ITEM_ID_ENTERMESSER, // loot 21
				ITEM_ID_ENTERMESSER, // loot 22
				ITEM_ID_DOLCH, // loot 23
				ITEM_ID_DOLCH, // loot 24
				ITEM_ID_SCHNAPSFLASCHE, // loot 25
				ITEM_ID_SCHNAPSFLASCHE, // loot 26
				ITEM_ID_SCHNAPSFLASCHE, // loot 27
				ITEM_ID_GOLDSCHMUCK, // loot 28
				ITEM_ID_GOLDSCHMUCK // loot 29
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_THOR8 == 213
			"THOR8",
			0, // intro_seen
			SCENARIO_ID_CITY4,
			{
				{ MONSTER_ID_THORWALER__LVL_5, 10, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_THORWALER__LVL_5, 11, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_STADTGARDIST__LVL_5, 8, 1, FIG_VIEWDIR_UP, 10 }, // enemy 2
				{ MONSTER_ID_STADTGARDIST__LVL_3, 11, 1, FIG_VIEWDIR_UP, 10 }, // enemy 3
				{ MONSTER_ID_STADTGARDIST__LVL_3, 6, 1, FIG_VIEWDIR_RIGHT, 10 }, // enemy 4
				{ MONSTER_ID_STADTGARDIST__LVL_3, 13, 1, FIG_VIEWDIR_LEFT, 10 }, // enemy 5
				{ MONSTER_ID_STADTGARDIST__LVL_4, 19, 5, FIG_VIEWDIR_LEFT, 20 }, // enemy 6
				{ MONSTER_ID_STADTGARDIST__LVL_3, 20, 7, FIG_VIEWDIR_LEFT, 20 }, // enemy 7
				{ MONSTER_ID_STADTGARDIST__LVL_3, 21, 8, FIG_VIEWDIR_LEFT, 20 }, // enemy 8
				{ MONSTER_ID_STADTGARDIST__LVL_3, 20, 6, FIG_VIEWDIR_LEFT, 0 } // enemy 9
			},
			{
				{ 11, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 13, 7, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 8, 7, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 9, 7, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 11, 7, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_STREITAXT, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_SKRAJA, // loot 2
				ITEM_ID_SKRAJA, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_KNOCHEN_MIT_RUNE, // loot 8
				ITEM_ID_RAPIER, // loot 9
				ITEM_ID_RAPIER, // loot 10
				ITEM_ID_SILBERSCHMUCK, // loot 11
				ITEM_ID_SILBERSCHMUCK, // loot 12
				ITEM_ID_SILBERSCHMUCK // loot 13
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_CITYFIGHT1 == 214
			"CITYFIGHT1",
			0, // intro_seen
			SCENARIO_ID_CITY2,
			{
				{ MONSTER_ID_STADTGARDIST__LVL_5, 14, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_STADTGARDIST__LVL_3, 15, 12, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_STADTGARDIST__LVL_3, 15, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_STADTGARDIST__LVL_5, 9, 19, FIG_VIEWDIR_DOWN, 10 }, // enemy 3
				{ MONSTER_ID_STADTGARDIST__LVL_3, 8, 21, FIG_VIEWDIR_DOWN, 10 }, // enemy 4
				{ MONSTER_ID_STADTGARDIST__LVL_3, 10, 20, FIG_VIEWDIR_DOWN, 10 } // enemy 5
			},
			{
				{ 8, 17, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 10, 16, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 7, 14, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 9, 14, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 11, 15, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_SKRAJA, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_SCHWERT, // loot 3
				ITEM_ID_SCHWERT, // loot 4
				ITEM_ID_SILBERSCHMUCK, // loot 5
				ITEM_ID_GOLDSCHMUCK, // loot 6
				ITEM_ID_GOLDSCHMUCK, // loot 7
				ITEM_ID_RAPIER, // loot 8
				ITEM_ID_RAPIER, // loot 9
				ITEM_ID_SCHNAPSFLASCHE // loot 10
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_CITYFIGHT2 == 215
			"CITYFIGHT2",
			0, // intro_seen
			SCENARIO_ID_CITY4,
			{
				{ MONSTER_ID_STADTGARDIST__LVL_5, 9, 3, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_STADTGARDIST__LVL_3, 11, 1, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_STADTGARDIST__LVL_3, 12, 3, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_STADTGARDIST__LVL_3, 11, 5, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_STADTGARDIST__LVL_5, 11, 11, FIG_VIEWDIR_DOWN, 10 }, // enemy 4
				{ MONSTER_ID_STADTGARDIST__LVL_3, 9, 12, FIG_VIEWDIR_DOWN, 10 }, // enemy 5
				{ MONSTER_ID_STADTGARDIST__LVL_3, 12, 12, FIG_VIEWDIR_DOWN, 10 }, // enemy 6
				{ MONSTER_ID_STADTGARDIST__LVL_3, 10, 14, FIG_VIEWDIR_DOWN, 10 }, // enemy 7
				{ MONSTER_ID_STADTGARDIST__LVL_5, 18, 8, FIG_VIEWDIR_LEFT, 20 }, // enemy 8
				{ MONSTER_ID_STADTGARDIST__LVL_3, 20, 9, FIG_VIEWDIR_LEFT, 20 }, // enemy 9
				{ MONSTER_ID_STADTGARDIST__LVL_3, 20, 7, FIG_VIEWDIR_LEFT, 20 }, // enemy 10
				{ MONSTER_ID_STADTGARDIST__LVL_3, 21, 8, FIG_VIEWDIR_LEFT, 20 } // enemy 11
			},
			{
				{ 6, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 4, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 3, 4, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 3, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 2, 8, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_RAPIER, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_ENTERMESSER, // loot 3
				ITEM_ID_ENTERMESSER, // loot 4
				ITEM_ID_ENTERMESSER, // loot 5
				ITEM_ID_ENTERMESSER, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_SCHNAPSFLASCHE, // loot 9
				ITEM_ID_SCHNAPSFLASCHE, // loot 10
				ITEM_ID_SILBERSCHMUCK, // loot 11
				ITEM_ID_SILBERSCHMUCK, // loot 12
				ITEM_ID_GOLDSCHMUCK, // loot 13
				ITEM_ID_DOLCH, // loot 14
				ITEM_ID_SCHWERT // loot 15
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_CITYFIGHT3 == 216
			"CITYFIGHT3",
			0, // intro_seen
			SCENARIO_ID_CITY2,
			{
				{ MONSTER_ID_STADTGARDIST__LVL_5, 9, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_STADTGARDIST__LVL_5, 8, 16, FIG_VIEWDIR_DOWN, 10 }, // enemy 1
				{ MONSTER_ID_STADTGARDIST__LVL_5, 11, 17, FIG_VIEWDIR_DOWN, 20 }, // enemy 2
				{ MONSTER_ID_STADTGARDIST__LVL_3, 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_STADTGARDIST__LVL_3, 7, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_STADTGARDIST__LVL_3, 7, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 5
				{ MONSTER_ID_STADTGARDIST__LVL_3, 6, 17, FIG_VIEWDIR_DOWN, 10 }, // enemy 6
				{ MONSTER_ID_STADTGARDIST__LVL_3, 7, 18, FIG_VIEWDIR_DOWN, 10 }, // enemy 7
				{ MONSTER_ID_STADTGARDIST__LVL_3, 8, 18, FIG_VIEWDIR_DOWN, 10 }, // enemy 8
				{ MONSTER_ID_STADTGARDIST__LVL_3, 10, 19, FIG_VIEWDIR_DOWN, 20 }, // enemy 9
				{ MONSTER_ID_STADTGARDIST__LVL_3, 11, 19, FIG_VIEWDIR_DOWN, 20 }, // enemy 10
				{ MONSTER_ID_STADTGARDIST__LVL_3, 12, 18, FIG_VIEWDIR_DOWN, 20 } // enemy 11
			},
			{
				{ 12, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 13, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 13, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 13, 12, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 14, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 15, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 14, 12, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_SKRAJA, // loot 0
				ITEM_ID_RAPIER, // loot 1
				ITEM_ID_SCHWERT, // loot 2
				ITEM_ID_ENTERMESSER, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_GOLDSCHMUCK, // loot 5
				ITEM_ID_SILBERSCHMUCK, // loot 6
				ITEM_ID_SCHNAPSFLASCHE, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_WEINFLASCHE, // loot 9
				ITEM_ID_ENTERMESSER // loot 10
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_CAMPFIGHT1 == 217
			"CAMPFIGHT1",
			0, // intro_seen
			SCENARIO_ID_CAMP1,
			{
				{ MONSTER_ID_OGER, 13, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_1, 15, 13, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 14, 15, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_1, 17, 12, FIG_VIEWDIR_LEFT, 5 } // enemy 3
			},
			{
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 8, FIG_VIEWDIR_DOWN, 1 }, // hero 1
				{ 6, 10, FIG_VIEWDIR_RIGHT, 1 }, // hero 2
				{ 4, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 3
				{ 3, 8, FIG_VIEWDIR_DOWN, 1 }, // hero 4
				{ 7, 6, FIG_VIEWDIR_RIGHT, 1 }, // hero 5
				{ 5, 6, FIG_VIEWDIR_DOWN, 1 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_DOLCH // loot 2
			},
			0, 2, 53 // ducats, silver, heller
		},
		{ // FIGHT_ID_CAMPFIGHT2 == 218
			"CAMPFIGHT2",
			0, // intro_seen
			SCENARIO_ID_CAMP1,
			{
				{ MONSTER_ID_ORK__LVL_1, 15, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_1, 16, 12, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 17, 9, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_1, 16, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 3
			},
			{
				{ 10, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 7, 14, FIG_VIEWDIR_LEFT, 1 }, // hero 1
				{ 8, 16, FIG_VIEWDIR_RIGHT, 1 }, // hero 2
				{ 8, 12, FIG_VIEWDIR_DOWN, 1 }, // hero 3
				{ 11, 16, FIG_VIEWDIR_LEFT, 1 }, // hero 4
				{ 10, 12, FIG_VIEWDIR_DOWN, 1 }, // hero 5
				{ 11, 14, FIG_VIEWDIR_RIGHT, 1 } // hero 6
			},
			{
				ITEM_ID_SCHWERT, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_SILBERSCHMUCK, // loot 6
				ITEM_ID_SILBERSCHMUCK // loot 7
			},
			3, 0, 25 // ducats, silver, heller
		},
		{ // FIGHT_ID_CAMPFIGHT3 == 219
			"CAMPFIGHT3",
			0, // intro_seen
			SCENARIO_ID_CAMP2,
			{
				{ MONSTER_ID_HARPYIE, 9, 4, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_HARPYIE, 17, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_HARPYIE, 9, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 2
			},
			{
				{ 15, 8, FIG_VIEWDIR_RIGHT, 1 }, // hero 0
				{ 6, 6, FIG_VIEWDIR_DOWN, 1 }, // hero 1
				{ 7, 8, FIG_VIEWDIR_DOWN, 1 }, // hero 2
				{ 8, 8, FIG_VIEWDIR_LEFT, 1 }, // hero 3
				{ 9, 7, FIG_VIEWDIR_LEFT, 1 }, // hero 4
				{ 7, 5, FIG_VIEWDIR_UP, 1 }, // hero 5
				{ 8, 6, FIG_VIEWDIR_LEFT, 1 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP10_1 == 220
			"SHIP10.1",
			0, // intro_seen
			SCENARIO_ID_SHIP10,
			{
				{ MONSTER_ID_HESHTHOT__LVL_2, 3, 2, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 1, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 3, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 4, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 6, 15, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 11, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 7, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 2, 10, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F061_6B == 221
			"F0616#",
			0, // intro_seen
			SCENARIO_ID_F061_5,
			{
				{ MONSTER_ID_SAEBELZAHNTIGER, 4, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_SAEBELZAHNTIGER, 8, 12, FIG_VIEWDIR_RIGHT, 11 }, // enemy 1
				{ MONSTER_ID_SAEBELZAHNTIGER, 9, 11, FIG_VIEWDIR_RIGHT, 11 }, // enemy 2
				{ MONSTER_ID_SAEBELZAHNTIGER, 7, 10, FIG_VIEWDIR_DOWN, 11 }, // enemy 3
				{ MONSTER_ID_SAEBELZAHNTIGER, 10, 4, FIG_VIEWDIR_LEFT, 16 }, // enemy 4
				{ MONSTER_ID_SAEBELZAHNTIGER, 12, 5, FIG_VIEWDIR_LEFT, 16 }, // enemy 5
				{ MONSTER_ID_SAEBELZAHNTIGER, 10, 7, FIG_VIEWDIR_UP, 16 }, // enemy 6
				{ MONSTER_ID_SAEBELZAHNTIGER, 12, 8, FIG_VIEWDIR_UP, 16 }, // enemy 7
				{ MONSTER_ID_SAEBELZAHNTIGER, 15, 10, FIG_VIEWDIR_LEFT, 16 }, // enemy 8
				{ MONSTER_ID_SAEBELZAHNTIGER, 14, 9, FIG_VIEWDIR_LEFT, 16 } // enemy 9
			},
			{
				{ 6, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 7, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 7, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 11, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 2, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 10, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_16A == 222
			"F05116A",
			0, // intro_seen
			SCENARIO_ID_F051_16,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 12, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_HOHEPRIESTER, 2, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 3, 19, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 9, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_HOEHLENSPINNE__LVL_3, 7, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 7, 18, FIG_VIEWDIR_RIGHT, 0 }, // enemy 5
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 8, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 6
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 9, 19, FIG_VIEWDIR_DOWN, 0 } // enemy 7
			},
			{
				{ 6, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 7, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 11, 8, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 6, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 6, 3, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_16B == 223
			"F05116B",
			0, // intro_seen
			SCENARIO_ID_F051_16,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 12, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 3, 19, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 9, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_HOEHLENSPINNE__LVL_3, 7, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 7, 18, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 8, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 5
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 9, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 6
				{ MONSTER_ID_NONE, 7, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 7
				{ MONSTER_ID_NONE, 7, 19, FIG_VIEWDIR_DOWN, 0 } // enemy 8
			},
			{
				{ 6, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 7, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 11, 8, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 6, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 6, 3, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_14A == 224
			"F05114A",
			0, // intro_seen
			SCENARIO_ID_F051_14,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 6, 22, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_HOHEPRIESTER, 8, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_PRIESTER__LVL_3, 3, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_PRIESTER__LVL_3, 4, 10, FIG_VIEWDIR_RIGHT, 0 } // enemy 3
			},
			{
				{ 8, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 8, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 9, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 11, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 11, 7, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_VULKANGLASDOLCH, // loot 0
				ITEM_ID_VULKANGLASDOLCH // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_14B == 225
			"F05114B",
			0, // intro_seen
			SCENARIO_ID_F051_14,
			{
				{ MONSTER_ID_PRIESTER__LVL_3, 3, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PRIESTER__LVL_3, 4, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_HOEHLENSPINNE__LVL_2, 5, 20, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_NONE, 5, 20, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_NONE, 5, 20, FIG_VIEWDIR_DOWN, 0 } // enemy 4
			},
			{
				{ 8, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 8, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 9, 9, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 9, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 11, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 11, 7, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_VULKANGLASDOLCH, // loot 0
				ITEM_ID_VULKANGLASDOLCH // loot 1
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_117 == 226
			"F051117",
			0, // intro_seen
			SCENARIO_ID_F051_17,
			{
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 5, 12, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_HOEHLENSPINNE__LVL_1, 6, 13, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 7, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 5, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 4, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 6, 8, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 3, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_124 == 227
			"F129124",
			0, // intro_seen
			SCENARIO_ID_F129_24,
			{
				{ MONSTER_ID_ZOMBIE, 3, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ZOMBIE, 5, 4, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_ZOMBIE, 12, 8, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 7, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 9, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 7, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 9, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 8, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 7, 10, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_08A == 228
			"F13108A",
			0, // intro_seen
			SCENARIO_ID_F131_05,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 7, 7, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 9, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 10, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 11, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 12, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 12, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_08B == 229
			"F13108B",
			0, // intro_seen
			SCENARIO_ID_F131_05,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 7, 7, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_3, 7, 10, FIG_VIEWDIR_RIGHT, 0 } // enemy 1
			},
			{
				{ 9, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 10, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 11, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 12, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 12, 7, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_SILBERSCHMUCK, // loot 2
				ITEM_ID_SAEBEL, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_SILBERSCHMUCK // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F031_14A == 230
			"F03114A",
			0, // intro_seen
			SCENARIO_ID_F131_14,
			{
				{ MONSTER_ID_PIRAT__LVL_5, 2, 10, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 11, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 12, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 13, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 11, 7, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 14, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 11, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 12, 7, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SILBERSCHMUCK, // loot 0
				ITEM_ID_ENTERMESSER, // loot 1
				ITEM_ID_SKRAJA // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTH021A == 231
			"DTH021A",
			0, // intro_seen
			SCENARIO_ID_DTHO21A,
			{
				{ MONSTER_ID_RAEUBER__LVL_2, 6, 18, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_2, 7, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_RAEUBER__LVL_2, 9, 18, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_RAEUBER__LVL_3, 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_RAEUBER__LVL_3, 6, 6, FIG_VIEWDIR_UP, 0 }, // enemy 4
				{ MONSTER_ID_RAEUBER__LVL_6, 15, 12, FIG_VIEWDIR_LEFT, 0 } // enemy 5
			},
			{
				{ 9, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 12, 5, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 9, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 13, 4, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 10, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 10, 2, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_RAPIER, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_SKRAJA, // loot 6
				ITEM_ID_SKRAJA, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_DOLCH, // loot 9
				ITEM_ID_BIER, // loot 10
				ITEM_ID_BIER, // loot 11
				ITEM_ID_ORKNASE, // loot 12
				ITEM_ID_SAEBEL, // loot 13
				ITEM_ID_DOLCH, // loot 14
				ITEM_ID_GOLDSCHMUCK, // loot 15
				ITEM_ID_WEINFLASCHE, // loot 16
				ITEM_ID_SCHLUESSEL__YELLOW // loot 17
			},
			0, 51, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_CAMPFIGHT4 == 232
			"CAMPFIGHT4",
			0, // intro_seen
			SCENARIO_ID_CAMPFIGHT4,
			{
				{ MONSTER_ID_WALDLOEWE, 14, 3, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_WALDLOEWE, 12, 6, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 9, 3, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 5, 2, FIG_VIEWDIR_DOWN, 1 }, // hero 1
				{ 6, 2, FIG_VIEWDIR_DOWN, 1 }, // hero 2
				{ 3, 3, FIG_VIEWDIR_DOWN, 1 }, // hero 3
				{ 5, 7, FIG_VIEWDIR_DOWN, 1 }, // hero 4
				{ 3, 6, FIG_VIEWDIR_RIGHT, 1 }, // hero 5
				{ 7, 4, FIG_VIEWDIR_DOWN, 1 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_SHIP10_2 == 233
			"SHIP10.2",
			0, // intro_seen
			SCENARIO_ID_SHIP10_1,
			{
				{ MONSTER_ID_HESHTHOT__LVL_2, 3, 4, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 1, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 2, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 3, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 4, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 5, 6, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 1, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 4, 8, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_02 == 234
			"F05102",
			0, // intro_seen
			SCENARIO_ID_F051_2,
			{
				{ MONSTER_ID_KULTIST__LVL_3__1, 9, 11, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_KULTIST__LVL_3__1, 6, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_PRIESTER__LVL_3, 9, 5, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_KULTIST__LVL_3__1, 11, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_NONE, 6, 12, FIG_VIEWDIR_LEFT, 0 } // enemy 4
			},
			{
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 4, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 5, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 4, 6, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_ARAXGIFT, // loot 0
				ITEM_ID_ARAXGIFT, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_ARAXGIFT, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_VULKANGLASDOLCH // loot 6
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_5 == 235
			"F0515",
			0, // intro_seen
			SCENARIO_ID_F061_5,
			{
				{ MONSTER_ID_GRIMWOLF__LVL_1, 3, 3, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_GRIMWOLF__LVL_1, 3, 4, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_GRIMWOLF__LVL_1, 4, 5, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_GRIMWOLF__LVL_1, 6, 3, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_GRIMWOLF__LVL_1, 3, 6, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_GRIMWOLF__LVL_1, 6, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 5
				{ MONSTER_ID_GRIMWOLF__LVL_3, 11, 13, FIG_VIEWDIR_LEFT, 10 }, // enemy 6
				{ MONSTER_ID_GRIMWOLF__LVL_3, 10, 12, FIG_VIEWDIR_LEFT, 10 }, // enemy 7
				{ MONSTER_ID_GRIMWOLF__LVL_1, 12, 9, FIG_VIEWDIR_DOWN, 10 }, // enemy 8
				{ MONSTER_ID_GRIMWOLF__LVL_1, 11, 10, FIG_VIEWDIR_LEFT, 10 } // enemy 9
			},
			{
				{ 8, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 7, 9, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 9, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 7, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 10, 7, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 8, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 9, 9, FIG_VIEWDIR_LEFT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_PHEX3 == 236
			"PHEX3",
			0, // intro_seen
			SCENARIO_ID_CITY2,
			{
				{ MONSTER_ID_STREUNER__LVL_2, 9, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_STREUNER__LVL_2, 7, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_STREUNER__LVL_2, 13, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_STREUNER__LVL_2, 13, 12, FIG_VIEWDIR_LEFT, 0 } // enemy 3
			},
			{
				{ 5, 9, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 9, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 11, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 3, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 8, 6, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 5, 7, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_SILBERSCHMUCK, // loot 4
				ITEM_ID_SCHNAPSFLASCHE, // loot 5
				ITEM_ID_BIER // loot 6
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_PHEX22 == 237
			"PHEX22",
			0, // intro_seen
			SCENARIO_ID_CITY4,
			{
				{ MONSTER_ID_STREUNER__LVL_2, 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_STREUNER__LVL_2, 16, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_STREUNER__LVL_2, 8, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_STREUNER__LVL_2, 11, 17, FIG_VIEWDIR_DOWN, 0 } // enemy 3
			},
			{
				{ 11, 8, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 10, 10, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 12, 9, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 11, 11, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 10, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 4
				{ 12, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 11, 13, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_DOLCH, // loot 1
				ITEM_ID_DOLCH, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_LEDERHELM, // loot 4
				ITEM_ID_LEDERHELM // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_PHEX23 == 238
			"PHEX23",
			0, // intro_seen
			SCENARIO_ID_CITY3,
			{
				{ MONSTER_ID_STREUNER__LVL_2, 6, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_STREUNER__LVL_2, 11, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_STREUNER__LVL_2, 8, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_STREUNER__LVL_2, 12, 6, FIG_VIEWDIR_LEFT, 0 } // enemy 3
			},
			{
				{ 5, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 4, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 4, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 3, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 2, 12, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 0, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 1, 14, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_SAEBEL, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_SAEBEL, // loot 2
				ITEM_ID_SAEBEL, // loot 3
				ITEM_ID_IRON_SHIELD, // loot 4
				ITEM_ID_IRON_SHIELD, // loot 5
				ITEM_ID_IRON_SHIELD // loot 6
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_PHEX24 == 239
			"PHEX24",
			0, // intro_seen
			SCENARIO_ID_CITY1,
			{
				{ MONSTER_ID_STREUNER__LVL_3, 8, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_STREUNER__LVL_3, 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_STREUNER__LVL_2, 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_STREUNER__LVL_3, 4, 11, FIG_VIEWDIR_DOWN, 0 } // enemy 3
			},
			{
				{ 9, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 10, 12, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 14, FIG_VIEWDIR_DOWN, 0 }, // hero 2
				{ 12, 14, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 12, 13, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 12, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 9, 13, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_BASTARDSCHWERT, // loot 0
				ITEM_ID_SCHWERT, // loot 1
				ITEM_ID_SCHWERT, // loot 2
				ITEM_ID_SCHWERT, // loot 3
				ITEM_ID_LEDERHARNISCH, // loot 4
				ITEM_ID_LEDERHARNISCH // loot 5
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F131_01 == 240
			"F13101",
			0, // intro_seen
			SCENARIO_ID_F131_01,
			{
				{ MONSTER_ID_PIRAT__LVL_3, 10, 14, FIG_VIEWDIR_DOWN, 0 } // enemy 0
			},
			{
				{ 7, 7, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 5, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 5, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 9, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 7, 4, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 8, 4, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_SAEBEL, // loot 1
				ITEM_ID_SILBERSCHMUCK // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_08A == 241
			"F12908A",
			0, // intro_seen
			SCENARIO_ID_F129_09,
			{
				{ MONSTER_ID_PIRAT__LVL_2, 8, 4, FIG_VIEWDIR_UP, 0 } // enemy 0
			},
			{
				{ 7, 16, FIG_VIEWDIR_DOWN, 0 }, // hero 0
				{ 6, 15, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 5, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 3, 16, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 3, 17, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 2, 15, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{
				ITEM_ID_DOLCH, // loot 0
				ITEM_ID_LEDERZEUG, // loot 1
				ITEM_ID_ENTERMESSER // loot 2
			},
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DTHO14 == 242
			"DTHO14",
			0, // intro_seen
			SCENARIO_ID_DTHO14,
			{
				{ MONSTER_ID_RAEUBER__LVL_3, 8, 19, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_RAEUBER__LVL_2, 4, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_RAEUBER__LVL_3, 10, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_RAEUBER__LVL_3, 13, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 3
				{ MONSTER_ID_RAEUBER__LVL_2, 14, 5, FIG_VIEWDIR_UP, 0 }, // enemy 4
				{ MONSTER_ID_RAEUBER__LVL_2, 2, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 5
				{ MONSTER_ID_NONE, 2, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 6
				{ MONSTER_ID_NONE, 2, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 7
				{ MONSTER_ID_NONE, 2, 16, FIG_VIEWDIR_RIGHT, 0 }, // enemy 8
				{ MONSTER_ID_NONE, 2, 16, FIG_VIEWDIR_RIGHT, 0 } // enemy 9
			},
			{
				{ 7, 14, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 5, 13, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 10, 12, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 8, 14, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 12, 11, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 14, 11, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 9, 10, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{
				ITEM_ID_RAPIER, // loot 0
				ITEM_ID_RAPIER, // loot 1
				ITEM_ID_RAPIER, // loot 2
				ITEM_ID_DOLCH, // loot 3
				ITEM_ID_DOLCH, // loot 4
				ITEM_ID_DOLCH, // loot 5
				ITEM_ID_DOLCH, // loot 6
				ITEM_ID_DOLCH, // loot 7
				ITEM_ID_DOLCH, // loot 8
				ITEM_ID_SKRAJA, // loot 9
				ITEM_ID_SKRAJA, // loot 10
				ITEM_ID_SKRAJA, // loot 11
				ITEM_ID_BIER, // loot 12
				ITEM_ID_BIER, // loot 13
				ITEM_ID_BIER // loot 14
			},
			0, 66, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_WILD1 == 243
			"WILD1",
			0, // intro_seen
			SCENARIO_ID_WILD1,
			{
				{ MONSTER_ID_GOBLIN__LVL_1, 7, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_GOBLIN__LVL_1, 12, 2, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_GOBLIN__LVL_1, 9, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_GOBLIN__LVL_1, 4, 2, FIG_VIEWDIR_UP, 0 }, // enemy 3
				{ MONSTER_ID_GOBLIN__LVL_1, 11, 11, FIG_VIEWDIR_LEFT, 0 }, // enemy 4
				{ MONSTER_ID_GOBLIN__LVL_1, 3, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 5
				{ MONSTER_ID_GOBLIN__LVL_1, 6, 13, FIG_VIEWDIR_DOWN, 0 } // enemy 6
			},
			{
				{ 7, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 6, 7, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 6, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 6, 5, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 7, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 5, 4, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_WILD2 == 244
			"WILD2",
			0, // intro_seen
			SCENARIO_ID_WILD2,
			{
				{ MONSTER_ID_OGER, 7, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_OGER, 4, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_OGER, 14, 4, FIG_VIEWDIR_LEFT, 0 } // enemy 2
			},
			{
				{ 6, 10, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 7, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 4, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 2, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 2, 6, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_WILD3 == 245
			"WILD3",
			0, // intro_seen
			SCENARIO_ID_WILD3,
			{
				{ MONSTER_ID_SAEBELZAHNTIGER, 3, 3, FIG_VIEWDIR_UP, 0 }, // enemy 0
				{ MONSTER_ID_SAEBELZAHNTIGER, 17, 7, FIG_VIEWDIR_LEFT, 0 } // enemy 1
			},
			{
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 9, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // hero 2
				{ 6, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 3
				{ 5, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 3, 10, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_WILD4 == 246
			"WILD4",
			0, // intro_seen
			SCENARIO_ID_WILD4,
			{
				{ MONSTER_ID_GRIMWOLF__LVL_1, 10, 13, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_GRIMWOLF__LVL_1, 9, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_GRIMWOLF__LVL_1, 16, 5, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_GRIMWOLF__LVL_1, 14, 15, FIG_VIEWDIR_LEFT, 0 }, // enemy 3
				{ MONSTER_ID_GRIMWOLF__LVL_1, 18, 9, FIG_VIEWDIR_DOWN, 0 }, // enemy 4
				{ MONSTER_ID_GRIMWOLF__LVL_1, 15, 14, FIG_VIEWDIR_LEFT, 0 }, // enemy 5
				{ MONSTER_ID_GRIMWOLF__LVL_1, 6, 2, FIG_VIEWDIR_RIGHT, 0 } // enemy 6
			},
			{
				{ 9, 8, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 7, FIG_VIEWDIR_RIGHT, 0 }, // hero 1
				{ 7, 7, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 10, 5, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 7, 5, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 9, 4, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 8, 4, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_WILD5 == 247
			"WILD5",
			0, // intro_seen
			SCENARIO_ID_WILD5,
			{
				{ MONSTER_ID_STEPPENHUND, 10, 8, FIG_VIEWDIR_LEFT, 0 }, // enemy 0
				{ MONSTER_ID_STEPPENHUND, 14, 10, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_STEPPENHUND, 7, 3, FIG_VIEWDIR_UP, 0 } // enemy 2
			},
			{
				{ 7, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 0
				{ 3, 12, FIG_VIEWDIR_DOWN, 0 }, // hero 1
				{ 4, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 3, 10, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 2, 10, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 1, 11, FIG_VIEWDIR_RIGHT, 0 }, // hero 5
				{ 2, 12, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_WILD6 == 248
			"WILD6",
			0, // intro_seen
			SCENARIO_ID_WILD6,
			{
				{ MONSTER_ID_WALDSPINNE, 6, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_WALDSPINNE, 13, 2, FIG_VIEWDIR_UP, 0 }, // enemy 1
				{ MONSTER_ID_WALDSPINNE, 9, 16, FIG_VIEWDIR_DOWN, 0 } // enemy 2
			},
			{
				{ 10, 10, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 10, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 12, 8, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 10, 6, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 11, 7, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 12, 6, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 9, 5, FIG_VIEWDIR_RIGHT, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_WILD7 == 249
			"WILD7",
			0, // intro_seen
			SCENARIO_ID_WILD7,
			{
				{ MONSTER_ID_HARPYIE, 7, 12, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_HARPYIE, 9, 10, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 7, 5, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 6, 4, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 8, 4, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 3, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 8, 3, FIG_VIEWDIR_UP, 0 }, // hero 4
				{ 5, 3, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 6, 2, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_WILD8 == 250
			"WILD8",
			0, // intro_seen
			SCENARIO_ID_WILD8,
			{
				{ MONSTER_ID_ORK__LVL_1, 9, 15, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_1, 9, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 10, 17, FIG_VIEWDIR_DOWN, 0 }, // enemy 2
				{ MONSTER_ID_ORK__LVL_1, 8, 1, FIG_VIEWDIR_UP, 0 }, // enemy 3
				{ MONSTER_ID_ORK__LVL_1, 4, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_ORK__LVL_1, 7, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 5
				{ MONSTER_ID_ORK__LVL_1, 11, 14, FIG_VIEWDIR_DOWN, 0 }, // enemy 6
				{ MONSTER_ID_ORK__LVL_1, 10, 1, FIG_VIEWDIR_UP, 0 }, // enemy 7
				{ MONSTER_ID_ORK__LVL_1, 4, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 8
				{ MONSTER_ID_ORK__LVL_1, 6, 2, FIG_VIEWDIR_UP, 0 } // enemy 9
			},
			{
				{ 9, 10, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 9, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 9, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 8, 8, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 9, 7, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_S001 == 251
			"S001",
			0, // intro_seen
			SCENARIO_ID_S001,
			{
				{ MONSTER_ID_PIRAT__LVL_1, 8, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 0
				{ MONSTER_ID_PIRAT__LVL_1, 1, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_PIRAT__LVL_1, 2, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_PIRAT__LVL_1, 2, 13, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_PIRAT__LVL_1, 4, 1, FIG_VIEWDIR_RIGHT, 0 }, // enemy 4
				{ MONSTER_ID_PIRAT__LVL_1, 5, 16, FIG_VIEWDIR_DOWN, 0 }, // enemy 5
				{ MONSTER_ID_PIRAT__LVL_1, 10, 18, FIG_VIEWDIR_DOWN, 0 }, // enemy 6
				{ MONSTER_ID_PIRAT__LVL_1, 1, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 7
				{ MONSTER_ID_NONE, 6, 2, FIG_VIEWDIR_UP, 0 }, // enemy 8
				{ MONSTER_ID_NONE, 6, 2, FIG_VIEWDIR_UP, 0 } // enemy 9
			},
			{
				{ 9, 10, FIG_VIEWDIR_UP, 0 }, // hero 0
				{ 8, 10, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 10, 9, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 7, 9, FIG_VIEWDIR_UP, 0 }, // hero 3
				{ 10, 8, FIG_VIEWDIR_RIGHT, 0 }, // hero 4
				{ 8, 8, FIG_VIEWDIR_UP, 0 }, // hero 5
				{ 9, 7, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F051_14C == 252
			"F05114C",
			0, // intro_seen
			SCENARIO_ID_F051_14,
			{
				{ MONSTER_ID_SPINNENDAEMON, 2, 12, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 6, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 4, 8, FIG_VIEWDIR_UP, 0 }, // hero 1
				{ 2, 9, FIG_VIEWDIR_UP, 0 }, // hero 2
				{ 4, 15, FIG_VIEWDIR_DOWN, 0 }, // hero 3
				{ 6, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 7, 5, FIG_VIEWDIR_DOWN, 0 }, // hero 5
				{ 7, 17, FIG_VIEWDIR_UP, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_F129_17 == 253
			"F12917",
			0, // intro_seen
			SCENARIO_ID_F129_17,
			{
				{ MONSTER_ID_HESHTOT__WEAK, 6, 11, FIG_VIEWDIR_RIGHT, 0 } // enemy 0
			},
			{
				{ 9, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 0
				{ 9, 12, FIG_VIEWDIR_LEFT, 0 }, // hero 1
				{ 10, 10, FIG_VIEWDIR_LEFT, 0 }, // hero 2
				{ 9, 13, FIG_VIEWDIR_LEFT, 0 }, // hero 3
				{ 11, 11, FIG_VIEWDIR_LEFT, 0 }, // hero 4
				{ 11, 13, FIG_VIEWDIR_LEFT, 0 }, // hero 5
				{ 10, 14, FIG_VIEWDIR_DOWN, 0 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DCAMPFIGHT1 == 254
			"DCAMPFIGHT1",
			0, // intro_seen
			SCENARIO_ID_F129_17,
			{
				{ MONSTER_ID_ORK__LVL_2, 5, 7, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_ORK__LVL_1, 8, 12, FIG_VIEWDIR_DOWN, 0 } // enemy 1
			},
			{
				{ 7, 4, FIG_VIEWDIR_UP, 1 }, // hero 0
				{ 9, 4, FIG_VIEWDIR_UP, 1 }, // hero 1
				{ 5, 5, FIG_VIEWDIR_UP, 1 }, // hero 2
				{ 7, 9, FIG_VIEWDIR_UP, 1 }, // hero 3
				{ 9, 8, FIG_VIEWDIR_DOWN, 1 }, // hero 4
				{ 6, 7, FIG_VIEWDIR_DOWN, 1 }, // hero 5
				{ 8, 8, FIG_VIEWDIR_DOWN, 1 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DCAMPFIGHT2 == 255
			"DCAMPFIGHT2",
			0, // intro_seen
			SCENARIO_ID_F076_06,
			{
				{ MONSTER_ID_OGER, 5, 6, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_GOBLIN__LVL_1, 13, 7, FIG_VIEWDIR_LEFT, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 13, 5, FIG_VIEWDIR_LEFT, 0 }, // enemy 2
				{ MONSTER_ID_OGER, 13, 4, FIG_VIEWDIR_LEFT, 0 } // enemy 3
			},
			{
				{ 6, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 0
				{ 7, 10, FIG_VIEWDIR_DOWN, 1 }, // hero 1
				{ 8, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 2
				{ 9, 10, FIG_VIEWDIR_DOWN, 1 }, // hero 3
				{ 10, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 4
				{ 11, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 5
				{ 8, 7, FIG_VIEWDIR_LEFT, 1 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DCAMPFIGHT3 == 256
			"DCAMPFIGHT3",
			0, // intro_seen
			SCENARIO_ID_F100_13,
			{
				{ MONSTER_ID_ZOMBIE, 5, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_OGER, 0, 12, FIG_VIEWDIR_RIGHT, 3 }, // enemy 1
				{ MONSTER_ID_OGER, 0, 10, FIG_VIEWDIR_RIGHT, 4 }, // enemy 2
				{ MONSTER_ID_OGER, 0, 11, FIG_VIEWDIR_RIGHT, 5 } // enemy 3
			},
			{
				{ 14, 9, FIG_VIEWDIR_LEFT, 1 }, // hero 0
				{ 10, 10, FIG_VIEWDIR_DOWN, 1 }, // hero 1
				{ 10, 7, FIG_VIEWDIR_RIGHT, 1 }, // hero 2
				{ 13, 11, FIG_VIEWDIR_DOWN, 1 }, // hero 3
				{ 12, 8, FIG_VIEWDIR_RIGHT, 1 }, // hero 4
				{ 11, 10, FIG_VIEWDIR_DOWN, 1 }, // hero 5
				{ 10, 12, FIG_VIEWDIR_RIGHT, 1 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
		{ // FIGHT_ID_DCAMPFIGHT4 == 257
			"DCAMPFIGHT4",
			0, // intro_seen
			SCENARIO_ID_F100_13,
			{
				{ MONSTER_ID_GOBLIN__LVL_1, 8, 11, FIG_VIEWDIR_RIGHT, 0 }, // enemy 0
				{ MONSTER_ID_GOBLIN__LVL_1, 6, 9, FIG_VIEWDIR_RIGHT, 0 }, // enemy 1
				{ MONSTER_ID_ORK__LVL_1, 7, 10, FIG_VIEWDIR_RIGHT, 0 }, // enemy 2
				{ MONSTER_ID_SKELETT__LVL_1, 8, 8, FIG_VIEWDIR_RIGHT, 0 }, // enemy 3
				{ MONSTER_ID_OGER, 0, 11, FIG_VIEWDIR_RIGHT, 6 } // enemy 4
			},
			{
				{ 15, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 0
				{ 14, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 1
				{ 13, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 2
				{ 12, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 3
				{ 11, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 4
				{ 10, 9, FIG_VIEWDIR_DOWN, 1 }, // hero 5
				{ 13, 12, FIG_VIEWDIR_DOWN, 1 } // hero 6
			},
			{ 0 }, // loot
			0, 0, 0 // ducats, silver, heller
		},
	}
};
