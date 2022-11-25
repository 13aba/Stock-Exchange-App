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

		/* Check and match if there is compatible bid and ask*/
		std::vector<OrderBookEntry> matchAskBid(std::string products, std::string timestamp);

		/* Get highest bid from given orders vector*/
		static double getHighestPrice(std::vector<OrderBookEntry>& orders);

		/* Get lowest ask from given orders*/
		static double getLowestPrice(std::vector<OrderBookEntry>& orders);

		/* Get qouted spread of bid-ask*/
		static double getQuotedSpread(double min, double max);

		/* Return earliest timeframe from dataset*/
		std::string getEarliestTime();

		/* Return next timeframe of given timestamp*/
		std::string getNextTime(std::string timestamp);

		/* insert order to the book*/
		void insertOrder(OrderBookEntry order);

	private:
		/* Order Book in vector form*/
		std::vector<OrderBookEntry> orders;

};

