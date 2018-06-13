//
//  query.hpp
//  Get information about a URL that may serve pvc repositories.
//  pvc
//
//  Created by Jack on 6/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef query_hpp
#define query_hpp

#include "util.hpp"

typedef struct {
    std::string name;
    std::string creator;
    strvec nodes;
    int commits;
} repo_info;

// Check if url is serving pvc repositories.
bool queryURL(std::string url);

// Get list of repositories at url.
strvec getRepos(std::string url);

// Get information about a repository.
repo_info getInfo(std::string url, std::string name);

#endif /* query_hpp */
