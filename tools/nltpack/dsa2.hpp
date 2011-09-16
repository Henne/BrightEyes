#ifndef __DSA2_HPP__
#define __DSA2_HPP__

#include "nltpack.hpp"

/** Format des Headers:
| Offset | Zweck                                        |
|    0-1 | Anzahl n der enthaltenen Dateien, Big-Endian |
Direkt darauf folgt ab Offset 2 die Dateitabelle.

*** Format der Einträge:
| Offset | Zweck                                                       |
|    0-1 | Gibt an, ob die Datei im Archiv oder auf CD ist. |
|   2-15 | Dateiname                                                   |
|  16-19 | Offset der Datei im Archiv, abzüglich 2+20*n Bytes Header   |
*/
class DSA2_Entry : public Header_Entry {
public:
    u16 no_dummy;

    bool read(istream& is);
    void write(ostream& os);
    string type() { return "DSA2"; }
};
class DSA2 : public Header {
public:
    //u16 count;
    //vector<DSA2_Entry*> entries;

    bool read(istream& is);
    void write(ostream& os);
    void synchronize(CmdlineList& flist);
    string type() { return "DSA2"; }
    bool readMaster(string filename);
    void writeMaster(string filename);
    virtual Header_Entry* newHeaderEntry() { return new DSA2_Entry; }
};
#endif
