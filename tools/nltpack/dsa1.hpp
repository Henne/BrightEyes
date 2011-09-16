#ifndef __DSA1_HPP__
#define __DSA1_HPP__

#include "nltpack.hpp"

/**
 * Sehr primitiver Header, der nur aus den Offsets der Dateien besteht.
 * Anzahl und Namen der Dateien sind hardcoded in der SCHICKM.EXE.
**/
class DSA1Main_Entry : public Header_Entry {
public:
    //u32 offset;
    bool read(istream& is);
    void write(ostream& os);
    string type() { return "DSA1:MAIN"; }
};

class DSA1Main : public Header {
public:
    bool read(istream& is);
    void write(ostream& os);
    void synchronize(CmdlineList& flist);
    bool readMaster(string filename);
    void writeMaster(string filename);
    string type() { return "DSA1:MAIN"; }
    virtual Header_Entry* newHeaderEntry() { return new DSA1Main_Entry; }
};



/** Einfacher Header mit Dateinamen und Offsets **/
class DSA1Gen_Entry : public Header_Entry {
public:
    bool read(istream& is);
    void write(ostream& os);
    string type() { return "DSA1:GEN"; }
};

class DSA1Gen : public Header {
    bool read(istream& is);
    void write(ostream& os);
    void synchronize(CmdlineList& flist);
    string type() { return "DSA1:GEN"; }
    virtual Header_Entry* newHeaderEntry() { return new DSA1Gen_Entry; }
};



/** Vielen Dank an Borbaradwurm für die Struktur der INTRODAT-Dateien.
 * INTRODAT.DIR - Directory Datei - Eintrag-Struktur
 * Code:
 * 1 byte (unsigned)    Länge Dateiname
 * 12 byte              Dateiname
 * 1 byte (unsigned)    Länge Volumename
 * 12 byte              Volumename
 * 4 byte (unsigned)    Offset im Volume
 * 2 byte (unsigned)    Dateigröße
 * Datei- und Volumename sind C-Strings ohne das terminierende Nullbyte, weshalb für beide
 * jeweils in einem Byte die Länge im Stringbuffer vorangestellt ist. Offset im Volume und Dateigröße
 * sind selbsterklärend. Diese Struktur kommt (Dateigröße INTRODAT.DIR)/Strukturgröße(=32 byte) mal
 * in der Directory Datei vor, dabei gibt es Müll-Einträge und Dublikate. Mülleinträge können
 * ausgeschloßen werden indem man die Längen von Datei- und Volumenname prüft, bei Längen kleiner als 13 Zeichen
 * sind ist die Struktur zu gebrauchen. Zum Schluß noch Dublikate ausschließen und schon kann auf die
 * 34 Dateien (deutsche CD-Version 3.02) zugegriffen werden.
 * 
 * INTRODAT.VOL - Volume Datei - raw-byte-data
 **/
class DSA1Intro_Entry : public Header_Entry {
public:
    string volname;
    bool read(istream& is);
    void write(ostream& os);
    string type() { return "DSA1:INTRO"; }
};

class DSA1Intro : public Header {
    bool read(istream& is);
    void write(ostream& os);
    void synchronize(CmdlineList& flist);
    //bool readMaster(string filename);
    //void writeMaster(string filename);
    string type() { return "DSA1:GEN"; }
    virtual Header_Entry* newHeaderEntry() { return new DSA1Intro_Entry; }
    errcode a_extract(Config* config);
    errcode a_create(Config* config);
};

