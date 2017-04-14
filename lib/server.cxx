#include "server.hxx"
#include <cstring>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using namespace httpd;

auto srv() { return Server::get(); }

int sendResponse(Response r, MHD_Connection *c)
{
  cout << "response: " << r.content << endl;
  MHD_Response *mr = r.toMHD();
  int ret = MHD_queue_response(c, (unsigned int)r.status, mr);
  MHD_destroy_response(mr);
  return ret;
}

int Server::run__(
  void *cls,
  MHD_Connection *cnx,
  const char *url,
  const char *method,
  const char *version,
  const char *upload_data,
  size_t * upload_data_size,
  void ** ptr)
{
  thread_local static int token;
  thread_local static string data{""};

  if(0 == strcmp(method, "GET"))
  {
    if(&token != *ptr)
    {
      *ptr = &token;
      return MHD_YES;
    }
    Response r = srv().handleGet(string{url});
    return sendResponse(r, cnx);
  }
  
  else if(0 == strcmp(method, "POST"))
  {
    if(&token != *ptr)
    {
      *ptr = &token;
      return MHD_YES;
    }
    if(*upload_data_size != 0)
    {
      data += string{upload_data, *upload_data_size};
      *upload_data_size = 0;
      return MHD_YES;
    }
    Response r = srv().handlePost(string{url}, data);
    return sendResponse(r, cnx);
  }

  else return MHD_NO;
  
}
    
Response Server::handleGet(string path)
{
  auto h = get_handlers_.find(path);
  if(h == get_handlers_.end())
  {
    return Response{Status::NotFound};
  }
  else return (h->second)(GetRequest{});
}

Response Server::handlePost(string path, string content)
{
  cout << "path: " << path << endl;
  cout << "post: " << content << endl;
  cout << "size: " << content.size() << endl;
  auto h = post_handlers_.find(path);
  if(h == post_handlers_.end())
  {
    return Response{Status::NotFound};
  }
  else return (h->second)(PostRequest{content});
}

Server & Server::get()
{
  static Server instance;
  return instance;
}

void Server::onGet(string path, GetHandler h)
{
  get_handlers_[path] = h; 
}

void Server::onPost(string path, PostHandler h)
{
  post_handlers_[path] = h;
}

void Server::run()
{
  int token{47};

  d_ = MHD_start_daemon(
      MHD_USE_THREAD_PER_CONNECTION,
      port,
      nullptr,
      nullptr,
      &Server::run__,
      &token,
      MHD_OPTION_END);

  if(d_ == nullptr) exit(1);

  while(1) MHD_run(d_);

  //(void) getc (stdin);
  MHD_stop_daemon(d_);
}

Response::Response(Status s, string c)
  : status{s}, content{c}
{}

MHD_Response * Response::toMHD()
{
  return MHD_create_response_from_buffer(
      content.size(),
      (void*)content.c_str(),
      MHD_RESPMEM_MUST_COPY);
}

PostRequest::PostRequest(string data)
  : data{data}
{}


