#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "CSVReader.h"

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    std::vector<std::string> input;
    //set current time to earliest one in the order book
    currentTime = orderBook.getEarliestTime();
    while (true) 
    {
        input = getUserInput();
        processUserInput(input);
        //Space for better readibility
        std::cout << "" << std::endl;
    }
}

std::vector<std::string>  MerkelMain::getUserInput() {
    //User choice
    std::vector<std::string> userInput;
    std::string line;
    std::getline(std::cin, line);
    try //Get user choice from console line and convert it to integer
    {
        userInput = CSVReader::tokenise(line, ' ');
    }
    catch (const std::exception& e) //If there is problem converting
    {
        std::cout << "problem" << std::endl;
        //No specific response since userChoice will returned as 0 and bad input message will be delivered by processUserInput
    }
    return userInput;
}


//Handles user input and call the proper function
void MerkelMain::processUserInput(std::vector<std::string>  userChoise) {
     std::string pointer = userChoise[0];
        if (pointer == "help") 
        {
            if (userChoise.size() == 1)
            {
                printHelp();
            }
            else if (userChoise.size() == 2)
            {
                printDetailedHelp(userChoise[1]);
            }
            else
            {
                printBadOption();
            }
        }
        else if (pointer == "prod")
        {
            printProducts();
        }
        else if (pointer == "time")
        {
            std::cout << "Current time is: " << currentTime << std::endl;
        }
        else if (pointer == "step")
        {
            goToNext();
        }
        else if (pointer == "spread")
        {
            printSpread(userChoise[1]);
        }
        else if (pointer == "min")
        {
            OrderBookType type = OrderBookEntry::stringToOrderType(userChoise[2]);
            printMin(type , userChoise[1]);
        }
        else if (pointer == "max")
        {
            OrderBookType type = OrderBookEntry::stringToOrderType(userChoise[2]);
            printMax(type, userChoise[1]);
        }
        else if (pointer == "avg")
        {
            std::cout << "avg" << std::endl;
        }
        else if (pointer == "predict")
        {
            std::cout << "predict" << std::endl;
        }
        else
        {
            printBadOption();
        }
}

void MerkelMain::printHelp() {
    std::cout << "Possible commands are: prod, time, step, spread, max, min, avg, predict." << std::endl;
    std::cout << "Use help <cmd> for more detailed explanation of commands. eg: help max" << std::endl;
}

void MerkelMain::printDetailedHelp(std::string command) {
    if (command == "prod")
    {
        std::cout << "prod: Gives the list of all products" << std::endl;
    }
    else if (command == "time")
    {
        std::cout << "time: Current time of the simulation" << std::endl;
    }
    else if (command == "step")
    {
        std::cout << "step: Go to next time frame of the simulation" << std::endl;
    }
    else if (command == "spread")
    {
        std::cout << "spread: Calculates quoted spread of the current time step. example: spread BTC/USDT" << std::endl;
    }
    else if (command == "min")
    {
        std::cout << "min: Finds the smallest ask/bid for any product. example: min BTC/USDT ask" << std::endl;
    }
    else if (command == "max")
    {
        std::cout << "max: Finds the biggest ask/bid for any product. example: min BTC/USDT ask" << std::endl;
    }
    else if (command == "avg")
    {
        std::cout << "avg: Finds the average ask/bid for any product in certain timestep. example: avg BTC/USDT ask 10" << std::endl;
    }
    else if (command == "predict")
    {
        std::cout << "predict: Predicts the min/max of ask/bid for any product. example: predict max BTC/USDT ask" << std::endl;
    }
    else
    {
        printBadOption();
    }
}

void MerkelMain::printBadOption() {
    std::cout << "Invalid input. Use help for all the commands list" << std::endl;
}

void MerkelMain::printProducts() {
    std::string product = "";
    for (std::string s : orderBook.getKnownProducts())
    {
        product = product + " " + s;
    }
    std::cout << "All products:" << product << std::endl;
}

void MerkelMain::goToNext() {

    std::cout << "Going to next time frame. " << std::endl;
    //Set the timer to next timeframe
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "Time is : " << currentTime << std::endl;
}

void MerkelMain::printSpread(std::string product) {
    //Filter order book with given type and product
    std::vector<OrderBookEntry> askOrders = orderBook.getOrders(OrderBookType::ask, product, currentTime);
    std::vector<OrderBookEntry> bidOrders = orderBook.getOrders(OrderBookType::bid, product, currentTime);
    //If user input correct product
    if (askOrders.size() > 0)
    {
        //Find minimum ask and maximum bid
        double minAsk = orderBook.getLowestPrice(askOrders);
        double maxBid = orderBook.getHighestPrice(bidOrders);
        //Find spread 
        double spread = orderBook.getQuotedSpread(minAsk, maxBid);
        //Print spread
        std::cout << "Quoted spread for " << product << " product is: " << spread << std::endl;
    }
    else  //If user inputted wrong product or there is no order matching that product
    {
        std::cout << "Could not find orders with given product! Please check and try again" << std::endl;
    }
}

void MerkelMain::printMin(OrderBookType type, std::string product) {
    //Filter order book with given type and product
    std::vector<OrderBookEntry> filteredOrders = orderBook.getOrders(type, product, currentTime);
    //Incase user entered wrong type and product 
    if (filteredOrders.size() == 0)
    {
        std::cout << "User input invalid! Please enter input in these order: min product orderType" << std::endl;
    }
    else //If the input is correst
    {
        //Find the min price from filtered orders
        double min = orderBook.getLowestPrice(filteredOrders);
        if (type == OrderBookType::bid)  //If the input is bid
        {
            std::cout << "Minimum bid price for " << product << " is: " << min << std::endl;
        }
        if (type == OrderBookType::ask) //If the input is ask
        {
            std::cout << "Minimum ask price for " << product << " is: " << min << std::endl;
        }
    }
}

void MerkelMain::printMax(OrderBookType type, std::string product) {
    //Filter order book with given type and product
    std::vector<OrderBookEntry> filteredOrders = orderBook.getOrders(type, product, currentTime);
    //Incase user entered wrong type and product 
    if (filteredOrders.size() == 0)
    {
        std::cout << "User input invalid! Please enter input in these order: max product orderType" << std::endl;
    }
    else //If the input is correst
    {
        //Find the max price from filtered orders
        double max = orderBook.getHighestPrice(filteredOrders);
        if (type == OrderBookType::bid)  //If the input is bid
        {
            std::cout << "Maximum bid price for " << product << " is: " << max << std::endl;
        }
        if (type == OrderBookType::ask) //If the input is ask
        {
            std::cout << "Maximum ask price for " << product << " is: " << max << std::endl;
        }
    }
}

void MerkelMain::exit() {
    std::cout << "Goodbye" << std::endl;
}

