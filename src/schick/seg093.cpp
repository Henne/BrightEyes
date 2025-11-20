/**
 *	Rewrite of DSA1 v3.02_de functions of seg093 (travelmode)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg093.cpp
 */

#include <stdio.h>
#include <string.h>

#if !defined(__BORLANDC__)
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg025.h"
#include "seg028.h"
#include "seg075.h"
#include "seg094.h"
#include "seg097.h"

struct struct_land_route g_land_routes[59] = {
	{ TOWN_ID_THORWAL            , TOWN_ID_VAERMHAG           ,  34,  1, 10,  25,  0, 10, 100 }, //  1
	{ TOWN_ID_VAERMHAG           , TOWN_ID_VARNHEIM           ,  40,  1, 10,  20,  0, 10, 100 }, //  2
	{ TOWN_ID_VARNHEIM           , TOWN_ID_DASPOTA            ,  21,  1, 10,   5,  0, 10, 100 }, //  3
	{ TOWN_ID_THORWAL            , TOWN_ID_SERSKE             ,   9,  2,  5, 100,  0,  5, 100 }, //  4
	{ TOWN_ID_SERSKE             , TOWN_ID_MERSKE             ,  27,  2,  5, 100,  0,  5, 100 }, //  5
	{ TOWN_ID_MERSKE             , TOWN_ID_EFFERDUN           ,  15,  2,  5, 100,  0,  5, 100 }, //  6
	{ TOWN_ID_SERSKE             , TOWN_ID_BREIDA             ,  36, -1, 25,  10,  0, 10,  90 }, //  7
	{ TOWN_ID_FAEHRSTATION_TJOILA, TOWN_ID_TJOILA             ,   0,  7,  0,   0,  0,  0,   0 }, //  8
	{ TOWN_ID_TJOILA             , TOWN_ID_BREIDA             ,  28,  0, 20,  15,  5, 20,  75 }, //  9
	{ TOWN_ID_BREIDA             , TOWN_ID_PEILINEN           ,  33,  0, 25,  15,  5, 20,  75 }, // 10
	{ TOWN_ID_PEILINEN           , TOWN_ID_ROVAMUND           ,  30,  0, 30,  15,  5, 20,  75 }, // 11
	{ TOWN_ID_ROVAMUND           , TOWN_ID_NORDVEST           ,  38,  0, 30,  15,  5, 20,  75 }, // 12
	{ TOWN_ID_NORDVEST           , TOWN_ID_KRAVIK             ,  24,  0, 35,  15,  5, 20,  75 }, // 13
	{ TOWN_ID_KRAVIK             , TOWN_ID_SKELELLEN          ,  27, -1, 40,   5,  5, 30,  50 }, // 14
	{ TOWN_ID_THORWAL            , TOWN_ID_FAEHRSTATION_TJOILA,  21,  1, 15,  65,  0, 20, 100 }, // 15
	{ TOWN_ID_FAEHRSTATION_TJOILA, TOWN_ID_RUKIAN             ,  11,  1, 15,  55,  0, 20, 100 }, // 16
	{ TOWN_ID_RUKIAN             , TOWN_ID_FAEHRE_ANGBODIRTAL ,  22,  1, 15,  55,  0, 25, 100 }, // 17
	{ TOWN_ID_FAEHRE_ANGBODIRTAL , TOWN_ID_AUPLOG             ,  12,  1, 20,  55,  0, 25, 100 }, // 18
	{ TOWN_ID_FAEHRE_ANGBODIRTAL , TOWN_ID_ANGBODIRTAL        ,   0,  7,  0,   0,  0,  0,   0 }, // 19
	{ TOWN_ID_AUPLOG             , TOWN_ID_VILNHEIM           ,  24,  1, 15,  60,  0, 30, 100 }, // 20
	{ TOWN_ID_VILNHEIM           , TOWN_ID_BODON              ,   0,  8,  0,   0,  0,  0,   0 }, // 21
	{ TOWN_ID_VILNHEIM           , TOWN_ID_PHEXCAER           , 105, -1, 35,  10,  5, 45, 100 }, // 22
	{ TOWN_ID_PHEXCAER           , TOWN_ID_GROENVELDEN        ,  17, -1, 35,  25,  5, 60, 100 }, // 23
	{ TOWN_ID_PHEXCAER           , TOWN_ID_EINSIEDLERSEE      ,  57, -2, 25,   0,  0, 50, 100 }, // 24
	{ TOWN_ID_VARNHEIM           , TOWN_ID_AUPLOG             ,  52, -1, 15,  25,  5, 15,  75 }, // 25
	{ TOWN_ID_DASPOTA            , TOWN_ID_RYBON              ,  19, -2, 15,   5,  5,  5,  40 }, // 26
	{ TOWN_ID_DASPOTA            , TOWN_ID_OTTARJE            ,  36, -1, 20,   5, 30,  0,  20 }, // 27
	{ TOWN_ID_OTTARJE            , TOWN_ID_SKJAL              ,  48, -1, 15,  10, 30,  0,  10 }, // 28
	{ TOWN_ID_SKJAL              , TOWN_ID_PREM               ,  27,  0, 15,  25, 35,  0,   5 }, // 29
	{ TOWN_ID_PREM               , TOWN_ID_KORD               ,  34,  0, 10,  25, 30,  0,   0 }, // 30
	{ TOWN_ID_OTTARJE            , TOWN_ID_ORVIL              ,  65, -2, 30,   5, 10,  5,   5 }, // 31
	{ TOWN_ID_ORVIL              , TOWN_ID_ALA                ,  29,  0, 15,  30, 25, 10,  20 }, // 32
	{ TOWN_ID_ALA                , TOWN_ID_TJANSET            ,  25,  0, 20,  30, 25, 10,  15 }, // 33
	{ TOWN_ID_TJANSET            , TOWN_ID_LISKOR             ,  24,  0,  5,  25, 20, 10,  10 }, // 34
	{ TOWN_ID_LISKOR             , TOWN_ID_CLANEGH            ,  21,  0, 10,  30, 25, 10,  20 }, // 35
	{ TOWN_ID_ALA                , TOWN_ID_THOSS              ,  42, -2, 25,  20, 15,  5,  15 }, // 36
	{ TOWN_ID_THOSS              , TOWN_ID_LISKOR             ,  19,  0, 20,  30, 25,  5,  20 }, // 37
	{ TOWN_ID_TJANSET            , TOWN_ID_THOSS              ,  47,  0, 20,  25, 20,  5,  15 }, // 38
	{ TOWN_ID_VILNHEIM           , TOWN_ID_OBERORKEN          ,  12,  1, 15,  60,  0, 25, 100 }, // 39
	{ TOWN_ID_OBERORKEN          , TOWN_ID_FELSTEYN           ,  20, -2, 50,   5, 10, 30,  75 }, // 40
	{ TOWN_ID_FELSTEYN           , TOWN_ID_ORKANGER           ,  50, -2, 50,   5, 10, 30,  75 }, // 41
	{ TOWN_ID_ORKANGER           , TOWN_ID_CLANEGH            ,  30, -2, 50,   5, 10, 30,  75 }, // 42
	{ TOWN_ID_CLANEGH            , TOWN_ID_TYLDON             ,  28,  1, 20,  40, 50,  5,  15 }, // 43
	{ TOWN_ID_TYLDON             , TOWN_ID_VIDSAND            ,  29,  1, 20,  30, 50,  5,  10 }, // 44
	{ TOWN_ID_OBERORKEN          , TOWN_ID_EINSIEDLERSEE      ,  81, -3, 50,   0,  0, 50, 100 }, // 45
	{ TOWN_ID_FELSTEYN           , TOWN_ID_EINSIEDLERSEE      ,  67, -3, 50,   0,  0, 50, 100 }, // 46
	{ TOWN_ID_RYBON              , TOWN_ID_THOSS              ,  60, -4, 40,   0, 10,  5,  10 }, // 47
	{ TOWN_ID_SKJAL              , TOWN_ID_ORVIL              ,  68, -4, 40,   0,  5,  5,   5 }, // 48
	{ TOWN_ID_SKELELLEN          , TOWN_ID_PHEXCAER           , 140, -4, 50,   0,  0, 10,  30 }, // 49
	{ TOWN_ID_MERSKE             , TOWN_ID_ROVAMUND           ,  71, -2, 25,   5,  5, 10,  50 }, // 50
	{ TOWN_ID_ORVIL              , TOWN_ID_ROVIK              ,  23, -2, 25,   5,  5,  0,  20 }, // 51
	{ TOWN_ID_LJASDAHL           , TOWN_ID_LJASDAHL           ,  70, -3, 35,   0,  0,  0,   0 }, // 52 // circular route
	{ TOWN_ID_LJASDAHL           , TOWN_ID_HJALLANDER_HOF     ,   9, -2, 40,   5,  5,  0,   0 }, // 53
	{ TOWN_ID_RUNINSHAVEN        , TOWN_ID_LEUCHTTURM_RUNIN   ,  24, -3, 30,   0,  0,  0,   0 }, // 54 // not linked at LEUCHTTURM_RUNIN
	{ TOWN_ID_RUNINSHAVEN        , TOWN_ID_LEUCHTTURM_RUNIN   ,  17, -2, 35,   5,  5,  0,   0 }, // 55 // middle route; the only route linked at LEUCHTTURM_RUNIN
	{ TOWN_ID_BRENDHIL           , TOWN_ID_MANRIN             ,  30, -3, 35,   0,  0,  0,   0 }, // 56
	{ TOWN_ID_EINSIEDLERSEE      , TOWN_ID_EINSIEDLERSEE      ,  47, -3, 35,   0,  0, 50, 100 }, // 57 // circular route
	{ TOWN_ID_LEUCHTTURM_RUNIN   , TOWN_ID_RUNINSHAVEN        ,  27, -3, 25,   0,  0,  0,   0 }, // 58 // not linked at LEUCHTTURM_RUNIN
	{ 0                          , 0                          ,  50, -4, 50,   0,  0,  5,   9 }
}; // ds:0x9dc6
// towns not connected to any land route: TOWN_ID_OVERTHORN, TOWN_ID_HJALSINGOR, TOWN_ID_GUDDASUNDEN, TOWN_ID_TREBAN, TOWN_ID_ARYN

