#include "scheduler.hpp"
#include "http_client.hpp"
#include "currency_rate_processor.hpp"
#include "fetch_config_data.hpp"
#include <boost/asio.hpp>
#include <iostream>

void fetch_currency_rates() {
    boost::asio::io_context io_context;

    std::string API_KEY;
    try {
        API_KEY = fetch_config_data("api-key");
        std::cout << "Api key loaded\n";
    } catch (const std::exception& error) {
        throw error;
    }
                                    // Данные обновятся через полтора часа после запуска бота (в данном случае через 30 секунд)
    Scheduler scheduler(io_context, std::chrono::seconds(30)/* std::chrono::hours(1) + std::chrono::minutes(30) */);
    Client http_client(io_context, "api.currencylayer.com", "/live?access_key=" + API_KEY);
    CurrencyRateProcessor processor;

    scheduler.start([&http_client, &processor]() {
    
        http_client.getResponse([&processor](const std::string& response) {
            processor.process(response);
        });
    });

    io_context.run();
}
