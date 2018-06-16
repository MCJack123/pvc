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

// The commit code may be externalized later.

typedef enum {
    COMMIT_TYPE_NORMAL,
    COMMIT_TYPE_INITIAL,
    COMMIT_TYPE_CHANGE_OWNER,
    COMMIT_TYPE_NEW_MEMBER
} special_commit_type; // may add more as needed

struct commit {
    std::string id;
    std::string creator;
    std::string creator_signature; // will change once the signature system is implemented
    std::string title;
    std::string description;
    time_t timestamp;
    std::vector<patch> patches;
};

struct special_commit : commit {
    special_commit_type type;
    std::map<std::string, std::string> info;
}; // idk, whatever

typedef std::vector<commit> commits;

// Returns all nodes that are currently active and serving.
strvec resolveNodes(std::string url, std::string name, bool recurse = true);

// Downloads a list of commit information.
commits getCommits(repo_info repo);

// Downloads all commits after a timestamp.
commits getCommits(repo_info repo, time_t timestamp);

// Reconstructs a directory from a list of commits.
void reconstructDir(commits c, std::string dir);

#endif /* pull_hpp */
