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
#include <unistd.h>
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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct struct_land_route g_land_routes[59] = {
	{ TOWNS_THORWAL            , TOWNS_VAERMHAG           ,  34,  1, 10,  25,  0, 10, 100 }, //  1
	{ TOWNS_VAERMHAG           , TOWNS_VARNHEIM           ,  40,  1, 10,  20,  0, 10, 100 }, //  2
	{ TOWNS_VARNHEIM           , TOWNS_DASPOTA            ,  21,  1, 10,   5,  0, 10, 100 }, //  3
	{ TOWNS_THORWAL            , TOWNS_SERSKE             ,   9,  2,  5, 100,  0,  5, 100 }, //  4
	{ TOWNS_SERSKE             , TOWNS_MERSKE             ,  27,  2,  5, 100,  0,  5, 100 }, //  5
	{ TOWNS_MERSKE             , TOWNS_EFFERDUN           ,  15,  2,  5, 100,  0,  5, 100 }, //  6
	{ TOWNS_SERSKE             , TOWNS_BREIDA             ,  36, -1, 25,  10,  0, 10,  90 }, //  7
	{ TOWNS_FAEHRSTATION_TJOILA, TOWNS_TJOILA             ,   0,  7,  0,   0,  0,  0,   0 }, //  8
	{ TOWNS_TJOILA             , TOWNS_BREIDA             ,  28,  0, 20,  15,  5, 20,  75 }, //  9
	{ TOWNS_BREIDA             , TOWNS_PEILINEN           ,  33,  0, 25,  15,  5, 20,  75 }, // 10
	{ TOWNS_PEILINEN           , TOWNS_ROVAMUND           ,  30,  0, 30,  15,  5, 20,  75 }, // 11
	{ TOWNS_ROVAMUND           , TOWNS_NORDVEST           ,  38,  0, 30,  15,  5, 20,  75 }, // 12
	{ TOWNS_NORDVEST           , TOWNS_KRAVIK             ,  24,  0, 35,  15,  5, 20,  75 }, // 13
	{ TOWNS_KRAVIK             , TOWNS_SKELELLEN          ,  27, -1, 40,   5,  5, 30,  50 }, // 14
	{ TOWNS_THORWAL            , TOWNS_FAEHRSTATION_TJOILA,  21,  1, 15,  65,  0, 20, 100 }, // 15
	{ TOWNS_FAEHRSTATION_TJOILA, TOWNS_RUKIAN             ,  11,  1, 15,  55,  0, 20, 100 }, // 16
	{ TOWNS_RUKIAN             , TOWNS_FAEHRE_ANGBODIRTAL ,  22,  1, 15,  55,  0, 25, 100 }, // 17
	{ TOWNS_FAEHRE_ANGBODIRTAL , TOWNS_AUPLOG             ,  12,  1, 20,  55,  0, 25, 100 }, // 18
	{ TOWNS_FAEHRE_ANGBODIRTAL , TOWNS_ANGBODIRTAL        ,   0,  7,  0,   0,  0,  0,   0 }, // 19
	{ TOWNS_AUPLOG             , TOWNS_VILNHEIM           ,  24,  1, 15,  60,  0, 30, 100 }, // 20
	{ TOWNS_VILNHEIM           , TOWNS_BODON              ,   0,  8,  0,   0,  0,  0,   0 }, // 21
	{ TOWNS_VILNHEIM           , TOWNS_PHEXCAER           , 105, -1, 35,  10,  5, 45, 100 }, // 22
	{ TOWNS_PHEXCAER           , TOWNS_GROENVELDEN        ,  17, -1, 35,  25,  5, 60, 100 }, // 23
	{ TOWNS_PHEXCAER           , TOWNS_EINSIEDLERSEE      ,  57, -2, 25,   0,  0, 50, 100 }, // 24
	{ TOWNS_VARNHEIM           , TOWNS_AUPLOG             ,  52, -1, 15,  25,  5, 15,  75 }, // 25
	{ TOWNS_DASPOTA            , TOWNS_RYBON              ,  19, -2, 15,   5,  5,  5,  40 }, // 26
	{ TOWNS_DASPOTA            , TOWNS_OTTARJE            ,  36, -1, 20,   5, 30,  0,  20 }, // 27
	{ TOWNS_OTTARJE            , TOWNS_SKJAL              ,  48, -1, 15,  10, 30,  0,  10 }, // 28
	{ TOWNS_SKJAL              , TOWNS_PREM               ,  27,  0, 15,  25, 35,  0,   5 }, // 29
	{ TOWNS_PREM               , TOWNS_KORD               ,  34,  0, 10,  25, 30,  0,   0 }, // 30
	{ TOWNS_OTTARJE            , TOWNS_ORVIL              ,  65, -2, 30,   5, 10,  5,   5 }, // 31
	{ TOWNS_ORVIL              , TOWNS_ALA                ,  29,  0, 15,  30, 25, 10,  20 }, // 32
	{ TOWNS_ALA                , TOWNS_TJANSET            ,  25,  0, 20,  30, 25, 10,  15 }, // 33
	{ TOWNS_TJANSET            , TOWNS_LISKOR             ,  24,  0,  5,  25, 20, 10,  10 }, // 34
	{ TOWNS_LISKOR             , TOWNS_CLANEGH            ,  21,  0, 10,  30, 25, 10,  20 }, // 35
	{ TOWNS_ALA                , TOWNS_THOSS              ,  42, -2, 25,  20, 15,  5,  15 }, // 36
	{ TOWNS_THOSS              , TOWNS_LISKOR             ,  19,  0, 20,  30, 25,  5,  20 }, // 37
	{ TOWNS_TJANSET            , TOWNS_THOSS              ,  47,  0, 20,  25, 20,  5,  15 }, // 38
	{ TOWNS_VILNHEIM           , TOWNS_OBERORKEN          ,  12,  1, 15,  60,  0, 25, 100 }, // 39
	{ TOWNS_OBERORKEN          , TOWNS_FELSTEYN           ,  20, -2, 50,   5, 10, 30,  75 }, // 40
	{ TOWNS_FELSTEYN           , TOWNS_ORKANGER           ,  50, -2, 50,   5, 10, 30,  75 }, // 41
	{ TOWNS_ORKANGER           , TOWNS_CLANEGH            ,  30, -2, 50,   5, 10, 30,  75 }, // 42
	{ TOWNS_CLANEGH            , TOWNS_TYLDON             ,  28,  1, 20,  40, 50,  5,  15 }, // 43
	{ TOWNS_TYLDON             , TOWNS_VIDSAND            ,  29,  1, 20,  30, 50,  5,  10 }, // 44
	{ TOWNS_OBERORKEN          , TOWNS_EINSIEDLERSEE      ,  81, -3, 50,   0,  0, 50, 100 }, // 45
	{ TOWNS_FELSTEYN           , TOWNS_EINSIEDLERSEE      ,  67, -3, 50,   0,  0, 50, 100 }, // 46
	{ TOWNS_RYBON              , TOWNS_THOSS              ,  60, -4, 40,   0, 10,  5,  10 }, // 47
	{ TOWNS_SKJAL              , TOWNS_ORVIL              ,  68, -4, 40,   0,  5,  5,   5 }, // 48
	{ TOWNS_SKELELLEN          , TOWNS_PHEXCAER           , 140, -4, 50,   0,  0, 10,  30 }, // 49
	{ TOWNS_MERSKE             , TOWNS_ROVAMUND           ,  71, -2, 25,   5,  5, 10,  50 }, // 50
	{ TOWNS_ORVIL              , TOWNS_ROVIK              ,  23, -2, 25,   5,  5,  0,  20 }, // 51
	{ TOWNS_LJASDAHL           , TOWNS_LJASDAHL           ,  70, -3, 35,   0,  0,  0,   0 }, // 52 // circular route
	{ TOWNS_LJASDAHL           , TOWNS_HJALLANDER_HOF     ,   9, -2, 40,   5,  5,  0,   0 }, // 53
	{ TOWNS_RUNINSHAVEN        , TOWNS_LEUCHTTURM_RUNIN   ,  24, -3, 30,   0,  0,  0,   0 }, // 54 // not linked at LEUCHTTURM_RUNIN
	{ TOWNS_RUNINSHAVEN        , TOWNS_LEUCHTTURM_RUNIN   ,  17, -2, 35,   5,  5,  0,   0 }, // 55 // middle route; only route linked at LEUCHTTURM_RUNIN
	{ TOWNS_BRENDHIL           , TOWNS_MANRIN             ,  30, -3, 35,   0,  0,  0,   0 }, // 56
	{ TOWNS_EINSIEDLERSEE      , TOWNS_EINSIEDLERSEE      ,  47, -3, 35,   0,  0, 50, 100 }, // 57 // circular route
	{ TOWNS_LEUCHTTURM_RUNIN   , TOWNS_RUNINSHAVEN        ,  27, -3, 25,   0,  0,  0,   0 }, // 58 // not linked at LEUCHTTURM_RUNIN
	{ 0                        , 0                        ,  50, -4, 50,   0,  0,  5,   9 }
}; // ds:0x9dc6
// towns not connected to any land route: TOWNS_OVERTHORN, TOWNS_HJALSINGOR, TOWNS_GUDDASUNDEN, TOWNS_TREBAN, TOWNS_ARYN

