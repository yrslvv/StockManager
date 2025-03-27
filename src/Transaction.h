#pragma once
#include <string>

class Transaction {
public:
    std::string stockSymbol;
    bool isBuy;  // true for buy, false for sell
    float price;
    int quantity;
    std::string date;

    Transaction(const std::string& symbol, bool buy, float price, int qty, const std::string& transDate)
        : stockSymbol(symbol), isBuy(buy), price(price), quantity(qty), date(transDate) {}
};
