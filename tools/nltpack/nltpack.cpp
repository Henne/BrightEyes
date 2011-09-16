#include "nltpack.hpp"
#include "dsa1.hpp"
#include "dsa2.hpp"
#include "dsa3.hpp"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <iomanip>

/* =================== Globale Variablen ============================ */
std::ostream& cdebug = std::cout;
std::ostream& cinfo  = std::cout;
std::ostream& cwarn  = std::cout;
std::ostream& cerror = std::cerr;
Config* config;


Header::~Header() {
    for (u32 i=0; i<entries.size(); i++) delete entries[i];
}

void Header::glob(string pattern) {
    FileList::iterator f = entries.begin();
    while (f != entries.end()) {
	if (! match((*f)->name.c_str(), pattern.c_str())) {
	    f = entries.erase(f);
	} else {
	    f++;
	}
    }
}

errcode Header::a_list(Config* config) {
    std::ifstream ifs(config->archive_name.c_str(), std::ios::binary);
    if (!ifs.good()) {
	cerror << "could not read archive file " << config->archive_name << std::endl;
	return ERR_EXIT;
    }
    read(ifs);
    std::cout << "listing contents of " << type() << " archive " << config->archive_name << ":\n";
    glob(config->file_pattern);
    for(FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	std::cout << std::setw(16) << std::setfill(' ') << (*f)->name
		  << " | " << (*f)->size << std::endl;
    }
    ifs.close();
    return ERR_OK;
}

errcode Header::a_extract(Config* config) {
    std::ifstream ifs(config->archive_name.c_str(), std::ios::binary);
    if (!ifs.good()) {
	cerror << "could not read archive file " << config->archive_name << std::endl;
	return ERR_EXIT;
    }
    read(ifs);
    glob(config->file_pattern);

    for(FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	// TODO: Fehler abfangen (Datei existiert, Datenträger voll/nur lesbar o.ä.)
	// DSA2-Dummy-Dateien müssen nicht mit ausgepackt werden.
	// TODO: Diese Feature über einen Switch steuerbar machen.
	DSA2_Entry* dsa2entry = dynamic_cast<DSA2_Entry*>(*f);
	if (dsa2entry != 0  &&  dsa2entry->no_dummy == 0) continue;
	
	// Datei schreiben.
	string outname = (*f)->name;
	std::ofstream ofs(outname.c_str(), std::ios::binary);
	if (ofs.good()) {
	    if (type().substr(0,4) == "DSA1") {
		// TODO: Vorsicht bei den noch unimplementierten DSA1-Archiven!
		ifs.seekg((*f)->offset);
	    } else {
		ifs.seekg((*f)->offset + header_size);
	    }
	    copy(ifs, ofs, (*f)->size);
	    ofs.close();
	} else {
	    std::cerr << "Fehler beim Schreiben der Datei " << (*f)->name << std::endl;
	}
    }
    ifs.close();
    return ERR_OK;
}

errcode Header::a_create(Config* config) {
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
    for(FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	DSA2_Entry* dsa2entry = dynamic_cast<DSA2_Entry*>(*f);
	if (dsa2entry != 0  &&  dsa2entry->no_dummy == 0) continue;
	Cmdline_Entry cmde = config->fileList.find((*f)->name)->second;

	std::ifstream ifs(cmde.name.c_str(), std::ios::binary);
	if (ifs.good()) {
	    ifs.seekg(0);
	    if (type().substr(0,4) == "DSA1") {
		ofs.seekp((*f)->offset);
	    } else {
		ofs.seekp((*f)->offset + header_size);
	    }
	    copy(ifs, ofs, (*f)->size);
	} else {
	    std::cerr << "Error reading file »" << cmde.name <<"«"<< std::endl;
	    exit(0);
	}
	ifs.close();
    }
    ofs.close();
    return ERR_OK;
}

errcode Header::a_update(Config* config) {
    // TODO
    cerror << "Error: Updates are not implemented yet." << std::endl;
    return ERR_EXIT;
}

errcode Header::a_createMaster(Config* config) {
    std::ifstream ifs(config->archive_name.c_str(), std::ios::binary);
    if (!ifs.good()) {
	cerror << "could not read archive file " << config->archive_name << std::endl;
	exit(-1);
    }
    read(ifs);
    glob(config->file_pattern);
    
    // TODO: Überschreibschutz
    writeMaster(config->archive_name);
    return ERR_OK;
}