// Dank an Borbaradwurm für die Liste der Dateien in SCHICK.DAT.
static string schickdat_filenames[308] = {
    "PLAYM_UK",
    "KDBACK.DAT",
    "KCBACK.DAT",
    "KLBACK.DAT",
    "KSBACK.DAT",
    "KARTE.DAT",
    "BICONS",
    "OBJECTS.NVF",
    "LROUT.DAT",
    "SROUT.DAT",
    "HSROUT.DAT",
    "FONT6",
    "FONT8",
    "GGSTS.NVF",
    "TEXT.LTX",
    "ICONS",
    "MONSTER",
    "MONSTER.TAB",
    "COMPASS",
    "MAPTEXT.LTX",
    "ZUSTA_UK",
    "FIGHTTXT.LTX",
    "ANIS",
    "ANIS.TAB",
    "MFIGS",
    "MFIGS.TAB",
    "THORWAL.DAT",
    "SERSKE.DAT",
    "BREIDA.DAT",
    "PEILINEN.DAT",
    "ROVAMUND.DAT",
    "NORDVEST.DAT",
    "KRAVIK.DAT",
    "SKELELLE.DAT",
    "MERSKE.DAT",
    "EFFERDUN.DAT",
    "TJOILA.DAT",
    "RUKIAN.DAT",
    "ANGBODIRTAL.DAT",
    "AUPLOG.DAT",
    "VILNHEIM.DAT",
    "BODON.DAT",
    "OBERORKEN.DAT",
    "PHEXCAER.DAT",
    "GROENVEL.DAT",
    "FELSTEYN.DAT",
    "EINSIEDL.DAT",
    "ORKANGER.DAT",
    "CLANEGH.DAT",
    "LISKOR.DAT",
    "THOSS.DAT",
    "TJANSET.DAT",
    "ALA.DAT",
    "ORVIL.DAT",
    "OVERTHORN.DAT",
    "ROVIK.DAT",
    "HJALSING.DAT",
    "GUDDASUN.DAT",
    "KORD.DAT",
    "TREBAN.DAT",
    "ARYN.DAT",
    "RUNINSHA.DAT",
    "OTTARJE.DAT",
    "SKJAL.DAT",
    "PREM.DAT",
    "DASPOTA.DAT",
    "RYBON.DAT",
    "LJASDAHL.DAT",
    "VARNHEIM.DAT",
    "VAERMHAG.DAT",
    "TYLDON.DAT",
    "VIDSAND.DAT",
    "BRENDHIL.DAT",
    "MANRIN.DAT",
    "FTJOILA.DAT",
    "FANGBODI.DAT",
    "HJALLAND.DAT",
    "RUNIN.DAT",
    "THORWAL.LTX",
    "SERSKE.LTX",
    "BREIDA.LTX",
    "PEILINEN.LTX",
    "ROVAMUND.LTX",
    "NORDVEST.LTX",
    "KRAVIK.LTX",
    "SKELELLE.LTX",
    "MERSKE.LTX",
    "EFFERDUN.LTX",
    "TJOILA.LTX",
    "RUKIAN.LTX",
    "ANGBODIR.LTX",
    "AUPLOG.LTX",
    "VILNHEIM.LTX",
    "BODON.LTX",
    "OBERORKE.LTX",
    "PHEXCAER.LTX",
    "GROENVEL.LTX",
    "FELSTEYN.LTX",
    "EINSIEDL.LTX",
    "ORKANGER.LTX",
    "CLANEGH.LTX",
    "LISKOR.LTX",
    "THOSS.LTX",
    "TJANSET.LTX",
    "ALA.LTX",
    "ORVIL.LTX",
    "OVERTHOR.LTX",
    "ROVIK.LTX",
    "HJALSING.LTX",
    "GUDDASUN.LTX",
    "KORD.LTX",
    "TREBAN.LTX",
    "ARYN.LTX",
    "RUNINSHA.LTX",
    "OTTARJE.LTX",
    "SKJAL.LTX",
    "PREM.LTX",
    "DASPOTA.LTX",
    "RYBON.LTX",
    "LJASDAHL.LTX",
    "VARNHEIM.LTX",
    "VAERMHAG.LTX",
    "TYLDON.LTX",
    "VIDSAND.LTX",
    "BRENDHIL.LTX",
    "MANRIN.LTX",
    "F_TJOILA.LTX",
    "F_ANGBOD.LTX",
    "HJALLAND.LTX",
    "L_RUNIN.LTX",
    "TAVERN.TLK",
    "SPSTAR.NVF",
    "GUERTEL.NVF",
    "HAFEN.LTX",
    "FACE.NVF",
    "HYGBACK.NVF",
    "HYGGELIK.NVF",
    "SKULL.NVF",
    "OUTRO1.NVF",
    "OUTRO2.NVF",
    "OUTRO3.NVF",
    "TOWNPAL.DAT",
    "THORWAL.XMI",
    "SUMMARY.XMI",
    "TEMPLE.XMI",
    "TERMS.XMI",
    "SMITH.XMI",
    "INN.XMI",
    "HEALER.XMI",
    "CAMP.XMI",
    "VICTORY.XMI",
    "SAMPLE.AD",
    "ATTIC",
    "POPUP.DAT",
    "154", // this file has no filename entry in the main executable
    "155", // this file has no filename entry in the main executable
    "F092.TLK",
    "HERBERG.TLK",
    "158", // this file has no filename entry in the main executable
    "DASPOTA1.TLK",
    "DASPOTA2.TLK",
    "DASPOTA3.TLK",
    "INFO1.TLK",
    "INFO2.TLK",
    "INFO3.TLK",
    "INFO4.TLK",
    "INFO5.TLK",
    "INFO6.TLK",
    "INFO7.TLK",
    "SCHMIED.TLK",
    "GHANDEL.TLK",
    "KHANDEL.TLK",
    "WHANDEL.TLK",
    "F115.TLK",
    "EREMIT.TLK",
    "HYGGELIK.TLK",
    "WEAPONS.NVF",
    "BUCH.DAT",
    "SPELLOBJ.NVF",
    "IN_HEADS.NVF",
    "TEMPICON",
    "SHIPSL.NVF",
    "STONESL.NVF",
    "MARBLESL.NVF",
    "TFLOOR1.NVF",
    "TDIVERSE.NVF",
    "HOUSE1.NVF",
    "HOUSE2.NVF",
    "HOUSE3.NVF",
    "HOUSE4.NVF",
    "FEATURE.LTX",
    "FEATURE1.LTX",
    "FEATURE2.LTX",
    "FEATURE3.LTX",
    "FEATURE4.LTX",
    "FEATURE5.LTX",
    "FEATURE6.LTX",
    "FEATURE7.LTX",
    "FEATURE8.LTX",
    "FIGHTOBJ.NVF",
    "SCENARIO.LST",
    "KDLBACK.DAT",
    "KCLBACK.DAT",
    "KLLBACK.DAT",
    "KSLBACK.DAT",
    "FIGHT.LST",
    "SKARTE.NVF",
    "GAMES.NAM",
    "ITEMNAME",
    "MONNAMES",
    "THORWAL1.LTX",
    "THORWAL2.LTX",
    "DUNGEON.XMI",
    "COMBAT.XMI",
    "PLAYM_US",
    "ZUSTA_US",
    "ANI.DAT",
    "WEAPANI.DAT",
    "MONSTER.DAT",
    "BSKILLS.DAT",
    "ITEMS.DAT",
    "CHARTEXT.LTX",
    "SPELLTXT.LTX",
    "WONDER.LTX",
    "STRASSE.LTX",
    "NSC.LTX",
    "NARIELL.NPC",
    "HARIKA.NPC",
    "CURIAN.NPC",
    "ARDORA.NPC",
    "GARSVIK.NPC",
    "ERWO.NPC",
    "FINGER.NVF",
    "LTURM.NVF",
    "WFIGS",
    "WFIGS.TAB",
    "SPLASHES.DAT",
    "FEATURE9.LTX",
    "PHEX1.LTX",
    "PHEX2.LTX",
    "WILD.LTX",
    "WAFFINFO.LTX",
    "SHIP.DNG",
    "F046.DNG",
    "F051.DNG",
    "F061.DNG",
    "F076.DNG",
    "F094.DNG",
    "F100.DNG",
    "F108.DNG",
    "F126.DNG",
    "F129.DNG",
    "F131.DNG",
    "OBER.DNG",
    "PREM.DNG",
    "THORWAL.DNG",
    "FINAL.DNG",
    "SHIP.DDT",
    "F046.DDT",
    "F051.DDT",
    "F061.DDT",
    "F076.DDT",
    "F094.DDT",
    "F100.DDT",
    "F108.DDT",
    "F126.DDT",
    "F129.DDT",
    "F131.DDT",
    "OBER.DDT",
    "PREM.DDT",
    "THORWAL.DDT",
    "FINAL.DDT",
    "SHIP.DTX",
    "F046.DTX",
    "F051.DTX",
    "F061.DTX",
    "F076.DTX",
    "F094.DTX",
    "F100.DTX",
    "F108.DTX",
    "F126.DTX",
    "F129.DTX",
    "F131.DTX",
    "OBER.DTX",
    "PREM.DTX",
    "THORWAL.DTX",
    "FINAL.DTX",
    "TFLOOR2.NVF",
    "STORY.LTX",
    "FX1.VOC",
    "FX2.VOC",
    "FX3.VOC",
    "FX4.VOC",
    "FX5.VOC",
    "FX6.VOC",
    "FX7.VOC",
    "FX8.VOC",
    "FX9.VOC",
    "FX10.VOC",
    "FX11.VOC",
    "FX12.VOC",
    "FX13.VOC",
    "FX14.VOC",
    "FX15.VOC",
    "FX16.VOC",
    "FX17.VOC",
    "FX18.VOC",
    "OUTRO.XMI" // this file has an invalid offset (0)
};

#endif
