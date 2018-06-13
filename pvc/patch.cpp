//
//  patch.cpp
//  pvc
//
//  Created by Jack on 6/12/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "patch.hpp"

std::string applyPatch(std::string text, patch p, int baseline) {
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
    std::string file;
    for (std::vector<patch>::iterator it = p.begin(); it != p.end(); it++) if (it->filename != filename) p.erase(it);
    if (p[0].type == PATCH_NEW_FILE) file = p[0].content;
    else file = applyPatches(convertFileData(readFile(filename)), p);
    char * data = (char*)malloc(file.size() + 1);
    strcpy(data, file.c_str());
    writeFile(filename, (void*)data, file.size() + 1);
}

int patchDir(std::string dirname, std::vector<patch> p) {
    int patches = (int)p.size();
    strvec files = listDir(dirname);
    std::map<std::string, std::vector<patch>> filesplit;
    for (patch pa : p) {
        if (std::find(files.begin(), files.end(), pa.filename) == files.end()) {patches--; break;}
        if (filesplit.find(pa.filename) == filesplit.end()) filesplit[pa.filename] = std::vector<patch>();
        filesplit[pa.filename].push_back(pa);
    }
    for (std::map<std::string, std::vector<patch>>::iterator it = filesplit.begin(); it != filesplit.end(); it++) patchFile(it->first, it->second);
    return patches;
}
