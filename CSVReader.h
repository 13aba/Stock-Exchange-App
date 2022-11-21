#pragma once

#include "OrderBookEntry.h"
#include <vector>
class CSVReader
{
	public:
		CSVReader();

		static std::vector<OrderBookEntry> readCSV(std::string csvFile);
		static std::vector<std::string> tokenise(std::string line, char separator);
		static OrderBookEntry stringsToBe(std::string price, std::string amount, std::string timestamp, std::string product, OrderBookType orderType);

	private: 
		
		static OrderBookEntry stringsToBe(std::vector < std::string> strings);
};

