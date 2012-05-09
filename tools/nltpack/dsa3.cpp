#include "dsa3.hpp"
#include "string.h"
#include <algorithm>
#include <cctype>
#include <iomanip>

extern std::ostream& cdebug;
extern std::ostream& cinfo;
extern std::ostream& cwarn;
extern std::ostream& cerror;
extern Config* config;

DSA3::~DSA3() {
    for (u32 i=0; i<modules.size(); i++) delete modules[i];
}

bool DSA3::read(istream& strm) {
    file_size          = filesize(strm);
    header_size        = 48; // veraltet, siehe unten
    strm.read(signature, 4);
    strm.read(reinterpret_cast<char*>(version), 4);
    filetable_size     = read16(strm);
    filetable_offset   = read32(strm);
    count              = read16(strm);
    data_offset        = read32(strm);
    moduletable_size   = read16(strm);
    moduletable_offset = read32(strm);
    module_count       = read16(strm);
    modmap_offset      = read32(strm);
    strm.read(reinterpret_cast<char*>(unknown1), 16);
    header_size        = data_offset;
    
    strm.seekg(moduletable_offset, std::ios::beg);

    // Einlesen der Modultabelle
    modmap_size = 0;
    for (u32 i=0;  i < module_count;  i++) {
	DSA3_ModuleEntry* module = new DSA3_ModuleEntry;
	module->read(strm);
	modules.push_back(module);
	if ( (module->offset / 2 + module->size) > modmap_size ) {
	    modmap_size = (module->offset / 2) + module->size;
	}
    }

    // Einlesen der Dateitabelle
    strm.seekg(filetable_offset, std::ios::beg);
    for (u32 i=0;  i < count;  i++) {
	DSA3_Entry* entry = new DSA3_Entry;
	entry->read(strm);
	entries.push_back(entry);
    }

    // Einlesen der Modul-Datei-Zuordnungen
    strm.seekg(modmap_offset, std::ios::beg);
    for (u32 i=0; i<modmap_size; i++) {
	modmap.push_back(read16(strm));
    }

    // Den Modulen die Dateien zuordnen
    for (u32 i=0;  i < module_count;  i++) {
	for (u32 j=0; j < modules[i]->size; j++) {
	    u16 entry_index = modmap[ modules[i]->offset / 2 + j ];
	    if (entry_index == 0xFFFF) continue;
	    modules[i]->entry_indices.push_back(entry_index);
	    DSA3_Entry* entry = dynamic_cast<DSA3_Entry*>(entries[entry_index]);
	    assert(entry != 0);
	    assert(modules[i]->offset / 2 + j  <  modmap.size());
	}
    }
    return true;
}

void DSA3::write(ostream& strm) {
    // Ermitteln der benötigten Größen und Offsets
    header_size        = 48; // Nur der obligatorische Header.
    initArray(signature,4, 'A','L','F',' ');
    initArray(version,4,    0,0,1,0);
    initArray(unknown1,16,  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);
    
    filetable_size     = count = entries.size();
    filetable_size    += 0x10 - (filetable_size % 0x10);
    moduletable_size   = module_count = modules.size();
    moduletable_size  += 0x10 - (moduletable_size % 0x10);
    modmap_size        = modmap.size();

    // Größe des Archives bestimmen
    u32 e_offset = 0;
    for (u32 i=0;  i < count;  i++) {
	entries[i]->offset = e_offset;
	e_offset += entries[i]->size;
    }

    filetable_offset   = e_offset + header_size;
    moduletable_offset = filetable_offset   + filetable_size*28;
    modmap_offset      = moduletable_offset + moduletable_size*28;
    data_offset        = header_size;
    
    cinfo << "writing " <<modmap_size<< " entries in " <<module_count<< " modules\n";
    // Schreiben der Daten
    strm.seekp(0, std::ios::beg);
    strm.write(signature, 4);
    strm.write(reinterpret_cast<char*>(version), 4);
    write16(strm, filetable_size);
    write32(strm, filetable_offset);
    write16(strm, count);
    write32(strm, data_offset);
    write16(strm, moduletable_size);
    write32(strm, moduletable_offset);
    write16(strm, module_count);
    write32(strm, modmap_offset);
    strm.write(reinterpret_cast<char*>(unknown1), 16);


    // Dateitabelle
    strm.seekp(filetable_offset, std::ios::beg);
    u32 offset = 0;
    for(FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	// TODO: Du bestimmst das Offset doppelt.
	DSA3_Entry* entry = dynamic_cast<DSA3_Entry*>(*f);
	assert(entry != 0);
	entry->offset = offset;
	entry->write(strm);
	offset += entry->size;
    }

    // Modultabelle
    strm.seekp(moduletable_offset, std::ios::beg);
    for (u32 i=0;  i < module_count;  i++) {
	modules[i]->write(strm);
    }
    for (u32 i=module_count; i<moduletable_size; i++) for (u32 j=0; j<28;j++) write8(strm, 0);

    // Modul-Datei-Zuordnungstabelle (modmap)
    strm.seekp(modmap_offset, std::ios::beg);
    for (u32 i=0;  i < modmap_size;  i++) {
	write16(strm, modmap[i]);
    }
}

