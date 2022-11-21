#pragma once
#include "CSVReader.h"
#include "OrderBookEntry.h"
#include <string>
#include <vector>
#include <iostream>

class OrderBook
{
	public:  
		OrderBook(std::string fileName);

		std::vector<std::string> getKnownProducts();
		std::vector<OrderBookEntry> getOrders(OrderBookType type, std::string product, std::string timestamp);
		std::vector<OrderBookEntry> matchAskBid(std::string products, std::string timestamp);

		static double getHighestPrice(std::vector<OrderBookEntry>& orders);
		static double getLowestPrice(std::vector<OrderBookEntry>& orders);
		static double getQuotedSpread(std::vector<OrderBookEntry>& orders);

		std::string getEarliestTime();
		std::string getNextTime(std::string timestamp);

		void insertOrder(OrderBookEntry order);

	private:
		std::vector<OrderBookEntry> orders;

};

