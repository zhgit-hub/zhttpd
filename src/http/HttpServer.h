#ifndef _HTTPSERVER_
#define _HTTPSERVER_

#include <boost/noncopyable.hpp>

#include "TcpServer.h"
#include "RequestAndResponse.h"


namespace base{

class HttpServer : boost::noncopyable{
public:
    typedef std::function<void (Request &, Response &)> Functor;

    HttpServer(std::string port, int pool_size, Functor &&func) : http_callback_(std::move(func)),
                                                                server_(port, pool_size, std::bind(&HttpServer::http_read, this, std::placeholders::_1, std::placeholders::_2)){
        ;
    }

    void start(){
        server_.start();
    }
private:
    void http_read(std::string &msg, Channel *channel){
        Request req(msg);
        Response res;
        http_callback_(req, res);

        channel->sock()->write(res.message());
        if(res.is_send_file()){
            channel->sock()->send_file(res.file());
        }
    }

    Functor http_callback_;
    TcpServer server_;
};

}

#endif