// ds:0x9fd9, 105 arrays, each terminated by -1
/* Numbers here are entries in g_land_routes */
int8_t g_signposts_linked_lroute_001[] = {15, -1};		//ds:0x9fd9;
int8_t g_signposts_linked_lroute_002[] = {4, -1};		//ds:0x9fdb;
int8_t g_signposts_linked_lroute_003[] = {1, -1};		//ds:0x9fdd;
int8_t g_signposts_linked_lroute_004[] = {4, -1};		//ds:0x9fdf;
int8_t g_signposts_linked_lroute_005[] = {5, 7, -1};		//ds:0x9fe1;
int8_t g_signposts_linked_lroute_006[] = {10, -1};		//ds:0x9fe4;
int8_t g_signposts_linked_lroute_007[] = {7, -1};		//ds:0x9fe6;
int8_t g_signposts_linked_lroute_008[] = {9, -1};		//ds:0x9fe8;
int8_t g_signposts_linked_lroute_009[] = {11, -1};		//ds:0x9fea;
int8_t g_signposts_linked_lroute_010[] = {10, -1};		//ds:0x9fec;
int8_t g_signposts_linked_lroute_011[] = {50, -1};		//ds:0x9fee;
int8_t g_signposts_linked_lroute_012[] = {12, -1};		//ds:0x9ff0;
int8_t g_signposts_linked_lroute_013[] = {11, -1};		//ds:0x9ff2;
int8_t g_signposts_linked_lroute_014[] = {12, -1};		//ds:0x9ff4;
int8_t g_signposts_linked_lroute_015[] = {13, -1};		//ds:0x9ff6;
int8_t g_signposts_linked_lroute_016[] = {13, -1};		//ds:0x9ff8;
int8_t g_signposts_linked_lroute_017[] = {14, -1};		//ds:0x9ffa;
int8_t g_signposts_linked_lroute_018[] = {14, -1};		//ds:0x9ffc;
int8_t g_signposts_linked_lroute_019[] = {49, -1};		//ds:0x9ffe;
int8_t g_signposts_linked_lroute_020[] = {50, -1};		//ds:0xa000;
int8_t g_signposts_linked_lroute_021[] = {5, -1};		//ds:0xa002;
int8_t g_signposts_linked_lroute_022[] = {6, -1};		//ds:0xa004;
int8_t g_signposts_linked_lroute_023[] = {6, -1};		//ds:0xa006;
int8_t g_signposts_linked_lroute_024[] = {8, -1};		//ds:0xa008;
int8_t g_signposts_linked_lroute_025[] = {9, -1};		//ds:0xa00a;
int8_t g_signposts_linked_lroute_026[] = {16, -1};		//ds:0xa00c;
int8_t g_signposts_linked_lroute_027[] = {17, -1};		//ds:0xa00e;
int8_t g_signposts_linked_lroute_028[] = {19, -1};		//ds:0xa010;
int8_t g_signposts_linked_lroute_029[] = {18, -1};		//ds:0xa012;
int8_t g_signposts_linked_lroute_030[] = {20, -1};		//ds:0xa014;
int8_t g_signposts_linked_lroute_031[] = {25, -1};		//ds:0xa016;
int8_t g_signposts_linked_lroute_032[] = {21, -1};		//ds:0xa018;
int8_t g_signposts_linked_lroute_033[] = {22, -1};		//ds:0xa01a;
int8_t g_signposts_linked_lroute_034[] = {20, -1};		//ds:0xa01c;
int8_t g_signposts_linked_lroute_035[] = {39, -1};		//ds:0xa01e;
int8_t g_signposts_linked_lroute_036[] = {21, -1};		//ds:0xa020;
int8_t g_signposts_linked_lroute_037[] = {39, -1};		//ds:0xa022;
int8_t g_signposts_linked_lroute_038[] = {40, -1};		//ds:0xa024;
int8_t g_signposts_linked_lroute_039[] = {45, -1};		//ds:0xa026;
int8_t g_signposts_linked_lroute_040[] = {23, -1};		//ds:0xa028;
int8_t g_signposts_linked_lroute_041[] = {49, -1};		//ds:0xa02a;
int8_t g_signposts_linked_lroute_042[] = {22, -1};		//ds:0xa02c;
int8_t g_signposts_linked_lroute_043[] = {24, -1};		//ds:0xa02e;
int8_t g_signposts_linked_lroute_044[] = {23, -1};		//ds:0xa030;
int8_t g_signposts_linked_lroute_045[] = {40, -1};		//ds:0xa032;
int8_t g_signposts_linked_lroute_046[] = {41, -1};		//ds:0xa034;
int8_t g_signposts_linked_lroute_047[] = {46, -1};		//ds:0xa036;
int8_t g_signposts_linked_lroute_048[] = {24, 46, 45, 57, 57, -1};	//ds:0xa038;
int8_t g_signposts_linked_lroute_049[] = {41, -1};		//ds:0xa03e;
int8_t g_signposts_linked_lroute_050[] = {42, -1};		//ds:0xa040;
int8_t g_signposts_linked_lroute_051[] = {42, -1};		//ds:0xa042;
int8_t g_signposts_linked_lroute_052[] = {35, -1};		//ds:0xa044;
int8_t g_signposts_linked_lroute_053[] = {43, -1};		//ds:0xa046;
int8_t g_signposts_linked_lroute_054[] = {35, -1};		//ds:0xa048;
int8_t g_signposts_linked_lroute_055[] = {37, -1};		//ds:0xa04a;
int8_t g_signposts_linked_lroute_056[] = {34, -1};		//ds:0xa04c;
int8_t g_signposts_linked_lroute_057[] = {37, -1};		//ds:0xa04e;
int8_t g_signposts_linked_lroute_058[] = {36, 38, -1};		//ds:0xa050;
int8_t g_signposts_linked_lroute_059[] = {47, -1};		//ds:0xa052;
int8_t g_signposts_linked_lroute_060[] = {34, -1};		//ds:0xa054;
int8_t g_signposts_linked_lroute_061[] = {33, 38, -1};		//ds:0xa056;
int8_t g_signposts_linked_lroute_062[] = {36, 33, -1};		//ds:0xa059;
int8_t g_signposts_linked_lroute_063[] = {32, -1};		//ds:0xa05c;
int8_t g_signposts_linked_lroute_064[] = {48, 31, -1};		//ds:0xa05e;
int8_t g_signposts_linked_lroute_065[] = {32, -1};		//ds:0xa062;
int8_t g_signposts_linked_lroute_066[] = {51, -1};		//ds:0xa064;
int8_t g_signposts_linked_lroute_067[] = {51, -1};		//ds:0xa066;
int8_t g_signposts_linked_lroute_068[] = {30, -1};		//ds:0xa068;
int8_t g_signposts_linked_lroute_069[] = {54, -1};		//ds:0xa06a;
int8_t g_signposts_linked_lroute_070[] = {58, -1};		//ds:0xa06c;
int8_t g_signposts_linked_lroute_071[] = {55, -1};		//ds:0xa06e;
int8_t g_signposts_linked_lroute_072[] = {27, -1};		//ds:0xa070;
int8_t g_signposts_linked_lroute_073[] = {28, -1};		//ds:0xa072;
int8_t g_signposts_linked_lroute_074[] = {31, -1};		//ds:0xa074;
int8_t g_signposts_linked_lroute_075[] = {28, -1};		//ds:0xa076;
int8_t g_signposts_linked_lroute_076[] = {29, -1};		//ds:0xa078;
int8_t g_signposts_linked_lroute_077[] = {48, -1};		//ds:0xa07a;
int8_t g_signposts_linked_lroute_078[] = {29, -1};		//ds:0xa07c;
int8_t g_signposts_linked_lroute_079[] = {30, -1};		//ds:0xa07e;
int8_t g_signposts_linked_lroute_080[] = {27, -1};		//ds:0xa080;
int8_t g_signposts_linked_lroute_081[] = {3, -1};		//ds:0xa082;
int8_t g_signposts_linked_lroute_082[] = {26, -1};		//ds:0xa084;
int8_t g_signposts_linked_lroute_083[] = {47, -1};		//ds:0xa086;
int8_t g_signposts_linked_lroute_084[] = {26, -1};		//ds:0xa088;
int8_t g_signposts_linked_lroute_085[] = {53, -1};		//ds:0xa08a;
int8_t g_signposts_linked_lroute_086[] = {52, -1};		//ds:0xa08c;
int8_t g_signposts_linked_lroute_087[] = {52, -1};		//ds:0xa08e;
int8_t g_signposts_linked_lroute_088[] = {2, -1};		//ds:0xa090;
int8_t g_signposts_linked_lroute_089[] = {25, -1};		//ds:0xa092;
int8_t g_signposts_linked_lroute_090[] = {3, -1};		//ds:0xa094;
int8_t g_signposts_linked_lroute_091[] = {1, -1};		//ds:0xa096;
int8_t g_signposts_linked_lroute_092[] = {2, -1};		//ds:0xa098;
int8_t g_signposts_linked_lroute_093[] = {43, -1};		//ds:0xa09a;
int8_t g_signposts_linked_lroute_094[] = {44, -1};		//ds:0xa09c;
int8_t g_signposts_linked_lroute_095[] = {44, -1};		//ds:0xa09e;
int8_t g_signposts_linked_lroute_096[] = {56, -1};		//ds:0xa0a0;
int8_t g_signposts_linked_lroute_097[] = {56, -1};		//ds:0xa0a2;
int8_t g_signposts_linked_lroute_098[] = {16, -1};		//ds:0xa0a4;
int8_t g_signposts_linked_lroute_099[] = {15, -1};		//ds:0xa0a6;
int8_t g_signposts_linked_lroute_100[] = {8, -1};		//ds:0xa0a8;
int8_t g_signposts_linked_lroute_101[] = {17, -1};		//ds:0xa0aa;
int8_t g_signposts_linked_lroute_102[] = {18, -1};		//ds:0xa0ac;
int8_t g_signposts_linked_lroute_103[] = {19, -1};		//ds:0xa0ae;
int8_t g_signposts_linked_lroute_104[] = {53, -1};		//ds:0xa0b0;
int8_t g_signposts_linked_lroute_105[] = {55, -1};		//ds:0xa0b2;

