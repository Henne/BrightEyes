/**
 *	Rewrite of DSA1 v3.02_de functions of seg026 (texts savegames)
 *	Functions rewritten: 15/15 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg026.cpp
 */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

#if defined(__BORLANDC__)
#include <DIR.H>
#include <DOS.H>
#include <IO.H>
#else
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg026.h"
#include "seg028.h"
#include "seg097.h"
#include "seg106.h"

extern char g_str_temp_xx2[8];
char *g_str_temp_xx_ptr2 = (char*)&g_str_temp_xx2[0]; // ds:0x4c88, to STR_TEMP_XX2; char*

const char* g_fnames_v302de[] = {
	g_fn_v302de_000, g_fn_v302de_001, g_fn_v302de_002, g_fn_v302de_003,
	g_fn_v302de_004, g_fn_v302de_005, g_fn_v302de_006, g_fn_v302de_007,
	g_fn_v302de_008, g_fn_v302de_009, g_fn_v302de_010, g_fn_v302de_011,
	g_fn_v302de_012, g_fn_v302de_013, g_fn_v302de_014, g_fn_v302de_015,
	g_fn_v302de_016, g_fn_v302de_017, g_fn_v302de_018, g_fn_v302de_019,
	g_fn_v302de_020, g_fn_v302de_021, g_fn_v302de_022, g_fn_v302de_023,
	g_fn_v302de_024, g_fn_v302de_025, g_fn_v302de_026, g_fn_v302de_027,
	g_fn_v302de_028, g_fn_v302de_029, g_fn_v302de_030, g_fn_v302de_031,
	g_fn_v302de_032, g_fn_v302de_033, g_fn_v302de_034, g_fn_v302de_035,
	g_fn_v302de_036, g_fn_v302de_037, g_fn_v302de_038, g_fn_v302de_039,
	g_fn_v302de_040, g_fn_v302de_041, g_fn_v302de_042, g_fn_v302de_043,
	g_fn_v302de_044, g_fn_v302de_045, g_fn_v302de_046, g_fn_v302de_047,
	g_fn_v302de_048, g_fn_v302de_049, g_fn_v302de_050, g_fn_v302de_051,
	g_fn_v302de_052, g_fn_v302de_053, g_fn_v302de_054, g_fn_v302de_055,
	g_fn_v302de_056, g_fn_v302de_057, g_fn_v302de_058, g_fn_v302de_059,
	g_fn_v302de_060, g_fn_v302de_061, g_fn_v302de_062, g_fn_v302de_063,
	g_fn_v302de_064, g_fn_v302de_065, g_fn_v302de_066, g_fn_v302de_067,
	g_fn_v302de_068, g_fn_v302de_069, g_fn_v302de_070, g_fn_v302de_071,
	g_fn_v302de_072, g_fn_v302de_073, g_fn_v302de_074, g_fn_v302de_075,
	g_fn_v302de_076, g_fn_v302de_077, g_fn_v302de_078, g_fn_v302de_079,
	g_fn_v302de_080, g_fn_v302de_081, g_fn_v302de_082, g_fn_v302de_083,
	g_fn_v302de_084, g_fn_v302de_085, g_fn_v302de_086, g_fn_v302de_087,
	g_fn_v302de_088, g_fn_v302de_089, g_fn_v302de_090, g_fn_v302de_091,
	g_fn_v302de_092, g_fn_v302de_093, g_fn_v302de_094, g_fn_v302de_095,
	g_fn_v302de_096, g_fn_v302de_097, g_fn_v302de_098, g_fn_v302de_099,
	g_fn_v302de_100, g_fn_v302de_101, g_fn_v302de_102, g_fn_v302de_103,
	g_fn_v302de_104, g_fn_v302de_105, g_fn_v302de_106, g_fn_v302de_107,
	g_fn_v302de_108, g_fn_v302de_109, g_fn_v302de_110, g_fn_v302de_111,
	g_fn_v302de_112, g_fn_v302de_113, g_fn_v302de_114, g_fn_v302de_115,
	g_fn_v302de_116, g_fn_v302de_117, g_fn_v302de_118, g_fn_v302de_119,
	g_fn_v302de_120, g_fn_v302de_121, g_fn_v302de_122, g_fn_v302de_123,
	g_fn_v302de_124, g_fn_v302de_125, g_fn_v302de_126, g_fn_v302de_127,
	g_fn_v302de_128, g_fn_v302de_129, g_fn_v302de_130, g_fn_v302de_131,
	g_fn_v302de_132, g_fn_v302de_133, g_fn_v302de_134, g_fn_v302de_135,
	g_fn_v302de_136, g_fn_v302de_137, g_fn_v302de_138, g_fn_v302de_139,
	g_fn_v302de_140, g_fn_v302de_141, g_fn_v302de_142, g_fn_v302de_143,
	g_fn_v302de_144, g_fn_v302de_145, g_fn_v302de_146, g_fn_v302de_147,
	g_fn_v302de_148, g_fn_v302de_149, g_fn_v302de_150, g_fn_v302de_151,
	g_fn_v302de_152, g_fn_v302de_153, g_fn_v302de_154, g_fn_v302de_155,
	g_fn_v302de_156, g_fn_v302de_157, g_fn_v302de_158, g_fn_v302de_159,
	g_fn_v302de_160, g_fn_v302de_161, g_fn_v302de_162, g_fn_v302de_163,
	g_fn_v302de_164, g_fn_v302de_165, g_fn_v302de_166, g_fn_v302de_167,
	g_fn_v302de_168, g_fn_v302de_169, g_fn_v302de_170, g_fn_v302de_171,
	g_fn_v302de_172, g_fn_v302de_173, g_fn_v302de_174, g_fn_v302de_175,
	g_fn_v302de_176, g_fn_v302de_177, g_fn_v302de_178, g_fn_v302de_179,
	g_fn_v302de_180, g_fn_v302de_181, g_fn_v302de_182, g_fn_v302de_183,
	g_fn_v302de_184, g_fn_v302de_185, g_fn_v302de_186, g_fn_v302de_187,
	g_fn_v302de_188, g_fn_v302de_189, g_fn_v302de_190, g_fn_v302de_191,
	g_fn_v302de_192, g_fn_v302de_193, g_fn_v302de_194, g_fn_v302de_195,
	g_fn_v302de_196, g_fn_v302de_197, g_fn_v302de_198, g_fn_v302de_199,
	g_fn_v302de_200, g_fn_v302de_201, g_fn_v302de_202, g_fn_v302de_203,
	g_fn_v302de_204, g_fn_v302de_205, g_fn_v302de_206, g_fn_v302de_207,
	g_fn_v302de_208, g_fn_v302de_209, g_fn_v302de_210, g_fn_v302de_211,
	g_fn_v302de_212, g_fn_v302de_213, g_fn_v302de_214, g_fn_v302de_215,
	g_fn_v302de_216, g_fn_v302de_217, g_fn_v302de_218, g_fn_v302de_219,
	g_fn_v302de_220, g_fn_v302de_221, g_fn_v302de_222, g_fn_v302de_223,
	g_fn_v302de_224, g_fn_v302de_225, g_fn_v302de_226, g_fn_v302de_227,
	g_fn_v302de_228, g_fn_v302de_229, g_fn_v302de_230, g_fn_v302de_231,
	g_fn_v302de_232, g_fn_v302de_233, g_fn_v302de_234, g_fn_v302de_235,
	g_fn_v302de_236, g_fn_v302de_237, g_fn_v302de_238, g_fn_v302de_239,
	g_fn_v302de_240, g_fn_v302de_241, g_fn_v302de_242, g_fn_v302de_243,
	g_fn_v302de_244, g_fn_v302de_245, g_fn_v302de_246, g_fn_v302de_247,
	g_fn_v302de_248, g_fn_v302de_249, g_fn_v302de_250, g_fn_v302de_251,
	g_fn_v302de_252, g_fn_v302de_253, g_fn_v302de_254, g_fn_v302de_255,
	g_fn_v302de_256, g_fn_v302de_257, g_fn_v302de_258, g_fn_v302de_259,
	g_fn_v302de_260, g_fn_v302de_261, g_fn_v302de_262, g_fn_v302de_263,
	g_fn_v302de_264, g_fn_v302de_265, g_fn_v302de_266, g_fn_v302de_267,
	g_fn_v302de_268, g_fn_v302de_269, g_fn_v302de_270, g_fn_v302de_271,
	g_fn_v302de_272, g_fn_v302de_273, g_fn_v302de_274, g_fn_v302de_275,
	g_fn_v302de_276, g_fn_v302de_277, g_fn_v302de_278, g_fn_v302de_279,
	g_fn_v302de_280, g_fn_v302de_281, g_fn_v302de_282, g_fn_v302de_283,
	g_fn_v302de_284, g_fn_v302de_285, g_fn_v302de_286, g_fn_v302de_287,
	g_fn_v302de_288, g_fn_v302de_289, g_fn_v302de_290, g_fn_v302de_291,
	g_fn_v302de_292, g_fn_v302de_293, g_fn_v302de_294, g_fn_v302de_295,
	g_fn_v302de_296, g_fn_v302de_297, g_fn_v302de_298, g_fn_v302de_299,
	g_fn_v302de_300, g_fn_v302de_301, g_fn_v302de_302, g_fn_v302de_303,
	g_fn_v302de_304, g_fn_v302de_305, g_fn_v302de_306, g_fn_v302de_307,
}; // ds:0x4c8c

