#include "dsa1.hpp"

extern std::ostream& cdebug;
extern std::ostream& cinfo;
extern std::ostream& cwarn;
extern std::ostream& cerror;
extern Config* config;

/**** DSA1: SCHICK.DAT / BLADE.DAT ****************************************/
bool DSA1Main::read(istream& strm) {
    if (config->mode == 'm') return true;
    readMaster(config->archive_name);
    file_size   = filesize(strm);
    header_size = 308*4;
    DSA1Main_Entry* oldentry = 0;
    for (u32 i=0; i<308; i++) {
	DSA1Main_Entry* entry = dynamic_cast<DSA1Main_Entry*>(entries[i]);
	assert(entry != 0);
	entry->read(strm);
	if (oldentry) oldentry->size = entry->offset - oldentry->offset;
	// Hat outro.xmi einen ungültigen Offset, geht die Datei davor zum Archivende.
	if (i == 307 && entry->offset == 0) {
	    oldentry->size = file_size - oldentry->offset;
	}
	oldentry = entry;
    }
    // Hat outro.xmi (wie im Originalarchiv) einen ungültigen Offset, wird sie
    // als leer ausgegeben. Ansonsten wird sie wie jede normale Datei behandelt.
    if (oldentry->offset == 0) oldentry->size = 0;
    else                       oldentry->size = file_size - oldentry->offset;
    return true;
}
void DSA1Main::write(ostream& strm) {
    header_size = 308*4;
    u32 offset = header_size;
    assert(entries.size() == 308);
    for(FileList::iterator e = entries.begin() ; e != entries.end(); e++) {
	(*e)->offset = offset;
	(*e)->write(strm);
	offset += (*e)->size;
    }
}

void DSA1Main::synchronize(CmdlineList& flist) {
    vector<Header_Entry*>::iterator f;
    for (f = entries.begin(); f != entries.end(); f++) {
	const CmdlineList::iterator f_iter = flist.find((*f)->name);
	if (f_iter == flist.end()) {
	    cerror << "File " <<(*f)->name<< " is missing, archive creation impossible.\n";
	    exit(-1);
	} else {
	    f_iter->second.checked = true;
	    (*f)->size = f_iter->second.size;
	}
    }

    for (CmdlineList::iterator f = flist.begin(); f != flist.end(); f++) {
	if (! f->second.checked ) {
	    cwarn << "Warning: File »" << f->first << "« not part of archive; will be ignored.\n";
	}
    }
}

bool DSA1Main::readMaster(string filename) {
    // Öffnen der passenden .FN-Datei
    string fn_name = filename.substr(0, filename.length()-3)+"FN";
    std::ifstream fnfile(fn_name.c_str(), std::ios::binary);
    if (!fnfile.good()) {
	cerror << "Matching FN file (" <<fn_name<<") could not be opened.\n";
	exit(-1);
    }
    
    entries.clear();
    while(true) {
	string str = read0String(fnfile);
	if (fnfile.eof()) break;
	DSA1Main_Entry* entry = new DSA1Main_Entry;
	entry->name = str;
	entries.push_back(entry);
    }
    fnfile.close();
    return true;
}

void DSA1Main::writeMaster(string filename) {
    // Öffnen der passenden .FN-Datei
    string fn_name = filename.substr(0, filename.length()-3)+"FN";
    std::ofstream fnfile(fn_name.c_str(), std::ios::binary);
    if (!fnfile.good()) {
	cerror << "Matching FN file (" <<fn_name<<") could not be opened.\n";
	exit(-1);
    }

    for (u32 i = 0; i < 308; i++) {
	fnfile << schickdat_filenames[i] << '\0';
    }
    fnfile.close();
}

bool DSA1Main_Entry::read(istream& strm) {
    offset = read32(strm);
    return true;
}
void DSA1Main_Entry::write(ostream& strm) {
    write32(strm, offset);
}

/**** DSA1: DSAGEN.DAT ****************************************************/