// ds:0x9fd9, 105 arrays, each terminated by -1
/* Numbers here are entries in g_land_routes */
Bit8s g_signposts_linked_lroute_001[] = {15, -1};		//ds:0x9fd9;
Bit8s g_signposts_linked_lroute_002[] = {4, -1};		//ds:0x9fdb;
Bit8s g_signposts_linked_lroute_003[] = {1, -1};		//ds:0x9fdd;
Bit8s g_signposts_linked_lroute_004[] = {4, -1};		//ds:0x9fdf;
Bit8s g_signposts_linked_lroute_005[] = {5, 7, -1};		//ds:0x9fe1;
Bit8s g_signposts_linked_lroute_006[] = {10, -1};		//ds:0x9fe4;
Bit8s g_signposts_linked_lroute_007[] = {7, -1};		//ds:0x9fe6;
Bit8s g_signposts_linked_lroute_008[] = {9, -1};		//ds:0x9fe8;
Bit8s g_signposts_linked_lroute_009[] = {11, -1};		//ds:0x9fea;
Bit8s g_signposts_linked_lroute_010[] = {10, -1};		//ds:0x9fec;
Bit8s g_signposts_linked_lroute_011[] = {50, -1};		//ds:0x9fee;
Bit8s g_signposts_linked_lroute_012[] = {12, -1};		//ds:0x9ff0;
Bit8s g_signposts_linked_lroute_013[] = {11, -1};		//ds:0x9ff2;
Bit8s g_signposts_linked_lroute_014[] = {12, -1};		//ds:0x9ff4;
Bit8s g_signposts_linked_lroute_015[] = {13, -1};		//ds:0x9ff6;
Bit8s g_signposts_linked_lroute_016[] = {13, -1};		//ds:0x9ff8;
Bit8s g_signposts_linked_lroute_017[] = {14, -1};		//ds:0x9ffa;
Bit8s g_signposts_linked_lroute_018[] = {14, -1};		//ds:0x9ffc;
Bit8s g_signposts_linked_lroute_019[] = {49, -1};		//ds:0x9ffe;
Bit8s g_signposts_linked_lroute_020[] = {50, -1};		//ds:0xa000;
Bit8s g_signposts_linked_lroute_021[] = {5, -1};		//ds:0xa002;
Bit8s g_signposts_linked_lroute_022[] = {6, -1};		//ds:0xa004;
Bit8s g_signposts_linked_lroute_023[] = {6, -1};		//ds:0xa006;
Bit8s g_signposts_linked_lroute_024[] = {8, -1};		//ds:0xa008;
Bit8s g_signposts_linked_lroute_025[] = {9, -1};		//ds:0xa00a;
Bit8s g_signposts_linked_lroute_026[] = {16, -1};		//ds:0xa00c;
Bit8s g_signposts_linked_lroute_027[] = {17, -1};		//ds:0xa00e;
Bit8s g_signposts_linked_lroute_028[] = {19, -1};		//ds:0xa010;
Bit8s g_signposts_linked_lroute_029[] = {18, -1};		//ds:0xa012;
Bit8s g_signposts_linked_lroute_030[] = {20, -1};		//ds:0xa014;
Bit8s g_signposts_linked_lroute_031[] = {25, -1};		//ds:0xa016;
Bit8s g_signposts_linked_lroute_032[] = {21, -1};		//ds:0xa018;
Bit8s g_signposts_linked_lroute_033[] = {22, -1};		//ds:0xa01a;
Bit8s g_signposts_linked_lroute_034[] = {20, -1};		//ds:0xa01c;
Bit8s g_signposts_linked_lroute_035[] = {39, -1};		//ds:0xa01e;
Bit8s g_signposts_linked_lroute_036[] = {21, -1};		//ds:0xa020;
Bit8s g_signposts_linked_lroute_037[] = {39, -1};		//ds:0xa022;
Bit8s g_signposts_linked_lroute_038[] = {40, -1};		//ds:0xa024;
Bit8s g_signposts_linked_lroute_039[] = {45, -1};		//ds:0xa026;
Bit8s g_signposts_linked_lroute_040[] = {23, -1};		//ds:0xa028;
Bit8s g_signposts_linked_lroute_041[] = {49, -1};		//ds:0xa02a;
Bit8s g_signposts_linked_lroute_042[] = {22, -1};		//ds:0xa02c;
Bit8s g_signposts_linked_lroute_043[] = {24, -1};		//ds:0xa02e;
Bit8s g_signposts_linked_lroute_044[] = {23, -1};		//ds:0xa030;
Bit8s g_signposts_linked_lroute_045[] = {40, -1};		//ds:0xa032;
Bit8s g_signposts_linked_lroute_046[] = {41, -1};		//ds:0xa034;
Bit8s g_signposts_linked_lroute_047[] = {46, -1};		//ds:0xa036;
Bit8s g_signposts_linked_lroute_048[] = {24, 46, 45, 57, 57, -1};	//ds:0xa038;
Bit8s g_signposts_linked_lroute_049[] = {41, -1};		//ds:0xa03e;
Bit8s g_signposts_linked_lroute_050[] = {42, -1};		//ds:0xa040;
Bit8s g_signposts_linked_lroute_051[] = {42, -1};		//ds:0xa042;
Bit8s g_signposts_linked_lroute_052[] = {35, -1};		//ds:0xa044;
Bit8s g_signposts_linked_lroute_053[] = {43, -1};		//ds:0xa046;
Bit8s g_signposts_linked_lroute_054[] = {35, -1};		//ds:0xa048;
Bit8s g_signposts_linked_lroute_055[] = {37, -1};		//ds:0xa04a;
Bit8s g_signposts_linked_lroute_056[] = {34, -1};		//ds:0xa04c;
Bit8s g_signposts_linked_lroute_057[] = {37, -1};		//ds:0xa04e;
Bit8s g_signposts_linked_lroute_058[] = {36, 38, -1};		//ds:0xa050;
Bit8s g_signposts_linked_lroute_059[] = {47, -1};		//ds:0xa052;
Bit8s g_signposts_linked_lroute_060[] = {34, -1};		//ds:0xa054;
Bit8s g_signposts_linked_lroute_061[] = {33, 38, -1};		//ds:0xa056;
Bit8s g_signposts_linked_lroute_062[] = {36, 33, -1};		//ds:0xa059;
Bit8s g_signposts_linked_lroute_063[] = {32, -1};		//ds:0xa05c;
Bit8s g_signposts_linked_lroute_064[] = {48, 31, -1};		//ds:0xa05e;
Bit8s g_signposts_linked_lroute_065[] = {32, -1};		//ds:0xa062;
Bit8s g_signposts_linked_lroute_066[] = {51, -1};		//ds:0xa064;
Bit8s g_signposts_linked_lroute_067[] = {51, -1};		//ds:0xa066;
Bit8s g_signposts_linked_lroute_068[] = {30, -1};		//ds:0xa068;
Bit8s g_signposts_linked_lroute_069[] = {54, -1};		//ds:0xa06a;
Bit8s g_signposts_linked_lroute_070[] = {58, -1};		//ds:0xa06c;
Bit8s g_signposts_linked_lroute_071[] = {55, -1};		//ds:0xa06e;
Bit8s g_signposts_linked_lroute_072[] = {27, -1};		//ds:0xa070;
Bit8s g_signposts_linked_lroute_073[] = {28, -1};		//ds:0xa072;
Bit8s g_signposts_linked_lroute_074[] = {31, -1};		//ds:0xa074;
Bit8s g_signposts_linked_lroute_075[] = {28, -1};		//ds:0xa076;
Bit8s g_signposts_linked_lroute_076[] = {29, -1};		//ds:0xa078;
Bit8s g_signposts_linked_lroute_077[] = {48, -1};		//ds:0xa07a;
Bit8s g_signposts_linked_lroute_078[] = {29, -1};		//ds:0xa07c;
Bit8s g_signposts_linked_lroute_079[] = {30, -1};		//ds:0xa07e;
Bit8s g_signposts_linked_lroute_080[] = {27, -1};		//ds:0xa080;
Bit8s g_signposts_linked_lroute_081[] = {3, -1};		//ds:0xa082;
Bit8s g_signposts_linked_lroute_082[] = {26, -1};		//ds:0xa084;
Bit8s g_signposts_linked_lroute_083[] = {47, -1};		//ds:0xa086;
Bit8s g_signposts_linked_lroute_084[] = {26, -1};		//ds:0xa088;
Bit8s g_signposts_linked_lroute_085[] = {53, -1};		//ds:0xa08a;
Bit8s g_signposts_linked_lroute_086[] = {52, -1};		//ds:0xa08c;
Bit8s g_signposts_linked_lroute_087[] = {52, -1};		//ds:0xa08e;
Bit8s g_signposts_linked_lroute_088[] = {2, -1};		//ds:0xa090;
Bit8s g_signposts_linked_lroute_089[] = {25, -1};		//ds:0xa092;
Bit8s g_signposts_linked_lroute_090[] = {3, -1};		//ds:0xa094;
Bit8s g_signposts_linked_lroute_091[] = {1, -1};		//ds:0xa096;
Bit8s g_signposts_linked_lroute_092[] = {2, -1};		//ds:0xa098;
Bit8s g_signposts_linked_lroute_093[] = {43, -1};		//ds:0xa09a;
Bit8s g_signposts_linked_lroute_094[] = {44, -1};		//ds:0xa09c;
Bit8s g_signposts_linked_lroute_095[] = {44, -1};		//ds:0xa09e;
Bit8s g_signposts_linked_lroute_096[] = {56, -1};		//ds:0xa0a0;
Bit8s g_signposts_linked_lroute_097[] = {56, -1};		//ds:0xa0a2;
Bit8s g_signposts_linked_lroute_098[] = {16, -1};		//ds:0xa0a4;
Bit8s g_signposts_linked_lroute_099[] = {15, -1};		//ds:0xa0a6;
Bit8s g_signposts_linked_lroute_100[] = {8, -1};		//ds:0xa0a8;
Bit8s g_signposts_linked_lroute_101[] = {17, -1};		//ds:0xa0aa;
Bit8s g_signposts_linked_lroute_102[] = {18, -1};		//ds:0xa0ac;
Bit8s g_signposts_linked_lroute_103[] = {19, -1};		//ds:0xa0ae;
Bit8s g_signposts_linked_lroute_104[] = {53, -1};		//ds:0xa0b0;
Bit8s g_signposts_linked_lroute_105[] = {55, -1};		//ds:0xa0b2;

