/*
 * Created by Crow on 8/23/18.
 * Description: I have used httpxx to parse the HTTP Request
 *           But I must build a response by myself
 *           So, I should inherit a class from http::MessageBuilder to build my class
 * API:
 *          (non)                http_response::http_response();      // Constructor
 *          const std::string   &http_response::get_response();      // return the http-response
 *          bool                 http_response::build();             // Build the response string
 *          bool                 http_response::set_header();        // set the header;
 */

#ifndef PLATINUMSERVER_HTTP_RESPONSE_H
#define PLATINUMSERVER_HTTP_RESPONSE_H

#include <httpxx/Request.hpp>
#include <httpxx/ResponseBuilder.hpp>
#include <string>

namespace PlatinumServer {
class http_response : public http::ResponseBuilder {
public:
    http_response() = default;
    ~http_response() = default;
    const std::string &get_response();
    bool build();                                                         // build the response base on the Message class
    bool set_header(std::string &&key, std::string &&val);                  // the important function to set headers
private:
    std::string server_response;
};
}

#endif //PLATINUMSERVER_HTTP_RESPONSE_H
