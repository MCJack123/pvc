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

// Keeps track of if the server is running.
extern bool serverIsRunning;

// The main server hub.
extern uWS::Hub h;

// A list of repos currently being served. Add to the map to register a repo.
extern std::map<std::string, local_repo_info> reposServed;

// The main server loop.
void runServer();

// Stops the server if it's running. (Works with signal(int, void(int)))
void stopServer(int param);

#endif /* serve_hpp */
