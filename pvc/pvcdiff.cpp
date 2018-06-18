//
//  main.cpp
//  pvcdiff
//
//  Created by Jack on 6/16/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include <iostream>
#include "diff.hpp"

int main(int argc, const char * argv[]) {
	messageLevel = -1;
	if (argc < 3) {
		std::cerr << "Usage: " << argv[0] << " [-r] <file1> <file2>\n";
		return 1;
	}
	std::vector<patch> retval;
	if (std::string(argv[1]) == "-r") {
		if (argc < 4) {
			std::cerr << "Usage: " << argv[0] << " [-r] <file1> <file2>\n";
			return 1;
		}
		retval = diffDirs(std::string(argv[2]), std::string(argv[3]));
	} else retval = diffFiles(std::string(argv[1]), std::string(argv[2]));
	Json::Value root(Json::objectValue);
	root["patches"] = JSONARR;
	for (patch p : retval) {
		Json::Value pval(Json::objectValue);
		pval["filename"] = Json::Value(p.filename);
		pval["lineno"] = Json::Value(p.lineno);
		pval["content"] = Json::Value(p.content);
		pval["type"] = Json::Value((int)p.type);
		root["patches"].append(pval);
	}
	std::cout << root << "\n";
	if (argc > 3 && (std::string(argv[3]) == "pause" || (argc > 4 && std::string(argv[4]) == "pause"))) std::cin.get();
	return 0;
}
