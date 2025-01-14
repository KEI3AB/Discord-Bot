#include "currency_rate_processor.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>


void CurrencyRateProcessor::process(const std::string& response) {
    nlohmann::json parsed = nlohmann::json::parse(response);
    if (parsed["success"] != true) {
        std::cerr << "Error in API response: " << parsed["success"] << std::endl;
        std::cerr << response << std::endl;
        return;
    }

    auto conversation_rates = parsed["quotes"];

    std::filesystem::path rates_file = std::filesystem::current_path().parent_path() / "data" / "rates.json";

    std::ofstream out(rates_file);
    if (!out.is_open()) {
        std::cerr << "Could not open the rates.json file";
        return;
    }

    out << parsed.dump(4);
}
