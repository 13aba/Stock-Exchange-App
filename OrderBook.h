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

		static double getHighestPrice(std::vector<OrderBookEntry>& orders);
		static double getLowestPrice(std::vector<OrderBookEntry>& orders);

	private:
		std::vector<OrderBookEntry> orders;

};

