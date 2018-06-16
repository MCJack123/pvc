//
//  main.cpp
//  pvc
//
//  Created by Jack on 6/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include <iostream>
#include "serve.hpp"

int main(int argc, const char * argv[]) {
	local_repo_info info;
	info.name = "TestServer";
	info.info.name = "TestServer";
	info.info.creator = "JackMacWindows";
	info.info.commits = 0;
	std::cout << "Running server\n";
	runServer(info);
	return 0;
}
