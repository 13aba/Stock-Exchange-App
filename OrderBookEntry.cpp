#include "OrderBookEntry.h"

OrderBookEntry::OrderBookEntry(double _price, double _amount, std::string _timestamp, std::string _product, OrderBookType _orderType, std::string _username) :
	price(_price),
	amount(_amount),
	timestamp(_timestamp),
	product(_product),
	orderType(_orderType),
	username(_username)
{

}

OrderBookType OrderBookEntry::stringToOrderType(std::string s) {
	//Return order book type depending on the string
	if (s == "ask") {
		return OrderBookType::ask;
	}
	if (s == "bid") {
		return OrderBookType::bid;
	}
	//Return NaN if string does not match anything else
	return OrderBookType::nan;
}



