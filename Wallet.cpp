#include "Wallet.h"

Wallet::Wallet() 
{

}

void Wallet::insertCurrency(std::string type, double amount)
{
	
	double balance;
	//Throw exception if user tries to add negative amount to wallet
	if (amount < 0)
	{
		throw std::exception{};
	}
	//Check if the given currency is in wallet 
	if (currencies.count(type) == 0)
	{
		balance = 0; //If it does not exist clear the balance
	}
	else
	{
		balance = currencies[type];  //If it does exist save the amount of it
	}

	balance += amount;  //Increase the balance by given amount

	currencies[type] = balance;  //Assign the new balance back into the wallet map
}

bool Wallet::removeCurrency(std::string type, double amount)
{
	//Throw exception if user tries to add negative amount to wallet
	if (amount < 0)
	{
		throw std::exception{};
	}
	//Check if the given currency is in wallet
	if (currencies.count(type) == 0)
	{
		return false;  //If it does not exist exit the function with false return
	}
	else
	{
		if (containsCurrency(type, amount)) //If it does exist check if its possible to remove the given amount
		{
			currencies[type] -= amount; //If its possible update the wallet map
			return true; 
		}
		else
			return false; //If wallet fund is insufficient return false
	}
}

bool Wallet::containsCurrency(std::string type, double amount)
{
	if (currencies.count(type) == 0) //Check if wallet has given product
		return false;
	else
		return currencies[type] >= amount; //If yes check if given amount is higher than wallet fund
}

bool Wallet::canFulfillOrder(OrderBookEntry order)
{
	std::vector<std::string> currencies = CSVReader::tokenise(order.product, '/');

	if (order.orderType == OrderBookType::ask)
	{
		return containsCurrency(currencies[0], order.amount);
	}
	if (order.orderType == OrderBookType::bid)
	{
		return containsCurrency(currencies[1], order.amount * order.price);
	}

	return false;
}

std::string Wallet::walletInString()
{
	std::string walletString;
	for (std::pair<std::string, double> pair : currencies)
	{
		std::string currency = pair.first;  
		double amount = pair.second;
		walletString += currency + " : " + std::to_string(amount) + "\n"; //Add every product name and amount to one big string
	}
	return walletString; //Return the one big string
}

void Wallet::processSale(OrderBookEntry& sale)
{
	//Tokenise the order entry into strings
	std::vector<std::string> currenciesReader = CSVReader::tokenise(sale.product, '/');

	if (sale.orderType == OrderBookType::asksale)
	{
		//Out going fund and amount
		double outgoingAmount = sale.amount;
		std::string outgoingCurrency = currenciesReader[0];
		//Incoming fund and amount
		double incomingAmount = sale.amount * sale.price;
		std::string incomingCurrency = currenciesReader[1];
		//Update the wallet map with new funds
		currencies[incomingCurrency] += incomingAmount;  
		currencies[outgoingCurrency] -= outgoingAmount;
	}
	if (sale.orderType == OrderBookType::bidsale)
	{
		//Out going fund and amount
		double outgoingAmount = sale.amount*sale.price;
		std::string outgoingCurrency = currenciesReader[1];
		//Incoming fund and amount
		double incomingAmount = sale.amount;
		std::string incomingCurrency = currenciesReader[0];
		//Update the wallet map with new funds
		currencies[incomingCurrency] += incomingAmount;
		currencies[outgoingCurrency] -= outgoingAmount;
	}
}
