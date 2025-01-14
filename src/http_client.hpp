#ifndef HTTP_CLIENT
#define HTTP_CLIENT

#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace http = boost::beast::http;

class Client {
public:
    Client(boost::asio::io_context& io_context,const std::string& MAIN_API, const std::string& API_ARGUMENTS);

    void getResponse(const std::function<void(const std::string&)>& process);

private:
    boost::asio::io_context& io_context;
    const std::string MAIN_API;
    const std::string API_ARGUMENTS;
};

#endif // HTTP_CLIENT
