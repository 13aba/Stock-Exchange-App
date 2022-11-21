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

double OrderBook::getQuotedSpread(std::vector<OrderBookEntry>& orders) {
	double min = getLowestPrice(orders);
	double max = getHighestPrice(orders);

	double normalSpread = (max - min);
	double midpoint = (max + min) / 2;

	double qSpread = (normalSpread / midpoint) * 100;
	return qSpread;
}

std::string OrderBook::getEarliestTime() {
	return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp) {

	std::string nextTime ="";

	for (OrderBookEntry& e : orders) {
		if (e.timestamp > timestamp) {
			nextTime = e.timestamp;
			break;
		}
	}
	if (nextTime == "") {
		nextTime = orders[0].timestamp;
	}
	return nextTime;
}