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

typedef struct {
    
} commit;

typedef std::map<std::string, commit> commits;

// Downloads a list of commit information.
commits getCommits(repo_info repo);



#endif /* pull_hpp */