void DSA3::synchronize(CmdlineList& flist) {
    std::map<string, DSA3_ModuleEntry*> modulemap;

    for (u32 i=0;  i < module_count;  i++) {
	DSA3_ModuleEntry* module = modules[i];
	module->unknown1 = module->unknown2 = 0;
	for (u32 j=0; j < module->size; j++) {
	    u32 entry_index = modmap[ module->offset / 2 + j];
	    if (entry_index == 0xFFFF) continue;
	    DSA3_Entry* entry = dynamic_cast<DSA3_Entry*>(entries[entry_index]);
	    assert(entry != 0);

	    string outname = module->name + config->pathDelim + entry->name;
	    const CmdlineList::iterator f_iter = flist.find(outname);
	    if (f_iter == flist.end()) {
		cerror << "File " <<outname<< " is missing, archive creation impossible.\n";
		exit(-1);
	    } else {
		f_iter->second.checked = true;
		entry->size = f_iter->second.size;
		entry->unknown1 = 0;
		entry->unknown2 = 0;
		entry->unknown3 = 0;
	    }
	}
    }
}

bool DSA3::readMaster(string filename) {
    // Öffnen der passenden .FN-Datei
    string fn_name = filename.substr(0, filename.length()-3)+"FN";
    std::ifstream fnfile(fn_name.c_str(), std::ios::binary);

    if (!fnfile.good()) {
	cerror << "Matching FN file (" <<fn_name<< ") could not be opened.\n";
	exit(-1);
    }

    u32 modoffs = 0;
    string str;
    entries.clear();
    modules.clear();
    module_count = 0;

    while(!fnfile.eof()) {
	str = read0String(fnfile);
	if (fnfile.eof()) break;
	assert(str[0] == '#');
	DSA3_ModuleEntry* module = new DSA3_ModuleEntry;
	module->name = str.substr(1);
	module->size   = read32(fnfile);
	module->offset = modoffs * 2;
	modules.push_back(module);
	module_count++;

	for (u32 j=0; j < module->size; j++) {
	    str = read0String(fnfile);
	    if (str == "$DUMMY") {
		modmap.push_back(0xFFFF);
	    } else {
		DSA3_Entry* entry = new DSA3_Entry;
		entry->name = str;
		u16 entry_idx = entries.size();
		entries.push_back(entry);
		module->entry_indices.push_back(entry_idx);
		modmap.push_back(entry_idx);
	    }
	    modoffs++;
	}
    }
    fnfile.close();
    
    return true;
}

void DSA3::writeMaster(string filename) {
    // Öffnen der passenden .FN-Datei
    string fn_name = filename.substr(0, filename.length()-3)+"FN";
    std::ofstream fnfile(fn_name.c_str(), std::ios::binary);
    if (!fnfile.good()) {
	cerror << "Matching FN file (" <<fn_name<< ") could not be opened.\n";
	exit(-1);
    }

    for (u32 i=0;  i < module_count;  i++) {
	fnfile << "#" << modules[i]->name << '\0';
	write32(fnfile, modules[i]->size);
	for (u32 j=0; j < modules[i]->size; j++) {
	    u16 entry_index = modmap[ modules[i]->offset / 2 + j];
	    if (entry_index == 0xFFFF) {
		fnfile << "$DUMMY" << '\0';
	    } else {
		DSA3_Entry* entry = dynamic_cast<DSA3_Entry*>(entries[entry_index]);
		assert(entry != 0);
		fnfile << entry->name << '\0';
	    }
	}
    }
    fnfile.close();
}

bool DSA3_ModuleEntry::read(istream& strm) {
    char n[14];
    strm.read(n, 14);
    name     = n;
    size     = read32(strm);
    unknown1 = read32(strm);
    unknown2 = read16(strm);
    offset   = read32(strm);
    return true;
}

void DSA3_ModuleEntry::write(ostream& strm) {
    char n[14];
    strncpy(n, name.c_str(), 14);
    strm.write(n, 14);
    write32(strm, size);
    write32(strm, unknown1);
    write16(strm, unknown2);
    write32(strm, offset);
}

