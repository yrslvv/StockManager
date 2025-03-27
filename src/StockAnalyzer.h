#pragma once
#include "StockData.h"
#include <float.h>  

class StockAnalyzer {
public:
    // Calculate the max profit that can be made by buying low and selling high
    static float calculateMaxProfit(const StockData& stockData) {
        float minPrice = FLT_MAX;
        float maxProfit = 0.0f;

        for (const auto& entry : stockData.getDailyData()) {
            const std::string& date = entry.first; 
            const StockData::DailyData& data = entry.second;  

            if (data.close < minPrice) {
                minPrice = data.close;  // Update min price if new low is found
            }
            else {
                float profit = data.close - minPrice;
                if (profit > maxProfit) {
                    maxProfit = profit; 
                }
            }
        }

        return maxProfit;
    }
};
