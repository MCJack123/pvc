//
//  fetch.hpp
//  Fetches all commits for a repository and reconstructs the code base from the commits.
//  pvc
//
//  Created by Jack on 6/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef fetch_hpp
#define fetch_hpp

#include "pull.hpp"
#include "serve.hpp"

// The repository info can be stored either as a single node's IP or as part of a node service.
// After one node is discovered, the rest are searched for from the first one.
// The files in the repository are then reconstructed from the commits from the repository.

// Downloads all data for repo from url and places it into dir.
bool fetchRepository(std::string url, std::string repo, std::string dir);

#endif /* fetch_hpp */
