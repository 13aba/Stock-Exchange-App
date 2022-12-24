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

		/* Return every distinct products in the book*/
		std::vector<std::string> getKnownProducts();

		/* Filter book by type, product and timestamp*/
		std::vector<OrderBookEntry> getOrders(OrderBookType type, std::string product, std::string timestamp);

		/* Get highest bid from given orders vector*/
		static double getHighestPrice(std::vector<OrderBookEntry>& orders);

		/* Get lowest ask from given orders*/
		static double getLowestPrice(std::vector<OrderBookEntry>& orders);

		/* Get qouted spread of bid-ask*/
		static double getQuotedSpread(double min, double max);

		/* Get average price of given order entries*/
		static double getAverage(std::vector<OrderBookEntry>& orders);

		/* Return earliest timeframe from dataset*/
		std::string getEarliestTime();

		/* Return next timeframe of given timestamp*/
		std::string getNextTime(std::string timestamp);


	private:
		/* Order Book in vector form*/
		std::vector<OrderBookEntry> orders;

};

