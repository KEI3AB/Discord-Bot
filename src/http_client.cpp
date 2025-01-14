#include "http_client.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <functional>



Client::Client(boost::asio::io_context& io_context,const std::string& MAIN_API, const std::string& API_ARGUMENTS)
    : io_context(io_context), MAIN_API(MAIN_API), API_ARGUMENTS(API_ARGUMENTS) {}

void Client::getResponse(const std::function<void(const std::string&)>& process) {
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::socket socket(io_context);

    boost::asio::connect(socket, resolver.resolve(MAIN_API, "80"));

    http::request<http::string_body> req(http::verb::get, API_ARGUMENTS, 11);

    req.set(http::field::host, MAIN_API);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(socket, req);

    std::string response;
    {
        boost::beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(socket, buffer, res);
        response = boost::beast::buffers_to_string(res.body().data());
    }

    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    
    process(response);
}
