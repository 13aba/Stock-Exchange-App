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
        std::vector<std::string>  getUserInput();
        /* Print out warning if user added wrong input*/
        void printBadOption();
        /* Print out help options*/
        void printHelp();
        /* Print out detailed help for specific command*/
        void printDetailedHelp(std::string command);
        /* Print all products*/
        void printProducts();
        /*Find spread for given product*/
        void printSpread(std::string product);
        /*Find min for given type and product*/
        void printMin(OrderBookType type, std::string product);
        /*Find min for given type and product*/
        void printMax(OrderBookType type, std::string product);
        /*Print average of type and product over the specified timestep*/
        void printAvg(OrderBookType type, std::string product, std::string timesteps);
        /*Go to next time step*/
        void goToNext();
        /* Call the proper functions depending on the user input*/
        void processUserInput(std::vector<std::string>  userChoise);
        /*Current time in string form*/
        std::string currentTime;
        /*Vector containing previous time frames*/
        std::vector<std::string> timebank;

        OrderBook orderBook = OrderBook{ "20200601.csv" };
        Wallet wallet;
       
};

