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

void OrderBook::insertOrder(OrderBookEntry order) {
	orders.push_back(order); //Insert the order to back of the order book
	std::sort(orders.begin(), orders.end(), OrderBookEntry::compareOrdersTimestamp); //Sort the order book again
}

std::vector<OrderBookEntry> OrderBook::matchAskBid(std::string product, std::string timestamp) {

	std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
	std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);
	std::vector<OrderBookEntry> sales;

	std::sort(asks.begin(), asks.end(), OrderBookEntry::compareOrdersPriceAsc); //sort ask orders beginning from lowest to highest
	std::sort(bids.begin(), bids.end(), OrderBookEntry::compareOrdersPriceDesc); //sort bid orders beginning from highest to lowest 

	for (OrderBookEntry& a : asks) 
	{
		for (OrderBookEntry& b : bids) 
		{	
			if (b.price >= a.price)   //Only match if bid price is higher than ask price
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
				if (b.amount == a.amount)  //If bid amount is equal to ask amount
				{
					sale.amount = a.amount; 
					sales.push_back(sale);  //add sale order entry
					b.amount = 0;  //clear the bid since its cleared
					break;
				}
				if (b.amount > a.amount)   //If bid amount is bigger than ask amount
				{
					sale.amount = a.amount;
					sales.push_back(sale); //add sale order entry 
					b.amount = b.amount - a.amount;  //decrease the bid amount by fulfilled amount
					break;
				}
				if (b.amount < a.amount)   //If ask amount is bigger than bid
				{
					sale.amount = b.amount;
					sales.push_back(sale);  //add sale order entry
					a.amount = a.amount - b.amount;   //descrease the ask amount by fulfilled bid amount 
					//No break out of this loop since ask amount is still not fulfilled
				}
			}
		}
	}

	return sales;
}