bool DSA1Gen::read(istream& strm) {
    file_size   = filesize(strm);
    header_size = 0x33*16;
    DSA1Gen_Entry* oldentry = 0;
    for (u32 i=0; i<0x33; i++) {
	DSA1Gen_Entry* entry = new DSA1Gen_Entry;
	entry->read(strm);
	if (entry->offset == 0) continue;
	entries.push_back(entry);
	if (oldentry) oldentry->size = entry->offset - oldentry->offset;
	oldentry = entry;
    }
    oldentry->size = file_size - oldentry->offset;
    return true;
}
void DSA1Gen::write(ostream& strm) {
    header_size = 0x33*16;
    u32 offset = header_size;
    assert(entries.size() <= 0x33);
    for (u32 i=0; i<0x33; i++) {
	if (i < entries.size()) {
	    entries[i]->offset = offset;
	    entries[i]->write(strm);
	    offset += entries[i]->size;
	} else {
	    // Leerer Eintrag: 16 0-Bytes
	    for (u8 j=0; j<4; j++) write32(strm, 0);
	}
    }
}

void DSA1Gen::synchronize(CmdlineList& flist) {
    // Prüfen, ob die Einträge in der .FN-Datei und von der Kommandozeile übereinstimmen
    for(FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	DSA1Gen_Entry* entry = dynamic_cast<DSA1Gen_Entry*>(*f);
	assert(entry != 0);
	const CmdlineList::iterator search = flist.find(entry->name);
	if (search == flist.end()) {
	    cerror << "File " <<entry->name<< " is missing, cannot write archive.\n";
	    exit(-1);
	} else {
	    search->second.checked = true;
	    entry->size = search->second.size;
	}
    }
    for (CmdlineList::iterator f = flist.begin(); f != flist.end(); f++) {
	if (! f->second.checked ) {
	    cwarn << "Warning: File »" << f->first << "« not part of archive; will be ignored.\n";
	}
    }
}

bool DSA1Gen_Entry::read(istream& strm) {
    char n[13];
    strm.read(n, 12);
    n[12] = 0;
    name   = n;
    offset = read32(strm);
    return true;
}
void DSA1Gen_Entry::write(ostream& strm) {
    strm.write(name.c_str(), 12);
    write32(strm, offset);
}

/**** DSA1: INTRODAT.DIR & INTRODAT.VOL (TODO) ****************************/

bool DSA1Intro::read(istream& strm) {
    header_size = 20;
    file_size   = filesize(strm);
    count       = read16(strm);
    // TODO: Was bedeutet der Rest des Headers?
    strm.seekg(0x20);
    while(strm.tellg() < file_size) {
	DSA1Intro_Entry* entry = new DSA1Intro_Entry;
	if (!entry->read(strm)) {
	    entry->name    = "DUMMY";
	    entry->size    = 0;
	    entry->volname = "DUMMY";
	    entry->offset  = 0;
	    entries.push_back(entry);
	    //delete entry;
	} else {
	    // Nach Duplikaten suchen und diese entfernen
	    /*for (u32 j=0; j<entries.size(); j++) {
		if (entry->name == entries[j]->name) {
		    delete entry;
		    entry = 0;
		    break;
		}
		}*/
	    if (entry == 0) continue;
	    else entries.push_back(entry);
	}
    }
    //assert(count == entries.size());
    return true;
 }
void DSA1Intro::write(ostream& strm) {
    header_size = 20;
    count = entries.size(); // WRONG! Come back later to change it
    u16 truecount = 0;
    write16(strm, count);
    for (u8 i=0; i<0x20-2; i++) write8(strm, 0);

    u32 offset = 0;
    for (u32 i=0; i<count; i++) {
	// Nach Duplikaten suchen und diese markieren
	u32 j = 0;
	for ( ; j<i; j++) {
	    if (entries[i]->name == entries[j]->name) break;
	}
	if (j == i) { // Kein Duplikat gefunden
	    entries[i]->offset = offset;
	    entries[i]->write(strm);
	    offset += entries[i]->size;
	    truecount++;
	} else {      // Duplikat gefunden
	    entries[i]->offset = entries[j]->offset;
	    assert(entries[i]->size == entries[j]->size);
	    entries[i]->write(strm);
	}
    }
    for (u32 i=count; i < 139 ; i++) {
	for (u32 j=0; j < 0x20; j++) write8(strm, 0x00);
    }
    strm.seekp(0);
    write16(strm, truecount-1); // DUMMY nicht mitzählen
}

