//
//  patch.hpp
//  Patches a file.
//  pvc
//
//  Created by Jack on 6/12/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef patch_hpp
#define patch_hpp

#include "util.hpp"

typedef enum {
    PATCH_ADD_LINE,
    PATCH_DEL_LINE,
    PATCH_EDIT_LINE,
    PATCH_NEW_FILE
} patch_type;

typedef struct {
    std::string filename;
    int lineno;
    patch_type type;
    std::string content;
} patch;

// Applies a patch to a string.
std::string applyPatch(std::string text, patch p, int baseline = 0);

// Applies multiple patches to a string.
std::string applyPatches(std::string text, std::vector<patch> p);

// Applies multiple patches to a file.
void patchFile(std::string filename, std::vector<patch> p);

// Applies multiple patches to a directory, checking filenames as needed.
int patchDir(std::string dirname, std::vector<patch> p);

#endif /* patch_hpp */