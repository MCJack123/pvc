//
//  pull.cpp
//  pvc
//
//  Created by Jack on 6/12/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "pull.hpp"

bool sortCommits(commit first, commit second) {
	return first.timestamp < second.timestamp;
}

strvec resolveNodes(std::string url, std::string name, bool recurse) {
    strvec retval;
    if (!queryURL(url) || !queryRepo(url, name)) return retval;
    repo_info info = getInfo(url, name);
    retval.push_back(url);
    for (std::string node : info.nodes) {
        if (!queryURL(node)) continue;
        strvec repos = getRepos(node);
        if (std::find(repos.begin(), repos.end(), name) == repos.end()) continue;
        retval.push_back(node);
    }
    if (recurse) {
        for (std::string n : retval) {
            strvec newn = resolveNodes(n, name, false);
			if (!newn.empty())
				for (std::string a : newn)
					if (std::find(retval.begin(), retval.end(), a) == retval.end())
						retval.push_back(a);
        }
    }
    return retval;
}

commits getCommits(repo_info repo) {
    commits retval;
	std::map<std::string, int> comcount;
	int total = 0;
	for (std::string node : repo.nodes) {
		if (!queryURL(node) || !queryRepo(node, repo.name)) continue;
		http_response resp = http_get(node + "/repos/" + repo.name + "/commits");
		if (resp.ok && resp.response_code == 200) {
			Json::Value root = parseJSON(std::string(const_cast<const char *>((char*)resp.data), resp.size));
			for (int i = 0; i < root["commits"].size(); i++) {
				if (std::find(retval.begin(), retval.end(), root["commits"][i]["id"].asString()) == retval.end()) {
					commit c;
					c.id = root["commits"][i]["id"].asString();
					c.creator = root["commits"][i]["creator"].asString();
					c.creator_signature = root["commits"][i]["creator_signature"].asString();
					c.description = root["commits"][i]["description"].asString();
					c.timestamp = root["commits"][i]["timestamp"].asUInt64();
					c.title = root["commits"][i]["title"].asString();
					for (int j = 0; j < root["commits"][i]["patches"].size(); j++) {
						patch p;
						p.content = root["commits"][i]["patches"][j]["content"].asString();
						p.filename = root["commits"][i]["patches"][j]["filename"].asString();
						p.lineno = root["commits"][i]["patches"][j]["lineno"].asInt();
						p.type = (patch_type)root["commits"][i]["patches"][j]["type"].asInt();
						c.patches.push_back(p);
					}
					retval.push_back(c);
					comcount.insert(std::make_pair(root["commits"][i]["id"].asString(), 0));
				}
				comcount[root["commits"][i]["id"].asString()]++;
			}
			total++;
		}
	}
	for (auto i = comcount.begin(); i != comcount.end(); i++)
		if (i->second != total)
			for (commit u : retval)
				if (u.id == i->first)
					retval.erase(std::find(retval.begin(), retval.end(), u));
	std::sort(retval.begin(), retval.end(), sortCommits);
	return retval;
}

commits getCommits(repo_info repo, time_t timestamp) {
	commits retval = getCommits(repo);
	for (commit u : retval)
		if (u.timestamp < timestamp)
			retval.erase(std::find(retval.begin(), retval.end(), u));
	return retval;
}

void reconstructDir(commits c, std::string dir) {
	std::sort(c.begin(), c.end(), sortCommits);
	std::vector<patch> p;
	for (int i = 0; i < c.size(); i++) p.insert(p.end(), c[i].patches.begin(), c[i].patches.end());
	patchDir(dir, p);
}
