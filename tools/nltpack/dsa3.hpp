#ifndef __DSA3_HPP__
#define __DSA3_HPP__

#include "nltpack.hpp"

/**
   Format einer DSA3-Datendatei:
   *** Header:
   | Offset | Zweck                                          |
   |  0-3   | Signatur "ALF "=0x414C4620                     |
   |  4-7   | ?Version des Archivs (0x00000100)              |
   |  8-9   | Länge der Dateitabelle in Einträgen            |
   | 10-13  | Offset Beginn der Dateitabelle                 |
   | 14-15  | Tatsächliche Anzahl der Dateien in der Tabelle |
   | 16-19  | ?Offset Beginn der Daten ("Header-Länge")      |
   | 20-21  | Länge der Modultabelle in Einträgen            |
   | 22-25  | Offset Beginn der Modultabelle                 |
   | 26-27  | Tatsächliche Anzahl der Moduln in der Tabelle  |
   | 28-31  | Offset der unbekannten Daten am Ende der Datei |
   | 32-47  | Leer (16-mal 0x00)                             |
   
   *** Dateitabellen-Einträge: jeweils 28 Bytes
   | Offset | Zweck                                 |
   | 0-12   | Dateiname, 0-Terminiert               |
   | 13     | Unklar. (1)                           |
   | 14-17  | Dateilänge                            |
   | 18-21  | Unklar, vielleicht das Datum? (2)     |
   [ 22-23  | Unklar. (3)                           |
   | 24-27  | Offset in Big-Endian, -30 Bytes Header|

   *** Modultabellen-Einträge: jeweils 28 Bytes
   | Offset | Zweck                                 |
   |--------+---------------------------------------|
   |  0-13? | Name, 0-Terminiert (1)                |
   |  14-17 | Anzahl der entsprechenden Einträge    |
   |  18-21 | Unklar, vielleicht das Datum? (2)     |
   |  22-23 | Unklar.                               |
   |  24-27 | Startnummer in der Dateitabelle*2 (3) |
**/

class DSA3_ModuleEntry : public Header_Entry {
public:
    //string name;  // 14 Bytes
    //u32 size;     // Anzahl der Einträge
    u32 unknown1; // Datum?
    u16 unknown2;
    //u32 offset;

    vector<u16> entry_indices;

    bool read(istream& is);
    void write(ostream& os);
    string type() { return "DSA3"; }
};
class DSA3_Entry : public Header_Entry {
public:
    //string name;
    u8   unknown1;
    //u32  size;
    u32  unknown2;
    u16  unknown3;
    //u32  offset;
    
    bool read(istream& is);
    void write(ostream& os);
    string type() { return "DSA3"; }
};
class DSA3 : public Header {
public:
    char signature[4];
    u8   version[4];
    u16  filetable_size;
    u32  filetable_offset;
    //u16  count;
    u32  data_offset;
    u16  moduletable_size;
    u32  moduletable_offset;
    u16  module_count;
    u32  modmap_offset;
    u8   unknown1[16];
    
    // Die folgenden Strukturen sind nich direkter Bestandteil des Headers,
    //vector<DSA3_Entry*> entries;
    vector<DSA3_ModuleEntry*> modules;
    u32 modmap_size;
    vector<u16> modmap;
    
    virtual ~DSA3();
    bool read(istream& is);
    void write(ostream& os);
    bool readMaster(string filename);
    void writeMaster(string filename);
    void synchronize(CmdlineList& flist);
    string type() { return "DSA3"; }
    virtual Header_Entry* newHeaderEntry() { return new DSA3_Entry; }
    void glob(string pattern);
    errcode a_list(Config* config);
    errcode a_extract(Config* config);
    errcode a_create(Config* config);
};
#endif
