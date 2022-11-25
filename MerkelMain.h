#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class MerkelMain
{
	public:
		MerkelMain();
        /*Call this to start the sim*/
        void init();

    private:
        /* Print menu in the console*/
        void printMenu();
        /* Take user input from console*/
        int getUserInput();
        /* Print out warning if user added wrong input*/
        void printBadOption();
        /* Print out help options*/
        void printHelp();
        /* Print out exchange stats including rate, min ask, max bid, and quoted spread*/
        void printExchangeRate();
        /* Create offer entry from user input*/
        void makeOffer();
        /* Create bid entry from user input*/
        void makeBid();
        /* Display user wallet in the console*/
        void checkWallet();
        /* Process the deals in this timeframe and go to next one*/
        void goToNext();
        /* Close the programm*/
        void exit();
        /* Call the proper functions depending on the user input*/
        void processUserInput(int userChoise);

        std::string currentTime;

        OrderBook orderBook = OrderBook{ "20200317.csv" };
        Wallet wallet;
       
};

