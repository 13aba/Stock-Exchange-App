#pragma once
#include <string>

enum class OrderBookType {bid, ask, nan};

class OrderBookEntry {
public:
    OrderBookEntry(double _price, double _amount, std::string _timestamp, std::string _product, OrderBookType _orderType);

    static OrderBookType stringToOrderType(std::string String);
    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
}; 
