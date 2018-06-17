//
//  util.cpp
//  pvc
//
//  Created by Jack on 6/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "util.hpp"

int messageLevel = 0;

typedef struct {
    char * data;
    size_t size;
    size_t offset;
} http_read_buffer;

size_t write_data(void * buffer, size_t size, size_t nmemb, void * userp) {
	//log("Got new block to write", MESSAGE_DEBUG);
    if (size * nmemb == 0) return 0;
    http_response * resp = (http_response*)userp;
	if (resp->size > 0) {
    	void * temp = malloc(resp->size + (size * nmemb));
		memcpy(temp, resp->data, resp->size);
    	memcpy((void*)&((char*)temp)[resp->size], buffer, size * nmemb);
		resp->size += size * nmemb;
		resp->data = malloc(resp->size);
    	memcpy(resp->data, temp, resp->size);
		free(temp);
	} else {
		resp->size = size * nmemb;
		resp->data = malloc(resp->size);
		memcpy(resp->data, buffer, resp->size);
	}
    return size * nmemb;
}

size_t read_data(char * buffer, size_t size, size_t nitems, void * instream) {
    http_read_buffer * buf = (http_read_buffer*)instream;
    if (buf->size == buf->offset) return 0;
    buffer = (char*)malloc(size * nitems);
    memcpy((void*)buffer, (void*)&buf->data[buf->offset], (size * nitems > buf->size ? buf->size - (size * nitems) : size * nitems));
    buf->offset += size * nitems;
    return (size * nitems > buf->size ? buf->size - (size * nitems) : size * nitems);
}

void log(std::string text, message_level level) {
	switch (level) {
		case MESSAGE_DEBUG:
			if (messageLevel <= -1) std::cout << "[DEBUG] " << text << "\n";
			break;
		case MESSAGE_LOG:
			if (messageLevel <= 0) std::cout << "[LOG] " << text << "\n";
			break;
		case MESSAGE_WARNING:
			if (messageLevel <= 1) std::cerr << "[WARNING]" << text << "\n";
			break;
		case MESSAGE_ERROR:
			std::cerr << "[ERROR] " << text << "\n";
			break;
		case MESSAGE_FATAL:
			std::cerr << "[FATAL] " << text << "\n";
			break;
	}
}

size_t find_first_of(std::string str, std::string c) {
	return (str.find_first_of(c) == std::string::npos ? str.size() : str.find_first_of(c));
}

http_response http_get(std::string url) {
    http_response retval;
    CURL * handle;
	retval.size = 0;
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_PORT, 28100);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &retval);
    CURLcode result = curl_easy_perform(handle);
    if (result != CURLE_OK) {
        retval.ok = false;
        retval.response_code = result;
        retval.size = 0;
        return retval;
    }
    retval.ok = true;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &retval.response_code);
    curl_easy_cleanup(handle);
    return retval;
}

http_response http_post(std::string url, void * data, size_t size) {
    http_response retval;
    http_read_buffer buf;
    buf.data = (char*)data;
    buf.size = size;
    buf.offset = 0;
    CURL * handle;
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_PORT, 28100);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_data);
    curl_easy_setopt(handle, CURLOPT_READDATA, (void*)&buf);
    CURLcode result = curl_easy_perform(handle);
    if (result != CURLE_OK) {
        retval.ok = false;
        retval.response_code = result;
        return retval;
    }
    retval.ok = true;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &retval.response_code);
    curl_easy_cleanup(handle);
    return retval;
}

void * readFile(std::string file) {
    std::ifstream in(file.c_str());
    http_response buf;
    while (!in.eof()) {
        char * ibuf = (char*)malloc(512);
        in.read(ibuf, 512);
        write_data((void*)ibuf, 1, in.gcount(), (void*)&buf);
		free((void*)ibuf);
    }
    void * retval = malloc(buf.size + 4);
    ((long*)buf.data)[0] = buf.size;
    memcpy((void*)&((char*)retval)[4], buf.data, buf.size);
    return retval;
}

void writeFile(std::string file, void * data, size_t size) {
    std::ofstream out(file.c_str());
    out.write((char*)data, size);
    out.close();
}

url_descriptor parseURL(std::string url) {
    url_descriptor retval;
    retval.full_url = url;
    retval.uri = url.substr(0, find_first_of(url, "/"));
    retval.endpoint = url.substr(find_first_of(url, "/"), find_first_of(url, "?") - find_first_of(url, "/"));
	if (url.find_first_of("?") != std::string::npos) {
		std::string querystr = url.substr(find_first_of(url, "?"));
		std::stringstream ss(querystr.c_str());
		std::string to;
		while (std::getline(ss, to, '&')) retval.query[to.substr(0, to.find_first_of("="))] = to.substr(to.find_first_of("="));
	}
    return retval;
}

Json::Value parseJSON(std::string json) {
    Json::Value root;
    std::stringstream ss(json.c_str());
    ss >> root;
    return root;
}

std::string stringifyJSON(Json::Value root) {
	std::stringstream ss;
	ss << root;
	return ss.str();
}

std::string convertFileData(void * filepointer2) {
    char * filepointer = (char*)filepointer2;
    int32_t size = ((int32_t*)filepointer)[0];
    char * filedata = (char*)malloc(size + 1);
    memcpy((void*)filedata, (void*)&filepointer[4], size);
    filedata[size] = '\0';
    std::string retval(const_cast<const char *>(filedata));
	free(filedata);
	free(filepointer2);
	return retval;
}

#if defined(__WIN32__) || defined(__WIN64__)
#include <windows.h>
strvec listDir(std::string directory) {
    strvec v;
    std::string pattern(directory);
    pattern.append("\\*");
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            v.push_back(data.cFileName);
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}

bool fileExists (std::string name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	} else {
		return false;
	}
}
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
strvec listDir(std::string directory) {
    strvec v;
    DIR* dirp = opendir(directory.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);
    return v;
}

bool fileExists (std::string name) {
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}
#endif
