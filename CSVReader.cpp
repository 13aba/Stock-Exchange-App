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

    std::cout << "Please wait app is loading" << std::endl;
    if (csvFile.is_open()) //Check if the file is open
    {
        while (std::getline(csvFile, line)) //Try to tokenise the line if there is one
        {
            try
            {
                tokens = tokenise(line, ',');
                book.push_back(stringsToBe(tokens));  //Add the newly created entry to the book
            }
            catch (const std::exception& e) {
                std::cout << "bad line skipped" << std::endl;  //Let the user know if there was bad line
            }
        }
        csvFile.close(); 
    }
    else
    {
        std::cout << "trouble opening CSV File";   //If file does not open alert the user
    }
    std::cout << "Welcome to advisor bot application! Use help to for possible commands" << std::endl;

    return book;
}

std::vector<std::string> CSVReader::tokenise(std::string line, char separator) {

    std::vector<std::string> tokens;
    std::string token;
    signed int start, end;
    
    //Find the index of first character that is not separator
    start = line.find_first_not_of(separator, 0);
    do {
        //Find the index of separator
        end = line.find_first_of(separator, start);
        //If the start character is the last index break out of loop
        if (start == line.length() || start == end) break;
        //If there is separator left, split the string from start to that separator index
        if (end >= 0) 
        {
            token = line.substr(start, end - start);
        }
        //If there are no separator, split string from start to last character of the string
        else 
        {
            token = line.substr(start, line.length() - start);
        }
        //Add splitted strings to the vector
        tokens.push_back(token);
        //Move the start index after succesfull split
        start = end + 1;
    } while (end > 0);

    return tokens;
}

OrderBookEntry CSVReader::stringsToBe(std::vector<std::string> tokens) {
	double price;
	double amount;

	if (tokens.size() != 5) //Check if vector is correct size for entry
	{
		std::cout << "Bad Line: Not enough data in a line" << std::endl;
		throw std::exception{}; 
	}
	try  //try to create doulbe out of tokens strings
    {
		price = std::stod(tokens[3]);
		amount = std::stod(tokens[4]);
	}  //If the there is problem throw exception and warning
	catch (std::exception& e) {
        std::cout << "double did not work";
		throw;
	}
    //If everything is succesfull create new OrderBookEntry
	OrderBookEntry entry{ price, amount, tokens[0], tokens[1], OrderBookEntry::stringToOrderType(tokens[2])};

	return entry;
}

/* Same function as the last stringsToBe but recieves the strings directly instead of in vector form */
OrderBookEntry CSVReader::stringsToBe(std::string _price, 
                                        std::string _amount, 
                                        std::string _timestamp,      
                                        std::string _product, 
                                        OrderBookType _orderType) 
{
    double price;
    double amount;

    try     //try to create doulbe out of tokens strings
    {
        price = std::stod(_price);
        amount = std::stod(_amount);
    }    //If the there is problem throw exception and warning
    catch (const std::exception& e) {
        std::cout << "CSVReader::stringsToBe BAD FLOAT" << std::endl;
        throw;
    }

    //If everything is succesfull create new OrderBookEntry
    OrderBookEntry obe{ price, amount, _timestamp, _product, _orderType };

    return obe;

}