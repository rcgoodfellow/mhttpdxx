#pragma once

#include <microhttpd.h>
#include <string>
#include <functional>
#include <unordered_map>

namespace httpd
{

enum class Status : unsigned int
{
  OK = 200,
  NotFound = 404,
  ServerError = 500
};

struct Response
{
  Status status;
  std::string content;

  Response(Status, std::string content = "");

  MHD_Response* toMHD();
};


struct GetRequest
{
  //placeholder for the future
};

struct PostRequest
{
  PostRequest(std::string data);
  std::string data;
};

class Server
{
  public:

    using GetHandler = std::function<Response(GetRequest)>;
    using PostHandler = std::function<Response(PostRequest)>;
    
    void onGet(std::string path, GetHandler);
    void onPost(std::string path, PostHandler);

    void run();

    static Server & get();

    size_t port{4747};

  private:
    MHD_Daemon *d_{nullptr};
    std::unordered_map<std::string, GetHandler> get_handlers_;
    std::unordered_map<std::string, PostHandler> post_handlers_;

    static int run__(
        void*, 
        MHD_Connection*, 
        const char*, 
        const char*, 
        const char*, 
        const char*,
        size_t *,
        void **);
                                             
    Response handleGet(std::string);
    Response handlePost(std::string, std::string);
};

}

