#include "Wallet.h"

Wallet::Wallet() 
{

}

void Wallet::insertCurrency(std::string type, double amount)
{
	double balance;
	if (amount < 0)
	{
		throw std::exception{};
	}
	if (currencies.count(type) == 0)
	{
		balance = 0;
	}
	else
	{
		balance = currencies[type];
	}

	balance += amount;

	currencies[type] = balance;
}

bool Wallet::removeCurrency(std::string type, double amount)
{
	if (amount < 0)
	{
		throw std::exception{};
	}
	if (currencies.count(type) == 0)
	{
		return false;
	}
	else
	{
		if (containsCurrency(type, amount))
		{
			currencies[type] -= amount;
			return true;
		}
		else
			return false;
	}
}

bool Wallet::containsCurrency(std::string type, double amount)
{
	if (currencies.count(type) == 0)
		return false;
	else
		return currencies[type] >= amount;
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
		walletString += currency + " : " + std::to_string(amount) + "\n";
	}
	return walletString;
}
