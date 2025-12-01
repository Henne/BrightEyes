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

struct struct_land_route g_land_routes[LROUTE_ID__END - 1] = {
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
	{ TOWN_ID_NONE               , TOWN_ID_NONE               ,  50, -4, 50,   0,  0,  5,   9 }  // 59
}; // ds:0x9dc6
// towns not connected to any land route: TOWN_ID_OVERTHORN, TOWN_ID_HJALSINGOR, TOWN_ID_GUDDASUNDEN, TOWN_ID_TREBAN, TOWN_ID_ARYN

// ds:0x9fd9, 105 arrays, each terminated by -1
int8_t g_signposts_linked_lroute_001[] = { LROUTE_ID_THORWAL_F_TJOILA                                 , -1}; //ds:0x9fd9;
int8_t g_signposts_linked_lroute_002[] = { LROUTE_ID_THORWAL_SERSKE                                   , -1}; //ds:0x9fdb;
int8_t g_signposts_linked_lroute_003[] = { LROUTE_ID_THORWAL_VAERMHAG                                 , -1}; //ds:0x9fdd;
int8_t g_signposts_linked_lroute_004[] = { LROUTE_ID_THORWAL_SERSKE                                   , -1}; //ds:0x9fdf;
int8_t g_signposts_linked_lroute_005[] = { LROUTE_ID_SERSKE_MERSKE           , LROUTE_ID_SERSKE_BREIDA, -1}; //ds:0x9fe1;
int8_t g_signposts_linked_lroute_006[] = { LROUTE_ID_BREIDA_PEILINEN                                  , -1}; //ds:0x9fe4;
int8_t g_signposts_linked_lroute_007[] = { LROUTE_ID_SERSKE_BREIDA                                    , -1}; //ds:0x9fe6;
int8_t g_signposts_linked_lroute_008[] = { LROUTE_ID_TJOILA_BREIDA                                    , -1}; //ds:0x9fe8;
int8_t g_signposts_linked_lroute_009[] = { LROUTE_ID_PEILINEN_ROVAMUND                                , -1}; //ds:0x9fea;
int8_t g_signposts_linked_lroute_010[] = { LROUTE_ID_BREIDA_PEILINEN                                  , -1}; //ds:0x9fec;
int8_t g_signposts_linked_lroute_011[] = { LROUTE_ID_MERSKE_ROVAMUND                                  , -1}; //ds:0x9fee;
int8_t g_signposts_linked_lroute_012[] = { LROUTE_ID_ROVAMUND_NORDVEST                                , -1}; //ds:0x9ff0;
int8_t g_signposts_linked_lroute_013[] = { LROUTE_ID_PEILINEN_ROVAMUND                                , -1}; //ds:0x9ff2;
int8_t g_signposts_linked_lroute_014[] = { LROUTE_ID_ROVAMUND_NORDVEST                                , -1}; //ds:0x9ff4;
int8_t g_signposts_linked_lroute_015[] = { LROUTE_ID_NORDVEST_KRAVIK                                  , -1}; //ds:0x9ff6;
int8_t g_signposts_linked_lroute_016[] = { LROUTE_ID_NORDVEST_KRAVIK                                  , -1}; //ds:0x9ff8;
int8_t g_signposts_linked_lroute_017[] = { LROUTE_ID_KRAVIK_SKELELLE                                  , -1}; //ds:0x9ffa;
int8_t g_signposts_linked_lroute_018[] = { LROUTE_ID_KRAVIK_SKELELLE                                  , -1}; //ds:0x9ffc;
int8_t g_signposts_linked_lroute_019[] = { LROUTE_ID_SKELELLE_PHEXCAER                                , -1}; //ds:0x9ffe;
int8_t g_signposts_linked_lroute_020[] = { LROUTE_ID_MERSKE_ROVAMUND                                  , -1}; //ds:0xa000;
int8_t g_signposts_linked_lroute_021[] = { LROUTE_ID_SERSKE_MERSKE                                    , -1}; //ds:0xa002;
int8_t g_signposts_linked_lroute_022[] = { LROUTE_ID_MERSKE_EFFERDUN                                  , -1}; //ds:0xa004;
int8_t g_signposts_linked_lroute_023[] = { LROUTE_ID_MERSKE_EFFERDUN                                  , -1}; //ds:0xa006;
int8_t g_signposts_linked_lroute_024[] = { LROUTE_ID_F_TJOILA_TJOILA                                  , -1}; //ds:0xa008;
int8_t g_signposts_linked_lroute_025[] = { LROUTE_ID_TJOILA_BREIDA                                    , -1}; //ds:0xa00a;
int8_t g_signposts_linked_lroute_026[] = { LROUTE_ID_F_TJOILA_RUKIAN                                  , -1}; //ds:0xa00c;
int8_t g_signposts_linked_lroute_027[] = { LROUTE_ID_RUKIAN_F_ANGBOD                                  , -1}; //ds:0xa00e;
int8_t g_signposts_linked_lroute_028[] = { LROUTE_ID_F_ANGBOD_ANGBODIR                                , -1}; //ds:0xa010;
int8_t g_signposts_linked_lroute_029[] = { LROUTE_ID_F_ANGBOD_AUPLOG                                  , -1}; //ds:0xa012;
int8_t g_signposts_linked_lroute_030[] = { LROUTE_ID_AUPLOG_VILNHEIM                                  , -1}; //ds:0xa014;
int8_t g_signposts_linked_lroute_031[] = { LROUTE_ID_VARNHEIM_AUPLOG                                  , -1}; //ds:0xa016;
int8_t g_signposts_linked_lroute_032[] = { LROUTE_ID_VILNHEIM_BODON                                   , -1}; //ds:0xa018;
int8_t g_signposts_linked_lroute_033[] = { LROUTE_ID_VILNHEIM_PHEXCAER                                , -1}; //ds:0xa01a;
int8_t g_signposts_linked_lroute_034[] = { LROUTE_ID_AUPLOG_VILNHEIM                                  , -1}; //ds:0xa01c;
int8_t g_signposts_linked_lroute_035[] = { LROUTE_ID_VILNHEIM_OBERORKE                                , -1}; //ds:0xa01e;
int8_t g_signposts_linked_lroute_036[] = { LROUTE_ID_VILNHEIM_BODON                                   , -1}; //ds:0xa020;
int8_t g_signposts_linked_lroute_037[] = { LROUTE_ID_VILNHEIM_OBERORKE                                , -1}; //ds:0xa022;
int8_t g_signposts_linked_lroute_038[] = { LROUTE_ID_OBERORKE_FELSTEYN                                , -1}; //ds:0xa024;
int8_t g_signposts_linked_lroute_039[] = { LROUTE_ID_OBERORKE_EINSIEDL                                , -1}; //ds:0xa026;
int8_t g_signposts_linked_lroute_040[] = { LROUTE_ID_PHEXCAER_GROENVEL                                , -1}; //ds:0xa028;
int8_t g_signposts_linked_lroute_041[] = { LROUTE_ID_SKELELLE_PHEXCAER                                , -1}; //ds:0xa02a;
int8_t g_signposts_linked_lroute_042[] = { LROUTE_ID_VILNHEIM_PHEXCAER                                , -1}; //ds:0xa02c;
int8_t g_signposts_linked_lroute_043[] = { LROUTE_ID_PHEXCAER_EINSIEDL                                , -1}; //ds:0xa02e;
int8_t g_signposts_linked_lroute_044[] = { LROUTE_ID_PHEXCAER_GROENVEL                                , -1}; //ds:0xa030;
int8_t g_signposts_linked_lroute_045[] = { LROUTE_ID_OBERORKE_FELSTEYN                                , -1}; //ds:0xa032;
int8_t g_signposts_linked_lroute_046[] = { LROUTE_ID_FELSTEYN_ORKANGER                                , -1}; //ds:0xa034;
int8_t g_signposts_linked_lroute_047[] = { LROUTE_ID_FELSTEYN_EINSIEDL                                , -1}; //ds:0xa036;
int8_t g_signposts_linked_lroute_048[] = {
 LROUTE_ID_PHEXCAER_EINSIEDL ,
 LROUTE_ID_FELSTEYN_EINSIEDL ,
 LROUTE_ID_OBERORKE_EINSIEDL ,
 LROUTE_ID_EINSIEDL__CIRCULAR,
 LROUTE_ID_EINSIEDL__CIRCULAR,
 -1
};                                                                                                           //ds:0xa038;
int8_t g_signposts_linked_lroute_049[] = { LROUTE_ID_FELSTEYN_ORKANGER                                , -1}; //ds:0xa03e;
int8_t g_signposts_linked_lroute_050[] = { LROUTE_ID_ORKANGER_CLANEGH                                 , -1}; //ds:0xa040;
int8_t g_signposts_linked_lroute_051[] = { LROUTE_ID_ORKANGER_CLANEGH                                 , -1}; //ds:0xa042;
int8_t g_signposts_linked_lroute_052[] = { LROUTE_ID_LISKOR_CLANEGH                                   , -1}; //ds:0xa044;
int8_t g_signposts_linked_lroute_053[] = { LROUTE_ID_CLANEGH_TYLDON                                   , -1}; //ds:0xa046;
int8_t g_signposts_linked_lroute_054[] = { LROUTE_ID_LISKOR_CLANEGH                                   , -1}; //ds:0xa048;
int8_t g_signposts_linked_lroute_055[] = { LROUTE_ID_THOSS_LISKOR                                     , -1}; //ds:0xa04a;
int8_t g_signposts_linked_lroute_056[] = { LROUTE_ID_TJANSET_LISKOR                                   , -1}; //ds:0xa04c;
int8_t g_signposts_linked_lroute_057[] = { LROUTE_ID_THOSS_LISKOR                                     , -1}; //ds:0xa04e;
int8_t g_signposts_linked_lroute_058[] = { LROUTE_ID_ALA_THOSS               , LROUTE_ID_TJANSET_THOSS, -1}; //ds:0xa050;
int8_t g_signposts_linked_lroute_059[] = { LROUTE_ID_RYBON_THOSS                                      , -1}; //ds:0xa052;
int8_t g_signposts_linked_lroute_060[] = { LROUTE_ID_TJANSET_LISKOR                                   , -1}; //ds:0xa054;
int8_t g_signposts_linked_lroute_061[] = { LROUTE_ID_ALA_TJANSET             , LROUTE_ID_TJANSET_THOSS, -1}; //ds:0xa056;
int8_t g_signposts_linked_lroute_062[] = { LROUTE_ID_ALA_THOSS               , LROUTE_ID_ALA_TJANSET  , -1}; //ds:0xa059;
int8_t g_signposts_linked_lroute_063[] = { LROUTE_ID_ORVIL_ALA                                        , -1}; //ds:0xa05c;
int8_t g_signposts_linked_lroute_064[] = { LROUTE_ID_SKJAL_ORVIL             , LROUTE_ID_OTTARJE_ORVIL, -1}; //ds:0xa05e;
int8_t g_signposts_linked_lroute_065[] = { LROUTE_ID_ORVIL_ALA                                        , -1}; //ds:0xa062;
int8_t g_signposts_linked_lroute_066[] = { LROUTE_ID_ORVIL_ROVIK                                      , -1}; //ds:0xa064;
int8_t g_signposts_linked_lroute_067[] = { LROUTE_ID_ORVIL_ROVIK                                      , -1}; //ds:0xa066;
int8_t g_signposts_linked_lroute_068[] = { LROUTE_ID_PREM_KORD                                        , -1}; //ds:0xa068;
int8_t g_signposts_linked_lroute_069[] = { LROUTE_ID_RUNINSHA_L_RUNIN__1                              , -1}; //ds:0xa06a;
int8_t g_signposts_linked_lroute_070[] = { LROUTE_ID_L_RUNIN_RUNINSHA__2                              , -1}; //ds:0xa06c;
int8_t g_signposts_linked_lroute_071[] = { LROUTE_ID_RUNINSHA_L_RUNIN__MIDDLE                         , -1}; //ds:0xa06e;
int8_t g_signposts_linked_lroute_072[] = { LROUTE_ID_DASPOTA_OTTARJE                                  , -1}; //ds:0xa070;
int8_t g_signposts_linked_lroute_073[] = { LROUTE_ID_OTTARJE_SKJAL                                    , -1}; //ds:0xa072;
int8_t g_signposts_linked_lroute_074[] = { LROUTE_ID_OTTARJE_ORVIL                                    , -1}; //ds:0xa074;
int8_t g_signposts_linked_lroute_075[] = { LROUTE_ID_OTTARJE_SKJAL                                    , -1}; //ds:0xa076;
int8_t g_signposts_linked_lroute_076[] = { LROUTE_ID_SKJAL_PREM                                       , -1}; //ds:0xa078;
int8_t g_signposts_linked_lroute_077[] = { LROUTE_ID_SKJAL_ORVIL                                      , -1}; //ds:0xa07a;
int8_t g_signposts_linked_lroute_078[] = { LROUTE_ID_SKJAL_PREM                                       , -1}; //ds:0xa07c;
int8_t g_signposts_linked_lroute_079[] = { LROUTE_ID_PREM_KORD                                        , -1}; //ds:0xa07e;
int8_t g_signposts_linked_lroute_080[] = { LROUTE_ID_DASPOTA_OTTARJE                                  , -1}; //ds:0xa080;
int8_t g_signposts_linked_lroute_081[] = { LROUTE_ID_VARNHEIM_DASPOTA                                 , -1}; //ds:0xa082;
int8_t g_signposts_linked_lroute_082[] = { LROUTE_ID_DASPOTA_RYBON                                    , -1}; //ds:0xa084;
int8_t g_signposts_linked_lroute_083[] = { LROUTE_ID_RYBON_THOSS                                      , -1}; //ds:0xa086;
int8_t g_signposts_linked_lroute_084[] = { LROUTE_ID_DASPOTA_RYBON                                    , -1}; //ds:0xa088;
int8_t g_signposts_linked_lroute_085[] = { LROUTE_ID_LJASDAHL_HJALLA_H                                , -1}; //ds:0xa08a;
int8_t g_signposts_linked_lroute_086[] = { LROUTE_ID_LJASDAHL__CIRCULAR                               , -1}; //ds:0xa08c;
int8_t g_signposts_linked_lroute_087[] = { LROUTE_ID_LJASDAHL__CIRCULAR                               , -1}; //ds:0xa08e;
int8_t g_signposts_linked_lroute_088[] = { LROUTE_ID_VAERMHAG_VARNHEIM                                , -1}; //ds:0xa090;
int8_t g_signposts_linked_lroute_089[] = { LROUTE_ID_VARNHEIM_AUPLOG                                  , -1}; //ds:0xa092;
int8_t g_signposts_linked_lroute_090[] = { LROUTE_ID_VARNHEIM_DASPOTA                                 , -1}; //ds:0xa094;
int8_t g_signposts_linked_lroute_091[] = { LROUTE_ID_THORWAL_VAERMHAG                                 , -1}; //ds:0xa096;
int8_t g_signposts_linked_lroute_092[] = { LROUTE_ID_VAERMHAG_VARNHEIM                                , -1}; //ds:0xa098;
int8_t g_signposts_linked_lroute_093[] = { LROUTE_ID_CLANEGH_TYLDON                                   , -1}; //ds:0xa09a;
int8_t g_signposts_linked_lroute_094[] = { LROUTE_ID_TYLDON_VIDSAND                                   , -1}; //ds:0xa09c;
int8_t g_signposts_linked_lroute_095[] = { LROUTE_ID_TYLDON_VIDSAND                                   , -1}; //ds:0xa09e;
int8_t g_signposts_linked_lroute_096[] = { LROUTE_ID_BRENDHIL_MANRIN                                  , -1}; //ds:0xa0a0;
int8_t g_signposts_linked_lroute_097[] = { LROUTE_ID_BRENDHIL_MANRIN                                  , -1}; //ds:0xa0a2;
int8_t g_signposts_linked_lroute_098[] = { LROUTE_ID_F_TJOILA_RUKIAN                                  , -1}; //ds:0xa0a4;
int8_t g_signposts_linked_lroute_099[] = { LROUTE_ID_THORWAL_F_TJOILA                                 , -1}; //ds:0xa0a6;
int8_t g_signposts_linked_lroute_100[] = { LROUTE_ID_F_TJOILA_TJOILA                                  , -1}; //ds:0xa0a8;
int8_t g_signposts_linked_lroute_101[] = { LROUTE_ID_RUKIAN_F_ANGBOD                                  , -1}; //ds:0xa0aa;
int8_t g_signposts_linked_lroute_102[] = { LROUTE_ID_F_ANGBOD_AUPLOG                                  , -1}; //ds:0xa0ac;
int8_t g_signposts_linked_lroute_103[] = { LROUTE_ID_F_ANGBOD_ANGBODIR                                , -1}; //ds:0xa0ae;
int8_t g_signposts_linked_lroute_104[] = { LROUTE_ID_LJASDAHL_HJALLA_H                                , -1}; //ds:0xa0b0;
int8_t g_signposts_linked_lroute_105[] = { LROUTE_ID_RUNINSHA_L_RUNIN__MIDDLE                         , -1}; //ds:0xa0b2;

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
	{ -1                         , 0, NULL                                    }
}; // ds:0xa0b4; uint8_t*