struct trv_start_point g_signposts[106] = {
	{ TOWN_ID_THORWAL            , 1, (uint8_t*)g_signposts_linked_lroute_001 }, //  1
	{ TOWN_ID_THORWAL            , 2, (uint8_t*)g_signposts_linked_lroute_002 }, //  2
	{ TOWN_ID_THORWAL            , 4, (uint8_t*)g_signposts_linked_lroute_003 }, //  3
	{ TOWN_ID_SERSKE             , 1, (uint8_t*)g_signposts_linked_lroute_004 }, //  4
	{ TOWN_ID_SERSKE             , 2, (uint8_t*)g_signposts_linked_lroute_005 }, //  5
	{ TOWN_ID_BREIDA             , 1, (uint8_t*)g_signposts_linked_lroute_006 }, //  6
	{ TOWN_ID_BREIDA             , 2, (uint8_t*)g_signposts_linked_lroute_007 }, //  7
	{ TOWN_ID_BREIDA             , 3, (uint8_t*)g_signposts_linked_lroute_008 }, //  8
	{ TOWN_ID_PEILINEN           , 1, (uint8_t*)g_signposts_linked_lroute_009 }, //  9
	{ TOWN_ID_PEILINEN           , 2, (uint8_t*)g_signposts_linked_lroute_010 }, // 10
	{ TOWN_ID_ROVAMUND           , 1, (uint8_t*)g_signposts_linked_lroute_011 }, // 11
	{ TOWN_ID_ROVAMUND           , 2, (uint8_t*)g_signposts_linked_lroute_012 }, // 12
	{ TOWN_ID_ROVAMUND           , 3, (uint8_t*)g_signposts_linked_lroute_013 }, // 13
	{ TOWN_ID_NORDVEST           , 1, (uint8_t*)g_signposts_linked_lroute_014 }, // 14
	{ TOWN_ID_NORDVEST           , 2, (uint8_t*)g_signposts_linked_lroute_015 }, // 15
	{ TOWN_ID_KRAVIK             , 1, (uint8_t*)g_signposts_linked_lroute_016 }, // 16
	{ TOWN_ID_KRAVIK             , 2, (uint8_t*)g_signposts_linked_lroute_017 }, // 17
	{ TOWN_ID_SKELELLEN          , 1, (uint8_t*)g_signposts_linked_lroute_018 }, // 18
	{ TOWN_ID_SKELELLEN          , 2, (uint8_t*)g_signposts_linked_lroute_019 }, // 19
	{ TOWN_ID_MERSKE             , 2, (uint8_t*)g_signposts_linked_lroute_020 }, // 20
	{ TOWN_ID_MERSKE             , 3, (uint8_t*)g_signposts_linked_lroute_021 }, // 21
	{ TOWN_ID_MERSKE             , 4, (uint8_t*)g_signposts_linked_lroute_022 }, // 22
	{ TOWN_ID_EFFERDUN           , 2, (uint8_t*)g_signposts_linked_lroute_023 }, // 23
	{ TOWN_ID_TJOILA             , 1, (uint8_t*)g_signposts_linked_lroute_024 }, // 24
	{ TOWN_ID_TJOILA             , 2, (uint8_t*)g_signposts_linked_lroute_025 }, // 25
	{ TOWN_ID_RUKIAN             , 1, (uint8_t*)g_signposts_linked_lroute_026 }, // 26
	{ TOWN_ID_RUKIAN             , 2, (uint8_t*)g_signposts_linked_lroute_027 }, // 27
	{ TOWN_ID_ANGBODIRTAL        , 1, (uint8_t*)g_signposts_linked_lroute_028 }, // 28
	{ TOWN_ID_AUPLOG             , 1, (uint8_t*)g_signposts_linked_lroute_029 }, // 29
	{ TOWN_ID_AUPLOG             , 2, (uint8_t*)g_signposts_linked_lroute_030 }, // 30
	{ TOWN_ID_AUPLOG             , 3, (uint8_t*)g_signposts_linked_lroute_031 }, // 31
	{ TOWN_ID_VILNHEIM           , 1, (uint8_t*)g_signposts_linked_lroute_032 }, // 32
	{ TOWN_ID_VILNHEIM           , 2, (uint8_t*)g_signposts_linked_lroute_033 }, // 33
	{ TOWN_ID_VILNHEIM           , 3, (uint8_t*)g_signposts_linked_lroute_034 }, // 34
	{ TOWN_ID_VILNHEIM           , 4, (uint8_t*)g_signposts_linked_lroute_035 }, // 35
	{ TOWN_ID_BODON              , 1, (uint8_t*)g_signposts_linked_lroute_036 }, // 36
	{ TOWN_ID_OBERORKEN          , 1, (uint8_t*)g_signposts_linked_lroute_037 }, // 37
	{ TOWN_ID_OBERORKEN          , 2, (uint8_t*)g_signposts_linked_lroute_038 }, // 38
	{ TOWN_ID_OBERORKEN          , 3, (uint8_t*)g_signposts_linked_lroute_039 }, // 39
	{ TOWN_ID_PHEXCAER           , 1, (uint8_t*)g_signposts_linked_lroute_040 }, // 40
	{ TOWN_ID_PHEXCAER           , 2, (uint8_t*)g_signposts_linked_lroute_041 }, // 41
	{ TOWN_ID_PHEXCAER           , 3, (uint8_t*)g_signposts_linked_lroute_042 }, // 42
	{ TOWN_ID_PHEXCAER           , 4, (uint8_t*)g_signposts_linked_lroute_043 }, // 43
	{ TOWN_ID_GROENVELDEN        , 1, (uint8_t*)g_signposts_linked_lroute_044 }, // 44
	{ TOWN_ID_FELSTEYN           , 1, (uint8_t*)g_signposts_linked_lroute_045 }, // 45
	{ TOWN_ID_FELSTEYN           , 2, (uint8_t*)g_signposts_linked_lroute_046 }, // 46
	{ TOWN_ID_FELSTEYN           , 3, (uint8_t*)g_signposts_linked_lroute_047 }, // 47
	{ TOWN_ID_EINSIEDLERSEE      , 1, (uint8_t*)g_signposts_linked_lroute_048 }, // 48
	{ TOWN_ID_ORKANGER           , 1, (uint8_t*)g_signposts_linked_lroute_049 }, // 49
	{ TOWN_ID_ORKANGER           , 2, (uint8_t*)g_signposts_linked_lroute_050 }, // 50
	{ TOWN_ID_CLANEGH            , 1, (uint8_t*)g_signposts_linked_lroute_051 }, // 51
	{ TOWN_ID_CLANEGH            , 2, (uint8_t*)g_signposts_linked_lroute_052 }, // 52
	{ TOWN_ID_CLANEGH            , 3, (uint8_t*)g_signposts_linked_lroute_053 }, // 53
	{ TOWN_ID_LISKOR             , 1, (uint8_t*)g_signposts_linked_lroute_054 }, // 54
	{ TOWN_ID_LISKOR             , 2, (uint8_t*)g_signposts_linked_lroute_055 }, // 55
	{ TOWN_ID_LISKOR             , 3, (uint8_t*)g_signposts_linked_lroute_056 }, // 56
	{ TOWN_ID_THOSS              , 1, (uint8_t*)g_signposts_linked_lroute_057 }, // 57
	{ TOWN_ID_THOSS              , 2, (uint8_t*)g_signposts_linked_lroute_058 }, // 58
	{ TOWN_ID_THOSS              , 3, (uint8_t*)g_signposts_linked_lroute_059 }, // 59
	{ TOWN_ID_TJANSET            , 1, (uint8_t*)g_signposts_linked_lroute_060 }, // 60
	{ TOWN_ID_TJANSET            , 2, (uint8_t*)g_signposts_linked_lroute_061 }, // 61
	{ TOWN_ID_ALA                , 1, (uint8_t*)g_signposts_linked_lroute_062 }, // 62
	{ TOWN_ID_ALA                , 2, (uint8_t*)g_signposts_linked_lroute_063 }, // 63
	{ TOWN_ID_ORVIL              , 1, (uint8_t*)g_signposts_linked_lroute_064 }, // 64
	{ TOWN_ID_ORVIL              , 2, (uint8_t*)g_signposts_linked_lroute_065 }, // 65
	{ TOWN_ID_ORVIL              , 3, (uint8_t*)g_signposts_linked_lroute_066 }, // 66
	{ TOWN_ID_ROVIK              , 1, (uint8_t*)g_signposts_linked_lroute_067 }, // 67
	{ TOWN_ID_KORD               , 1, (uint8_t*)g_signposts_linked_lroute_068 }, // 68
	{ TOWN_ID_RUNINSHAVEN        , 1, (uint8_t*)g_signposts_linked_lroute_069 }, // 69
	{ TOWN_ID_RUNINSHAVEN        , 2, (uint8_t*)g_signposts_linked_lroute_070 }, // 70
	{ TOWN_ID_RUNINSHAVEN        , 4, (uint8_t*)g_signposts_linked_lroute_071 }, // 71
	{ TOWN_ID_OTTARJE            , 1, (uint8_t*)g_signposts_linked_lroute_072 }, // 72
	{ TOWN_ID_OTTARJE            , 2, (uint8_t*)g_signposts_linked_lroute_073 }, // 73
	{ TOWN_ID_OTTARJE            , 3, (uint8_t*)g_signposts_linked_lroute_074 }, // 74
	{ TOWN_ID_SKJAL              , 1, (uint8_t*)g_signposts_linked_lroute_075 }, // 75
	{ TOWN_ID_SKJAL              , 2, (uint8_t*)g_signposts_linked_lroute_076 }, // 76
	{ TOWN_ID_SKJAL              , 3, (uint8_t*)g_signposts_linked_lroute_077 }, // 77
	{ TOWN_ID_PREM               , 1, (uint8_t*)g_signposts_linked_lroute_078 }, // 78
	{ TOWN_ID_PREM               , 2, (uint8_t*)g_signposts_linked_lroute_079 }, // 79
	{ TOWN_ID_DASPOTA            , 1, (uint8_t*)g_signposts_linked_lroute_080 }, // 80
	{ TOWN_ID_DASPOTA            , 2, (uint8_t*)g_signposts_linked_lroute_081 }, // 81
	{ TOWN_ID_DASPOTA            , 3, (uint8_t*)g_signposts_linked_lroute_082 }, // 82
	{ TOWN_ID_RYBON              , 1, (uint8_t*)g_signposts_linked_lroute_083 }, // 83
	{ TOWN_ID_RYBON              , 2, (uint8_t*)g_signposts_linked_lroute_084 }, // 84
	{ TOWN_ID_LJASDAHL           , 2, (uint8_t*)g_signposts_linked_lroute_085 }, // 85
	{ TOWN_ID_LJASDAHL           , 3, (uint8_t*)g_signposts_linked_lroute_086 }, // 86
	{ TOWN_ID_LJASDAHL           , 4, (uint8_t*)g_signposts_linked_lroute_087 }, // 87
	{ TOWN_ID_VARNHEIM           , 1, (uint8_t*)g_signposts_linked_lroute_088 }, // 88
	{ TOWN_ID_VARNHEIM           , 2, (uint8_t*)g_signposts_linked_lroute_089 }, // 89
	{ TOWN_ID_VARNHEIM           , 3, (uint8_t*)g_signposts_linked_lroute_090 }, // 90
	{ TOWN_ID_VAERMHAG           , 1, (uint8_t*)g_signposts_linked_lroute_091 }, // 91
	{ TOWN_ID_VAERMHAG           , 2, (uint8_t*)g_signposts_linked_lroute_092 }, // 92
	{ TOWN_ID_TYLDON             , 1, (uint8_t*)g_signposts_linked_lroute_093 }, // 93
	{ TOWN_ID_TYLDON             , 2, (uint8_t*)g_signposts_linked_lroute_094 }, // 94
	{ TOWN_ID_VIDSAND            , 1, (uint8_t*)g_signposts_linked_lroute_095 }, // 95
	{ TOWN_ID_BRENDHIL           , 1, (uint8_t*)g_signposts_linked_lroute_096 }, // 96
	{ TOWN_ID_MANRIN             , 1, (uint8_t*)g_signposts_linked_lroute_097 }, // 97
	{ TOWN_ID_FAEHRSTATION_TJOILA, 1, (uint8_t*)g_signposts_linked_lroute_098 }, // 98
	{ TOWN_ID_FAEHRSTATION_TJOILA, 2, (uint8_t*)g_signposts_linked_lroute_099 }, // 99
	{ TOWN_ID_FAEHRSTATION_TJOILA, 3, (uint8_t*)g_signposts_linked_lroute_100 }, // 100
	{ TOWN_ID_FAEHRE_ANGBODIRTAL , 1, (uint8_t*)g_signposts_linked_lroute_101 }, // 101
	{ TOWN_ID_FAEHRE_ANGBODIRTAL , 2, (uint8_t*)g_signposts_linked_lroute_102 }, // 102
	{ TOWN_ID_FAEHRE_ANGBODIRTAL , 3, (uint8_t*)g_signposts_linked_lroute_103 }, // 103
	{ TOWN_ID_HJALLANDER_HOF     , 1, (uint8_t*)g_signposts_linked_lroute_104 }, // 104
	{ TOWN_ID_LEUCHTTURM_RUNIN   , 2, (uint8_t*)g_signposts_linked_lroute_105 }, // 105
	{ -1                    , 0,    NULL }
}; // ds:0xa0b4; uint8_t*