struct trv_start_point g_signposts[106] = {
	{ TOWNS_THORWAL            , 1, (Bit8u*)g_signposts_linked_lroute_001 }, //  1
	{ TOWNS_THORWAL            , 2, (Bit8u*)g_signposts_linked_lroute_002 }, //  2
	{ TOWNS_THORWAL            , 4, (Bit8u*)g_signposts_linked_lroute_003 }, //  3
	{ TOWNS_SERSKE             , 1, (Bit8u*)g_signposts_linked_lroute_004 }, //  4
	{ TOWNS_SERSKE             , 2, (Bit8u*)g_signposts_linked_lroute_005 }, //  5
	{ TOWNS_BREIDA             , 1, (Bit8u*)g_signposts_linked_lroute_006 }, //  6
	{ TOWNS_BREIDA             , 2, (Bit8u*)g_signposts_linked_lroute_007 }, //  7
	{ TOWNS_BREIDA             , 3, (Bit8u*)g_signposts_linked_lroute_008 }, //  8
	{ TOWNS_PEILINEN           , 1, (Bit8u*)g_signposts_linked_lroute_009 }, //  9
	{ TOWNS_PEILINEN           , 2, (Bit8u*)g_signposts_linked_lroute_010 }, // 10
	{ TOWNS_ROVAMUND           , 1, (Bit8u*)g_signposts_linked_lroute_011 }, // 11
	{ TOWNS_ROVAMUND           , 2, (Bit8u*)g_signposts_linked_lroute_012 }, // 12
	{ TOWNS_ROVAMUND           , 3, (Bit8u*)g_signposts_linked_lroute_013 }, // 13
	{ TOWNS_NORDVEST           , 1, (Bit8u*)g_signposts_linked_lroute_014 }, // 14
	{ TOWNS_NORDVEST           , 2, (Bit8u*)g_signposts_linked_lroute_015 }, // 15
	{ TOWNS_KRAVIK             , 1, (Bit8u*)g_signposts_linked_lroute_016 }, // 16
	{ TOWNS_KRAVIK             , 2, (Bit8u*)g_signposts_linked_lroute_017 }, // 17
	{ TOWNS_SKELELLEN          , 1, (Bit8u*)g_signposts_linked_lroute_018 }, // 18
	{ TOWNS_SKELELLEN          , 2, (Bit8u*)g_signposts_linked_lroute_019 }, // 19
	{ TOWNS_MERSKE             , 2, (Bit8u*)g_signposts_linked_lroute_020 }, // 20
	{ TOWNS_MERSKE             , 3, (Bit8u*)g_signposts_linked_lroute_021 }, // 21
	{ TOWNS_MERSKE             , 4, (Bit8u*)g_signposts_linked_lroute_022 }, // 22
	{ TOWNS_EFFERDUN           , 2, (Bit8u*)g_signposts_linked_lroute_023 }, // 23
	{ TOWNS_TJOILA             , 1, (Bit8u*)g_signposts_linked_lroute_024 }, // 24
	{ TOWNS_TJOILA             , 2, (Bit8u*)g_signposts_linked_lroute_025 }, // 25
	{ TOWNS_RUKIAN             , 1, (Bit8u*)g_signposts_linked_lroute_026 }, // 26
	{ TOWNS_RUKIAN             , 2, (Bit8u*)g_signposts_linked_lroute_027 }, // 27
	{ TOWNS_ANGBODIRTAL        , 1, (Bit8u*)g_signposts_linked_lroute_028 }, // 28
	{ TOWNS_AUPLOG             , 1, (Bit8u*)g_signposts_linked_lroute_029 }, // 29
	{ TOWNS_AUPLOG             , 2, (Bit8u*)g_signposts_linked_lroute_030 }, // 30
	{ TOWNS_AUPLOG             , 3, (Bit8u*)g_signposts_linked_lroute_031 }, // 31
	{ TOWNS_VILNHEIM           , 1, (Bit8u*)g_signposts_linked_lroute_032 }, // 32
	{ TOWNS_VILNHEIM           , 2, (Bit8u*)g_signposts_linked_lroute_033 }, // 33
	{ TOWNS_VILNHEIM           , 3, (Bit8u*)g_signposts_linked_lroute_034 }, // 34
	{ TOWNS_VILNHEIM           , 4, (Bit8u*)g_signposts_linked_lroute_035 }, // 35
	{ TOWNS_BODON              , 1, (Bit8u*)g_signposts_linked_lroute_036 }, // 36
	{ TOWNS_OBERORKEN          , 1, (Bit8u*)g_signposts_linked_lroute_037 }, // 37
	{ TOWNS_OBERORKEN          , 2, (Bit8u*)g_signposts_linked_lroute_038 }, // 38
	{ TOWNS_OBERORKEN          , 3, (Bit8u*)g_signposts_linked_lroute_039 }, // 39
	{ TOWNS_PHEXCAER           , 1, (Bit8u*)g_signposts_linked_lroute_040 }, // 40
	{ TOWNS_PHEXCAER           , 2, (Bit8u*)g_signposts_linked_lroute_041 }, // 41
	{ TOWNS_PHEXCAER           , 3, (Bit8u*)g_signposts_linked_lroute_042 }, // 42
	{ TOWNS_PHEXCAER           , 4, (Bit8u*)g_signposts_linked_lroute_043 }, // 43
	{ TOWNS_GROENVELDEN        , 1, (Bit8u*)g_signposts_linked_lroute_044 }, // 44
	{ TOWNS_FELSTEYN           , 1, (Bit8u*)g_signposts_linked_lroute_045 }, // 45
	{ TOWNS_FELSTEYN           , 2, (Bit8u*)g_signposts_linked_lroute_046 }, // 46
	{ TOWNS_FELSTEYN           , 3, (Bit8u*)g_signposts_linked_lroute_047 }, // 47
	{ TOWNS_EINSIEDLERSEE      , 1, (Bit8u*)g_signposts_linked_lroute_048 }, // 48
	{ TOWNS_ORKANGER           , 1, (Bit8u*)g_signposts_linked_lroute_049 }, // 49
	{ TOWNS_ORKANGER           , 2, (Bit8u*)g_signposts_linked_lroute_050 }, // 50
	{ TOWNS_CLANEGH            , 1, (Bit8u*)g_signposts_linked_lroute_051 }, // 51
	{ TOWNS_CLANEGH            , 2, (Bit8u*)g_signposts_linked_lroute_052 }, // 52
	{ TOWNS_CLANEGH            , 3, (Bit8u*)g_signposts_linked_lroute_053 }, // 53
	{ TOWNS_LISKOR             , 1, (Bit8u*)g_signposts_linked_lroute_054 }, // 54
	{ TOWNS_LISKOR             , 2, (Bit8u*)g_signposts_linked_lroute_055 }, // 55
	{ TOWNS_LISKOR             , 3, (Bit8u*)g_signposts_linked_lroute_056 }, // 56
	{ TOWNS_THOSS              , 1, (Bit8u*)g_signposts_linked_lroute_057 }, // 57
	{ TOWNS_THOSS              , 2, (Bit8u*)g_signposts_linked_lroute_058 }, // 58
	{ TOWNS_THOSS              , 3, (Bit8u*)g_signposts_linked_lroute_059 }, // 59
	{ TOWNS_TJANSET            , 1, (Bit8u*)g_signposts_linked_lroute_060 }, // 60
	{ TOWNS_TJANSET            , 2, (Bit8u*)g_signposts_linked_lroute_061 }, // 61
	{ TOWNS_ALA                , 1, (Bit8u*)g_signposts_linked_lroute_062 }, // 62
	{ TOWNS_ALA                , 2, (Bit8u*)g_signposts_linked_lroute_063 }, // 63
	{ TOWNS_ORVIL              , 1, (Bit8u*)g_signposts_linked_lroute_064 }, // 64
	{ TOWNS_ORVIL              , 2, (Bit8u*)g_signposts_linked_lroute_065 }, // 65
	{ TOWNS_ORVIL              , 3, (Bit8u*)g_signposts_linked_lroute_066 }, // 66
	{ TOWNS_ROVIK              , 1, (Bit8u*)g_signposts_linked_lroute_067 }, // 67
	{ TOWNS_KORD               , 1, (Bit8u*)g_signposts_linked_lroute_068 }, // 68
	{ TOWNS_RUNINSHAVEN        , 1, (Bit8u*)g_signposts_linked_lroute_069 }, // 69
	{ TOWNS_RUNINSHAVEN        , 2, (Bit8u*)g_signposts_linked_lroute_070 }, // 70
	{ TOWNS_RUNINSHAVEN        , 4, (Bit8u*)g_signposts_linked_lroute_071 }, // 71
	{ TOWNS_OTTARJE            , 1, (Bit8u*)g_signposts_linked_lroute_072 }, // 72
	{ TOWNS_OTTARJE            , 2, (Bit8u*)g_signposts_linked_lroute_073 }, // 73
	{ TOWNS_OTTARJE            , 3, (Bit8u*)g_signposts_linked_lroute_074 }, // 74
	{ TOWNS_SKJAL              , 1, (Bit8u*)g_signposts_linked_lroute_075 }, // 75
	{ TOWNS_SKJAL              , 2, (Bit8u*)g_signposts_linked_lroute_076 }, // 76
	{ TOWNS_SKJAL              , 3, (Bit8u*)g_signposts_linked_lroute_077 }, // 77
	{ TOWNS_PREM               , 1, (Bit8u*)g_signposts_linked_lroute_078 }, // 78
	{ TOWNS_PREM               , 2, (Bit8u*)g_signposts_linked_lroute_079 }, // 79
	{ TOWNS_DASPOTA            , 1, (Bit8u*)g_signposts_linked_lroute_080 }, // 80
	{ TOWNS_DASPOTA            , 2, (Bit8u*)g_signposts_linked_lroute_081 }, // 81
	{ TOWNS_DASPOTA            , 3, (Bit8u*)g_signposts_linked_lroute_082 }, // 82
	{ TOWNS_RYBON              , 1, (Bit8u*)g_signposts_linked_lroute_083 }, // 83
	{ TOWNS_RYBON              , 2, (Bit8u*)g_signposts_linked_lroute_084 }, // 84
	{ TOWNS_LJASDAHL           , 2, (Bit8u*)g_signposts_linked_lroute_085 }, // 85
	{ TOWNS_LJASDAHL           , 3, (Bit8u*)g_signposts_linked_lroute_086 }, // 86
	{ TOWNS_LJASDAHL           , 4, (Bit8u*)g_signposts_linked_lroute_087 }, // 87
	{ TOWNS_VARNHEIM           , 1, (Bit8u*)g_signposts_linked_lroute_088 }, // 88
	{ TOWNS_VARNHEIM           , 2, (Bit8u*)g_signposts_linked_lroute_089 }, // 89
	{ TOWNS_VARNHEIM           , 3, (Bit8u*)g_signposts_linked_lroute_090 }, // 90
	{ TOWNS_VAERMHAG           , 1, (Bit8u*)g_signposts_linked_lroute_091 }, // 91
	{ TOWNS_VAERMHAG           , 2, (Bit8u*)g_signposts_linked_lroute_092 }, // 92
	{ TOWNS_TYLDON             , 1, (Bit8u*)g_signposts_linked_lroute_093 }, // 93
	{ TOWNS_TYLDON             , 2, (Bit8u*)g_signposts_linked_lroute_094 }, // 94
	{ TOWNS_VIDSAND            , 1, (Bit8u*)g_signposts_linked_lroute_095 }, // 95
	{ TOWNS_BRENDHIL           , 1, (Bit8u*)g_signposts_linked_lroute_096 }, // 96
	{ TOWNS_MANRIN             , 1, (Bit8u*)g_signposts_linked_lroute_097 }, // 97
	{ TOWNS_FAEHRSTATION_TJOILA, 1, (Bit8u*)g_signposts_linked_lroute_098 }, // 98
	{ TOWNS_FAEHRSTATION_TJOILA, 2, (Bit8u*)g_signposts_linked_lroute_099 }, // 99
	{ TOWNS_FAEHRSTATION_TJOILA, 3, (Bit8u*)g_signposts_linked_lroute_100 }, // 100
	{ TOWNS_FAEHRE_ANGBODIRTAL , 1, (Bit8u*)g_signposts_linked_lroute_101 }, // 101
	{ TOWNS_FAEHRE_ANGBODIRTAL , 2, (Bit8u*)g_signposts_linked_lroute_102 }, // 102
	{ TOWNS_FAEHRE_ANGBODIRTAL , 3, (Bit8u*)g_signposts_linked_lroute_103 }, // 103
	{ TOWNS_HJALLANDER_HOF     , 1, (Bit8u*)g_signposts_linked_lroute_104 }, // 104
	{ TOWNS_LEUCHTTURM_RUNIN   , 2, (Bit8u*)g_signposts_linked_lroute_105 }, // 105
	{ -1                    , 0,    NULL }
}; // ds:0xa0b4; Bit8u*

