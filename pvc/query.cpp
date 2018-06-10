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
    if (!resp.ok || resp.response_code != 200 || resp.size > 18 || resp.size < 10 || std::string((char*)resp.data) != "Powered by pvc") return false;
    else return true;
}

std::vector<std::string> getRepos(std::string url) {
    if (!queryURL(url)) return {};
    std::vector<std::string> retval;
    http_response resp = http_get(url + "/repos");
    if (resp.ok && resp.response_code == 200) {
        std::stringstream ss((char*)resp.data);
        std::string to;
        while (std::getline(ss, to, '\n')) retval.push_back(to);
    }
    return retval;
}

repo_info getInfo(std::string url, std::string name) {
    repo_info retval;
    if (!queryURL(url)) return retval;
    http_response resp = http_get(url + "/repos/" + name);
    if (resp.ok && resp.response_code == 200) {
        Json::Value info = parseJSON(std::string((char*)resp.data));
        retval.name = info["name"].asString();
        retval.creator = info["creator"].asString();
        retval.commits = info["commits"].asInt();
        for (int i = 0; i < info["nodes"].size(); i++) retval.nodes.push_back(info["nodes"][i].asString());
    }
    return retval;
}
