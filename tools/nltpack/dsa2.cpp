#include "dsa2.hpp"
#include "string.h"

extern std::ostream& cdebug;
extern std::ostream& cinfo;
extern std::ostream& cwarn;
extern std::ostream& cerror;
extern Config* config;

bool DSA2_Entry::read(istream& strm) {
    char n[14];
    no_dummy = read16(strm);
    strm.read(n, 14);
    name   = n;
    offset = read32(strm);
    return true;
}

void DSA2_Entry::write(ostream& strm) {
    write16(strm, no_dummy);
    char n[14];
    strncpy(n, name.c_str(), 14);
    strm.write(n, 14);
    write32(strm, offset);
}

bool DSA2::read(istream& strm) {
    file_size = filesize(strm);
    count = read16(strm);
    header_size = 2 + count * 20;
    DSA2_Entry* oldentry = 0;
    for (u32 i=0;  i<count;  i++) {
	DSA2_Entry* entry = new DSA2_Entry;
	entry->read(strm);
	entries.push_back(entry);
	if (!entry->no_dummy) {
	    entry->size = 0;
	    continue;
	}
	if (oldentry) oldentry->size = entry->offset-oldentry->offset;
	oldentry = entry;
    }
    assert(file_size >= oldentry->offset);
    oldentry->size = file_size-oldentry->offset;
    return true;
}

void DSA2::write(ostream& strm) {
    count = entries.size();
    header_size = 2 + count * 20;
    write16(strm, count);
    u32 offset = 0;
    for(FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	DSA2_Entry* entry = dynamic_cast<DSA2_Entry*>(*f);
	assert(entry != 0);
	entry->offset = offset;
	entry->write(strm);
	if (entry->no_dummy) {
	    offset += entry->size;
	}
    }
}

bool DSA2::readMaster(string filename) {
    // Öffnen der passenden .FN-Datei
    string fn_name = filename.substr(0, filename.length()-3)+"FN";
    std::ifstream fnfile(fn_name.c_str(), std::ios::binary);
    if (!fnfile.good()) {
	cerror << "Matching FN file (" <<fn_name<< ") could not be opened.\n";
	exit(-1);
    }
    
    char buf[14];
    entries.clear();
    while (!fnfile.eof()) {
	fnfile.read(buf, 14);
	if (fnfile.eof()) break;
	DSA2_Entry* entry = new DSA2_Entry;
	entries.push_back(entry);
	entry->name = buf;
    }
    fnfile.close();
    return true;
}

void DSA2::writeMaster(string filename) {
    // Öffnen der passenden .FN-Datei
    string fn_name = filename.substr(0, filename.length()-3)+"FN";
    std::ofstream fnfile(fn_name.c_str(), std::ios::binary);
    if (!fnfile.good()) {
	cerror << "Matching FN file (" <<fn_name<< ") could not be opened.\n";
	exit(-1);
    }

    for (FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	fnfile.write((*f)->name.c_str(), 14);
    }
    fnfile.close();
}

void DSA2::synchronize(CmdlineList& flist) {
    // Prüfen, ob die Einträge in der .FN-Datei und von der Kommandozeile übereinstimmen
    for(FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	DSA2_Entry* entry = dynamic_cast<DSA2_Entry*>(*f);
	assert(entry != 0);
	const CmdlineList::iterator search = flist.find(entry->name);
	if (search == flist.end()) {
	    entry->no_dummy = 0;
	    entry->size = entry->offset = 0;
	    std::cerr << "File " << entry->name << " is missing, will be looked up on CD.\n";
	} else {
	    entry->no_dummy = 1;
	    search->second.checked = true;
	    entry->size = search->second.size;
	}
    }
    for (CmdlineList::iterator f = flist.begin(); f != flist.end(); f++) {
	if (! f->second.checked ) {
	    std::cerr << "Warning: File »" << f->first << "« not part of archive; will be ignored.\n";
	}
    }
}
