#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"

class MerkelMain
{
	public:
		MerkelMain();
        /*Call this to start the sim*/
        void init();

    private:
        void loadOrderBook();
        void printMenu();
        int getUserInput();
        void printBadOption();
        void printHelp();
        void printExchangeRate();
        void makeOffer();
        void makeBid();
        void checkWallet();
        void goToNext();
        void exit();
        void processUserInput(int userChoise);

        std::string currentTime;

        OrderBook orderBook = OrderBook{ "20200317.csv" };
};

