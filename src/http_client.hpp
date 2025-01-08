#ifndef HTTP_CLIENT
#define HTTP_CLIENT

#include <boost/asio.hpp>
#include <functional>
#include <string>


class HttpClient {
public:
    explicit HttpClient(boost::asio::io_context& io_context);

    void async_get(const std::string& host, const std::string& target,
                   const std::function<void(const std::string&)>& on_success,
                   const std::function<void(const std::string&)>& on_error);

private:
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf response;
};


#endif // HTTP_CLIENT