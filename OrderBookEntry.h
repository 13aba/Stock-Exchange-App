#pragma once
#include <string>

//Create possible order book types
enum class OrderBookType {bid, ask, nan, bidsale, asksale};

class OrderBookEntry {
    public:
        /* New entry constructor*/
        OrderBookEntry(double _price, double _amount, std::string _timestamp, std::string _product, OrderBookType _orderType, std::string username = "dataset");

        /* Return order book type from string*/
        static OrderBookType stringToOrderType(std::string String);

        /* check which order has earlier timestamp from two orders*/
        static bool compareOrdersTimestamp(OrderBookEntry e1, OrderBookEntry e2)
        {
            return e1.timestamp < e2.timestamp;
        }

         /* check if second order is more pricier than first one*/
        static bool compareOrdersPriceAsc(OrderBookEntry e1, OrderBookEntry e2)
        {
            return e1.amount < e2.amount;
        }
        
        /* check if first order is more pricier than second one*/
        static bool compareOrdersPriceDesc(OrderBookEntry e1, OrderBookEntry e2)
        {
            return e1.amount > e2.amount;
        }


        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType orderType;
        std::string username;
}; 
