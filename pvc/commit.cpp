//
//  commit.cpp
//  pvc
//
//  Created by Jack on 6/16/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "commit.hpp"

commit createCommit(std::vector<patch> patches, std::string creator, std::string title, std::string description, std::string creator_signature) {
	commit c;
	c.creator = creator;
	c.title = title;
	c.patches = patches;
	c.description = description;
	if (creator_signature == "") picosha2::hash256_hex_string(creator, creator_signature); // temporary signature
	c.creator_signature = creator_signature;
	c.timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	http_response resp = http_get("https://www.random.org/cgi-bin/randbyte?nbytes=4&format=h", 443);
	if (resp.ok && resp.response_code == 200) {
		c.id = std::string(const_cast<const char *>((char*)resp.data), resp.size);
		c.id.erase(remove_if(c.id.begin(), c.id.end(), isspace), c.id.end());
	} else {
		log("Falling back to pseudo-randomness for id generation", MESSAGE_LOG);
		srand((unsigned)time(0));
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(8) << std::hex << rand() << rand() << rand() << rand();
		c.id = ss.str();
	}
	return c;
}
