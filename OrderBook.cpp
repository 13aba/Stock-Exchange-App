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

double OrderBook::getQuotedSpread(double minAsk, double maxBid) {

	double normalSpread = (minAsk - maxBid);
	double midpoint = (maxBid + minAsk) / 2;

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

void OrderBook::insertOrder(OrderBookEntry order) {
	orders.push_back(order);
	std::sort(orders.begin(), orders.end(), OrderBookEntry::compareOrdersTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAskBid(std::string product, std::string timestamp) {

	std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
	std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);
	std::vector<OrderBookEntry> sales;

	std::sort(asks.begin(), asks.end(), OrderBookEntry::compareOrdersPriceAsc);
	std::sort(bids.begin(), bids.end(), OrderBookEntry::compareOrdersPriceDesc);

	for (OrderBookEntry& a : asks) 
	{
		for (OrderBookEntry& b : bids) 
		{	
			if (b.price >= a.price) 
			{
				OrderBookEntry sale{ a.price, 0, timestamp, product, OrderBookType::asksale};

				if (b.username == "simuser") 
				{
					sale.username = "simuser";
					sale.orderType = OrderBookType::bidsale;
				}
				if (a.username == "simuser")
				{
					sale.username = "simuser";
					sale.orderType = OrderBookType::asksale;
				}
				if (b.amount == a.amount)
				{
					sale.amount = a.amount;
					sales.push_back(sale);
					b.amount = 0;
					break;
				}
				if (b.amount > a.amount)
				{
					sale.amount = a.amount;
					sales.push_back(sale);
					b.amount = b.amount - a.amount;
					break;
				}
				if (b.amount < a.amount)
				{
					sale.amount = b.amount;
					sales.push_back(sale);
					a.amount = a.amount - b.amount;
				}
			}
		}
	}

	return sales;
}