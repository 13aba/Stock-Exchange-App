#include "OrderBook.h"
#include <map>
#include <algorithm>


OrderBook::OrderBook(std::string fileName){
	orders = CSVReader::readCSV(fileName);
}

std::vector<std::string> OrderBook::getKnownProducts() {
	std::vector<std::string> products;
	std::map<std::string, bool> productMap;

	for (OrderBookEntry& e : orders)
	{
		productMap[e.product] = true; //for every entry add map with entries product and true boolean
	}

	for (auto const& e : productMap)
	{
		products.push_back(e.first); //Push every product from map to vector 
	}

	return products;
}

std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, std::string product, std::string timestamp) {
	std::vector<OrderBookEntry> ordersFiltered;

	for (OrderBookEntry& e : orders) //check if entry match given variables and push it to new vector if matched
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

double OrderBook::getQuotedSpread(double minAsk, double maxBid) {

	double normalSpread = (minAsk - maxBid);
	double midpoint = (maxBid + minAsk) / 2;

	double qSpread = (normalSpread / midpoint) * 100; //Quoted spread in percentile
	return qSpread;
}

std::string OrderBook::getEarliestTime() {
	return orders[0].timestamp; //Return first orders timestamp since the order book is sorted
}

std::string OrderBook::getNextTime(std::string timestamp) {

	std::string nextTime ="";

	for (OrderBookEntry& e : orders) {
		if (e.timestamp > timestamp) {     //Check if entry timestamp is older than current one 
			nextTime = e.timestamp;  //If it is older then set current time to entry timestmap
			break;
		}
	}
	if (nextTime == "") {
		nextTime = orders[0].timestamp; //If there is no older timestamp then loop back to first one
	}
	return nextTime;
}

double OrderBook::getAverage(std::vector<OrderBookEntry>& orders) {
	//double to hold all price sum
	double netPrice = 0;
	for (OrderBookEntry order : orders) 
	{
		netPrice = netPrice + order.price;
	}
	double average = netPrice / orders.size();
	return average;
}