/* Numbers here are entries in g_sea_route */
int8_t g_harbors_linked_sea_routes_01[] = {1, 8, 33, 35, 39, -1};		//ds:0xa330
int8_t g_harbors_linked_sea_routes_02[] = {36, 37, -1};				//ds:0xa336
int8_t g_harbors_linked_sea_routes_03[] = {33, 34, 36, -1};			//ds:0xa339
int8_t g_harbors_linked_sea_routes_04[] = {34, 35, 37, -1};			//ds:0xa33d
int8_t g_harbors_linked_sea_routes_05[] = {28, 27, 38, -1};			//ds:0xa341
int8_t g_harbors_linked_sea_routes_06[] = {27, 29, 25, -1};			//ds:0xa345
int8_t g_harbors_linked_sea_routes_07[] = {24, -1};				//ds:0xa349
int8_t g_harbors_linked_sea_routes_08[] = {6, 25, 26, 30, 38, 23, 45, -1};	//ds:0xa34b
int8_t g_harbors_linked_sea_routes_09[] = {22, 23, 24, 32, -1};			//ds:0xa353
int8_t g_harbors_linked_sea_routes_10[] = {
#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 40: After doing a ship passage from Overthorn to Hjalsingor and leaving the harbor,
	 * the party is positioned in a wrong square behind the buildings. */
	/* Reason: In the hard-coded list of the sea routes linked to the Hjalsingor harbor, the route Overthorn-Hjalsignor (ID 6) is missing.
	 * Instead, the route Varnheim-Ljasdahl (ID 10) is wrongly linked. */
	2, 5,	7, /* diff */ 10,	21, 22, -1,
#else
	2, 5,	/* diff */ 6, 7,	21, 22, -1,
#endif
};										//ds:0xa358
int8_t g_harbors_linked_sea_routes_11[] = {20, 21, -1};				//ds:0xa35f
int8_t g_harbors_linked_sea_routes_12[] = {4, 5, 19, 20, -1};			//ds:0xa362
int8_t g_harbors_linked_sea_routes_13[] = {18, 19, 44, -1};			//ds:0xa367
int8_t g_harbors_linked_sea_routes_14[] = {15, 17, -1};				//ds:0xa36b
int8_t g_harbors_linked_sea_routes_15[] = {16, 17, 18, 41, 42, 43, -1};		//ds:0xa36e
int8_t g_harbors_linked_sea_routes_16[] = {13, 12, 11, 40, 41, -1};		//ds:0xa375
int8_t g_harbors_linked_sea_routes_17[] = {13, 42, 14, -1};			//ds:0xa37b
int8_t g_harbors_linked_sea_routes_18[] = {1, 2, 3, 4, 14, 15, 16, 40, -1};      //ds:0xa37f
int8_t g_harbors_linked_sea_routes_19[] = {10, 12, -1};				//ds:0xa388
int8_t g_harbors_linked_sea_routes_20[] = {9, 10, 11, 39, -1};			//ds:0xa38b
int8_t g_harbors_linked_sea_routes_21[] = {8, 9, -1};				//ds:0xa390
int8_t g_harbors_linked_sea_routes_22[] = {26, 28, 29, -1};			//ds:0xa393
int8_t g_harbors_linked_sea_routes_23[] = {30, 31, -1};				//ds:0xa397
int8_t g_harbors_linked_sea_routes_24[] = {3, 7, 31, 32, 45, -1};		//ds:0xa39a
int8_t g_harbors_linked_sea_routes_25[] = {43, 44, -1};					  //ds:0xa3a0

