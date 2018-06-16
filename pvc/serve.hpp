//
//  serve.hpp
//  Serves a repository to be downloaded.
//  pvc
//
//  Created by Jack on 6/15/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef serve_hpp
#define serve_hpp

#include "util.h"
#include "pull.hpp"

struct local_repo_info {
	std::string name;
	repo_info info;
	commits commits;
};

// Keeps track of if the server is running. Change to false to stop the server.
static bool serverIsRunning;

// The main server loop.
void runServer(local_repo_info repo);

#endif /* serve_hpp */
