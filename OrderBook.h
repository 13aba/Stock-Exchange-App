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
		std::vector<std::string> getOrders(OrderBookType type, std::string product, std::string timestamp);
	private:
		std::vector<OrderBookEntry> orders;
};

