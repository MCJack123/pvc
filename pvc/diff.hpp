//
//  diff.hpp
//  Creates patches based on the differences in a file(s).
//  pvc
//
//  Created by Jack on 6/16/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef diff_hpp
#define diff_hpp

#include "patch.hpp"
#include "commit.hpp"

// Creates a list of patches between two strings.
std::vector<patch> diffStrings(std::string text1, std::string text2);

// Creates a list of patches between two files.
std::vector<patch> diffFiles(std::string file1, std::string file2);

// Creates a list of patches between two directories.
std::vector<patch> diffDirs(std::string dir1, std::string dir2);

// Creates a list of patches since the last commit.
std::vector<patch> diffCommits(commits state, std::string dir);

#endif /* diff_hpp */