/* Numbers here are entries in g_sea_route */
Bit8s g_harbors_linked_sea_routes_01[] = {1, 8, 33, 35, 39, -1};		//ds:0xa330
Bit8s g_harbors_linked_sea_routes_02[] = {36, 37, -1};				//ds:0xa336
Bit8s g_harbors_linked_sea_routes_03[] = {33, 34, 36, -1};			//ds:0xa339
Bit8s g_harbors_linked_sea_routes_04[] = {34, 35, 37, -1};			//ds:0xa33d
Bit8s g_harbors_linked_sea_routes_05[] = {28, 27, 38, -1};			//ds:0xa341
Bit8s g_harbors_linked_sea_routes_06[] = {27, 29, 25, -1};			//ds:0xa345
Bit8s g_harbors_linked_sea_routes_07[] = {24, -1};				//ds:0xa349
Bit8s g_harbors_linked_sea_routes_08[] = {6, 25, 26, 30, 38, 23, 45, -1};	//ds:0xa34b
Bit8s g_harbors_linked_sea_routes_09[] = {22, 23, 24, 32, -1};			//ds:0xa353
Bit8s g_harbors_linked_sea_routes_10[] = {
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
Bit8s g_harbors_linked_sea_routes_11[] = {20, 21, -1};				//ds:0xa35f
Bit8s g_harbors_linked_sea_routes_12[] = {4, 5, 19, 20, -1};			//ds:0xa362
Bit8s g_harbors_linked_sea_routes_13[] = {18, 19, 44, -1};			//ds:0xa367
Bit8s g_harbors_linked_sea_routes_14[] = {15, 17, -1};				//ds:0xa36b
Bit8s g_harbors_linked_sea_routes_15[] = {16, 17, 18, 41, 42, 43, -1};		//ds:0xa36e
Bit8s g_harbors_linked_sea_routes_16[] = {13, 12, 11, 40, 41, -1};		//ds:0xa375
Bit8s g_harbors_linked_sea_routes_17[] = {13, 42, 14, -1};			//ds:0xa37b
Bit8s g_harbors_linked_sea_routes_18[] = {1, 2, 3, 4, 14, 15, 16, 40, -1};      //ds:0xa37f
Bit8s g_harbors_linked_sea_routes_19[] = {10, 12, -1};				//ds:0xa388
Bit8s g_harbors_linked_sea_routes_20[] = {9, 10, 11, 39, -1};			//ds:0xa38b
Bit8s g_harbors_linked_sea_routes_21[] = {8, 9, -1};				//ds:0xa390
Bit8s g_harbors_linked_sea_routes_22[] = {26, 28, 29, -1};			//ds:0xa393
Bit8s g_harbors_linked_sea_routes_23[] = {30, 31, -1};				//ds:0xa397
Bit8s g_harbors_linked_sea_routes_24[] = {3, 7, 31, 32, 45, -1};		//ds:0xa39a
Bit8s g_harbors_linked_sea_routes_25[] = {43, 44, -1};					  //ds:0xa3a0

// for each of the 25 harbors (see g_harbors), a -1 terminated list of the IDs of the linked sea routes

struct trv_start_point g_harbors[26] = { // a list of the available sea routes for each of the 25 harbors in the game.
	{ TOWNS_THORWAL         , 3, (Bit8u*)g_harbors_linked_sea_routes_01 }, //  1
	{ TOWNS_SERSKE          , 3, (Bit8u*)g_harbors_linked_sea_routes_02 }, //  2
	{ TOWNS_MERSKE          , 1, (Bit8u*)g_harbors_linked_sea_routes_03 }, //  3
	{ TOWNS_EFFERDUN        , 1, (Bit8u*)g_harbors_linked_sea_routes_04 }, //  4
	{ TOWNS_LISKOR          , 4, (Bit8u*)g_harbors_linked_sea_routes_05 }, //  5
	{ TOWNS_TJANSET         , 3, (Bit8u*)g_harbors_linked_sea_routes_06 }, //  6
	{ TOWNS_ORVIL           , 4, (Bit8u*)g_harbors_linked_sea_routes_07 }, //  7
	{ TOWNS_OVERTHORN       , 1, (Bit8u*)g_harbors_linked_sea_routes_08 }, //  8
	{ TOWNS_ROVIK           , 2, (Bit8u*)g_harbors_linked_sea_routes_09 }, //  9
	{ TOWNS_HJALSINGOR      , 1, (Bit8u*)g_harbors_linked_sea_routes_10 }, // 10
	{ TOWNS_GUDDASUNDEN     , 1, (Bit8u*)g_harbors_linked_sea_routes_11 }, // 11
	{ TOWNS_KORD            , 2, (Bit8u*)g_harbors_linked_sea_routes_12 }, // 12
	{ TOWNS_TREBAN          , 1, (Bit8u*)g_harbors_linked_sea_routes_13 }, // 13
	{ TOWNS_ARYN            , 1, (Bit8u*)g_harbors_linked_sea_routes_14 }, // 14
	{ TOWNS_RUNINSHAVEN     , 3, (Bit8u*)g_harbors_linked_sea_routes_15 }, // 15
	{ TOWNS_OTTARJE         , 4, (Bit8u*)g_harbors_linked_sea_routes_16 }, // 16
	{ TOWNS_SKJAL           , 4, (Bit8u*)g_harbors_linked_sea_routes_17 }, // 17
	{ TOWNS_PREM            , 3, (Bit8u*)g_harbors_linked_sea_routes_18 }, // 18
	{ TOWNS_LJASDAHL        , 1, (Bit8u*)g_harbors_linked_sea_routes_19 }, // 19
	{ TOWNS_VARNHEIM        , 4, (Bit8u*)g_harbors_linked_sea_routes_20 }, // 20
	{ TOWNS_VAERMHAG        , 3, (Bit8u*)g_harbors_linked_sea_routes_21 }, // 21
	{ TOWNS_VIDSAND         , 2, (Bit8u*)g_harbors_linked_sea_routes_22 }, // 22
	{ TOWNS_BRENDHIL        , 2, (Bit8u*)g_harbors_linked_sea_routes_23 }, // 23
	{ TOWNS_MANRIN          , 2, (Bit8u*)g_harbors_linked_sea_routes_24 }, // 24
#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 41: After doing a ship passage to Leuchtturm Runin and leaving the harbor,
	 * the party is positioned in a remote town square. */
	/* Reason: The following line is incorrect; it is a duplicate of the corresponding entry in g_signposts. */
//	{ TOWNS_LEUCHTTURM_RUNIN, 2, (Bit8s*)0x14fca0b2 }, // 25 links to land route 105
	{ TOWNS_LEUCHTTURM_RUNIN, 2, (Bit8s*)g_signposts_linked_lroute_105 }, // 25 links to land route 105
#else
	{ TOWNS_LEUCHTTURM_RUNIN, 1, (Bit8u*)g_harbors_linked_sea_routes_25 }, // 25
#endif
	{ -1                 , 0, NULL }
}; // ds:0xa3a3; Bit8u*
struct struct_point g_town_positions[52] = {
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

signed short do_travel_mode(void)
{
	signed short l_di;
	signed short i;
	signed short answer;
	signed short l4;
	signed short route_id;
	struct trv_start_point *signpost_ptr;
	signed short l6;
	signed short l7;
	signed short bak1;
	signed short tw_bak;
	char *destinations_tab[6];

	bak1 = g_wallclock_update;
	g_route59_flag = g_wallclock_update = gs_travel_detour = 0;
	gs_current_town = gs_current_typeindex;

	update_mouse_cursor();

	for (i = 0; i < 6; i++)
	{
		g_food_message_shown[i] = 0;
	}

	if (g_travel_map_loaded != gs_show_travel_map)
	{
		load_map();
	}

	memmove((void*)g_renderbuf_ptr, (void*)gs_travel_map_ptr, 64000);

	map_effect((Bit8u*)g_renderbuf_ptr);

	wait_for_vsync();

	set_palette(gs_travel_map_ptr + 64000 + 2, 0, 0x20);

	refresh_screen_size();

	prepare_map_marker();

	g_mouse1_event1 = 0;

	signpost_ptr = &g_signposts[0];

	do {
		if (g_request_refresh)
		{
			update_mouse_cursor();

			if (g_travel_map_loaded != gs_show_travel_map)
			{
				load_map();
			}

			wait_for_vsync();

			/* TODO: update window */
			memmove((void*)g_vga_memstart, (void*)gs_travel_map_ptr, 320 * 200);

			wait_for_vsync();

			set_palette(gs_travel_map_ptr + 64000 + 2, 0, 0x20);

			refresh_screen_size();

			g_request_refresh = 0;
		}

		if ((signpost_ptr->town == gs_current_town) && (signpost_ptr->typeindex == gs_current_signpost))
		{
			while (1) {
				handle_input();

				if (g_mouse2_event || g_action == ACTION_ID_PAGE_UP)
				{
					i = 0;
					while ((l_di = signpost_ptr->end_points[i]) != 0xff)
					{
						destinations_tab[i] = get_ttx(235 + (gs_trv_menu_towns[i] = (
							(answer = g_land_routes[l_di - 1].town1_id) != gs_current_town ?
							(signed char) answer : g_land_routes[l_di - 1].town2_id)));

						i++;
					}

					gs_trv_menu_towns[i] = gs_current_town;
					destinations_tab[i] = get_ttx(613);
					i++;

					sprintf(g_text_output_buf, get_ttx(545), get_ttx(235 + gs_current_town));

					tw_bak = g_textbox_width;
					g_textbox_width = 4;

					answer = GUI_radio(g_text_output_buf, i,
								destinations_tab[0], destinations_tab[1], destinations_tab[2],
								destinations_tab[3], destinations_tab[4], destinations_tab[5]) - 1;

					g_textbox_width = tw_bak;

					g_current_town_anix = 0;

					set_and_spin_lock();

					if (i - 1 == answer || answer == -2)
					{
						gs_show_travel_map = 0;
						gs_direction = ((gs_direction + 2) & 3);
						break;
					}

					route_id = signpost_ptr->end_points[answer];
					gs_trv_destination = gs_trv_menu_towns[answer];

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

					TM_func1(signpost_ptr->end_points[answer],
						(g_land_routes[signpost_ptr->end_points[answer] - 1].town1_id == gs_current_town ? 0 : 1));
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
						gs_current_town = gs_travel_destination_town_id;
						gs_x_target_bak = gs_travel_destination_x;
						gs_y_target_bak = gs_travel_destination_y;
						gs_direction = (gs_travel_destination_viewdir + 2) & 3;

					} else if (g_game_state == GAME_STATE_MAIN && gs_travel_detour != 99)
					{
						DNG_enter_dungeon(gs_travel_detour);
					}

					break;

				} else if (g_mouse1_event1)
				{

					for (i = 0, l4 = -1; i < 52; i++)
					{
						if (is_mouse_in_rect(l_di - 4, answer - 4,
								(l_di = g_town_positions[i].x) + 4,
								(answer = g_town_positions[i].y) + 4))
						{
							l4 = i;
							break;
						}
					}

					if (l4 == -1 && (l_di = get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table_travelmap)))
					{
						l4 = l_di + 51;
					}

					if (l4 != -1)
					{
						answer = g_current_town_anix;
						g_current_town_anix = 0;
						l6 = g_basepos_x;
						l7 = g_basepos_y;
						g_basepos_y = 0;
						g_basepos_x = (g_mouse_posx >= 0 && g_mouse_posx <= 159 ? 80 : -80);

						set_and_spin_lock();

						GUI_input(get_tx(l4), 0);

						g_basepos_x = l6;
						g_basepos_y = l7;
						g_current_town_anix = answer;
					}

					g_mouse1_event1 = 0;
				}
			}
			break;
		}

		signpost_ptr++;

	} while (signpost_ptr->town != -1);

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
		gs_current_town = TOWNS_NONE;
	}

	if (g_pp20_index == 5)
	{
		memset((void*)g_dtp2, 0, 0xc0);

		memcpy((void*)(g_dtp2 + 0xc0), (void*)(gs_travel_map_ptr + 64000 + 2), 0x60);

		memcpy((void*)(g_dtp2 + 0x120),	(void*)g_palette_special, 0x60);

		for (i = 0; i < 64; i++)
		{
			pal_fade((Bit8s*)g_dtp2 + 0xc0, (Bit8s*)g_dtp2);
			pal_fade((Bit8s*)g_dtp2 + 0x120, (Bit8s*)g_dtp2 + 0x60);
			wait_for_vsync();
			set_palette((Bit8u*)g_dtp2 + 0xc0, 0x00, 0x20);
			set_palette((Bit8u*)g_dtp2 + 0x120, 0xe0, 0x20);
		}

		wait_for_vsync();
		set_palette((Bit8u*)g_dtp2, 0x80, 0x40);
		set_palette((Bit8u*)g_dtp2, 0x00, 0x20);

		do_fill_rect(g_vga_memstart, 0, 0, 319, 199, 0);
	}

	g_current_ani = g_city_area_loaded = g_pp20_index = -1;
	g_request_refresh = g_fading_state = 1;
	g_wallclock_update = bak1;

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
