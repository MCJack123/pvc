//
//  commit.hpp
//  Creates commits for a repo.
//  pvc
//
//  Created by Jack on 6/16/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef commit_hpp
#define commit_hpp

#include "patch.hpp"
#include "query.hpp"
#include "picosha2.hpp"
#include <chrono>
#include <iomanip>

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

struct local_repo_info {
	std::string name;
	repo_info info;
	commits commits;
};

// Creates a commit at the current time.
commit createCommit(std::vector<patch> patches, std::string creator, std::string title, std::string description = "", std::string creator_signature = "");

#endif /* commit_hpp */
