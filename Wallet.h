#pragma once
#include <iostream>
#include <string>
#include <map>
#include "OrderBookEntry.h"
#include <vector>
#include "CSVReader.h"

class Wallet
{
	public: 
		Wallet();

		void insertCurrency(std::string type, double amount);

		bool removeCurrency(std::string, double amount);

		bool containsCurrency(std::string, double amount);

		bool canFulfillOrder(OrderBookEntry order);

		std::string walletInString();

		void processSale(OrderBookEntry& e);

	private: 

		std::map<std::string, double> currencies;
};

