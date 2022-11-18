#include "OrderBook.h"
#include <map>


OrderBook::OrderBook(std::string fileName){
	orders = CSVReader::readCSV(fileName);
}

std::vector<std::string> OrderBook::getKnownProducts() {
	std::vector<std::string> products;
	std::map<std::string, bool> productMap;

	for (OrderBookEntry& e : orders)
	{
		productMap[e.product] = true;
	}

	for (auto const& e : productMap)
	{
		products.push_back(e.first);
	}

	return products;
}

std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, std::string product, std::string timestamp) {
	std::vector<OrderBookEntry> ordersFiltered;
	for (OrderBookEntry& e : orders)
	{
		if (e.orderType == type && e.product == product && e.timestamp == timestamp)
		{
			ordersFiltered.push_back(e);
		}
	}
	return ordersFiltered;
}

double OrderBook::getHighestPrice(std::vector<OrderBookEntry>& orders) {
	double max =orders[0].price;
	for (OrderBookEntry& e : orders) 
	{
		if (max < e.price) 
		{
			max = e.price;
		}
	}
	return max;
}

double OrderBook::getLowestPrice(std::vector<OrderBookEntry>& orders) {
	double min = orders[0].price;
	for (OrderBookEntry& e : orders)
	{
		if (min > e.price)
		{
			min = e.price;
		}
	}
	return min;
}
