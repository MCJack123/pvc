//
//  util.hpp
//  Utility header
//  pvc
//
//  Created by Jack on 6/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef util_hpp
#define util_hpp

#if defined(__WIN32__) || defined(__WIN64__)
#define TEMP_DIR "C:\\Windows\\Temp\\"
#else
#define TEMP_DIR "/var/tmp/"
#endif
#define JSONOBJ Json::Value(Json::objectValue)
#define JSONARR Json::Value(Json::arrayValue)

#include <json/json.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "uWS/uWS.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

struct http_response {
    bool ok; // whether the curl process completed successfully
    long response_code; // either the curl result or http response
    void * data; // returned data
    size_t size; // size of returned data
};

struct url_descriptor {
    std::string full_url;
    std::string uri;
    std::string endpoint;
    std::map<std::string, std::string> query;
};

typedef enum {
	MESSAGE_DEBUG,
	MESSAGE_LOG,
	MESSAGE_WARNING,
	MESSAGE_ERROR,
	MESSAGE_FATAL
} message_level;

typedef std::vector<std::string> strvec;

// The level of logging that will be displayed. -1 = debugging, 0 = normal, 1 = suppress logs, 2 = suppress warnings
extern int messageLevel;

// Logs the output depending on mode.
void log(std::string text, message_level level);

// Sends an HTTP GET request to url.
http_response http_get(std::string url);

// Sends an HTTP POST request with data to url.
http_response http_post(std::string url, void * data, size_t size);

// Reads contents of file. First four bytes of buffer are size.
void * readFile(std::string file);

// Writes contents of data to file.
void writeFile(std::string file, void * data, size_t size);

// Parses a url and returns the information.
url_descriptor parseURL(std::string url);

// Parses a json string.
Json::Value parseJSON(std::string json);

// Serializes a json value.
std::string stringifyJSON(Json::Value root);

// Converts file data to string.
std::string convertFileData(void * filepointer2);

// Gets a list of files in the directory.
strvec listDir(std::string directory);

// Checks if a file exists.
bool fileExists(std::string name);

// find_first_of, but returns str.size() instead of std::string::npos.
size_t find_first_of(std::string str, std::string c);

// Gets all keys in a map.
template <typename K, typename V>
std::vector<K> getAllKeys(std::map<K, V> m) {
	std::vector<K> v;
	for (typename std::map<K, V>::iterator it = m.begin(); it != m.end(); ++it) v.push_back(it->first);
	return v;
}

#endif /* util_hpp */
