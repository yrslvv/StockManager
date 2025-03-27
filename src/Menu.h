#pragma once
#include "StockData.h"
#include "StockPortfolio.h"
#include "Transaction.h"
#include "StockAPI.h"
#include "StockAnalyzer.h"

#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <algorithm> 
#include <sstream>


class Menu {
private:
    StockPortfolio portfolio;
    StockAPI stockAPI;
    std::string apiKey;

public:
    Menu(const std::string& apiKey) : apiKey(apiKey) {}

    void displayMenu() {
        int choice;
        do {
            std::cout << "\nStock Portfolio Menu:\n";
            std::cout << "1. Buy Shares\n";
            std::cout << "2. Sell Shares\n";
            std::cout << "3. View Portfolio\n";
            std::cout << "4. View Earnings/Losses\n";
            std::cout << "5. View Max Profit\n";
            std::cout << "6. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1:
                buyShares();
                break;
            case 2:
                sellShares();
                break;
            case 3:
                viewPortfolio();
                break;
            case 4:
                viewEarningsLosses();
                break;
            case 5:
                viewMaxProfit();
                break;
            case 6:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice, please try again.\n";
            }
        } while (choice != 6);
    }

private:
    void buyShares() {
        std::string symbol;
        int quantity;

        std::cout << "Enter stock symbol: ";
        std::cin >> symbol;
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::toupper); // Normalize symbol

        std::cout << "Enter quantity: ";
        while (!(std::cin >> quantity) || quantity <= 0) {
            std::cout << "Invalid input. Please enter a valid quantity: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::string priceStr = stockAPI.fetchStockPrice(symbol, apiKey);
        if (priceStr.empty() || priceStr == "Error") {
            std::cout << "Failed to fetch stock price. Try again later.\n";
            return;
        }

        float price;
        try {
            price = std::stof(priceStr);
        }
        catch (...) {
            std::cout << "Invalid price format received from API.\n";
            return;
        }

        Transaction buyTransaction(symbol, true, price, quantity, getTodayDate());
        portfolio.addTransaction(buyTransaction);

        std::cout << "Bought " << quantity << " shares of " << symbol << " at " << price << " each.\n";
    }

    void sellShares() {
        if (portfolio.getPorfolioSize() == 0) {
            std::cout << "Your portfolio is empty. Nothing to sell.\n";
            return;
        }

        std::string symbol;
        int quantity;

        std::cout << "Enter stock symbol: ";
        std::cin >> symbol;
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::toupper); 

        if (portfolio.GetPortfolio().find(symbol) == portfolio.GetPortfolio().end()) {
            std::cout << "You do not own any shares of " << symbol << ".\n";
            return;
        }

        int ownedShares = portfolio.GetPortfolio().at(symbol).getShares();
        std::cout << "Enter quantity: ";
        while (!(std::cin >> quantity) || quantity <= 0 || quantity > ownedShares) {
            std::cout << "Invalid quantity. You own " << ownedShares << " shares. Try again: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::string priceStr = stockAPI.fetchStockPrice(symbol, apiKey);
        if (priceStr.empty() || priceStr == "Error") {
            std::cout << "Failed to fetch stock price.\n";
            return;
        }

        float price;
        try {
            price = std::stof(priceStr);
        }
        catch (...) {
            std::cout << "Invalid price format received from API.\n";
            return;
        }

        Transaction sellTransaction(symbol, false, price, quantity, getTodayDate());
        portfolio.addTransaction(sellTransaction);

        std::cout << "Sold " << quantity << " shares of " << symbol << " at " << price << " each.\n";

        // Remove stock if all shares are sold
        if (quantity == ownedShares) {
            portfolio.GetPortfolio().erase(symbol);
        }
    }


    void viewPortfolio() {
        for (auto& stockEntry : portfolio.GetPortfolio()) {
            std::string symbol = stockEntry.first;
            StockData& stockData = stockEntry.second;

            // Fetch current stock price from API (e.g. using the StockAPI class)
            std::string stockPriceStr = StockAPI::fetchStockPrice(symbol, apiKey);
            if (!stockPriceStr.empty()) {
                float stockPrice = std::stof(stockPriceStr);  
                stockData.setCurrentPrice(stockPrice);  
            }
            else {
                std::cerr << "Failed to fetch price for " << symbol << std::endl;
            }
        }

        portfolio.displayPortfolio(apiKey);
    }

    void viewEarningsLosses() {
        float profitLoss = portfolio.calculatePortfolioProfit();
        std::cout << "Total Profit/Loss: " << profitLoss << std::endl;
    }

    void viewMaxProfit() {
        std::string symbol;
        std::cout << "Enter stock symbol: ";
        std::cin >> symbol;

        // Fetch historical stock data and calculate max profit
        StockData stockData;
        float maxProfit = StockAnalyzer::calculateMaxProfit(stockData);
        std::cout << "Max profit for " << symbol << " is: " << maxProfit << std::endl;
    }

    std::string getTodayDate() {
        auto now = std::chrono::system_clock::now();

        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        std::tm tm = {};
        localtime_s(&tm, &currentTime);  // Used localtime_s to safely convert time_t to tm

        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d");  // Format as YYYY-MM-DD
        return ss.str();
    }
};