/* For each of the 25 harbors (see g_harbors), a -1 terminated list of the IDs of the linked sea routes */
int8_t g_harbors_linked_sea_routes_01[] = {
	SEA_ROUTE_ID_THORWAL_PREM,
	SEA_ROUTE_ID_THORWAL_VAERMHAG,
	SEA_ROUTE_ID_THORWAL_MERSKE,
	SEA_ROUTE_ID_THORWAL_EFFERDUN,
	SEA_ROUTE_ID_THORWAL_VARNHEIM,
	-1
}; //ds:0xa330
int8_t g_harbors_linked_sea_routes_02[] = {
	SEA_ROUTE_ID_SERSKE_MERSKE,
	SEA_ROUTE_ID_SERSKE_EFFERDUN,
	-1
}; //ds:0xa336
int8_t g_harbors_linked_sea_routes_03[] = {
	SEA_ROUTE_ID_THORWAL_MERSKE,
	SEA_ROUTE_ID_MERSKE_EFFERDUN,
	SEA_ROUTE_ID_SERSKE_MERSKE,
	-1
}; //ds:0xa339
int8_t g_harbors_linked_sea_routes_04[] = {
	SEA_ROUTE_ID_MERSKE_EFFERDUN,
	SEA_ROUTE_ID_THORWAL_EFFERDUN,
	SEA_ROUTE_ID_SERSKE_EFFERDUN,
	-1
}; //ds:0xa33d
int8_t g_harbors_linked_sea_routes_05[] = {
	SEA_ROUTE_ID_LISKOR_VIDSAND,
	SEA_ROUTE_ID_TJANSET_LISKOR,
	SEA_ROUTE_ID_OVERTHOR_LISKOR,
	-1
}; //ds:0xa341
int8_t g_harbors_linked_sea_routes_06[] = {
	SEA_ROUTE_ID_TJANSET_LISKOR,
	SEA_ROUTE_ID_TJANSET_VIDSAND,
	SEA_ROUTE_ID_OVERTHOR_TJANSET,
	-1
}; //ds:0xa345
int8_t g_harbors_linked_sea_routes_07[] = {
	SEA_ROUTE_ID_ROVIK_ORVIL,
	-1
}; //ds:0xa349
int8_t g_harbors_linked_sea_routes_08[] = {
	SEA_ROUTE_ID_HJALSING_OVERTHOR,
	SEA_ROUTE_ID_OVERTHOR_TJANSET,
	SEA_ROUTE_ID_OVERTHOR_VIDSAND,
	SEA_ROUTE_ID_OVERTHOR_BRENDHIL,
	SEA_ROUTE_ID_OVERTHOR_LISKOR,
	SEA_ROUTE_ID_ROVIK_OVERTHOR,
	SEA_ROUTE_ID_MANRIN_OVERTHOR,
	-1
}; //ds:0xa34b
int8_t g_harbors_linked_sea_routes_09[] = {
	SEA_ROUTE_ID_HJALSING_ROVIK,
	SEA_ROUTE_ID_ROVIK_OVERTHOR,
	SEA_ROUTE_ID_ROVIK_ORVIL,
	SEA_ROUTE_ID_ROVIK_MANRIN,
	-1
}; //ds:0xa353
int8_t g_harbors_linked_sea_routes_10[] = {
	SEA_ROUTE_ID_PREM_HJALSING,
	SEA_ROUTE_ID_KORD_HJALSING,
#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 40: After doing a ship passage from Overthorn to Hjalsingor and leaving the harbor,
	 * the party is positioned in a wrong square behind the buildings. */
	/* Reason: In the hard-coded list of the sea routes linked to the Hjalsingor harbor,the route Hjalsignor-Overthorn (ID 6) is missing.
	 * Instead,the route Varnheim-Ljasdahl (ID 10) is wrongly linked. */
	SEA_ROUTE_ID_HJALSING_OVERTHOR,
#endif
	SEA_ROUTE_ID_HJALSING_MANRIN,
#ifndef M302de_ORIGINAL_BUGFIX
	SEA_ROUTE_ID_VARNHEIM_LJASDAHL,
#endif
	SEA_ROUTE_ID_GUDDASUN_HJALSING,
	SEA_ROUTE_ID_HJALSING_ROVIK,
	-1
}; //ds:0xa358
int8_t g_harbors_linked_sea_routes_11[] = {
	SEA_ROUTE_ID_KORD_GUDDASUN,
	SEA_ROUTE_ID_GUDDASUN_HJALSING,
	-1
}; //ds:0xa35f
int8_t g_harbors_linked_sea_routes_12[] = {
	SEA_ROUTE_ID_PREM_KORD,
	SEA_ROUTE_ID_KORD_HJALSING,
	SEA_ROUTE_ID_TREBAN_KORD,
	SEA_ROUTE_ID_KORD_GUDDASUN,
	-1
}; //ds:0xa362
int8_t g_harbors_linked_sea_routes_13[] = {
	SEA_ROUTE_ID_RUNINSHA_TREBAN,
	SEA_ROUTE_ID_TREBAN_KORD,
	SEA_ROUTE_ID_TREBAN_L_RUNIN,
	-1
}; //ds:0xa367
int8_t g_harbors_linked_sea_routes_14[] = {
	SEA_ROUTE_ID_PREM_ARYN,
	SEA_ROUTE_ID_ARYN_RUNINSHA,
	-1
}; //ds:0xa36b
int8_t g_harbors_linked_sea_routes_15[] = {
	SEA_ROUTE_ID_PREM_RUNINSHA,
	SEA_ROUTE_ID_ARYN_RUNINSHA,
	SEA_ROUTE_ID_RUNINSHA_TREBAN,
	SEA_ROUTE_ID_OTTARJE_RUNINSHA,
	SEA_ROUTE_ID_SKJAL_RUNINSHA,
	SEA_ROUTE_ID_RUNINSHA_L_RUNIN,
	-1
}; //ds:0xa36e
int8_t g_harbors_linked_sea_routes_16[] = {
	SEA_ROUTE_ID_SKJAL_OTTARJE,
	SEA_ROUTE_ID_LJASDAHL_OTTARJE,
	SEA_ROUTE_ID_VARNHEIM_OTTARJE,
	SEA_ROUTE_ID_OTTARJE_PREM,
	SEA_ROUTE_ID_OTTARJE_RUNINSHA,
	-1
}; //ds:0xa375
int8_t g_harbors_linked_sea_routes_17[] = {
	SEA_ROUTE_ID_SKJAL_OTTARJE,
	SEA_ROUTE_ID_SKJAL_RUNINSHA,
	SEA_ROUTE_ID_SKJAL_PREM,
	-1
}; //ds:0xa37b
int8_t g_harbors_linked_sea_routes_18[] = {
	SEA_ROUTE_ID_THORWAL_PREM,
	SEA_ROUTE_ID_PREM_HJALSING,
	SEA_ROUTE_ID_PREM_MANRIN,
	SEA_ROUTE_ID_PREM_KORD,
	SEA_ROUTE_ID_SKJAL_PREM,
	SEA_ROUTE_ID_PREM_ARYN,
	SEA_ROUTE_ID_PREM_RUNINSHA,
	SEA_ROUTE_ID_OTTARJE_PREM,
	-1
};       //ds:0xa37f
int8_t g_harbors_linked_sea_routes_19[] = {
	SEA_ROUTE_ID_VARNHEIM_LJASDAHL,
	SEA_ROUTE_ID_LJASDAHL_OTTARJE,
	-1
}; //ds:0xa388
int8_t g_harbors_linked_sea_routes_20[] = {
	SEA_ROUTE_ID_VAERMHAG_VARNHEIM,
	SEA_ROUTE_ID_VARNHEIM_LJASDAHL,
	SEA_ROUTE_ID_VARNHEIM_OTTARJE,
	SEA_ROUTE_ID_THORWAL_VARNHEIM,
	-1
}; //ds:0xa38b
int8_t g_harbors_linked_sea_routes_21[] = {
	SEA_ROUTE_ID_THORWAL_VAERMHAG,
	SEA_ROUTE_ID_VAERMHAG_VARNHEIM,
	-1
}; //ds:0xa390
int8_t g_harbors_linked_sea_routes_22[] = {
	SEA_ROUTE_ID_OVERTHOR_VIDSAND,
	SEA_ROUTE_ID_LISKOR_VIDSAND,
	SEA_ROUTE_ID_TJANSET_VIDSAND,
	-1
}; //ds:0xa393
int8_t g_harbors_linked_sea_routes_23[] = {
	SEA_ROUTE_ID_OVERTHOR_BRENDHIL,
	SEA_ROUTE_ID_BRENDHIL_MANRIN,
	-1
}; //ds:0xa397
int8_t g_harbors_linked_sea_routes_24[] = {
	SEA_ROUTE_ID_PREM_MANRIN,
	SEA_ROUTE_ID_HJALSING_MANRIN,
	SEA_ROUTE_ID_BRENDHIL_MANRIN,
	SEA_ROUTE_ID_ROVIK_MANRIN,
	SEA_ROUTE_ID_MANRIN_OVERTHOR,
	-1
}; //ds:0xa39a
int8_t g_harbors_linked_sea_routes_25[] = {
	SEA_ROUTE_ID_RUNINSHA_L_RUNIN,
	SEA_ROUTE_ID_TREBAN_L_RUNIN,
	-1
}; //ds:0xa3a0

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
struct struct_point g_town_positions[TOWN_ID__END - 1] = {
/* note that the index is "off by 1" compared to TOWN_ID_... */
	{ 0x00bd, 0x0098 }, // TOWN_ID_THORWAL
	{ 0x00be, 0x00a1 }, // TOWN_ID_SERSKE
	{ 0x00e2, 0x009e }, // TOWN_ID_BREIDA
	{ 0x0100, 0x00a7 }, // TOWN_ID_PEILINEN
	{ 0x00fc, 0x00c0 }, // TOWN_ID_ROVAMUND
	{ 0x0123, 0x00c4 }, // TOWN_ID_NORDVEST
	{ 0x0131, 0x00b4 }, // TOWN_ID_KRAVIK
	{ 0x0134, 0x009d }, // TOWN_ID_SKELELLEN
	{ 0x00b7, 0x00b4 }, // TOWN_ID_MERSKE
	{ 0x00b2, 0x00c2 }, // TOWN_ID_EFFERDUN
	{ 0x00cf, 0x008d }, // TOWN_ID_TJOILA
	{ 0x00ce, 0x0083 }, // TOWN_ID_RUKIAN
	{ 0x00cc, 0x006e }, // TOWN_ID_ANGBODIRTAL
	{ 0x00c5, 0x0063 }, // TOWN_ID_AUPLOG
	{ 0x00cc, 0x0051 }, // TOWN_ID_VILNHEIM
	{ 0x00cf, 0x0052 }, // TOWN_ID_BODON
	{ 0x00c4, 0x0049 }, // TOWN_ID_OBERORKEN
	{ 0x012b, 0x0029 }, // TOWN_ID_PHEXCAER
	{ 0x0135, 0x001c }, // TOWN_ID_GROENVELDEN
	{ 0x00c6, 0x0038 }, // TOWN_ID_FELSTEYN
	{ 0x00f0, 0x0012 }, // TOWN_ID_EINSIEDLERSEE
	{ 0x00a1, 0x0020 }, // TOWN_ID_ORKANGER
	{ 0x008c, 0x0013 }, // TOWN_ID_CLANEGH
	{ 0x0077, 0x0012 }, // TOWN_ID_LISKOR
	{ 0x007e, 0x0020 }, // TOWN_ID_THOSS
	{ 0x0065, 0x001e }, // TOWN_ID_TJANSET
	{ 0x0058, 0x002f }, // TOWN_ID_ALA
	{ 0x0049, 0x0046 }, // TOWN_ID_ORVIL
	{ 0x0045, 0x000b }, // TOWN_ID_OVERTHORN
	{ 0x0039, 0x0043 }, // TOWN_ID_ROVIK
	{ 0x0020, 0x0061 }, // TOWN_ID_HJALSINGOR
	{ 0x001e, 0x0072 }, // TOWN_ID_GUDDASUNDEN
	{ 0x0025, 0x00a0 }, // TOWN_ID_KORD
	{ 0x0039, 0x00b3 }, // TOWN_ID_TREBAN
	{ 0x0042, 0x00a7 }, // TOWN_ID_ARYN
	{ 0x005b, 0x00a7 }, // TOWN_ID_RUNINSHAVEN
	{ 0x006e, 0x0069 }, // TOWN_ID_OTTARJE
	{ 0x0047, 0x007b }, // TOWN_ID_SKJAL
	{ 0x0040, 0x0090 }, // TOWN_ID_PREM
	{ 0x0086, 0x005b }, // TOWN_ID_DASPOTA
	{ 0x008a, 0x004e }, // TOWN_ID_RYBON
	{ 0x0077, 0x0070 }, // TOWN_ID_LJASDAHL
	{ 0x0091, 0x0069 }, // TOWN_ID_VARNHEIM
	{ 0x00af, 0x007f }, // TOWN_ID_VAERMHAG
	{ 0x008f, 0x0000 }, // TOWN_ID_TYLDON
	{ 0x0074, 0x0000 }, // TOWN_ID_VIDSAND
	{ 0x0027, 0x0002 }, // TOWN_ID_BRENDHIL
	{ 0x000d, 0x0005 }, // TOWN_ID_MANRIN
	{ 0x00cd, 0x008d }, // TOWN_ID_FAEHRSTATION_TJOILA
	{ 0x00cb, 0x0070 }, // TOWN_ID_FAEHRE_ANGBODIRTAL
	{ 0x007c, 0x0076 }, // TOWN_ID_HJALLANDER_HOF
	{ 0x0062, 0x00b4 }  // TOWN_ID_LEUCHTTURM_RUNIN
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

struct struct_land_route_tevent g_tevents_tab[156] = {
	{ LROUTE_ID_VAERMHAG_VARNHEIM, 26, 1 },
	{ LROUTE_ID_VARNHEIM_DASPOTA, 7, 2 },
	{ LROUTE_ID_THORWAL_SERSKE, 0, 3 },
	{ LROUTE_ID_SERSKE_BREIDA, 13, 4 },
	{ LROUTE_ID_SERSKE_BREIDA, 19, 5 },
	{ LROUTE_ID_F_TJOILA_TJOILA, 0, 6 },
	{ LROUTE_ID_TJOILA_BREIDA, 10, 7 },
	{ LROUTE_ID_TJOILA_BREIDA, 12, 8 },
	{ LROUTE_ID_BREIDA_PEILINEN, 17, 9 },
	{ LROUTE_ID_BREIDA_PEILINEN, 25, 10 },
	{ LROUTE_ID_PEILINEN_ROVAMUND, 10, 11 },
	{ LROUTE_ID_ROVAMUND_NORDVEST, 0, 12 },
	{ LROUTE_ID_ROVAMUND_NORDVEST, 8, 13 },
	{ LROUTE_ID_ROVAMUND_NORDVEST, 16, 14 },
	{ LROUTE_ID_ROVAMUND_NORDVEST, 22, 15 },
	{ LROUTE_ID_NORDVEST_KRAVIK, 8, 16 },
	{ LROUTE_ID_NORDVEST_KRAVIK, 19, 17 },
	{ LROUTE_ID_NORDVEST_KRAVIK, 24, 18 },
	{ LROUTE_ID_KRAVIK_SKELELLE, 0, 19 },
	{ LROUTE_ID_KRAVIK_SKELELLE, 8, 20 },
	{ LROUTE_ID_KRAVIK_SKELELLE, 22, 21 },
	{ LROUTE_ID_RUKIAN_F_ANGBOD, 15, 22 },
	{ LROUTE_ID_F_ANGBOD_ANGBODIR, 0, 23 },
	{ LROUTE_ID_AUPLOG_VILNHEIM, 10, 24 },
	{ LROUTE_ID_AUPLOG_VILNHEIM, 18, 25 },
	{ LROUTE_ID_VILNHEIM_BODON, 0, 26 },
	{ LROUTE_ID_VILNHEIM_PHEXCAER, 0, 27 },
	{ LROUTE_ID_VILNHEIM_PHEXCAER, 20, 28 },
	{ LROUTE_ID_VILNHEIM_PHEXCAER, 26, 29 },
	{ LROUTE_ID_VILNHEIM_PHEXCAER, 48, 30 },
	{ LROUTE_ID_VILNHEIM_PHEXCAER, 65, 31 },
	{ LROUTE_ID_VILNHEIM_PHEXCAER, 80, 32 },
	{ LROUTE_ID_VILNHEIM_PHEXCAER, 86, 33 },
	{ LROUTE_ID_VILNHEIM_PHEXCAER, 95, 34 },
	{ LROUTE_ID_PHEXCAER_EINSIEDL, 13, 35 },
	{ LROUTE_ID_PHEXCAER_EINSIEDL, 22, 36 },
	{ LROUTE_ID_PHEXCAER_EINSIEDL, 27, 37 },
	{ LROUTE_ID_PHEXCAER_EINSIEDL, 39, 38 },
	{ LROUTE_ID_PHEXCAER_EINSIEDL, 40, 39 },
	{ LROUTE_ID_PHEXCAER_EINSIEDL, 18, 144 },
	{ LROUTE_ID_VARNHEIM_AUPLOG, 18, 40 },
	{ LROUTE_ID_VARNHEIM_AUPLOG, 25, 41 },
	{ LROUTE_ID_VARNHEIM_AUPLOG, 38, 42 },
	{ LROUTE_ID_VARNHEIM_AUPLOG, 42, 43 },
	{ LROUTE_ID_DASPOTA_RYBON, 11, 44 },
	{ LROUTE_ID_DASPOTA_OTTARJE, 12, 45 },
	{ LROUTE_ID_DASPOTA_OTTARJE, 15, 46 },
	{ LROUTE_ID_DASPOTA_OTTARJE, 22, 47 },
	{ LROUTE_ID_DASPOTA_OTTARJE, 26, 48 },
	{ LROUTE_ID_OTTARJE_SKJAL, 19, 49 },
	{ LROUTE_ID_OTTARJE_SKJAL, 23, 50 },
	{ LROUTE_ID_OTTARJE_SKJAL, 27, 51 },
	{ LROUTE_ID_OTTARJE_SKJAL, 35, 52 },
	{ LROUTE_ID_OTTARJE_SKJAL, 38, 53 },
	{ LROUTE_ID_SKJAL_PREM, 18, 54 },
	{ LROUTE_ID_PREM_KORD, 14, 55 },
	{ LROUTE_ID_PREM_KORD, 23, 56 },
	{ LROUTE_ID_OTTARJE_ORVIL, 18, 57 },
	{ LROUTE_ID_OTTARJE_ORVIL, 20, 58 },
	{ LROUTE_ID_OTTARJE_ORVIL, 22, 59 },
	{ LROUTE_ID_OTTARJE_ORVIL, 33, 60 },
	{ LROUTE_ID_OTTARJE_ORVIL, 41, 61 },
	{ LROUTE_ID_OTTARJE_ORVIL, 47, 62 },
	{ LROUTE_ID_OTTARJE_ORVIL, 53, 63 },
	{ LROUTE_ID_OTTARJE_ORVIL, 58, 64 },
	{ LROUTE_ID_ORVIL_ALA, 4, 65 },
	{ LROUTE_ID_ORVIL_ALA, 19, 66 },
	{ LROUTE_ID_ALA_TJANSET, 7, 67 },
	{ LROUTE_ID_ALA_TJANSET, 10, 68 },
	{ LROUTE_ID_ALA_THOSS, 7, 67 },
	{ LROUTE_ID_ALA_THOSS, 10, 68 },
	{ LROUTE_ID_ALA_THOSS, 17, 69 },
	{ LROUTE_ID_ALA_THOSS, 29, 70 },
	{ LROUTE_ID_THOSS_LISKOR, 11, 71 },
	{ LROUTE_ID_TJANSET_THOSS, 15, 68 },
	{ LROUTE_ID_TJANSET_THOSS, 22, 69 },
	{ LROUTE_ID_TJANSET_THOSS, 34, 70 },
	{ LROUTE_ID_OBERORKE_FELSTEYN, 7, 71 },
	{ LROUTE_ID_OBERORKE_FELSTEYN, 12, 72 },
	{ LROUTE_ID_FELSTEYN_ORKANGER, 7, 73 },
	{ LROUTE_ID_FELSTEYN_ORKANGER, 12, 74 },
	{ LROUTE_ID_FELSTEYN_ORKANGER, 25, 75 },
	{ LROUTE_ID_FELSTEYN_ORKANGER, 35, 76 },
	{ LROUTE_ID_FELSTEYN_ORKANGER, 39, 77 },
	{ LROUTE_ID_ORKANGER_CLANEGH, 15, 78 },
	{ LROUTE_ID_ORKANGER_CLANEGH, 18, 79 },
	{ LROUTE_ID_ORKANGER_CLANEGH, 21, 80 },
	{ LROUTE_ID_CLANEGH_TYLDON, 18, 81 },
	{ LROUTE_ID_OBERORKE_EINSIEDL, 0, 82 },
	{ LROUTE_ID_OBERORKE_EINSIEDL, 9, 83 },
	{ LROUTE_ID_OBERORKE_EINSIEDL, 32, 84 },
	{ LROUTE_ID_OBERORKE_EINSIEDL, 35, 85 },
	{ LROUTE_ID_OBERORKE_EINSIEDL, 53, 86 },
	{ LROUTE_ID_OBERORKE_EINSIEDL, 59, 87 },
	{ LROUTE_ID_OBERORKE_EINSIEDL, 64, 88 },
	{ LROUTE_ID_FELSTEYN_EINSIEDL, 0, 89 },
	{ LROUTE_ID_FELSTEYN_EINSIEDL, 19, 90 },
	{ LROUTE_ID_FELSTEYN_EINSIEDL, 25, 91 },
	{ LROUTE_ID_FELSTEYN_EINSIEDL, 28, 92 },
	{ LROUTE_ID_FELSTEYN_EINSIEDL, 34, 93 },
	{ LROUTE_ID_FELSTEYN_EINSIEDL, 45, 87 },
	{ LROUTE_ID_FELSTEYN_EINSIEDL, 50, 88 },
	{ LROUTE_ID_RYBON_THOSS, 15, 94 },
	{ LROUTE_ID_RYBON_THOSS, 18, 95 },
	{ LROUTE_ID_RYBON_THOSS, 21, 96 },
	{ LROUTE_ID_RYBON_THOSS, 29, 97 },
	{ LROUTE_ID_RYBON_THOSS, 35, 98 },
	{ LROUTE_ID_RYBON_THOSS, 44, 99 },
	{ LROUTE_ID_RYBON_THOSS, 48, 100 },
	{ LROUTE_ID_RYBON_THOSS, 52, 101 },
	{ LROUTE_ID_SKJAL_ORVIL, 25, 102 },
	{ LROUTE_ID_SKJAL_ORVIL, 29, 103 },
	{ LROUTE_ID_SKJAL_ORVIL, 34, 104 },
	{ LROUTE_ID_SKJAL_ORVIL, 38, 105 },
	{ LROUTE_ID_SKJAL_ORVIL, 50, 62 },
	{ LROUTE_ID_SKJAL_ORVIL, 56, 63 },
	{ LROUTE_ID_SKJAL_ORVIL, 61, 64 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 20, 106 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 36, 107 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 40, 108 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 48, 109 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 57, 110 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 63, 111 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 68, 112 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 87, 113 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 100, 114 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 112, 115 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 122, 116 },
	{ LROUTE_ID_SKELELLE_PHEXCAER, 125, 117 },
	{ LROUTE_ID_MERSKE_ROVAMUND, 18, 118 },
	{ LROUTE_ID_MERSKE_ROVAMUND, 38, 119 },
	{ LROUTE_ID_MERSKE_ROVAMUND, 55, 120 },
	{ LROUTE_ID_ORVIL_ROVIK, 0, 121 },
	{ LROUTE_ID_ORVIL_ROVIK, 3, 122 },
	{ LROUTE_ID_ORVIL_ROVIK, 14, 123 },
	{ LROUTE_ID_ORVIL_ROVIK, 17, 124 },
	{ LROUTE_ID_LJASDAHL__CIRCULAR, 13, 125 },
	{ LROUTE_ID_LJASDAHL__CIRCULAR, 42, 126 },
	{ LROUTE_ID_LJASDAHL__CIRCULAR, 59, 127 },
	{ LROUTE_ID_RUNINSHA_L_RUNIN__1, 15, 128 },
	{ LROUTE_ID_RUNINSHA_L_RUNIN__MIDDLE, 10, 129 },
	{ LROUTE_ID_BRENDHIL_MANRIN, 14, 130 },
	{ LROUTE_ID_BRENDHIL_MANRIN, 17, 131 },
	{ LROUTE_ID_BRENDHIL_MANRIN, 19, 132 },
	{ LROUTE_ID_EINSIEDL__CIRCULAR, 12, 133 },
	{ LROUTE_ID_EINSIEDL__CIRCULAR, 17, 134 },
	{ LROUTE_ID_EINSIEDL__CIRCULAR, 20, 135 },
	{ LROUTE_ID_EINSIEDL__CIRCULAR, 24, 136 },
	{ LROUTE_ID_EINSIEDL__CIRCULAR, 28, 137 },
	{ LROUTE_ID_L_RUNIN_RUNINSHA__2, 18, 138 },
	{ LROUTE_ID__CROSSLINK, 10, 139 },
	{ LROUTE_ID__CROSSLINK, 23, 140 },
	{ LROUTE_ID__CROSSLINK, 28, 141 },
	{ LROUTE_ID__CROSSLINK, 39, 142 },
	{ LROUTE_ID__CROSSLINK, 43, 143 },
	{ -1,  0,   0 },
}; // ds:0xa66d

signed int do_travel_mode(void)
{
	signed int tmp1; /* multiple use: route_id, screen coordinate, etc. */
	signed int i;
	signed int tmp2; /* multiple use: tmp2, screen coordinate, etc. */
	signed int town_id;
	signed int route_id;
	struct trv_start_point *signpost_ptr;
	signed int x_bak;
	signed int y_bak;
	signed int wallclock_update_bak;
	signed int tw_bak;
	char *destinations_tab[6];

	wallclock_update_bak = g_wallclock_update;
	g_trv_crosslink_route_status = g_wallclock_update = gs_travel_detour = 0;
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

					if (g_trv_crosslink_route_status)
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

					for (i = 0, town_id = -1; i < TOWN_ID__END - 1; i++)
					{
#if defined(__BORLANDC__)
						if (is_mouse_in_rect(tmp1 - 4, tmp2 - 4,
								(tmp1 = g_town_positions[i].x) + 4,
								(tmp2 = g_town_positions[i].y) + 4))
#else
						const signed int x = g_town_positions[i].x;
						const signed int y = g_town_positions[i].y;

						if (is_mouse_in_rect(x - 4, y - 4, x + 4, y + 4))
#endif
						{
							town_id = i;
							break;
						}
					}

					if (town_id == -1 && (tmp1 = get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table_travelmap)))
					{
						town_id = tmp1 + 51;
					}

					if (town_id != -1)
					{
						tmp2 = g_current_town_anix;
						g_current_town_anix = 0;
						x_bak = g_basepos_x;
						y_bak = g_basepos_y;
						g_basepos_y = 0;
						g_basepos_x = (g_mouse_posx >= 0 && g_mouse_posx <= 159 ? 80 : -80);

						set_and_spin_lock();

						GUI_input(get_tx(town_id), 0);

						g_basepos_x = x_bak;
						g_basepos_y = y_bak;
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