bool DSA3_Entry::read(istream& strm) {
    char n[13];
    strm.read(n, 13);
    name     = n;
    unknown1 = read8(strm);
    size     = read32(strm);
    unknown2 = read32(strm);
    unknown3 = read16(strm);
    offset   = read32(strm);
    return true;
}

void DSA3_Entry::write(ostream& strm) {
    char n[13];
    strncpy(n, name.c_str(), 13);
    strm.write(n, 13);
    write8(strm, unknown1);
    write32(strm, size);
    write32(strm, unknown2);
    write16(strm, unknown3);
    write32(strm, offset);
}

void DSA3::glob(string pattern) {
    for (u32 i=0;  i < module_count;  i++) {
	DSA3_ModuleEntry* module = modules[i];
	for (u32 j=0; j < modules[i]->size; j++) {
	    u32 entry_index = modmap[ modules[i]->offset / 2 + j];
	    if (entry_index == 0xFFFF) continue;
	    assert(entry_index < entries.size());

	    Header_Entry* entry = entries[entry_index];
	    string outname = module->name + config->pathDelim + entry->name;
	    if (! match(outname.c_str(), pattern.c_str())) {
		modmap[ modules[i]->offset / 2 + j] = 0xFFFF;
	    }
	}
    }
}

errcode DSA3::a_list(Config* config) {
    std::ifstream ifs(config->archive_name.c_str(), std::ios::binary);
    if (!ifs.good()) {
	cerror << "could not read archive file " << config->archive_name << std::endl;
	return ERR_EXIT;
    }
    read(ifs);
    std::cout << "listing contents of " << type() << " archive " << config->archive_name << ":\n";
    glob(config->file_pattern);
    for (u32 i=0;  i < module_count;  i++) {
	DSA3_ModuleEntry* module = modules[i];
	for (u32 j=0; j < modules[i]->size; j++) {
	    u32 entry_index = modmap[ modules[i]->offset / 2 + j];
	    if (entry_index == 0xFFFF) continue;
	    assert(entry_index < entries.size());

	    Header_Entry* entry = entries[entry_index];
	    std::cout << std::setw(27) << std::setfill(' ')
		      << module->name << config->pathDelim << entry->name
		      << " | " << entry->size << std::endl;
	}
    }
    ifs.close();
    return ERR_OK;
}

errcode DSA3::a_extract(Config* config) {
    std::ifstream ifs(config->archive_name.c_str(), std::ios::binary);
    if (!ifs.good()) {
	cerror << "could not read archive file " << config->archive_name << std::endl;
	return ERR_EXIT;
    }
    read(ifs);
    glob(config->file_pattern);
    
    for (u32 i=0;  i < module_count;  i++) {
	DSA3_ModuleEntry* module = modules[i];
	makedir(module->name.c_str());

	for (u32 j=0; j < modules[i]->size; j++) {
	    u32 entry_index = modmap[ modules[i]->offset / 2 + j];
	    if (entry_index == 0xFFFF) continue;
	    Header_Entry* entry = entries[entry_index];
	    string outname = module->name + config->pathDelim + entry->name;
	    std::ofstream ofs(outname.c_str(), std::ios::binary);
	    if (ofs.good()) {
		ifs.seekg(entry->offset + header_size);
		copy(ifs, ofs, entry->size);
		ofs.close();
	    } else {
		std::cerr << "Fehler beim Schreiben der Datei " << outname << std::endl;
	    }
	}
    }
    ifs.close();
    return ERR_OK;
}

errcode DSA3::a_create(Config* config) {
    std::ofstream ofs(config->archive_name.c_str(), std::ios::binary);
    if (!ofs.good()) {
	cerror << "could not write archive file " << config->archive_name << std::endl;
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
    write(ofs);
    
    // Dateien schreiben
    for (u32 i=0;  i < module_count;  i++) {
	DSA3_ModuleEntry* module = modules[i];
	makedir(module->name.c_str());

	for (u32 j=0; j < modules[i]->size; j++) {
	    u32 entry_index = modmap[ modules[i]->offset / 2 + j];
	    if (entry_index == 0xFFFF) continue;
	    Header_Entry* entry = entries[entry_index];
	    string outname = module->name + config->pathDelim + entry->name;
	    Cmdline_Entry cmde = config->fileList.find(outname)->second;
	    std::ifstream ifs(cmde.name.c_str(), std::ios::binary);
	    if (ifs.good()) {
		ifs.seekg(0);
		ofs.seekp(entry->offset + header_size);
		copy(ifs, ofs, entry->size);
	    } else {
		std::cerr << "Error reading file »" << cmde.name <<"«"<< std::endl;
		exit(0);
	    }
	    ifs.close();
	}
    }
    ofs.close();
    return ERR_OK;
}
