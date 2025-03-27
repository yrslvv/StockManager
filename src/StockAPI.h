#pragma once

#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"  


class StockAPI {
public:
    static std::string fetchStockPrice(const std::string& symbol, const std::string& apiKey) {
        std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=" + symbol + "&interval=5min&apikey=" + apiKey;

        // Initialize CURL for HTTP request
        CURL* curl = curl_easy_init();
        if (!curl) {
            return "";
        }

        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        CURLcode res = curl_easy_perform(curl);

        // Clean up CURL
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
            return "";
        }

        // Parse JSON data using nlohmann/json
        return parseStockData(readBuffer);
    }

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    static std::string parseStockData(const std::string& jsonStr) {
        try {
            // Parse the JSON response using nlohmann/json
            auto jsonData = nlohmann::json::parse(jsonStr);

            if (jsonData.contains("Time Series (5min)")) {
                auto timeSeries = jsonData["Time Series (5min)"];

                // Get the latest time entry (first entry in time series)
                if (!timeSeries.empty()) {
                    auto latest = timeSeries.begin();
                    auto latestData = latest.value(); 

                    if (latestData.contains("4. close")) {
                        return latestData["4. close"];
                    }
                    else {
                        std::cerr << "'4. close' key not found in the latest entry." << std::endl;
                        return "";
                    }
                }
            }
            else {
                std::cerr << "'Time Series (5min)' key not found in the response." << std::endl;
            }
        }
        catch (const nlohmann::json::exception& e) {
            std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
        }

        return "Error";
    }
};