bool Header::readMaster(string filename) {
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
	// Erzeugen eines passenden Entry-Typs mit der Factory
	Header_Entry* entry = newHeaderEntry();
	entries.push_back(entry);
	entry->name = buf;
    }
    fnfile.close();
    return true;
}

void Header::writeMaster(string filename) {
    // Öffnen der passenden .FN-Datei
    string fn_name = filename.substr(0, filename.length()-3)+"FN";
    std::ofstream fnfile(fn_name.c_str(), std::ios::binary);
    if (!fnfile.good()) {
	cerror << "Matching FN file (" <<fn_name<<") could not be opened.\n";
	exit(-1);
    }

    for (FileList::iterator f = entries.begin(); f != entries.end(); f++) {
	fnfile.write((*f)->name.c_str(), 14);
    }
    fnfile.close();
}

Config* parseCommandline(int argc, char* argv[]) {
    Config* config = new Config;
    config->file_pattern = "*";
    config->mode = argv[1][0];
    if (config->mode!='l' && config->mode!='x' && config->mode!='c'
	&& config->mode!='u' && config->mode!='m') {
	cerror << "Unknown mode: " << argv[1] << std::endl;
	exit(1);
    }
    
    for (int i=2; i<argc; i++) {
	if (argv[i][0] == '-') { // Optionen auswerten
	    if (!strcmp(argv[i], "-p")) { // Datei-Pattern
		config->file_pattern = argv[++i];
	    }
	} else {
	    if (config->archive_name == "") {
		config->archive_name = argv[i];
	    } else {
		Cmdline_Entry entry;
		entry.name    = argv[i];
		entry.checked = false;
		//config->fileList.insert(std::make_pair(getFilenameOnly(entry.name), entry));
		// TODO: Das muss auch besser gehen.
		config->fileList.insert(std::make_pair(entry.name, entry));
	    }
	}
    }
    string filename = upcase(getFilenameOnly(config->archive_name));
    if (filename == "SCHICK.DAT" || filename == "BLADE.DAT") {
	config->header = new DSA1Main;
    } else if (filename == "DSAGEN.DAT") {
	config->header = new DSA1Gen;
    } else if (filename == "INTRODAT.DIR") {
	config->header = new DSA1Intro;
    } else if (!filename.compare(filename.length()-4, 4, ".DAT")) {
	config->header = new DSA2;
    } else if (!filename.compare(filename.length()-4, 4, ".ALF")) {
	config->header = new DSA3;
    } else {
	cerror << "unknown archive file." << std::endl;
	exit(1);
    }

    // TODO: Geschwätzigkeits-Switches
    //cdebug = std::cout;
    //cinfo  = std::cout;
    //cwarn  = std::cout;
    //cerror = std::cerr;
    // TODO: Delim-Switch
    #ifdef _WIN32
    config->pathDelim = '\\';
    #else
    config->pathDelim = '/';
    #endif
    return config;
}

void usage() {
    std::cout << "NLTpack v0.3a (C) 2008,2009 by Hendrik Radke" << std::endl;
    std::cout << "usage: nltpack <Mode> <File> [-p FilePattern]\n";
    std::cout << "Modes:\n"
	      << "x: extract all files from the archive matching the pattern.\n"
	      << "l: list all files from the archive matching the pattern.\n"
	      << "c: create an archive with all matching files. [1]\n"
	      << "u: update/add matching files to archive.\n"
	      << "m: create Master file (.FN) for the archive.\n";
    std::cout << "[1] Special care has to be taken that all needed files are present for SCHICK.DAT/BLADE.DAT.\n";
    // TODO: Beispiele und Erläuterungen zu dürftig.
    // TODO: weitere Optionen werden benötigt, je nach Archivtyp.
    exit(0);
}

int main(int argc, char* argv[]) {
    #if __BYTE_ORDER == __BIG_ENDIAN
    cdebug << "Running on big-endian system\n";
    #elif __BYTE_ORDER == __LITTLE_ENDIAN
    cdebug << "Running on little-endian system\n";
    #else
    cdebug << "Unknown Endianness, exiting\n";
    return -1;
    #endif

    if (argc < 3) usage();
    config = parseCommandline(argc, argv);
    Header* header = config->header;
    assert(header != 0);
    switch (config->mode) {
    case 'l': header->a_list(config); break;
    case 'x': header->a_extract(config); break;
    case 'c': header->a_create(config); break;
    case 'u': header->a_update(config); break;
    case 'm': header->a_createMaster(config); break;
    default: cerror << "unknown mode: " << config->mode << std::endl;
	exit(1); break;
    }
}

