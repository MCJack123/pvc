//
//  fetch.cpp
//  pvc
//
//  Created by Jack on 6/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "fetch.hpp"

bool fetchRepository(std::string url, std::string repo, std::string dir) {
	if (!queryURL(url) || !queryRepo(url, repo)) {log("fetchRepository: node " + url + " missing repo " + repo, MESSAGE_LOG); return false;}
	log("Downloading info...", MESSAGE_LOG);
	repo_info info = getInfo(url, repo);
	log("Downloading commits...", MESSAGE_LOG);
	commits c = getCommits(info);
	log("Writing commits...", MESSAGE_LOG);
	reconstructDir(c, dir);
	log("Writing info...", MESSAGE_LOG);
	local_repo_info linfo;
	linfo.name = repo;
	linfo.commits = c;
	linfo.info = info;
	Json::Value root(Json::objectValue);
	root["commits"] = JSONARR;
	for (commit c : linfo.commits) {
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
	root["info"] = JSONOBJ;
	root["info"]["name"] = Json::Value(linfo.info.name);
	root["info"]["creator"] = Json::Value(linfo.info.creator);
	root["info"]["commits"] = Json::Value(linfo.info.commits);
	root["info"]["nodes"] = JSONARR;
	for (std::string node : linfo.info.nodes) root["info"]["nodes"].append(Json::Value(node));
	root["name"] = linfo.name;
	std::string retval = stringifyJSON(root);
	writeFile(dir + "/.pvcRepo", (void*)retval.c_str(), retval.size());
	log("Done fetching repo.", MESSAGE_LOG);
	return true;
}
