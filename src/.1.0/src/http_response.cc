/*
 * Created by Crow on 8/24/18.
 * Description: the class http_response inherit from httpxx::ResponseBuilder
*/

#include <string>
#include <iostream>
#include "http_response.h"

bool PlatinumServer::http_response::set_header(std::string &&key, std::string &&val)
{
    this->headers().insert(std::make_pair(std::move(key), std::move(val)));

    return true;
}

bool PlatinumServer::http_response::build()
{
    auto http_status_line = this->status_line();
    auto http_headers = this->headers();

    // build the http-response
    this->server_response = http_status_line + "\r\n";        // The httpxx library has add "\r\n" (modify the code);
    for (auto &i : http_headers)
        this->server_response += i.first + ": " + i.second + "\r\n";
    this->server_response += "\r\n";

    return true;
}

const std::string &PlatinumServer::http_response::get_response()
{
    return this->server_response;
}