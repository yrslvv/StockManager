#pragma once
#include <string>
#include <map>
#include <vector>

class StockData {
public:
    struct DailyData {
        float open;
        float high;
        float low;
        float close;
        long volume;
    };

    struct Purchase {
        float price;
        int quantity;
    };

private:
    float currentPrice;
    int sharesOwned;
    std::vector<Purchase> purchases;  // Track individual purchases (price and quantity)
    std::map<std::string, DailyData> dailyData;  // Store daily stock data by date

public:
    StockData() : currentPrice(0), sharesOwned(0) {}

    // Buy shares and track individual purchase prices
    void buyShares(int quantity, float price) {
        sharesOwned += quantity;
        purchases.push_back({ price, quantity });
    }

    // Sell shares and calculate profit from the corresponding purchases
    void sellShares(int quantity, float price) {
        if (sharesOwned >= quantity) {
            float profit = 0.0f;
            int remainingQuantity = quantity;

            // Process sales and calculate profit from the earliest purchases
            for (auto& purchase : purchases) {
                if (remainingQuantity <= 0) break;

                // If more shares were purchased than are being sold, calculate profit for part of the purchase
                int sellQuantity = std::min(purchase.quantity, remainingQuantity);
                profit += (price - purchase.price) * sellQuantity;

                // Decrease the quantity of the purchase being sold
                purchase.quantity -= sellQuantity;
                remainingQuantity -= sellQuantity;
            }

            // Remove purchases that have been completely sold
            purchases.erase(std::remove_if(purchases.begin(), purchases.end(),
                [](const Purchase& purchase) { 
                    return purchase.quantity == 0; 
                }),
                purchases.end());

            sharesOwned -= quantity;  
        }
    }

    // Calculate profit when selling, this method is based on specific purchases
    float calculateProfit(float sellPrice, int quantity) const {
        float profit = 0.0f;
        int remainingQuantity = quantity;

        for (const auto& purchase : purchases) {
            if (remainingQuantity <= 0) break;

            int sellQuantity = std::min(purchase.quantity, remainingQuantity);
            profit += (sellPrice - purchase.price) * sellQuantity;
            remainingQuantity -= sellQuantity;
        }

        return profit;
    }

    int getShares() const {
        return sharesOwned;
    }

    float getCurrentPrice() const {
        return currentPrice;
    }

    void setCurrentPrice(float price) {
        currentPrice = price;
    }

    void addData(const std::string& date, const DailyData& data) {
        dailyData[date] = data;
    }

    const std::map<std::string, DailyData>& getDailyData() const {
        return dailyData;
    }
};
