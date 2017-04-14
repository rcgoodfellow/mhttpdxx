#include "microhttpd.hxx"

using namespace httpd;

int main()
{
  Server &s = Server::get();

  s.onGet("/ding", [](GetRequest) 
  {
    return Response{Status::OK, "dong"};
  });

  s.onPost("/turd", [](PostRequest)
  {
    return Response{Status::OK, "burgaler"};
  });

  s.run();
}