void DSA1Intro::synchronize(CmdlineList& flist) {
    for(FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	DSA1Intro_Entry* entry = dynamic_cast<DSA1Intro_Entry*>(*f);
	assert(entry != 0);

	const CmdlineList::iterator search = flist.find(entry->name);
	if (search == flist.end()) {
	    cerror << "File " <<entry->name<< " is missing, cannot write archive.\n";
	    exit(-1);
	} else {
	    search->second.checked = true;
	    entry->size = search->second.size;
	    #ifdef _WIN32
	    entry->volname = config->archive_name.substr(0, config->archive_name.length()-3)+"VOL";
	    #else
	    if (config->archive_name.substr(config->archive_name.length()-3) == "DIR") {
		entry->volname = config->archive_name.substr(0, config->archive_name.length()-3)+"VOL";
	    } else if (config->archive_name.substr(config->archive_name.length()-3) == "dir") {
		entry->volname = config->archive_name.substr(0, config->archive_name.length()-3)+"vol";
	    } else {
		cwarn << "Warning: Problems extrapolating exact introdat.vol Filename case.\n";
		entry->volname = config->archive_name.substr(0, config->archive_name.length()-3)+"vol";
	    }
	    #endif
	}
    }
    for (CmdlineList::iterator f = flist.begin(); f != flist.end(); f++) {
	if (! f->second.checked ) {
	    cwarn << "Warning: File »" << f->first << "« not part of archive; will be ignored.\n";
	}
    }
}

bool DSA1Intro_Entry::read(istream& strm) {
    u8 length;
    char buf[13];
    length = read8(strm);
    if (length < 1 || length > 12) { // Eintrag überspringen
	strm.seekg(0x20-1, std::ios::cur);
	return false;
    }
    strm.read(buf, 12);
    buf[length] = '\0';
    name    = buf;
    length  = read8(strm);
    if (length < 1 || length > 12) { // Eintrag überspringen
	strm.seekg(0x20-14, std::ios::cur);
	return false;
    }
    strm.read(buf, 12);
    buf[length] = '\0';
    volname = buf;
    offset  = read32(strm);
    size    = read16(strm);
    return true;
}

void DSA1Intro_Entry::write(ostream& strm) {
    assert(name.length() <= 12 && volname.length() <= 12);
    write8(strm, name.length());
    strm.write(name.c_str(),    12);
    write8(strm, volname.length());
    strm.write(volname.c_str(), 12);
    write32(strm, offset);
    write16(strm, size);
}

