#include "OrderBook.h"

OrderBook::OrderBook(std::string fileName){
	orders = CSVReader::readCSV(fileName);
}

std::vector<std::string> OrderBook::getKnownProducts() {
	std::vector<std::string> vectors;
	std::cout << "printed from getKnownProducts in orderbook cpp" << std::endl;
	return vectors;
}

std::vector<std::string> OrderBook::getOrders(OrderBookType type, std::string product, std::string timestamp) {
	std::vector<std::string> book;
	return book;
}