signed int g_loaded_head_id = -1; // ds:0x515c
char g_str_temp_xx2[8] = "TEMP\\%s"; // ds:0x515e

// ds:0x5166, names of files in SCHICK.DAT
char g_fn_v302de_XXX[] = "";
char g_fn_v302de_000[] = "PLAYM_UK";
char g_fn_v302de_001[] = "KDBACK.DAT";
char g_fn_v302de_002[] = "KCBACK.DAT";
char g_fn_v302de_003[] = "KLBACK.DAT";
char g_fn_v302de_004[] = "KSBACK.DAT";
char g_fn_v302de_005[] = "KARTE.DAT";
char g_fn_v302de_006[] = "BICONS";
char g_fn_v302de_007[] = "OBJECTS.NVF";
char g_fn_v302de_008[] = "LROUT.DAT";
char g_fn_v302de_009[] = "SROUT.DAT";
char g_fn_v302de_010[] = "HSROUT.DAT";
char g_fn_v302de_011[] = "FONT6";
char g_fn_v302de_012[] = "FONT8";
char g_fn_v302de_013[] = "GGSTS.NVF";
char g_fn_v302de_014[] = "TEXT.LTX";
char g_fn_v302de_015[] = "ICONS";
char g_fn_v302de_016[] = "MONSTER";
char g_fn_v302de_017[] = "MONSTER.TAB";
char g_fn_v302de_018[] = "COMPASS";
char g_fn_v302de_019[] = "MAPTEXT.LTX";
char g_fn_v302de_020[] = "ZUSTA_UK";
char g_fn_v302de_021[] = "FIGHTTXT.LTX";
char g_fn_v302de_022[] = "ANIS";
char g_fn_v302de_023[] = "ANIS.TAB";
char g_fn_v302de_024[] = "MFIGS";
char g_fn_v302de_025[] = "MFIGS.TAB";
char g_fn_v302de_026[] = "THORWAL.DAT";
char g_fn_v302de_027[] = "SERSKE.DAT";
char g_fn_v302de_028[] = "BREIDA.DAT";
char g_fn_v302de_029[] = "PEILINEN.DAT";
char g_fn_v302de_030[] = "ROVAMUND.DAT";
char g_fn_v302de_031[] = "NORDVEST.DAT";
char g_fn_v302de_032[] = "KRAVIK.DAT";
char g_fn_v302de_033[] = "SKELELLE.DAT";
char g_fn_v302de_034[] = "MERSKE.DAT";
char g_fn_v302de_035[] = "EFFERDUN.DAT";
char g_fn_v302de_036[] = "TJOILA.DAT";
char g_fn_v302de_037[] = "RUKIAN.DAT";
char g_fn_v302de_038[] = "ANGBODIRTAL.DAT";
char g_fn_v302de_039[] = "AUPLOG.DAT";
char g_fn_v302de_040[] = "VILNHEIM.DAT";
char g_fn_v302de_041[] = "BODON.DAT";
char g_fn_v302de_042[] = "OBERORKEN.DAT";
char g_fn_v302de_043[] = "PHEXCAER.DAT";
char g_fn_v302de_044[] = "GROENVEL.DAT";
char g_fn_v302de_045[] = "FELSTEYN.DAT";
char g_fn_v302de_046[] = "EINSIEDL.DAT";
char g_fn_v302de_047[] = "ORKANGER.DAT";
char g_fn_v302de_048[] = "CLANEGH.DAT";
char g_fn_v302de_049[] = "LISKOR.DAT";
char g_fn_v302de_050[] = "THOSS.DAT";
char g_fn_v302de_051[] = "TJANSET.DAT";
char g_fn_v302de_052[] = "ALA.DAT";
char g_fn_v302de_053[] = "ORVIL.DAT";
char g_fn_v302de_054[] = "OVERTHORN.DAT";
char g_fn_v302de_055[] = "ROVIK.DAT";
char g_fn_v302de_056[] = "HJALSING.DAT";
char g_fn_v302de_057[] = "GUDDASUN.DAT";
char g_fn_v302de_058[] = "KORD.DAT";
char g_fn_v302de_059[] = "TREBAN.DAT";
char g_fn_v302de_060[] = "ARYN.DAT";
char g_fn_v302de_061[] = "RUNINSHA.DAT";
char g_fn_v302de_062[] = "OTTARJE.DAT";
char g_fn_v302de_063[] = "SKJAL.DAT";
char g_fn_v302de_064[] = "PREM.DAT";
char g_fn_v302de_065[] = "DASPOTA.DAT";
char g_fn_v302de_066[] = "RYBON.DAT";
char g_fn_v302de_067[] = "LJASDAHL.DAT";
char g_fn_v302de_068[] = "VARNHEIM.DAT";
char g_fn_v302de_069[] = "VAERMHAG.DAT";
char g_fn_v302de_070[] = "TYLDON.DAT";
char g_fn_v302de_071[] = "VIDSAND.DAT";
char g_fn_v302de_072[] = "BRENDHIL.DAT";
char g_fn_v302de_073[] = "MANRIN.DAT";
char g_fn_v302de_074[] = "FTJOILA.DAT";
char g_fn_v302de_075[] = "FANGBODI.DAT";
char g_fn_v302de_076[] = "HJALLAND.DAT";
char g_fn_v302de_077[] = "RUNIN.DAT";
char g_fn_v302de_078[] = "THORWAL.LTX";
char g_fn_v302de_079[] = "SERSKE.LTX";
char g_fn_v302de_080[] = "BREIDA.LTX";
char g_fn_v302de_081[] = "PEILINEN.LTX";
char g_fn_v302de_082[] = "ROVAMUND.LTX";
char g_fn_v302de_083[] = "NORDVEST.LTX";
char g_fn_v302de_084[] = "KRAVIK.LTX";
char g_fn_v302de_085[] = "SKELELLE.LTX";
char g_fn_v302de_086[] = "MERSKE.LTX";
char g_fn_v302de_087[] = "EFFERDUN.LTX";
char g_fn_v302de_088[] = "TJOILA.LTX";
char g_fn_v302de_089[] = "RUKIAN.LTX";
char g_fn_v302de_090[] = "ANGBODIR.LTX";
char g_fn_v302de_091[] = "AUPLOG.LTX";
char g_fn_v302de_092[] = "VILNHEIM.LTX";
char g_fn_v302de_093[] = "BODON.LTX";
char g_fn_v302de_094[] = "OBERORKE.LTX";
char g_fn_v302de_095[] = "PHEXCAER.LTX";
char g_fn_v302de_096[] = "GROENVEL.LTX";
char g_fn_v302de_097[] = "FELSTEYN.LTX";
char g_fn_v302de_098[] = "EINSIEDL.LTX";
char g_fn_v302de_099[] = "ORKANGER.LTX";
char g_fn_v302de_100[] = "CLANEGH.LTX";
char g_fn_v302de_101[] = "LISKOR.LTX";
char g_fn_v302de_102[] = "THOSS.LTX";
char g_fn_v302de_103[] = "TJANSET.LTX";
char g_fn_v302de_104[] = "ALA.LTX";
char g_fn_v302de_105[] = "ORVIL.LTX";
char g_fn_v302de_106[] = "OVERTHOR.LTX";
char g_fn_v302de_107[] = "ROVIK.LTX";
char g_fn_v302de_108[] = "HJALSING.LTX";
char g_fn_v302de_109[] = "GUDDASUN.LTX";
char g_fn_v302de_110[] = "KORD.LTX";
char g_fn_v302de_111[] = "TREBAN.LTX";
char g_fn_v302de_112[] = "ARYN.LTX";
char g_fn_v302de_113[] = "RUNINSHA.LTX";
char g_fn_v302de_114[] = "OTTARJE.LTX";
char g_fn_v302de_115[] = "SKJAL.LTX";
char g_fn_v302de_116[] = "PREM.LTX";
char g_fn_v302de_117[] = "DASPOTA.LTX";
char g_fn_v302de_118[] = "RYBON.LTX";
char g_fn_v302de_119[] = "LJASDAHL.LTX";
char g_fn_v302de_120[] = "VARNHEIM.LTX";
char g_fn_v302de_121[] = "VAERMHAG.LTX";
char g_fn_v302de_122[] = "TYLDON.LTX";
char g_fn_v302de_123[] = "VIDSAND.LTX";
char g_fn_v302de_124[] = "BRENDHIL.LTX";
char g_fn_v302de_125[] = "MANRIN.LTX";
char g_fn_v302de_126[] = "F_TJOILA.LTX";
char g_fn_v302de_127[] = "F_ANGBOD.LTX";
char g_fn_v302de_128[] = "HJALLAND.LTX";
char g_fn_v302de_129[] = "L_RUNIN.LTX";
char g_fn_v302de_130[] = "TAVERN.TLK";
char g_fn_v302de_131[] = "SPSTAR.NVF";
char g_fn_v302de_132[] = "GUERTEL.NVF";
char g_fn_v302de_133[] = "HAFEN.LTX";
char g_fn_v302de_134[] = "FACE.NVF";
char g_fn_v302de_135[] = "HYGBACK.NVF";
char g_fn_v302de_136[] = "HYGGELIK.NVF";
char g_fn_v302de_137[] = "SKULL.NVF";
char g_fn_v302de_138[] = "OUTRO1.NVF";
char g_fn_v302de_139[] = "OUTRO2.NVF";
char g_fn_v302de_140[] = "OUTRO3.NVF";
char g_fn_v302de_141[] = "TOWNPAL.DAT";
char g_fn_v302de_142[] = "THORWAL.XMI";
char g_fn_v302de_143[] = "SUMMARY.XMI";
char g_fn_v302de_144[] = "TEMPLE.XMI";
char g_fn_v302de_145[] = "TERMS.XMI";
char g_fn_v302de_146[] = "SMITH.XMI";
char g_fn_v302de_147[] = "INN.XMI";
char g_fn_v302de_148[] = "HEALER.XMI";
char g_fn_v302de_149[] = "CAMP.XMI";
char g_fn_v302de_150[] = "VICTORY.XMI";
char g_fn_v302de_151[] = "SAMPLE.AD";
char g_fn_v302de_152[] = "ATTIC";
char g_fn_v302de_153[] = "POPUP.DAT";
char g_fn_v302de_154[] = "";
char g_fn_v302de_155[] = "";
char g_fn_v302de_156[] = "F092.TLK";
char g_fn_v302de_157[] = "HERBERG.TLK";
char g_fn_v302de_158[] = "";
char g_fn_v302de_159[] = "DASPOTA1.TLK";
char g_fn_v302de_160[] = "DASPOTA2.TLK";
char g_fn_v302de_161[] = "DASPOTA3.TLK";
char g_fn_v302de_162[] = "INFO1.TLK";
char g_fn_v302de_163[] = "INFO2.TLK";
char g_fn_v302de_164[] = "INFO3.TLK";
char g_fn_v302de_165[] = "INFO4.TLK";
char g_fn_v302de_166[] = "INFO5.TLK";
char g_fn_v302de_167[] = "INFO6.TLK";
char g_fn_v302de_168[] = "INFO7.TLK";
char g_fn_v302de_169[] = "SCHMIED.TLK";
char g_fn_v302de_170[] = "GHANDEL.TLK";
char g_fn_v302de_171[] = "KHANDEL.TLK";
char g_fn_v302de_172[] = "WHANDEL.TLK";
char g_fn_v302de_173[] = "F115.TLK";
char g_fn_v302de_174[] = "EREMIT.TLK";
char g_fn_v302de_175[] = "HYGGELIK.TLK";
char g_fn_v302de_176[] = "WEAPONS.NVF";
char g_fn_v302de_177[] = "BUCH.DAT";
char g_fn_v302de_178[] = "SPELLOBJ.NVF";
char g_fn_v302de_179[] = "IN_HEADS.NVF";
char g_fn_v302de_180[] = "TEMPICON";
char g_fn_v302de_181[] = "SHIPSL.NVF";
char g_fn_v302de_182[] = "STONESL.NVF";
char g_fn_v302de_183[] = "MARBLESL.NVF";
char g_fn_v302de_184[] = "TFLOOR1.NVF";
char g_fn_v302de_185[] = "TDIVERSE.NVF";
char g_fn_v302de_186[] = "HOUSE1.NVF";
char g_fn_v302de_187[] = "HOUSE2.NVF";
char g_fn_v302de_188[] = "HOUSE3.NVF";
char g_fn_v302de_189[] = "HOUSE4.NVF";
char g_fn_v302de_190[] = "FEATURE.LTX";
char g_fn_v302de_191[] = "FEATURE1.LTX";
char g_fn_v302de_192[] = "FEATURE2.LTX";
char g_fn_v302de_193[] = "FEATURE3.LTX";
char g_fn_v302de_194[] = "FEATURE4.LTX";
char g_fn_v302de_195[] = "FEATURE5.LTX";
char g_fn_v302de_196[] = "FEATURE6.LTX";
char g_fn_v302de_197[] = "FEATURE7.LTX";
char g_fn_v302de_198[] = "FEATURE8.LTX";
char g_fn_v302de_199[] = "FIGHTOBJ.NVF";
char g_fn_v302de_200[] = "SCENARIO.LST";
char g_fn_v302de_201[] = "KDLBACK.DAT";
char g_fn_v302de_202[] = "KCLBACK.DAT";
char g_fn_v302de_203[] = "KLLBACK.DAT";
char g_fn_v302de_204[] = "KSLBACK.DAT";
char g_fn_v302de_205[] = "FIGHT.LST";
char g_fn_v302de_206[] = "SKARTE.NVF";
char g_fn_v302de_207[] = "GAMES.NAM";
char g_fn_v302de_208[] = "ITEMNAME";
char g_fn_v302de_209[] = "MONNAMES";
char g_fn_v302de_210[] = "THORWAL1.LTX";
char g_fn_v302de_211[] = "THORWAL2.LTX";
char g_fn_v302de_212[] = "DUNGEON.XMI";
char g_fn_v302de_213[] = "COMBAT.XMI";
char g_fn_v302de_214[] = "PLAYM_US";
char g_fn_v302de_215[] = "ZUSTA_US";
char g_fn_v302de_216[] = "ANI.DAT";
char g_fn_v302de_217[] = "WEAPANI.DAT";
char g_fn_v302de_218[] = "MONSTER.DAT";
char g_fn_v302de_219[] = "BSKILLS.DAT";
char g_fn_v302de_220[] = "ITEMS.DAT";
char g_fn_v302de_221[] = "CHARTEXT.LTX";
char g_fn_v302de_222[] = "SPELLTXT.LTX";
char g_fn_v302de_223[] = "WONDER.LTX";
char g_fn_v302de_224[] = "STRASSE.LTX";
char g_fn_v302de_225[] = "NSC.LTX";
char g_fn_v302de_226[] = "NARIELL.NPC";
char g_fn_v302de_227[] = "HARIKA.NPC";
char g_fn_v302de_228[] = "CURIAN.NPC";
char g_fn_v302de_229[] = "ARDORA.NPC";
char g_fn_v302de_230[] = "GARSVIK.NPC";
char g_fn_v302de_231[] = "ERWO.NPC";
char g_fn_v302de_232[] = "FINGER.NVF";
char g_fn_v302de_233[] = "LTURM.NVF";
char g_fn_v302de_234[] = "WFIGS";
char g_fn_v302de_235[] = "WFIGS.TAB";
char g_fn_v302de_236[] = "SPLASHES.DAT";
char g_fn_v302de_237[] = "FEATURE9.LTX";
char g_fn_v302de_238[] = "PHEX1.LTX";
char g_fn_v302de_239[] = "PHEX2.LTX";
char g_fn_v302de_240[] = "WILD.LTX";
char g_fn_v302de_241[] = "WAFFINFO.LTX";
char g_fn_v302de_242[] = "SHIP.DNG";
char g_fn_v302de_243[] = "F046.DNG";
char g_fn_v302de_244[] = "F051.DNG";
char g_fn_v302de_245[] = "F061.DNG";
char g_fn_v302de_246[] = "F076.DNG";
char g_fn_v302de_247[] = "F094.DNG";
char g_fn_v302de_248[] = "F100.DNG";
char g_fn_v302de_249[] = "F108.DNG";
char g_fn_v302de_250[] = "F126.DNG";
char g_fn_v302de_251[] = "F129.DNG";
char g_fn_v302de_252[] = "F131.DNG";
char g_fn_v302de_253[] = "OBER.DNG";
char g_fn_v302de_254[] = "PREM.DNG";
char g_fn_v302de_255[] = "THORWAL.DNG";
char g_fn_v302de_256[] = "FINAL.DNG";
char g_fn_v302de_257[] = "SHIP.DDT";
char g_fn_v302de_258[] = "F046.DDT";
char g_fn_v302de_259[] = "F051.DDT";
char g_fn_v302de_260[] = "F061.DDT";
char g_fn_v302de_261[] = "F076.DDT";
char g_fn_v302de_262[] = "F094.DDT";
char g_fn_v302de_263[] = "F100.DDT";
char g_fn_v302de_264[] = "F108.DDT";
char g_fn_v302de_265[] = "F126.DDT";
char g_fn_v302de_266[] = "F129.DDT";
char g_fn_v302de_267[] = "F131.DDT";
char g_fn_v302de_268[] = "OBER.DDT";
char g_fn_v302de_269[] = "PREM.DDT";
char g_fn_v302de_270[] = "THORWAL.DDT";
char g_fn_v302de_271[] = "FINAL.DDT";
char g_fn_v302de_272[] = "SHIP.DTX";
char g_fn_v302de_273[] = "F046.DTX";
char g_fn_v302de_274[] = "F051.DTX";
char g_fn_v302de_275[] = "F061.DTX";
char g_fn_v302de_276[] = "F076.DTX";
char g_fn_v302de_277[] = "F094.DTX";
char g_fn_v302de_278[] = "F100.DTX";
char g_fn_v302de_279[] = "F108.DTX";
char g_fn_v302de_280[] = "F126.DTX";
char g_fn_v302de_281[] = "F129.DTX";
char g_fn_v302de_282[] = "F131.DTX";
char g_fn_v302de_283[] = "OBER.DTX";
char g_fn_v302de_284[] = "PREM.DTX";
char g_fn_v302de_285[] = "THORWAL.DTX";
char g_fn_v302de_286[] = "FINAL.DTX";
char g_fn_v302de_287[] = "TFLOOR2.NVF";
char g_fn_v302de_288[] = "STORY.LTX";
char g_fn_v302de_289[] = "FX1.VOC";
char g_fn_v302de_290[] = "FX2.VOC";
char g_fn_v302de_291[] = "FX3.VOC";
char g_fn_v302de_292[] = "FX4.VOC";
char g_fn_v302de_293[] = "FX5.VOC";
char g_fn_v302de_294[] = "FX6.VOC";
char g_fn_v302de_295[] = "FX7.VOC";
char g_fn_v302de_296[] = "FX8.VOC";
char g_fn_v302de_297[] = "FX9.VOC";
char g_fn_v302de_298[] = "FX10.VOC";
char g_fn_v302de_299[] = "FX11.VOC";
char g_fn_v302de_300[] = "FX12.VOC";
char g_fn_v302de_301[] = "FX13.VOC";
char g_fn_v302de_302[] = "FX14.VOC";
char g_fn_v302de_303[] = "FX15.VOC";
char g_fn_v302de_304[] = "FX16.VOC";
char g_fn_v302de_305[] = "FX17.VOC";
char g_fn_v302de_306[] = "FX18.VOC";
char g_fn_v302de_307[] = "OUTRO.XMI";

