//
//  util.cpp
//  pvc
//
//  Created by Jack on 6/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "util.hpp"

CURL * handle;

typedef struct {
    char * data;
    size_t size;
    size_t offset;
} http_read_buffer;

size_t write_data(void * buffer, size_t size, size_t nmemb, void * userp) {
    if (size * nmemb == 0) return 0;
    http_response * resp = (http_response*)userp;
    void * temp = malloc(resp->size + (size * nmemb));
    memcpy(temp, resp->data, resp->size);
    memcpy((void*)&((char*)temp)[resp->size], buffer, size * nmemb);
    resp->data = temp;
    resp->size += size * nmemb;
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

http_response http_get(std::string url) {
    http_response retval;
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
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
    return retval;
}

http_response http_post(std::string url, void * data, size_t size) {
    http_response retval;
    http_read_buffer buf;
    buf.data = (char*)data;
    buf.size = size;
    buf.offset = 0;
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
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
    return retval;
}

void * readFile(std::string file) {
    std::ifstream in(file.c_str());
    http_response buf;
    while (!in.eof()) {
        char * ibuf = (char*)malloc(512);
        in.read(ibuf, 512);
        write_data((void*)ibuf, 1, in.gcount(), (void*)&buf);
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


