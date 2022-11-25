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

		/* Insert currency or add to existing one in the wallet*/
		void insertCurrency(std::string type, double amount);

		/* Remove currecy in the wallet*/
		bool removeCurrency(std::string, double amount);

		/* Check if the wallet has given amount of currency at the moment*/
		bool containsCurrency(std::string, double amount);

		/* Check if wallet can fund the given order*/
		bool canFulfillOrder(OrderBookEntry order);

		/* Display wallet in string form*/
		std::string walletInString();

		/* Modify the wallet according to order book entry made by user*/
		void processSale(OrderBookEntry& e);

	private: 
		// Wallet in map form
		std::map<std::string, double> currencies;
};

