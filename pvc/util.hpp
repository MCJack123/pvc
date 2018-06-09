//
//  util.hpp
//  pvc
//
//  Created by Jack on 6/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#ifndef util_hpp
#define util_hpp

#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <vector>
#include <fstream>

typedef struct {
    bool ok; // whether the curl process completed successfully
    long response_code; // either the curl result or http response
    void * data; // returned data
    size_t size; // size of returned data
} http_response;

// Sends an HTTP GET request to url.
http_response http_get(std::string url);

// Sends an HTTP POST request with data to url.
http_response http_post(std::string url, void * data, size_t size);

// Reads contents of file. First four bytes of buffer are size.
void * readFile(std::string file);

// Writes contents of data to file.
void writeFile(std::string file, void * data, size_t size);

#endif /* util_hpp */
