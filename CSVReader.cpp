#include "CSVReader.h"
#include <iostream>
#include <fstream>
CSVReader::CSVReader() {

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFileName) {
    std::ifstream csvFile{csvFileName};

    std::string line;

    std::vector<std::string> tokens;

    std::vector<OrderBookEntry> book;

    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {
            try
            {
                tokens = tokenise(line, ',');
                book.push_back(stringsToBe(tokens));
            }
            catch (const std::exception& e) {
                std::cout << "bad line skipped" << std::endl;
            }
        }
        csvFile.close();
    }
    else
    {
        std::cout << "trouble opening CSV File";
    }
    std::cout << "CSVReader::readCSV " << book.size() << std::endl;

    return book;
}

std::vector<std::string> CSVReader::tokenise(std::string line, char separator) {

    std::vector<std::string> tokens;
    std::string token;
    signed int start, end;

    start = line.find_first_not_of(separator, 0);
    do {
        end = line.find_first_of(separator, start);

        if (start == line.length() || start == end) break;
        if (end >= 0) 
        {
            token = line.substr(start, end - start);
        }
        else 
        {
            token = line.substr(start, line.length() - start);
        }
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}

OrderBookEntry CSVReader::stringsToBe(std::vector<std::string> tokens) {
	double price;
	double amount;

	if (tokens.size() != 5)
	{
		std::cout << "Bad Line: Not enough data in a line" << std::endl;
		throw std::exception{};
	}
	try 
    {
		price = std::stod(tokens[3]);
		amount = std::stod(tokens[4]);
	}
	catch (std::exception& e) {
        std::cout << "double did not work";
		throw;
	}
	OrderBookEntry entry{ price, amount, tokens[0], tokens[1], OrderBookEntry::stringToOrderType(tokens[2])};

	return entry;
}

OrderBookEntry CSVReader::stringsToBe(std::string _price, 
                                        std::string _amount, 
                                        std::string _timestamp,      
                                        std::string _product, 
                                        OrderBookType _orderType) 
{
    double price;
    double amount;

    try
    {
        price = std::stod(_price);
        amount = std::stod(_amount);
    }
    catch (const std::exception& e) {
        std::cout << "CSVReader::stringsToBe BAD FLOAT" << std::endl;
        throw;
    }

    OrderBookEntry obe{ price, amount, _timestamp, _product, _orderType };

    return obe;

}