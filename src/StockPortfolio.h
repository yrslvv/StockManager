#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "StockData.h"
#include "Transaction.h"
#include "StockAPI.h"  

class StockPortfolio {
private:
    std::map<std::string, StockData> portfolio;  
    std::vector<Transaction> transactions;  

public:

    void addStock(const std::string& symbol, const StockData& stock) {
        portfolio[symbol] = stock;
    }

    // Add a transaction (buy or sell)
    void addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);

        // Update portfolio based on the transaction
        if (transaction.isBuy) {
            portfolio[transaction.stockSymbol].buyShares(transaction.quantity, transaction.price);
        }
        else {
            portfolio[transaction.stockSymbol].sellShares(transaction.quantity, transaction.price);
        }
    }

    // Fetch and update stock prices
    void updateStockPrices(const std::string& apiKey) {
        for (auto& stockEntry : portfolio) {
            std::string symbol = stockEntry.first;
            StockData& stock = stockEntry.second;

            std::string stockPriceStr = StockAPI::fetchStockPrice(symbol, apiKey);
            if (!stockPriceStr.empty() && stockPriceStr != "Error") {
                try {
                    float stockPrice = std::stof(stockPriceStr);
                    stock.setCurrentPrice(stockPrice);
                }
                catch (...) {
                    std::cerr << "Error parsing stock price for " << symbol << std::endl;
                }
            }
            else {
                std::cerr << "Failed to fetch price for " << symbol << std::endl;
            }
        }
    }

    // Calculate the total profit or loss for the portfolio
    float calculatePortfolioProfit() const {
        float totalProfit = 0.0f;
        for (const auto& transaction : transactions) {
            // Track the profit when selling the stock
            if (!transaction.isBuy) {
                totalProfit += portfolio.at(transaction.stockSymbol).calculateProfit(transaction.price, transaction.quantity);
            }
        }
        return totalProfit;
    }

    // Display portfolio with updated prices
    void displayPortfolio(const std::string& api_key) {
        updateStockPrices(api_key);  // Ensure the stock prices are updated before displaying
        float totalValue = 0.0f;

        for (const auto& stock : portfolio) {
            float stockValue = stock.second.getShares() * stock.second.getCurrentPrice();
            totalValue += stockValue;

            std::cout << stock.first << " - Shares: " << stock.second.getShares()
                << " - Current Price: " << stock.second.getCurrentPrice()
                << " - Value: " << stockValue << std::endl;
        }

        std::cout << "Total Portfolio Value: " << totalValue << std::endl;
    
    }
    int getPorfolioSize() {
        return portfolio.size();
    }

    std::map<std::string, StockData>& GetPortfolio() { return portfolio; }
};
