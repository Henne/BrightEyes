#ifndef __NLTPACK_HPP__
#define __NLTPACK_HPP__

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <assert.h>
#include <cstdarg>
#include "fileutils.hpp"

using std::string;
using std::vector;
using std::istream;
using std::ostream;

struct Config;

class Header_Entry {
public:
    string name;
    u32 offset;
    u32 size;
    virtual ~Header_Entry() {};
    virtual bool read(istream& is) = 0;
    virtual void write(ostream& os) = 0;
    virtual string type() = 0;
};
class Cmdline_Entry : public Header_Entry {
public:
    bool checked;
    bool read(istream&) { return false; }
    void write(ostream&) { return; }
    string type() { return "Commandline"; }
};

typedef vector<Header_Entry*> FileList;
typedef std::map<string, Cmdline_Entry> CmdlineList;

template<typename T> void initArray(T array[], int size, ...) {
    va_list ap;
    va_start(ap, size);
    for(int i=0; i<size; i++) {
	array[i] = (T) va_arg(ap,int);
    }
    va_end(ap);
}

enum errcode {ERR_OK, ERR_WARN, ERR_EXIT};

class Header {
public:
    u16 count;
    vector<Header_Entry*> entries;
    u32 header_size, file_size;
    
    virtual ~Header();
    // Factory für passenden HeaderEntry
    virtual Header_Entry* newHeaderEntry() = 0;
    
    virtual bool read(istream& is) = 0;
    virtual void write(ostream& os) = 0;
    virtual bool readMaster(string filename);
    virtual void writeMaster(string filename);
    virtual void synchronize(CmdlineList& flist) = 0;
    virtual string type() = 0;
    virtual void glob(string pattern);

    virtual errcode a_list(Config* config);
    virtual errcode a_extract(Config* config);
    virtual errcode a_create(Config* config);
    virtual errcode a_update(Config* config);
    virtual errcode a_createMaster(Config* config);
};

struct Config {
    char mode;              // l,x,c,u
    string archive_name;    // Dateiname des Archives
    CmdlineList fileList;   // Liste der per Kommandozeile übergebenen Dateien
    Header* header;         // Header des Archivs
    string archive_type;    // DSA1:SCHICK, DSA1:DSAGEN, DSA2, DSA3 [-t]
    string file_pattern;    // Pattern als optionaler Parameter     [kein Switch]
    string module_pattern;  // optionales Pattern für Moduln        [-mp]
    bool dsa2_show_dummies; // Dummy-Dateien bei Ausgabe/Entpacken berücksichtigen [-d]
    bool dsa2_create_fn;    // Kein Archiv erzeugen, sondern eine .FN [?-d?]
    bool dsa3_modules_only; // Nur Moduln anzeigen [-m]
    bool dsa3_module_dirs;  // Verzeichnisnamen=Moduln, gilt für x,c und u gleichermaßen. [-md]
    bool update_only;       // Reines Update: Keine neuen Dateien hinzufügen.
    int overwrite_mode;     // Überschreib-Modus: Nie, Fragen, Immer
    bool warn;              // Warnmeldungen ausgeben/unterdrücken
    char pathDelim;         // Trennzeichen für Pfade (Linux: '/', WinDos: '\\')
};

#endif