// for each of the 25 harbors (see g_harbors), a -1 terminated list of the IDs of the linked sea routes

struct trv_start_point g_harbors[26] = { // a list of the available sea routes for each of the 25 harbors in the game.
	{ TOWN_ID_THORWAL         , 3, (uint8_t*)g_harbors_linked_sea_routes_01 }, //  1
	{ TOWN_ID_SERSKE          , 3, (uint8_t*)g_harbors_linked_sea_routes_02 }, //  2
	{ TOWN_ID_MERSKE          , 1, (uint8_t*)g_harbors_linked_sea_routes_03 }, //  3
	{ TOWN_ID_EFFERDUN        , 1, (uint8_t*)g_harbors_linked_sea_routes_04 }, //  4
	{ TOWN_ID_LISKOR          , 4, (uint8_t*)g_harbors_linked_sea_routes_05 }, //  5
	{ TOWN_ID_TJANSET         , 3, (uint8_t*)g_harbors_linked_sea_routes_06 }, //  6
	{ TOWN_ID_ORVIL           , 4, (uint8_t*)g_harbors_linked_sea_routes_07 }, //  7
	{ TOWN_ID_OVERTHORN       , 1, (uint8_t*)g_harbors_linked_sea_routes_08 }, //  8
	{ TOWN_ID_ROVIK           , 2, (uint8_t*)g_harbors_linked_sea_routes_09 }, //  9
	{ TOWN_ID_HJALSINGOR      , 1, (uint8_t*)g_harbors_linked_sea_routes_10 }, // 10
	{ TOWN_ID_GUDDASUNDEN     , 1, (uint8_t*)g_harbors_linked_sea_routes_11 }, // 11
	{ TOWN_ID_KORD            , 2, (uint8_t*)g_harbors_linked_sea_routes_12 }, // 12
	{ TOWN_ID_TREBAN          , 1, (uint8_t*)g_harbors_linked_sea_routes_13 }, // 13
	{ TOWN_ID_ARYN            , 1, (uint8_t*)g_harbors_linked_sea_routes_14 }, // 14
	{ TOWN_ID_RUNINSHAVEN     , 3, (uint8_t*)g_harbors_linked_sea_routes_15 }, // 15
	{ TOWN_ID_OTTARJE         , 4, (uint8_t*)g_harbors_linked_sea_routes_16 }, // 16
	{ TOWN_ID_SKJAL           , 4, (uint8_t*)g_harbors_linked_sea_routes_17 }, // 17
	{ TOWN_ID_PREM            , 3, (uint8_t*)g_harbors_linked_sea_routes_18 }, // 18
	{ TOWN_ID_LJASDAHL        , 1, (uint8_t*)g_harbors_linked_sea_routes_19 }, // 19
	{ TOWN_ID_VARNHEIM        , 4, (uint8_t*)g_harbors_linked_sea_routes_20 }, // 20
	{ TOWN_ID_VAERMHAG        , 3, (uint8_t*)g_harbors_linked_sea_routes_21 }, // 21
	{ TOWN_ID_VIDSAND         , 2, (uint8_t*)g_harbors_linked_sea_routes_22 }, // 22
	{ TOWN_ID_BRENDHIL        , 2, (uint8_t*)g_harbors_linked_sea_routes_23 }, // 23
	{ TOWN_ID_MANRIN          , 2, (uint8_t*)g_harbors_linked_sea_routes_24 }, // 24
#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 41: After doing a ship passage to Leuchtturm Runin and leaving the harbor,
	 * the party is positioned in a remote town square. */
	/* Reason: The following line is incorrect; it is a duplicate of the corresponding entry in g_signposts. */
//	{ TOWN_ID_LEUCHTTURM_RUNIN, 2, (int8_t*)0x14fca0b2 }, // 25 links to land route 105
	{ TOWN_ID_LEUCHTTURM_RUNIN, 2, (int8_t*)g_signposts_linked_lroute_105 }, // 25 links to land route 105
#else
	{ TOWN_ID_LEUCHTTURM_RUNIN, 1, (uint8_t*)g_harbors_linked_sea_routes_25 }, // 25
#endif
	{ -1                 , 0, NULL }
}; // ds:0xa3a3; uint8_t*
struct struct_point g_town_positions[TOWN_ID__TAIL] = {
	{ 0x00bd, 0x0098 },
	{ 0x00be, 0x00a1 },
	{ 0x00e2, 0x009e },
	{ 0x0100, 0x00a7 },
	{ 0x00fc, 0x00c0 },
	{ 0x0123, 0x00c4 },
	{ 0x0131, 0x00b4 },
	{ 0x0134, 0x009d },
	{ 0x00b7, 0x00b4 },
	{ 0x00b2, 0x00c2 },
	{ 0x00cf, 0x008d },
	{ 0x00ce, 0x0083 },
	{ 0x00cc, 0x006e },
	{ 0x00c5, 0x0063 },
	{ 0x00cc, 0x0051 },
	{ 0x00cf, 0x0052 },
	{ 0x00c4, 0x0049 },
	{ 0x012b, 0x0029 },
	{ 0x0135, 0x001c },
	{ 0x00c6, 0x0038 },
	{ 0x00f0, 0x0012 },
	{ 0x00a1, 0x0020 },
	{ 0x008c, 0x0013 },
	{ 0x0077, 0x0012 },
	{ 0x007e, 0x0020 },
	{ 0x0065, 0x001e },
	{ 0x0058, 0x002f },
	{ 0x0049, 0x0046 },
	{ 0x0045, 0x000b },
	{ 0x0039, 0x0043 },
	{ 0x0020, 0x0061 },
	{ 0x001e, 0x0072 },
	{ 0x0025, 0x00a0 },
	{ 0x0039, 0x00b3 },
	{ 0x0042, 0x00a7 },
	{ 0x005b, 0x00a7 },
	{ 0x006e, 0x0069 },
	{ 0x0047, 0x007b },
	{ 0x0040, 0x0090 },
	{ 0x0086, 0x005b },
	{ 0x008a, 0x004e },
	{ 0x0077, 0x0070 },
	{ 0x0091, 0x0069 },
	{ 0x00af, 0x007f },
	{ 0x008f, 0x0000 },
	{ 0x0074, 0x0000 },
	{ 0x0027, 0x0002 },
	{ 0x000d, 0x0005 },
	{ 0x00cd, 0x008d },
	{ 0x00cb, 0x0070 },
	{ 0x007c, 0x0076 },
	{ 0x0062, 0x00b4 }
}; // ds:0xa43f
static struct mouse_action g_action_table_travelmap[35] = {
	{ 0x55,  0xa4, 0x65, 0xb6, 0x01},
	{ 0x6d,  0x70, 0x88, 0x7e, 0x02},
	{ 0x60,  0x80, 0x6a, 0x86, 0x03},
	{ 0x5c,  0x86, 0x61, 0x8a, 0x03},
	{ 0x5e,  0x9b, 0x62, 0xa0, 0x03},
	{ 0x00,  0x00, 0x2e, 0x15, 0x04},
	{ 0x45,  0x7f, 0xb9, 0xa6, 0x05},
	{ 0x64,  0xa6, 0xbb, 0xbb, 0x05},
	{ 0x75,  0x58, 0x91, 0x70, 0x05},
	{ 0x91,  0x70, 0xa7, 0x7e, 0x05},
	{ 0x2f,  0x00, 0x4a, 0x09, 0x06},
	{ 0x4b,  0x00, 0x72, 0x1a, 0x06},
	{ 0x00,  0x16, 0x39, 0x42, 0x07},
	{ 0x00,  0x06, 0x14, 0x14, 0x07},
	{ 0x3a,  0x26, 0x44, 0x3e, 0x07},
	{ 0x41,  0x4b, 0x64, 0x76, 0x08},
	{ 0x52,  0x32, 0x6c, 0x4a, 0x08},
	{ 0x6d,  0x23, 0xaa, 0x4b, 0x08},
	{ 0xab,  0x20, 0xd7, 0x36, 0x08},
	{ 0xe1,  0x57, 0xfa, 0x6e, 0x09},
	{ 0xfb,  0x57, 0x117, 0x7a, 0x09},
	{ 0x118, 0x60, 0x13f, 0x82, 0x09},
	{ 0xd6,  0x6f, 0xfa, 0x82, 0x0a},
	{ 0xfb,  0x7b, 0x117, 0x92, 0x0a},
	{ 0x118, 0x83, 0x13f, 0x99, 0x0a},
	{ 0xa8,  0x00, 0xdb, 0x1f, 0x0b},
	{ 0xee,  0x0c, 0xfa, 0x12, 0x0d},
	{ 0xdc,  0x00, 0x13f, 0x41, 0x0c},
	{ 0x128, 0x41, 0x13f, 0x54, 0x0e},
	{ 0x11a, 0x00, 0x138, 0x35, 0x0f},
	{ 0xd8,  0x36, 0x125, 0x4b, 0x0f},
	{ 0xb9,  0x4a, 0xd7, 0xa0, 0x0f},
	{ 0xd8,  0x8c, 0xf0, 0xaf, 0x10},
	{ 0x95,  0x49, 0xbd, 0x6a, 0x11},
	{ -1, 0x00, 0x00, 0x00, 0x00 }
}; // ds:0xa50f

