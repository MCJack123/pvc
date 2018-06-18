//
//  pvcpatch.cpp
//  pvcpatch
//
//  Created by Jack on 6/17/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include <iostream>
#include "patch.hpp"

int main(int argc, const char * argv[]) {
	messageLevel = -1;
	if (argc < 3) {
		std::cerr << "Usage: " << argv[0] << " <patch file> [-r] <file/directory>\n";
		return 1;
	}
	Json::Value root = parseJSON(convertFileData(readFile(std::string(argv[1]))));
	std::vector<patch> patches;
	for (int j = 0; j < root["patches"].size(); j++) {
		patch p;
		p.content = root["patches"][j]["content"].asString();
		p.filename = root["patches"][j]["filename"].asString();
		p.lineno = root["patches"][j]["lineno"].asInt();
		p.type = (patch_type)root["patches"][j]["type"].asInt();
		patches.push_back(p);
	}
	if (argc > 3 && std::string(argv[2]) == "-r") patchDir(std::string(argv[3]), patches);
	else patchFile(std::string(argv[2]), patches, false);
	return 0;
}