char g_chr_file_suffix[5] = ".CHR"; // ds:0x5e3e
char g_savegame_suffix[5] = ".gam"; // ds:0x5e43
char g_all_files_wildcard[4] = "*.*"; // ds:0x5e48
char g_all_chr_wildcard[6] = "*.CHR"; // ds:0x5e4c
char g_empty_string1[1] = ""; // ds:0x5e52
char g_empty_string2[1] = ""; // ds:0x5e53
char g_savegame_suffix2[5] = ".gam"; // ds:0x5e54
char g_savegame_suffix3[5] = ".gam"; // ds:0x5e59
char g_all_chr_wildcard2[6] = "*.CHR"; // ds:0x5e5e
char g_all_chr_wildcard3[6] = "*.CHR"; // ds:0x5e64



static uint32_t *g_saved_files_buf;		// ds:0xe2d2
time_t g_last_save_time;			// ds:0xe2d6
char g_savegame_names[5][9];			// ds:0xe2da

void init_text(void)
{
	int32_t len;
	signed int handle;

	handle = load_archive_file(ARCHIVE_FILE_FONT6);
	read_archive_file(handle, g_buf_font6, 1000);
	close(handle);

	handle = load_archive_file(ARCHIVE_FILE_TEXT_LTX);
	len = (int16_t)read_archive_file(handle, (uint8_t*)g_text_ltx_buffer, 64000);
	close(handle);

	split_textbuffer((char**)g_text_ltx_index, g_text_ltx_buffer, len);

	handle = load_archive_file(ARCHIVE_FILE_ITEMNAME);
	len = (int16_t)read_archive_file(handle, (uint8_t*)g_buffer5_ptr, 5000);
	close(handle);

	split_textbuffer((char**)g_itemsname, g_buffer5_ptr, len);

	handle = load_archive_file(ARCHIVE_FILE_MONNAMES);
	len = (int16_t)read_archive_file(handle, (uint8_t*)g_monnames_buffer, 5000);
	close(handle);

	split_textbuffer((char**)g_monnames_index, g_monnames_buffer, len);
}

