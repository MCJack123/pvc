//
//  query.cpp
//  pvc
//
//  Created by Jack on 6/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "query.hpp"

bool queryURL(std::string url) {
    http_response resp = http_get(url);
	return !(!resp.ok || resp.response_code != 200 || resp.size > 18 || resp.size < 10 || std::string((char*)resp.data) != "Powered by pvc");
}

bool queryRepo(std::string url, std::string name) {
	if (!queryURL(url)) {log("queryRepo: url " + url + " is down", MESSAGE_LOG); return false;}
	http_response resp = http_get(url + "/repos/" + name);
	return resp.ok && resp.response_code == 200;
}

strvec getRepos(std::string url) {
	if (!queryURL(url)) {log("queryRepo: url " + url + " is down", MESSAGE_LOG); return {};}
    strvec retval;
    http_response resp = http_get(url + "/repos");
    if (resp.ok && resp.response_code == 200) {
        std::stringstream ss((char*)resp.data);
        std::string to;
        while (std::getline(ss, to, '\n')) retval.push_back(to);
	} else log("getRepos: returned response code " + std::to_string(resp.response_code), MESSAGE_LOG);
    return retval;
}

repo_info getInfo(std::string url, std::string name) {
    repo_info retval;
	if (!queryURL(url)) {log("queryRepo: url " + url + " is down", MESSAGE_LOG); return retval;}
    http_response resp = http_get(url + "/repos/" + name);
    if (resp.ok && resp.response_code == 200) {
        retval.nodes.push_back(url);
        Json::Value info = parseJSON(std::string((char*)resp.data));
        retval.name = info["name"].asString();
        retval.creator = info["creator"].asString();
        retval.commits = info["commits"].asInt();
        for (int i = 0; i < info["nodes"].size(); i++) retval.nodes.push_back(info["nodes"][i].asString());
    } else log("getInfo: returned response code " + std::to_string(resp.response_code), MESSAGE_LOG);
    return retval;
}
