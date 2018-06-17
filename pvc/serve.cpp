//
//  serve.cpp
//  pvc
//
//  Created by Jack on 6/15/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "serve.hpp"

bool serverIsRunning = false;
uWS::Hub h;
std::map<std::string, local_repo_info> reposServed;

std::string sendRepoInfo(local_repo_info repo, std::string endpoint, uWS::HttpMethod method, char * data, size_t length) {
	if (endpoint == "") {
		Json::Value root(Json::objectValue);
		root["name"] = Json::Value(repo.info.name);
		root["creator"] = Json::Value(repo.info.creator);
		root["commits"] = Json::Value(repo.info.commits);
		root["nodes"] = JSONARR;
		for (std::string node : repo.info.nodes) root["nodes"].append(Json::Value(node));
		return stringifyJSON(root);
	}
	if (endpoint == "/commits") {
		Json::Value root(Json::objectValue);
		root["commits"] = JSONARR;
		for (commit c : repo.commits) {
			Json::Value cval(Json::objectValue);
			cval["id"] = Json::Value(c.id);
			cval["title"] = Json::Value(c.title);
			cval["creator"] = Json::Value(c.creator);
			cval["creator_signature"] = Json::Value(c.creator_signature);
			cval["description"] = Json::Value(c.description);
			cval["timestamp"] = Json::Value((int)c.timestamp);
			cval["patches"] = JSONARR;
			for (patch p : c.patches) {
				Json::Value pval(Json::objectValue);
				pval["filename"] = Json::Value(p.filename);
				pval["lineno"] = Json::Value(p.lineno);
				pval["content"] = Json::Value(p.content);
				pval["type"] = Json::Value((int)p.type);
				cval["patches"].append(pval);
			}
			root["commits"].append(cval);
		}
		return stringifyJSON(root);
	}
	return "{\"error\": \"Invalid node\", \"code\": 2}";
}

void runServer() {
	if (!serverIsRunning) {
		serverIsRunning = true;
		//bool onlyServer = true;
		/*if (!fileExists(std::string(TEMP_DIR) + ".pvcServerRunning")) {
			writeFile(std::string(TEMP_DIR) + ".pvcServerRunning", malloc(1), 1);
			onlyServer = true;
		}*/
		h.onHttpRequest([&](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t length,
							size_t remainingBytes) {
			//res->end();
			log("Got request", MESSAGE_DEBUG);
			if (req.getMethod() == uWS::METHOD_POST) {
				log("Method POST. Data: " + std::string(const_cast<const char *>(data), length), MESSAGE_DEBUG);
			}
			url_descriptor url = parseURL(req.getUrl().toString());
			log(url.full_url, MESSAGE_DEBUG);
			std::string response = "{}";
			if (url.endpoint == "/") response = "Powered by pvc";
			else if (url.endpoint == "/repos") {
				strvec keys = getAllKeys(reposServed);
				response = "";
				for (std::string key : keys) response += key + "\n";
			}
			else if (url.endpoint.find("/repos/") != std::string::npos) {
				std::string repoName = url.endpoint.substr(url.endpoint.find_first_of("/repos/") + 7, find_first_of(url.endpoint.substr(url.endpoint.find_first_of("/repos/") + 7), "/"));
				std::string endpoint = url.endpoint.substr(repoName.size() + 7);
				log("Endpoint " + endpoint + " of " + repoName, MESSAGE_DEBUG);
				strvec keys = getAllKeys(reposServed);
				if (std::find(keys.begin(), keys.end(), repoName) != keys.end()) response = sendRepoInfo(reposServed[repoName], endpoint, req.getMethod(), data, length);
				else response = "{\"error\": \"Invalid repo\", \"code\": 1}";
			}
			res->end(response.c_str(), response.size());
		});
		while (serverIsRunning) {
			if (h.listen(28100)) h.run();
		}
		//remove(std::string(std::string(TEMP_DIR) + ".pvcServerRunning").c_str());
		serverIsRunning = false;
	}
}

void stopServer(int param) {
	if (serverIsRunning) {
		serverIsRunning = false;
		uv_stop(h.getLoop());
	}
}
