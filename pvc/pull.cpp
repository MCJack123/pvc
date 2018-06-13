//
//  pull.cpp
//  pvc
//
//  Created by Jack on 6/12/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "pull.hpp"

strvec resolveNodes(std::string url, std::string name, bool recurse) {
    strvec retval;
    if (!queryURL(url)) return retval;
    repo_info info = getInfo(url, name);
    retval.push_back(url);
    for (std::string node : info.nodes) {
        if (!queryURL(node)) continue;
        strvec repos = getRepos(node);
        if (std::find(repos.begin(), repos.end(), name) == repos.end()) continue;
        retval.push_back(node);
    }
    if (recurse) {
        for (std::string n : retval) {
            strvec newn = resolveNodes(n, name);
            //todo
        }
    }
    return retval;
}

commits getCommits(repo_info repo) {
    //todo
}
