//
//  main.cpp
//  pvcdiff
//
//  Created by Jack Bruienne on 6/16/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include <iostream>
#include "../pvc/diff.hpp"
#include "../pvc/commit.hpp"
#include "../pvc/pull.hpp"
#include "../pvc/patch.hpp"
#include "../pvc/picosha2.hpp"
#include "../pvc/serve.hpp"
#include "../pvc/util.hpp"

int main(int argc, const char * argv[]) {
	if (argc < 3) {
		std::cerr << "Usage: " << argv[1] << " [-r] <file1> <file2>\n";
		return 1;
	}
	std::vector<patch> retval;
	if (std::string(argv[1]) == "-r") {
		if (argc < 4) {
			std::cerr << "Usage: " << argv[1] << " [-r] <file1> <file2>\n";
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
	return 0;
}
