//
//  serve.cpp
//  pvc
//
//  Created by Jack on 6/15/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "serve.hpp"

void runServer(local_repo_info repo) {
	serverIsRunning = true;
	uWS::Hub h;
	bool onlyServer = false;
	if (!fileExists(std::string(TEMP_DIR) + ".pvcServerRunning")) {
		writeFile(std::string(TEMP_DIR) + ".pvcServerRunning", malloc(1), 1);
		onlyServer = true;
	}
	while (serverIsRunning) {
		h.onHttpRequest([&](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length,
							size_t remainingBytes) {
			//res->end();
			std::cout << "Got request\n";
			url_descriptor url = parseURL(req.getUrl().toString());
			if (onlyServer && url.endpoint == "/") res->end("Powered by pvc", 15);
			else if (url.endpoint == "/repos/" + repo.name) {
				Json::Value root(Json::objectValue);
				root["name"] = Json::Value(repo.info.name);
				root["creator"] = Json::Value(repo.info.creator);
				root["commits"] = Json::Value(repo.info.commits);
				root["nodes"] = Json::Value(Json::arrayValue);
				for (std::string node : repo.info.nodes) root["nodes"].append(Json::Value(node));
				std::stringstream ss;
				ss << root;
				std::string s = ss.str();
				res->end(s.c_str(), s.size());
			}
			else if (url.endpoint.find("/repos/" + repo.name)) {
				std::string endpoint = url.endpoint.substr(0, repo.name.size() + 7);
				//todo
			}
		});
		if (h.listen(28100)) h.run();
	}
}