struct struct_tevent g_tevents_tab[156] = {
	{ 2, 26, 1 },
	{ 3, 7, 2 },
	{ 4, 0, 3 },
	{ 7, 13, 4 },
	{ 7, 19, 5 },
	{ 8, 0, 6 },
	{ 9, 10, 7 },
	{ 9, 12, 8 },
	{ 10, 17, 9 },
	{ 10, 25, 10 },
	{ 11, 10, 11 },
	{ 12, 0, 12 },
	{ 12, 8, 13 },
	{ 12, 16, 14 },
	{ 12, 22, 15 },
	{ 13, 8, 16 },
	{ 13, 19, 17 },
	{ 13, 24, 18 },
	{ 14, 0, 19 },
	{ 14, 8, 20 },
	{ 14, 22, 21 },
	{ 17, 15, 22 },
	{ 19, 0, 23 },
	{ 20, 10, 24 },
	{ 20, 18, 25 },
	{ 21, 0, 26 },
	{ 22, 0, 27 },
	{ 22, 20, 28 },
	{ 22, 26, 29 },
	{ 22, 48, 30 },
	{ 22, 65, 31 },
	{ 22, 80, 32 },
	{ 22, 86, 33 },
	{ 22, 95, 34 },
	{ 24, 13, 35 },
	{ 24, 22, 36 },
	{ 24, 27, 37 },
	{ 24, 39, 38 },
	{ 24, 40, 39 },
	{ 24, 18, 144 },
	{ 25, 18, 40 },
	{ 25, 25, 41 },
	{ 25, 38, 42 },
	{ 25, 42, 43 },
	{ 26, 11, 44 },
	{ 27, 12, 45 },
	{ 27, 15, 46 },
	{ 27, 22, 47 },
	{ 27, 26, 48 },
	{ 28, 19, 49 },
	{ 28, 23, 50 },
	{ 28, 27, 51 },
	{ 28, 35, 52 },
	{ 28, 38, 53 },
	{ 29, 18, 54 },
	{ 30, 14, 55 },
	{ 30, 23, 56 },
	{ 31, 18, 57 },
	{ 31, 20, 58 },
	{ 31, 22, 59 },
	{ 31, 33, 60 },
	{ 31, 41, 61 },
	{ 31, 47, 62 },
	{ 31, 53, 63 },
	{ 31, 58, 64 },
	{ 32, 4, 65 },
	{ 32, 19, 66 },
	{ 33, 7, 67 },
	{ 33, 10, 68 },
	{ 36, 7, 67 },
	{ 36, 10, 68 },
	{ 36, 17, 69 },
	{ 36, 29, 70 },
	{ 37, 11, 71 },
	{ 38, 15, 68 },
	{ 38, 22, 69 },
	{ 38, 34, 70 },
	{ 40, 7, 71 },
	{ 40, 12, 72 },
	{ 41, 7, 73 },
	{ 41, 12, 74 },
	{ 41, 25, 75 },
	{ 41, 35, 76 },
	{ 41, 39, 77 },
	{ 42, 15, 78 },
	{ 42, 18, 79 },
	{ 42, 21, 80 },
	{ 43, 18, 81 },
	{ 45, 0, 82 },
	{ 45, 9, 83 },
	{ 45, 32, 84 },
	{ 45, 35, 85 },
	{ 45, 53, 86 },
	{ 45, 59, 87 },
	{ 45, 64, 88 },
	{ 46, 0, 89 },
	{ 46, 19, 90 },
	{ 46, 25, 91 },
	{ 46, 28, 92 },
	{ 46, 34, 93 },
	{ 46, 45, 87 },
	{ 46, 50, 88 },
	{ 47, 15, 94 },
	{ 47, 18, 95 },
	{ 47, 21, 96 },
	{ 47, 29, 97 },
	{ 47, 35, 98 },
	{ 47, 44, 99 },
	{ 47, 48, 100 },
	{ 47, 52, 101 },
	{ 48, 25, 102 },
	{ 48, 29, 103 },
	{ 48, 34, 104 },
	{ 48, 38, 105 },
	{ 48, 50, 62 },
	{ 48, 56, 63 },
	{ 48, 61, 64 },
	{ 49, 20, 106 },
	{ 49, 36, 107 },
	{ 49, 40, 108 },
	{ 49, 48, 109 },
	{ 49, 57, 110 },
	{ 49, 63, 111 },
	{ 49, 68, 112 },
	{ 49, 87, 113 },
	{ 49, 100, 114 },
	{ 49, 112, 115 },
	{ 49, 122, 116 },
	{ 49, 125, 117 },
	{ 50, 18, 118 },
	{ 50, 38, 119 },
	{ 50, 55, 120 },
	{ 51, 0, 121 },
	{ 51, 3, 122 },
	{ 51, 14, 123 },
	{ 51, 17, 124 },
	{ 52, 13, 125 },
	{ 52, 42, 126 },
	{ 52, 59, 127 },
	{ 54, 15, 128 },
	{ 55, 10, 129 },
	{ 56, 14, 130 },
	{ 56, 17, 131 },
	{ 56, 19, 132 },
	{ 57, 12, 133 },
	{ 57, 17, 134 },
	{ 57, 20, 135 },
	{ 57, 24, 136 },
	{ 57, 28, 137 },
	{ 58, 18, 138 },
	{ 59, 10, 139 },
	{ 59, 23, 140 },
	{ 59, 28, 141 },
	{ 59, 39, 142 },
	{ 59, 43, 143 },
	{ -1,  0,   0 },
}; // ds:0xa66d

