//
//  pull.hpp
//  Pulls the changes from a repository.
//  pvc
//
//  Created by Jack on 6/12/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef pull_hpp
#define pull_hpp

#include "query.hpp"
#include "patch.hpp"
#include "commit.hpp"

// Returns all nodes that are currently active and serving.
strvec resolveNodes(std::string url, std::string name, bool recurse = true);

// Downloads a list of commit information.
commits getCommits(repo_info repo);

// Downloads all commits after a timestamp.
commits getCommits(repo_info repo, time_t timestamp);

// Reconstructs a directory from a list of commits.
void reconstructDir(commits c, std::string dir);

#endif /* pull_hpp */