void load_tx(const signed int index)
{
	int32_t archive_file_len;
	signed int archive_file_handle;

	if (index == -1)
		return;

	archive_file_handle = load_archive_file(index);

	archive_file_len = (int16_t)read_archive_file(archive_file_handle, (uint8_t*)g_buffer7_ptr, 64000);

	close(archive_file_handle);

	split_textbuffer((char**)g_tx_index, g_buffer7_ptr, archive_file_len);

	g_tx_file_index = index;
}

void load_tx2(const signed int index)
{
	int32_t len;
	register signed int handle;

	if (index == -1)
		return;

	g_text_file_index = index;
	handle = load_archive_file(index);
	len = (int16_t)read_archive_file(handle, g_buffer8_ptr, 12000);
	close(handle);

	split_textbuffer((char**)g_tx2_index, (char*)g_buffer8_ptr, len);
}

/* REMARK: 1000 byte for pointer, means 250 strings on 16/32-bit machines and 125 strings on 64-bit machines.
 * WAFFINFO.LTX seems to have 19 + 40 + 67 = 126 strings, which will lead to problems */
void load_ltx(const signed int index)
{
	int32_t len;
	signed int handle = load_archive_file(index);

	g_area_prepared = AREA_TYPE_NONE;

	len = (int16_t)read_archive_file(handle, ((uint8_t*)g_buffer9_ptr3) + 1000, 64000);

	close(handle);

	split_textbuffer((char**)g_buffer9_ptr3, (char*)(g_buffer9_ptr3 + 1000L), len);
}

