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
    timebank.push_back(currentTime);
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
            if (userChoise.size() == 2)
            {
                printSpread(userChoise[1]);
            }
            else
            {
                printBadOption();
            }
        }
        else if (pointer == "min")
        {
            if (userChoise.size() == 3)
            {
                OrderBookType type = OrderBookEntry::stringToOrderType(userChoise[2]);
                printMin(type, userChoise[1]);
            }
            else
            {
                printBadOption();
            }
        }
        else if (pointer == "max")
        {
            if (userChoise.size() == 3)
            {
                OrderBookType type = OrderBookEntry::stringToOrderType(userChoise[2]);
                printMax(type, userChoise[1]);
            }
            else
            {
                printBadOption();
            }
        }
        else if (pointer == "avg")
        {
            if (userChoise.size() == 4)
            {
                OrderBookType type = OrderBookEntry::stringToOrderType(userChoise[2]);
                printAvg(type, userChoise[1], userChoise[3]);
            }
            else
            {
                printBadOption();
            }
            
        }
        else if (pointer == "predict")
        {
            if (userChoise.size() == 4)
            {
                OrderBookType type = OrderBookEntry::stringToOrderType(userChoise[3]);
                printPredict(type, userChoise[2], userChoise[1]);
            }
            else
            {
                printBadOption();
            }
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

    //Set the timer to next timeframe
    currentTime = orderBook.getNextTime(currentTime);
    //Add current time to timebank
    timebank.push_back(currentTime);
    //Print out the time to user
    std::cout << "Going to next time frame. " << std::endl;
    std::cout << "Time is : " << currentTime << std::endl;
    std::cout << timebank.size() << " :steps since simulation started" << std::endl;
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

void MerkelMain::printAvg(OrderBookType type, std::string product, std::string timesteps) {
    //Check if given string is convertable to integer
    try 
    {
        int steps = stoi(timesteps);
        //If user input bigger steps for average than the time passed
        if (timebank.size() < steps)
        {
            std::cout << "Please enter smaller step than time frames passed since simulation started" << std::endl;
            std::cout << "Current time frame is: " << timebank.size() << " step/s since simulation started" << std::endl;
        }
        else // if step input is valid
        {
            //New order book for to average
            std::vector<OrderBookEntry> averageOrders;
            //Add specified amount of last time frame to new order book
            for (int i=0 ; i< steps; i++)
            {
                std::vector<OrderBookEntry> newOrders = orderBook.getOrders(type, product, timebank[timebank.size()-1-i]);
                averageOrders.insert(averageOrders.end(), newOrders.begin(), newOrders.end());  
            }
            //If user input wrong type or product there will be 0 orders
            if (averageOrders.size() == 0)
            {
                std::cout << "Wrong type or product name entered! Please check and try again" << std::endl;
            }
            else //if input is valid 
            {
                //Find average using order book function
                double average = orderBook.getAverage(averageOrders);
                //Print out the average
                std::cout << "Average price of " << product << " in: " << steps << " time step is: " << average << std::endl;
            }
        }
    } //If not let user know
    catch(const std::exception& e)
    {
        std::cout << "Please enter integer for steps needed for average" << std::endl;
    }
    
}

void MerkelMain::printPredict(OrderBookType type, std::string product, std::string minMax) {
    
    try
    {
        std::cout << predictSmooth(timebank.size() - 1, type, product, minMax) << std::endl;
    }
    catch (std::invalid_argument& e)
    {
        std::cout << e.what() << std::endl;
    }
}

double MerkelMain::predictSmooth(int index, OrderBookType type, std::string product, std::string minMax) {
    //Prediction
    double prediction;
    //Exponential smoothing factor
    double alpha = 0.2;
    //Filter the order book for given type and product
    std::vector<OrderBookEntry> filteredOrders = orderBook.getOrders(type, product, timebank[index]);
    //Double to hold current value
    double currentValue;
    //Check if user inputted correct type and product 
    if (filteredOrders.size() == 0) 
    {
        throw std::invalid_argument("Invalid type or product please check and try again!");
    }
    //Check if min or max should be calculated
    if (minMax == "max") //If max
    {
        currentValue = orderBook.getHighestPrice(filteredOrders);
    }
    else if (minMax == "min") //IF min
    {
        currentValue = orderBook.getLowestPrice(filteredOrders);
    }
    else //If user input is invalid
    {
        throw std::invalid_argument("Input should be either max or min");
    }
    //If index is higher than 0 we have not reached first time frame so recurse the function
    if (index > 0)
    {

        prediction = alpha * currentValue + (1 - alpha) * predictSmooth(index - 1, type, product, minMax);
    }
    else //If index reached 0 prediction is equal to current value of the item and we stop recursing 
    {
        prediction = currentValue;
    }
    return prediction;
}