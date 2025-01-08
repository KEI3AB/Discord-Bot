#include "http_client.hpp"
#include <iostream>

HttpClient::HttpClient(boost::asio::io_context& io_context)
    : resolver(io_context), socket(io_context) {}

void HttpClient::async_get(const std::string& host, const std::string& target,
                           const std::function<void(const std::string&)>& on_success,
                           const std::function<void(const std::string&)>& on_error) {
    resolver.async_resolve(host, "80",
        [this, host, target, on_success, on_error](const boost::system::error_code& error,
                                                   boost::asio::ip::tcp::resolver::results_type results) {

        if (error) {
            on_error("Resolve error: " + error.message());
            return;
        }

        boost::asio::async_connect(socket, results,
            [this, host, target, on_success, on_error](const boost::system::error_code& error,
                                                       const boost::asio::ip::tcp::endpoint&) {

            if (error) {
                on_error("Connect error: " + error.message());
                return;
            }

            std::ostream request_stream(&response);
            request_stream << "GET " << target << " HTTP/1.1\r\n"
                           << "Host: " << host << "\r\n"
                           << "Connection: close\r\n\r\n";
            
            boost::asio::async_write(socket, response,
                [this, on_success, on_error](const boost::system::error_code& error, std::size_t) {

                if (error) {
                    on_error("Send error: " + error.message());
                    return;
                }

                boost::asio::async_read(socket, response,
                    [this, on_success, on_error](const boost::system::error_code& error, std::size_t) {

                    if (error && error != boost::asio::error::eof) {
                        on_error("Receive error: " + error.message());
                        return;
                    }
                    
                        // Если убрать внутренние скобки, то всё сломается. Не трогать!
                    std::string data((std::istreambuf_iterator<char>(&response)), std::istreambuf_iterator<char>());

                    on_success(data);
                });
            });
        });
    });
}