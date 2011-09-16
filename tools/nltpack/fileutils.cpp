#include "fileutils.hpp"
#include <sys/stat.h>
#include <algorithm>  // Für transform
#include <locale>     // Für toupper

#ifdef _WIN32
#include <io.h>
#endif

u32 read32(istream& strm) {
    u32 c = 0;
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    // Die Dateien sind ebenfalls mit little Endian codiert,
    // es muss also nichts gedreht werden.
    strm.read(reinterpret_cast<char*>(&c), 4);
    #elif __BYTE_ORDER == __BIG_ENDIAN
    // Die Werte müssen vor der Weiterverarbeitung gedreht werden.
    char a[4];
    strm.read(a, 4);
    std::swap(a[0], a[3]);
    std::swap(a[1], a[2]);
    c = *(reinterpret_cast<u32*>(a));
    #endif
    return c;
}

u16 read16(istream& strm) {
    u16 c = 0;
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    strm.read(reinterpret_cast<char*>(&c), 2);
    #elif __BYTE_ORDER == __BIG_ENDIAN
    char a[2];
    strm.read(a, 2);
    std::swap(a[0], a[1]);
    c = *(reinterpret_cast<u16*>(a));
    #endif
    return c;
}

u8 read8(istream& strm) {
    return strm.get();
}

void write32(ostream& strm, u32 c) {
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    strm.write(reinterpret_cast<char*>(&c), 4);
    #elif __BYTE_ORDER == __BIG_ENDIAN
    char* a = reinterpret_cast<char*>(&c);
    std::swap(a[0], a[3]);
    std::swap(a[1], a[2]);
    strm.write(a, 4);
    #endif
}

void write16(ostream& strm, u16 c) {
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    strm.write(reinterpret_cast<char*>(&c), 2);
    #elif __BYTE_ORDER == __BIG_ENDIAN
    char* a = reinterpret_cast<char*>(&c);
    std::swap(a[0], a[1]);
    strm.write(a, 2);
    #endif
}

void write8(ostream& strm, u8 c) {
    strm.put(c);
}

string read0String(istream& strm) {
    string ret = "";
    char c;
    while (!strm.eof() && (c = strm.get()) != '\0') ret += c;
    return ret;
}

string upcase(const string& str) {
    // Der Zielstring muss ausreichend Platz für den Quellstring haben.
    // So schlimme Unicode-Sachen wie ß --> SS werden nicht berücksichtigt :-(
    string ret = str;
    transform(str.begin(), str.end(), ret.begin(), toupper);
    return ret;
}

string getFilenameOnly(string path) {
    size_t idx = path.rfind("/");
    if (idx == string::npos) return path;
    else return path.substr(idx+1);
}

bool match(const char* str, const char* wild) {
    // Written by Jack Handy - jakkhandy@hotmail.com
    const char *cp = NULL, *mp = NULL;
    
    while ((*str) && (*wild != '*')) {
	if ((*wild != *str) && (*wild != '?')) {
	    return false;
	}
	wild++;
	str++;
    }
    
    while (*str) {
	if (*wild == '*') {
	    if (!*++wild) return true;
	    mp = wild;
	    cp = str+1;
	} else if ((*wild == *str) || (*wild == '?')) {
	    wild++;
	    str++;
	} else {
	    wild = mp;
	    str = cp++;
	}
    }
    
    while (*wild == '*') {
	wild++;
    }
    if (*wild == 0) return true;
    else            return false;
}

u32 filesize(std::istream& stream) {
    stream.seekg(0, std::ios::end);
    u32 size = stream.tellg();
    stream.seekg(0,std::ios::beg);
    return size;
}

bool makedir(string path) {
    struct stat dirstat;
    if (stat(path.c_str(), &dirstat) != 0) {
	// TODO: Genaueres Abfangen des Fehlers ( hier ^ und hier v)
	std::cout << "Creating module directory " << path << std::endl;
	#ifdef _WIN32
	mkdir(path.c_str());
	#else
	mkdir(path.c_str(), S_IRWXU|S_IRWXG|S_IRWXO);
	#endif
	return true;
    } else {
	return false;
    }
}

void copy(istream& in, ostream& out, u32 bytes) {
    // Precondition: Both in and out are binary streams.
    const u32 BUFSIZE = 4096;
    char buffer[BUFSIZE];
    u32 length = BUFSIZE;
    while(bytes > 0) {
	if (length > bytes) length = bytes;
	in.read(buffer, length);
	out.write(buffer, length);
	bytes -= length;
    }
}