void split_textbuffer(char **dst, char *src, uint32_t len)
{
	uint32_t i = 0;

	*dst = src;
	dst++;

	for (; i != len; src++, i++) {

		/* continue if not the end of the string */
		if (!(*src)) {

			/* write the adress of the next string */
			*dst = src + 1;
			dst++;
		}
	}
}

void load_ggsts_nvf(void)
{
	signed int handle;

	/* seek to GGSTS.NVF */
	handle = load_archive_file(ARCHIVE_FILE_GGSTS_NVF);

	/* read it */
	read_archive_file(handle, g_buffer10_ptr, 16771);

	/* close it */
	close(handle);

	g_area_prepared = AREA_TYPE_NONE;
}

void prepare_chr_name(char *dst, char *src)
{
	char tmp_str[40];
	signed int i;

	strcpy(tmp_str, src);

	for (i = 0; i < 8; i++) {

		if (!tmp_str[i])
			break;

		if (!isalnum(tmp_str[i])) {
			tmp_str[i] = '_';
		}
	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
	strcat(dst, g_chr_file_suffix);
}

void prepare_sg_name(char *dst, char *src)
{
	char tmp_str[40];
	signed int i;

	strcpy(tmp_str, src);

	i = 0;
	while (i < 8) {

		if (!tmp_str[i]) {

			while (i < 8) {
				/* fill up with underscores */
				tmp_str[i] = '_';
				i++;
			}
			break;
		}

		if (!isalnum(tmp_str[i])) {
			tmp_str[i] = '_';
		}
		i++;
	}

	strncpy(dst, tmp_str, 8);
	dst[8] = '\0';
}

signed int load_game_state(void)
{
#if defined(__BORLANDC__)
	register signed int handle_sg;
	signed int i;
	signed int handle;
	signed int answer;
	signed int sl_bak;
	HugePt p_status_start;
	HugePt p_status_end;
	signed int status_length;
	signed int done;
	signed int length_hero;
	signed int retval;
	signed int ani_bak;
	struct struct_hero* hero_i;
	signed char version[4];
	struct ffblk blk;
	char name[20];

	retval = 0;

	/* select a game state */
	answer = GUI_radio(get_ttx(0), 6, g_savegame_names[0],
			g_savegame_names[1], g_savegame_names[2],
			g_savegame_names[3], g_savegame_names[4],
			get_ttx(737)) - 1;

	/* sanity check if answer is in range */
	if (answer != -2 && answer != 5) {

		prepare_sg_name(g_text_output_buf, g_savegame_names[answer]);
		/* concat with ".gam" */
		strcat(g_text_output_buf, g_savegame_suffix);

		/* open the game state file */
		if ((handle_sg = open(g_text_output_buf, O_BINARY | O_RDONLY)) == -1)
		{
			GUI_output(get_ttx(635));
			retval = -1;
			return retval;
		}

		call_mouse_bg();

		/* something ani related */
		sl_bak = g_update_statusline;
		g_update_statusline = 0;

		ani_bak = g_ani_enabled;
		g_ani_enabled = 0;

		/* delete every file TEMP\\*.* */
		sprintf(g_text_output_buf, g_str_temp_xx_ptr2, g_all_files_wildcard);

#if defined(__BORLANDC__)
		done = findfirst(g_text_output_buf, &blk, 0);

		if (done == 0) {

			do {
				sprintf(g_text_output_buf, g_str_temp_xx_ptr2, ((char*)(&blk))+ 30);
				unlink(g_text_output_buf);

				done = findnext(&blk);

			} while (done == 0);
		}
#endif

		/* init */
		g_saved_files_buf = (uint32_t*)g_dtp2;
		memset(g_saved_files_buf, 0, 286 * sizeof(uint32_t));

		/* read version info */
		_read(handle_sg, (uint8_t*)g_text_output_buf, 12);
		_read(handle_sg, (uint8_t*)&version[3], 1);
		_read(handle_sg, (uint8_t*)&version[2], 1);
		_read(handle_sg, (uint8_t*)&version[0], 1);
		_read(handle_sg, (uint8_t*)&version[1], 1);

		_read(handle_sg, &gs_datseg_status_start, 4);

		/* read game status */
		/* TODO: check pointer arithmetics work with other pointers */
		p_status_start = (HugePt)&gs_datseg_status_start;
		p_status_end = (HugePt)&gs_datseg_status_end;
		status_length = p_status_end - p_status_start;	/* REMARK: pointer arithmetic */

		_read(handle_sg, &gs_datseg_status_start, status_length);

		g_special_screen = 1;

		/* read file table */
		_read(handle_sg, g_saved_files_buf, 286 * sizeof(uint32_t));

		/* create for each saved file in gam a file in TEMP */
		for (i = 0; i < 286; i++) {

			if (g_saved_files_buf[i]) {

				/* write file content to TEMP */
				sprintf(g_text_output_buf, g_str_temp_xx_ptr2, g_fnames_v302de[i]);

				/* TODO: should be O_BINARY | O_WRONLY */
				handle = _creat(g_text_output_buf, 0);

				_read(handle_sg, g_renderbuf_ptr, (uint16_t)g_saved_files_buf[i]);
				write(handle, g_renderbuf_ptr, (uint16_t)g_saved_files_buf[i]);
				close(handle);
			}
		}

		/* clear the heroes */
		hero_i = get_hero(0);
		for (i = 0; i <= 6; i++, hero_i++) {
			memset(hero_i, 0, sizeof(struct struct_hero));
		}

		hero_i = (struct struct_hero*)g_renderbuf_ptr;

		do {
			length_hero = _read(handle_sg, (uint8_t*)hero_i, sizeof(struct struct_hero));

			if (length_hero != 0) {

				prepare_chr_name(name, (char*)hero_i);

				/* write file content to TEMP */
				sprintf(g_text_output_buf, g_str_temp_xx_ptr2, name);

				/* TODO: should be O_BINARY | O_WRONLY */
				handle = _creat(g_text_output_buf, 0);

				write(handle, (uint8_t*)hero_i, sizeof(struct struct_hero));
				close(handle);

				if (hero_i->slot_pos != 0) {

					prepare_chr_name(name, (char*)hero_i);

					read_chr_temp(name, hero_i->slot_pos - 1, hero_i->group_id);
				}
			}

		} while (length_hero != 0);

		close(handle_sg);

#if defined(__BORLANDC__)
		/* search for "*.CHR" */
		done = findfirst(g_all_chr_wildcard, &blk, 0);

		while (done == 0) {

			sprintf(g_text_output_buf, g_str_temp_xx_ptr2, ((char*)(&blk)) + 30);

			if ((handle_sg = open(g_text_output_buf, O_BINARY | O_RDWR)) == -1) {
				handle = open(blk.ff_name, O_BINARY | O_RDWR);
				_read(handle, g_renderbuf_ptr, sizeof(struct struct_hero));
				close(handle);

				/* TODO: should be O_BINARY | O_WRONLY */
				handle_sg = _creat(g_text_output_buf, 0);
				write(handle_sg, g_renderbuf_ptr, sizeof(struct struct_hero));
			} else {
				/* Yes, indeed! */
			}

			close(handle_sg);

			done = findnext(&blk);
		}
#endif

		for (i = ARCHIVE_FILE_NPCS; i <= (ARCHIVE_FILE_NPCS+5); i++) {

			load_npc(i);

			if (get_hero(6)->slot_pos != 7) {

				memset(get_hero(6), 0, sizeof(struct struct_hero));
			} else {
				break;
			}
		}

		g_area_prepared = AREA_TYPE_NONE;
		g_request_refresh = retval = 1;
		g_check_disease = 0;
		g_check_poison = 0;
		g_fading_state = 3;

		if (gs_town_loc_type != LOCTYPE_TEMPLE) {
			g_new_menu_icons[6] = g_new_menu_icons[7] = g_new_menu_icons[8] = MENU_ICON_NONE;
		}

		load_area_description(2);

		g_update_statusline = sl_bak;
		g_ani_enabled = ani_bak;

		call_mouse();
	}

	return retval;
#else
	return 0;
#endif
}

/**
 * \brief   writes a game state file
 *
 * \return              1 = OK, 0 = error
 */
signed int save_game_state(void)
{
#if defined(__BORLANDC__)
	signed int handle_sg;
	HugePt p_status_start;
	HugePt p_status_end;
	unsigned int status_length;
	signed int handle;
	signed int tw_bak;
	signed int done;
	signed int slot;
	uint32_t filepos;
	uint32_t filepos2;
	signed int flag;
	uint32_t len;
	struct ffblk blk;

	tw_bak = g_textbox_width;
	g_textbox_width = 5;

	/* prepare the header for the radio box */
	if (g_game_state == GAME_STATE_VICTORY) {

		/* game won. creating savegame for import in DSA2 */
		strcpy(g_text_output_buf, get_ttx(810)); /* "Welcher Spielstand soll fuer die Fortsetzung abgespeichert werden?" */

	} else {

#ifndef M302de_FEATURE_MOD
		/* Feature mod 4: In the original game, when creating a savegame while not being in a temple, the AP of all heroes is decreased by 1. This feature mod stops the AP decrease.
		 * Here, the warning message "Dabei verliert jeder Held in der Gruppe einen Abenteuerpunkt" is displayed. */
		if ((gs_town_loc_type != LOCTYPE_TEMPLE) && (g_game_state != GAME_STATE_VICTORY)) {

			/* create savegame not in a temple */

			sprintf(g_dtp2,
				get_ttx(813), /* "Dabei verliert jeder Held in der Gruppe einen Abenteuerpunkt" */
				1, get_ttx(392), g_empty_string1);

			sprintf(g_text_output_buf,
				get_ttx(1), /* "Welchen Spielstand wollen Sie abspeichern ?" */
				g_dtp2);
		} else {
#endif

			/* create savegame inside a temple */
			sprintf(g_text_output_buf,
				get_ttx(1), /* "Welchen Spielstand wollen Sie abspeichern ?" */
				g_empty_string2);
#ifndef M302de_FEATURE_MOD
		/* Feature mod 4: In the original game, when creating a savegame while not being in a temple, the AP of all heroes is decreased by 1. This mod stops the AP decrease. */
		}
#endif
	}

	/* get the slot number */
	slot = GUI_radio(g_text_output_buf, 6, g_savegame_names[0], g_savegame_names[1],
					g_savegame_names[2], g_savegame_names[3],
					g_savegame_names[4], get_ttx(737)) - 1;

	g_textbox_width = tw_bak;

	g_saved_files_buf = (uint32_t*)g_dtp2;
	memset(g_saved_files_buf, 0, 286 * sizeof(uint32_t));

	if (slot != -2 && slot != 5) {

		do {
			/* ask for filename */
			g_gui_entering_savegame = 1;
			strcpy(g_text_input_buf, g_savegame_names[slot]);
			GUI_input(get_ttx(634), 8);
			g_gui_entering_savegame = 0;

			if (*g_text_input_buf == '\0') {
				return 0;
			}

			flag = 0;

			prepare_sg_name(g_text_output_buf, g_text_input_buf);

			for (tw_bak = 0; tw_bak < 5; tw_bak++) {

				prepare_sg_name(g_text_output_buf + 50, g_savegame_names[tw_bak]);

				if ((slot != tw_bak) && !strcmp(g_text_output_buf, g_text_output_buf + 50)) {

					GUI_output(get_ttx(806));
					flag = 1;
				}
			}
		} while (flag != 0);

		/* delete the previous file of that slot */
		prepare_sg_name(g_text_output_buf, g_savegame_names[slot]);
		strcat(g_text_output_buf, g_savegame_suffix2);
		unlink(g_text_output_buf);
		strcpy(g_savegame_names[slot], g_text_input_buf);

		/* create a CHR-file for each hero in TEMP */
		for (tw_bak = 0; tw_bak < 6; tw_bak++) {

			if (get_hero(tw_bak)->typus != HERO_TYPE_NONE) {

				/* save position on the playmask */
				get_hero(tw_bak)->slot_pos = tw_bak + 1;

#ifndef M302de_FEATURE_MOD
				/* Feature mod 4: In the original game, when creating a savegame while not being in a temple, the AP of all heroes is decrease by 1. This feature mod stops the AP decrease.
				 * Here, the actual decrease is executed */
				if ((g_game_state != GAME_STATE_VICTORY) && (gs_town_loc_type != LOCTYPE_TEMPLE) && (get_hero(tw_bak)->ap > 0))
				{
					add_hero_ap(get_hero(tw_bak), -1L);
				}
#endif

				write_chr_temp(tw_bak);
			}
		}

		/* save the current NPC in TEMP */
		if (get_hero(6)->typus != HERO_TYPE_NONE) {

			get_hero(6)->slot_pos = 7;
			save_npc(get_hero(6)->npc_id + 225);
		}

		load_area_description(1);

		/* TODO: check if pointer arithmetics work with other pointers */
		p_status_start = (HugePt)&gs_datseg_status_start;
		p_status_end = (HugePt)&gs_datseg_status_end;
		status_length = p_status_end - p_status_start;

		prepare_sg_name(g_text_output_buf, g_savegame_names[slot]);
		strcat(g_text_output_buf, g_savegame_suffix3);

		/* TODO: should be O_BINARY | O_RWONLY */
		while ((handle_sg = _creat(g_text_output_buf, 0)) == -1) {
			GUI_output(get_ttx(348));
			return 0;
		}

		time(&g_last_save_time);

		filepos = 0;

		/* write version identifier 16 bytes */
		filepos += write(handle_sg, &g_dsa_version_string, 12);
		filepos += write(handle_sg, &g_version_token4, 1);
		filepos += write(handle_sg, &g_version_token3, 1);
		filepos += write(handle_sg, &g_version_token1, 1);
		filepos += write(handle_sg, &g_version_token2, 1);

		/* write fileposition 4 bytes */
		/* this will be updated later to find the data of the CHR files */
		filepos += write(handle_sg, &filepos, 4);

		/* save the status section 5952 bytes */
		filepos += write(handle_sg, p_status_start, status_length);

		/* check if enough bytes were written */
		if (status_length + 16 + 4L != filepos) {

			GUI_output(get_ttx(348));
			close(handle_sg);
			return 0;
		}

		filepos2 = filepos;
		len = (uint16_t)write(handle_sg, g_saved_files_buf, 286 * sizeof(uint32_t));
		filepos += len;

		if (len != 4 * 286) {

			GUI_output(get_ttx(348));
			close(handle_sg);
			return 0;
		}

		/* save all changed files from SCHICK.DAT */
		for (tw_bak = 0; tw_bak < 286; tw_bak++) {

			sprintf(g_text_output_buf, g_str_temp_xx_ptr2, g_fnames_v302de[tw_bak]);

			done = findfirst(g_text_output_buf, &blk, 0);


			if (done == 0) {

				handle = load_archive_file(tw_bak + 0x8000);
				g_saved_files_buf[tw_bak] = get_readlength2(handle);
				_read(handle, g_renderbuf_ptr, (uint16_t)g_saved_files_buf[tw_bak]);
				close(handle);

				len = (uint16_t)write(handle_sg, g_renderbuf_ptr, (uint16_t)g_saved_files_buf[tw_bak]);
				filepos += len;

				if ((uint16_t)g_saved_files_buf[tw_bak] != len) {
					GUI_output(get_ttx(348));
					close(handle_sg);
					return 0;
				}
			}
		}

		/* skip back to the start of the offset of the CHR data */
		lseek(handle_sg, 16, 0);
		write(handle_sg, &filepos, 4);

		/* write the file table */
		lseek(handle_sg, filepos2, 0);
		write(handle_sg, g_saved_files_buf, 286 * sizeof(uint32_t));

		/* append all CHR files */
		lseek(handle_sg, filepos, 0);
		sprintf(g_text_output_buf, g_str_temp_xx_ptr2, g_all_chr_wildcard2);

		done = findfirst(g_text_output_buf, &blk, 0);
		do {
			/* create the CHR filename */
			sprintf(g_text_output_buf, g_str_temp_xx_ptr2, blk.ff_name);

			/* read the CHR file from temp */
			handle = open(g_text_output_buf, O_BINARY | O_RDWR);
			_read(handle, g_renderbuf_ptr, sizeof(struct struct_hero));
			close(handle);

			/* append it */
			len = write(handle_sg, g_renderbuf_ptr, sizeof(struct struct_hero));

			if (len != sizeof(struct struct_hero)) {

				GUI_output(get_ttx(348));
				close(handle_sg);
				return 0;
			}

			done = findnext(&blk);

		} while (done == 0);

		close(handle_sg);

		/* rewrite GAMES.NAM */
		handle_sg = _creat(g_fnames_v302de[207], 0);
		write(handle_sg, &g_savegame_names[0][0], 45);
		close(handle_sg);

		return 1;
	}
#endif

	return 0;
}

/**
 * \brief   reads a CHR file from TEMP-dir
 *
 * \param   fname       filename
 * \param   hero_pos    position of the hero
 * \param   group_id    id of the group
 * \return              1 = OK, 0 = Error
 */
signed int read_chr_temp(char *fname, const signed int hero_pos, const signed int group_id)
{
#if defined(__BORLANDC__)
	signed int handle;
	signed int hero_size = sizeof(struct struct_hero);
	struct struct_hero *hero;

	sprintf(g_text_output_buf, g_str_temp_xx_ptr2, (char*)fname);

	if ((handle = open(g_text_output_buf, O_BINARY | O_RDWR)) == -1) {
		copy_file_to_temp((char*)fname, g_text_output_buf);
		handle = open(g_text_output_buf, O_BINARY | O_RDWR);
	}

	if (handle != -1) {

		hero = get_hero(hero_pos);
		_read(handle, hero, hero_size);
		close(handle);

		hero->group_id = group_id;

		if (hero->sex == 1) {

			hero->sprite_id = hero->typus + 11;

			if (hero->sprite_id > 21) {
				hero->sprite_id = 21;
			}
		} else {
			hero->sprite_id = hero->typus;

			if (hero->sprite_id > 10) {
				hero->sprite_id = 10;
			}
		}

		/* In the following line it would be more consistent to check only bit 0.
		 * bit 1 is a flag which is set if the hero got the IN attribute bonus at the black eye at the Monolith (Einsiedlersee <-> Einsiedlersee, tevent135).
		 * However, this should still be ok, as it should never happen that bit 0 is unset and bit 1 is set. */

		if (!hero->start_gear__monolith_reward) {

			startup_equipment(hero);

			get_hero(hero_pos)->start_gear__monolith_reward = 1; /* it would be more consistent to set only bit 0 and leave the others untouched, see above. */

			write_chr_temp(hero_pos);
		}

	} else {
		GUI_output(get_ttx(4));
		return 0;
	}
#endif
	return 1;
}

/**
 * \brief   writes a CHR file to temp
 *
 * \param   hero_pos    position of the hero
 */
void write_chr_temp(const signed int hero_pos)
{
	char fname[20];
	signed int handle;

	prepare_chr_name(fname, get_hero(hero_pos)->name);

	sprintf(g_text_output_buf, g_str_temp_xx_ptr2, fname);

	/* TODO: should be O_BINARY | O_WRONLY */
	handle = _creat(g_text_output_buf, 0);

	write(handle, get_hero(hero_pos), sizeof(struct struct_hero));

	close(handle);
}

/**
 * \brief   copy the names from CHR file
 *
 * \param   ptr         pointer
 * \param   temple_id   > 0 the id of the temple, -1 on delete mode
 * \return              # of CHR-files in TEMP-dir
 */
signed int copy_chr_names(uint8_t *ptr, const signed int temple_id)
{
#if defined(__BORLANDC__)
	signed int count = 0;
	signed int done;
	signed int handle;
	struct struct_hero *hero;
	struct ffblk blk;

	hero = (struct struct_hero*)(g_renderbuf_ptr + 60000);

	sprintf(g_text_output_buf, g_str_temp_xx_ptr2, g_all_chr_wildcard3);

	done = findfirst(g_text_output_buf, &blk, 0);

	if (!done) {

		do {
			/* create the CHR filename */
			sprintf(g_text_output_buf, g_str_temp_xx_ptr2, blk.ff_name);

			/* read the CHR file from temp */
			handle = open(g_text_output_buf, O_BINARY | O_RDWR);
			_read(handle, hero, sizeof(struct struct_hero));
			close(handle);

			if (((hero->temple_id == temple_id) && !hero->slot_pos) ||
				(!hero->slot_pos && (temple_id == -1)))
			{
				strcpy((char*)ptr + 32 * count, hero->name);
				strcpy((char*)ptr + 32 * count + 16, hero->alias);
				count++;
			}

			done = findnext(&blk);

		} while (!done);

		return count;
	} else {
		return 0;
	}
#else
	return 0;
#endif
}

/**
 * \brief   loads a head icon from IN_HEADS.NVF
 *
 * \param   head        index of the desired head
 */
void load_in_head(const signed int head)
{
	signed int handle;

	if (head >= 0) {

		handle = load_archive_file(ARCHIVE_FILE_IN_HEADS_NVF);

		seek_archive_file(handle, 1024L * head, 0);

		read_archive_file(handle, (unsigned char*)g_dtp2, 1024);

		close(handle);

		g_loaded_head_id = head;
	}
}

/**
 * \brief   load a temple icon
 *
 * \param   icon_id          the number of the icon
 */
void load_tempicon(signed int icon_id)
{
	struct nvf_extract_desc nvf;
	signed int handle; /* REMARK: reused differently */

	if (icon_id == 14) {
		icon_id = 7;
	}

	/* load TEMPICON */
	handle = load_archive_file(ARCHIVE_FILE_TEMPICON);
	read_archive_file(handle, g_buffer8_ptr, 7000);
	close(handle);

	nvf.dst = g_buffer8_ptr + 7000;
	nvf.src = g_buffer8_ptr;
	nvf.image_num = icon_id;
	nvf.compression_type = 0;
	nvf.width = &handle;
	nvf.height = &handle;

	process_nvf_extraction(&nvf);
}