signed int do_travel_mode(void)
{
	signed int tmp1; /* multiple use: route_id, screen coordinate, etc. */
	signed int i;
	signed int tmp2; /* multiple use: tmp2, screen coordinate, etc. */
	signed int l4;
	signed int route_id;
	struct trv_start_point *signpost_ptr;
	signed int l6;
	signed int l7;
	signed int wallclock_update_bak;
	signed int tw_bak;
	char *destinations_tab[6];

	wallclock_update_bak = g_wallclock_update;
	g_route59_flag = g_wallclock_update = gs_travel_detour = 0;
	gs_town_id = gs_town_typeindex;

	call_mouse_bg();

	for (i = 0; i < 6; i++)
	{
		g_food_message_shown[i] = 0;
	}

	if (g_travel_map_loaded != gs_show_travel_map)
	{
		load_map();
	}

	memmove((void*)g_renderbuf_ptr, (void*)gs_travel_map_ptr, 64000);

	map_effect((uint8_t*)g_renderbuf_ptr);

	wait_for_vsync();

	set_palette(gs_travel_map_ptr + 64000 + 2, 0, 0x20);

	call_mouse();

	prepare_map_marker();

	g_mouse_leftclick_event = 0;

	signpost_ptr = &g_signposts[0];

	do {
		if (g_request_refresh)
		{
			call_mouse_bg();

			if (g_travel_map_loaded != gs_show_travel_map)
			{
				load_map();
			}

			wait_for_vsync();

			/* TODO: update window */
			memmove((void*)g_vga_memstart, (void*)gs_travel_map_ptr, 320 * 200);

			wait_for_vsync();

			set_palette(gs_travel_map_ptr + 64000 + 2, 0, 0x20);

			call_mouse();

			g_request_refresh = 0;
		}

		if ((signpost_ptr->town_id == gs_town_id) && (signpost_ptr->typeindex == gs_current_signpost_typeindex))
		{
			while (1) {
				handle_input();

				if (g_mouse_rightclick_event || g_action == ACTION_ID_PAGE_UP)
				{
					i = 0;
					while ((tmp1 = signpost_ptr->linked_travel_routes[i]) != 0xff)
					{
						destinations_tab[i] = get_ttx(235 + (gs_trv_menu_towns[i] = (
							(tmp2 = g_land_routes[tmp1 - 1].town_id_1) != gs_town_id ?
							(signed char) tmp2 : g_land_routes[tmp1 - 1].town_id_2)));

						i++;
					}

					gs_trv_menu_towns[i] = gs_town_id;
					destinations_tab[i] = get_ttx(613);
					i++;

					sprintf(g_text_output_buf, get_ttx(545), get_ttx(235 + gs_town_id));

					tw_bak = g_textbox_width;
					g_textbox_width = 4;

					tmp2 = GUI_radio(g_text_output_buf, i,
								destinations_tab[0], destinations_tab[1], destinations_tab[2],
								destinations_tab[3], destinations_tab[4], destinations_tab[5]) - 1;

					g_textbox_width = tw_bak;

					g_current_town_anix = 0;

					set_and_spin_lock();

					if (i - 1 == tmp2 || tmp2 == -2)
					{
						gs_show_travel_map = 0;
						gs_direction = ((gs_direction + 2) & 3);
						break;
					}

					route_id = signpost_ptr->linked_travel_routes[tmp2];
					gs_trv_destination = gs_trv_menu_towns[tmp2];

					if (!get_current_season() &&
						(route_id == 31 || route_id == 41 || route_id == 47 || route_id == 48 || route_id == 49))
					{
						/* routes not available in winter */
						GUI_input(get_tx(69), 0);
						break;
					}

					g_wallclock_x = g_basepos_x + 120;
					g_wallclock_y = g_basepos_y + 87;

					g_wallclock_update = 1;

					TM_func1(signpost_ptr->linked_travel_routes[tmp2],
						(g_land_routes[signpost_ptr->linked_travel_routes[tmp2] - 1].town_id_1 == gs_town_id ? 0 : 1));
					g_wallclock_update = 0;

					if (g_route59_flag)
					{
						TM_func9();
					}

					if (gs_trv_return == 2)
					{
						if (gs_travel_detour && gs_travel_detour != 99)
						{
							DNG_enter_dungeon(gs_travel_detour);
						}
						break;
					}

					TM_enter_target_town();

					if (!gs_travel_detour && g_game_state == GAME_STATE_MAIN)
					{
						gs_town_id = gs_travel_destination_town_id;
						gs_x_target_bak = gs_travel_destination_x;
						gs_y_target_bak = gs_travel_destination_y;
						gs_direction = (gs_travel_destination_viewdir + 2) & 3;

					} else if (g_game_state == GAME_STATE_MAIN && gs_travel_detour != 99)
					{
						DNG_enter_dungeon(gs_travel_detour);
					}

					break;

				} else if (g_mouse_leftclick_event)
				{

					for (i = 0, l4 = -1; i < TOWN_ID__TAIL; i++)
					{
						if (is_mouse_in_rect(tmp1 - 4, tmp2 - 4,
								(tmp1 = g_town_positions[i].x) + 4,
								(tmp2 = g_town_positions[i].y) + 4))
						{
							l4 = i;
							break;
						}
					}

					if (l4 == -1 && (tmp1 = get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table_travelmap)))
					{
						l4 = tmp1 + 51;
					}

					if (l4 != -1)
					{
						tmp2 = g_current_town_anix;
						g_current_town_anix = 0;
						l6 = g_basepos_x;
						l7 = g_basepos_y;
						g_basepos_y = 0;
						g_basepos_x = (g_mouse_posx >= 0 && g_mouse_posx <= 159 ? 80 : -80);

						set_and_spin_lock();

						GUI_input(get_tx(l4), 0);

						g_basepos_x = l6;
						g_basepos_y = l7;
						g_current_town_anix = tmp2;
					}

					g_mouse_leftclick_event = 0;
				}
			}
			break;
		}

		signpost_ptr++;

	} while (signpost_ptr->town_id != -1);

	g_current_town_anix = g_current_town_aniy = g_selected_town_anix = g_selected_town_aniy = 0;

	i = load_archive_file(ARCHIVE_FILE_COMPASS);
	read_archive_file(i, g_buffer6_ptr, 5000);
	close(i);

	gs_show_travel_map = g_basepos_x = g_basepos_y = g_current_town_over = g_trv_menu_selection = 0;

	if (!gs_travel_detour)
	{
		g_wallclock_update = 0;
		leave_location();

	} else if (gs_travel_detour != 99)
	{
		gs_town_id = TOWN_ID_NONE;
	}

	if (g_pp20_index == 5)
	{
		memset((void*)g_dtp2, 0, 0xc0);

		memcpy((void*)(g_dtp2 + 0xc0), (void*)(gs_travel_map_ptr + 64000 + 2), 0x60);

		memcpy((void*)(g_dtp2 + 0x120),	(void*)g_palette_special, 0x60);

		for (i = 0; i < 64; i++)
		{
			pal_fade((int8_t*)g_dtp2 + 0xc0, (int8_t*)g_dtp2);
			pal_fade((int8_t*)g_dtp2 + 0x120, (int8_t*)g_dtp2 + 0x60);
			wait_for_vsync();
			set_palette((uint8_t*)g_dtp2 + 0xc0, 0x00, 0x20);
			set_palette((uint8_t*)g_dtp2 + 0x120, 0xe0, 0x20);
		}

		wait_for_vsync();
		set_palette((uint8_t*)g_dtp2, 0x80, 0x40);
		set_palette((uint8_t*)g_dtp2, 0x00, 0x20);

		do_fill_rect(g_vga_memstart, 0, 0, 319, 199, 0);
	}

	g_current_ani = g_town_loaded_town_id = g_pp20_index = -1;
	g_request_refresh = g_fading_state = 1;
	g_wallclock_update = wallclock_update_bak;

	return 0;
}
