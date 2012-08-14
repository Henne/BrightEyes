#ifndef __FILEUTILS_HPP__
#define __FILEUTILS_HPP__

#ifdef _WIN32
#define __LITTLE_ENDIAN 0x1234
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>

using std::string;
using std::istream;
using std::ostream;

typedef uint32_t u32;
typedef int32_t  s32;
typedef uint16_t u16;
typedef int16_t  s16;
typedef uint8_t  u8;

// === Lese- und Schreibfunktionen für Big-Endian. =========================
u32 read32(istream& strm);
u16 read16(istream& strm);
u8 read8(istream& strm);
void write32(ostream& strm, u32 c);
void write32s(ostream& strm, s32 c);
void write16(ostream& strm, u16 c);
void write8(ostream& strm, u8 c);

// === String-Funktionen ===================================================
// Gibt /true/ zurück, genau dann wenn /regex/ in /str/ zu finden ist.
bool match(const char* str, const char* pattern);
// Einlesen eines nullterminierten Strings aus einer Datei
string read0String(istream& strm);
// Gibt /str/ in Großbuchstaben zurück.
string upcase(const string& str);
// Entfernt aus dem Dateipfad /path/ alle Verzeichnisse, so dass nur noch die Datei übrigbleibt.
string getFilenameOnly(string path);

// Größe der durch /stream/ repräsentierten Datei feststellen.
u32 filesize(std::istream& stream);
// Legt ein Verzeichnis an, sofern es noch nicht existiert.
bool makedir(string path);
// Kopiert /bytes/ Bytes von /in/ nach /out/.
void copy(istream& in, ostream& out, u32 bytes);

#endif