errcode DSA1Intro::a_extract(Config* config) {
    std::ifstream ifsdir(config->archive_name.c_str(), std::ios::binary);
    if (!ifsdir.good()) {
	cerror << "could not read archive file " << config->archive_name << std::endl;
	return ERR_EXIT;
    }
    read(ifsdir);
    glob(config->file_pattern);
    ifsdir.close();
    
    string vol_name;
    #ifdef _WIN32
    vol_name = config->archive_name.substr(0, config->archive_name.length()-3)+"VOL";
    #else
    if (config->archive_name.substr(config->archive_name.length()-3) == "DIR") {
	vol_name = config->archive_name.substr(0, config->archive_name.length()-3)+"VOL";
    } else if (config->archive_name.substr(config->archive_name.length()-3) == "dir") {
	vol_name = config->archive_name.substr(0, config->archive_name.length()-3)+"vol";
    } else {
	cwarn << "Warning: Problems extrapolating exact introdat.vol Filename case.\n";
	vol_name = config->archive_name.substr(0, config->archive_name.length()-3)+"vol";
    }
    #endif
    for(FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	DSA1Intro_Entry* entry = dynamic_cast<DSA1Intro_Entry*>(*f);
	assert(entry != 0);
	// Datei schreiben.
	string outname = entry->name;
	// TODO: Es wird nicht der Original-Volname verwendet, da es damit zu
	// CASE-bedingten Problemen kommt (introdat.vol != INTRODAT.VOL).
	//vol_name = entry->volname;
	std::ifstream ifs(vol_name.c_str(), std::ios::binary);
	if (!ifs.good()) {
	    std::cerr << "Fehler beim Lesen des Volumes " << vol_name << std::endl;
	    continue;
	}
	// TODO: Fehler abfangen (Datei existiert, Datenträger voll/nur lesbar o.ä.)
	std::ofstream ofs(outname.c_str(), std::ios::binary);
	if (ofs.good()) {
	    ifs.seekg((*f)->offset);
	    copy(ifs, ofs, (*f)->size);
	    ifs.close();
	    ofs.close();
	} else {
	    std::cerr << "Fehler beim Schreiben der Datei " << entry->name << std::endl;
	}
    }
    return ERR_OK;
}
errcode DSA1Intro::a_create(Config* config) {
    std::ofstream ofs_dir(config->archive_name.c_str(), std::ios::binary);
    if (!ofs_dir.good()) {
	cerror << "could not write archive directory '" << config->archive_name << "'" << std::endl;
	exit(1);
    }
    
    // Dateigrößen feststellen
    CmdlineList::iterator f = config->fileList.begin();
    while (f != config->fileList.end()) {
	std::ifstream ifs(f->second.name.c_str(), std::ios::binary);
	if (ifs.good()) {
	    f->second.size = filesize(ifs);
	    f++;
	} else {
	    std::cerr << "Warning: Could not open "<< f->second.name << std::endl;
	    CmdlineList::iterator temp = f; temp++;
	    config->fileList.erase(f);
	    f = temp;
	}
	ifs.close();
    }

    // Header schreiben
    readMaster(config->archive_name);
    synchronize(config->fileList);
    write(ofs_dir);
    ofs_dir.close();
    
    // Dateien schreiben
    for(FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	DSA1Intro_Entry* entry = dynamic_cast<DSA1Intro_Entry*>(*f);
	assert(entry != 0);
	Cmdline_Entry cmde = config->fileList.find(entry->name)->second;

	std::ifstream ifs(cmde.name.c_str(), std::ios::binary);
	std::fstream ofs;
	string volname = "";
	if (ifs.good()) {
	    /*if (volname != entry->volname) {
		if (ofs.is_open()) ofs.close();
		ofs.open(entry->volname.c_str(), std::ios::binary);
		if (!ofs.good()) {
		    cerror << "could not write archive volume '" << config->archive_name << "'" << std::endl;
		    exit(-1);
		}
		}*/
	    // TODO: Dieser Modus (app) bereitet Schwierigkeiten beim Überschreiben.
	    ofs.open(entry->volname.c_str(), std::ios::binary | std::ios::out | std::ios::in);
		if (!ofs.good()) {
		    cerror << "could not write archive volume '" << config->archive_name << "'" << std::endl;
		    exit(-1);
		}
	    ifs.seekg(0);
	    ofs.seekp(entry->offset, std::ios::beg);
	    std::cout << "writing from " << cmde.name << ":"<<ifs.tellg()<< " to "
		      << entry->volname << ":"<<entry->offset<<" ["<<entry->size <<"]"<< std::endl;
	    copy(ifs, ofs, entry->size);
	    ofs.close();
	} else {
	    std::cerr << "Error reading file »" << cmde.name <<"«"<< std::endl;
	    exit(0);
	}
	ofs.close();
	ifs.close();
    }
    return ERR_OK;
}
