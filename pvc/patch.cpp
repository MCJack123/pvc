//
//  patch.cpp
//  pvc
//
//  Created by Jack on 6/12/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "patch.hpp"

std::string applyPatch(std::string text, patch p, int baseline) {
	log("Applying patch for file " + p.filename, MESSAGE_DEBUG);
    std::stringstream ss(text.c_str());
    std::string line, retval = "";
    for (int i = 1 + baseline; std::getline(ss, line, '\n'); i++) {
        if (i == p.lineno) {
            switch (p.type) {
                case PATCH_ADD_LINE:
                    retval += line + "\n" + p.content + "\n";
                    break;
                case PATCH_DEL_LINE:
                    break;
                case PATCH_EDIT_LINE:
                    retval += p.content + "\n";
                    break;
                case PATCH_NEW_FILE:
                    return p.content;
            }
        } else retval += line + "\n";
    }
    return retval;
}

std::string applyPatches(std::string text, std::vector<patch> p) {
    std::string retval = text;
    int baseline = 0;
    for (patch pa : p) {
        retval = applyPatch(retval, pa, baseline);
        if (pa.type == PATCH_ADD_LINE) baseline++;
        else if (pa.type == PATCH_DEL_LINE) baseline--;
    }
    return retval;
}

void patchFile(std::string filename, std::vector<patch> p) {
	log("Patching file " + filename, MESSAGE_DEBUG);
	//log(filename.substr(filename.find_last_of("/")), MESSAGE_DEBUG);
    std::string file;
	for (std::vector<patch>::iterator it = p.begin(); it != p.end(); it++) if (it->filename != filename.substr(filename.find_last_of("/") + 1)) {p.erase(it); log("Not patching " + it->filename, MESSAGE_DEBUG);}
    if (p[0].type == PATCH_NEW_FILE) file = p[0].content;
    else file = applyPatches(convertFileData(readFile(filename)), p);
    char * data = (char*)malloc(file.size() + 1);
    strcpy(data, file.c_str());
    writeFile(filename, (void*)data, file.size() + 1);
	free((void*)data);
}

int patchDir(std::string dirname, std::vector<patch> p) {
	log("Patching directory " + dirname, MESSAGE_DEBUG);
    int patches = (int)p.size();
    strvec files = listDir(dirname);
    std::map<std::string, std::vector<patch>> filesplit;
    for (patch pa : p) {
        if (std::find(files.begin(), files.end(), pa.filename) == files.end() && pa.type != PATCH_NEW_FILE) {patches--; break;}
        if (filesplit.find(pa.filename) == filesplit.end()) filesplit[pa.filename] = std::vector<patch>();
        filesplit[pa.filename].push_back(pa);
    }
    for (std::map<std::string, std::vector<patch>>::iterator it = filesplit.begin(); it != filesplit.end(); it++) patchFile(dirname + "/" + it->first, it->second);
    return patches;
}
