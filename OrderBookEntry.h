#pragma once
#include <string>

enum class OrderBookType {bid, ask, nan, sale};

class OrderBookEntry {
public:
    OrderBookEntry(double _price, double _amount, std::string _timestamp, std::string _product, OrderBookType _orderType);

    static OrderBookType stringToOrderType(std::string String);
    static bool compareOrdersTimestamp(OrderBookEntry e1, OrderBookEntry e2)
    {
        return e1.timestamp < e2.timestamp;
    }

    static bool compareOrdersPriceAsc(OrderBookEntry e1, OrderBookEntry e2)
    {
        return e1.amount < e2.amount;
    }
    static bool compareOrdersPriceDesc(OrderBookEntry e1, OrderBookEntry e2)
    {
        return e1.amount > e2.amount;
    }
    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
}; 
