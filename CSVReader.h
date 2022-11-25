#pragma once

#include "OrderBookEntry.h"
#include <vector>
class CSVReader
{
	public:

		//Constructor
		CSVReader();

		/* Open CSV file from string name and add every line to vector*/
		static std::vector<OrderBookEntry> readCSV(std::string csvFile);

		/*Takes string line and split it according to given character */
		static std::vector<std::string> tokenise(std::string line, char separator);

		/* Create OrderBookEntry out of given parameters*/
		static OrderBookEntry stringsToBe(std::string price, std::string amount, std::string timestamp, std::string product, OrderBookType orderType);

	private: 
		
		/*Create ORderBookEntry out of given string*/
		static OrderBookEntry stringsToBe(std::vector < std::string> strings);
};

