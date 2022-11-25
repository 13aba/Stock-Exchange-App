#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "CSVReader.h"

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    int input;
    //set current time to earliest one in the order book
    currentTime = orderBook.getEarliestTime();
    //add some currency to the user wallet
    wallet.insertCurrency("BTC", 10);
    wallet.insertCurrency("ETH", 100);
    wallet.insertCurrency("USDT", 3000);
    while (true) 
    {
        printMenu();
        input = getUserInput();
        processUserInput(input);
        //Break the loop if user input exit code
        if (input == 7) break;
    }
}


void MerkelMain::printMenu() {

    //Print help
    std::cout << "1: Print Help" << std::endl;
    //Print exchange stats
    std::cout << "2: Exchange Stats" << std::endl;
    //Make offer
    std::cout << "3: Make an ask" << std::endl;
    //Make bid
    std::cout << "4: Make a bid" << std::endl;
    //Print wallet
    std::cout << "5: Check wallet" << std::endl;
    //Continue
    std::cout << "6: Continue" << std::endl;
    //Exit
    std::cout << "7: Exit" << std::endl;
    //GAP
    std::cout << "================" << std::endl;
    std::cout << "Type 1-7" << std::endl;

    std::cout << "Current time is " << currentTime << std::endl;
}

int MerkelMain::getUserInput() {
    //User choice
    int userChoise = 0;
    std::string line;
    std::getline(std::cin, line);
    try //Get user choice from console line and convert it to integer
    {
        userChoise = std::stoi(line);
    }
    catch (const std::exception& e) //If there is problem converting
    {
        //No specific response since userChoice will returned as 0 and bad input message will be delivered by processUserInput
    }
    return userChoise;
}

void MerkelMain::printBadOption() {
    std::cout << "Invalid input. Please type between 1-7" << std::endl;
}

void MerkelMain::printHelp() {
    std::cout << "You need to make offer and bids to increase your wallet value" << std::endl;
}

void MerkelMain::printExchangeRate() {
    //Print stats for every product on the order book
    for (std::string const s : orderBook.getKnownProducts())
    {
        // Product name
        std::cout << "Product: " << s << std::endl;
        // Ask order size
        std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, s, currentTime);
        std::cout << "Ask orders " << askEntries.size() << std::endl;
        //Bid order size
        std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, s, currentTime);
        std::cout << "Bid orders " << bidEntries.size() << std::endl;
        //Minimum ask and maximum bid
        double min = orderBook.getLowestPrice(askEntries);
        double max = orderBook.getHighestPrice(bidEntries);
        std::cout << "Max bid price: " << max << std::endl;
        std::cout << "Min ask price: " << min << std::endl;
        //Qouted Spread
        std::cout << "Quoted Spread: " << orderBook.getQuotedSpread(min, max) << "%" << std::endl;
    }
}

void MerkelMain::makeOffer() {
    // Print out instruction
    std::cout << "Make an ask: enter the amount: product, price, and amount. eg: ETH/BTC,200,0.5" << std::endl;
    std::string input;
    // Get user input
    std::getline(std::cin, input);
    std::cout << "You typed " << input << std::endl;
    // Split the user input into string vector
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)  //Check if the input is in correct form
    {
        std::cout << "Bad input for ask" << std::endl;   //Print out warning if input is in incorrect form
    }
    else 
    {
        try // Try to create new order book entry out of user input
        {
            OrderBookEntry obe = CSVReader::stringsToBe(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask
            );  // New entry
            obe.username = "simuser";   //Set different username for the entry so it is distinct from data set entry
            if (wallet.canFulfillOrder(obe))  //Check if the user has enough fund to place the order
            {
                std::cout << "Order has been placed" << std::endl;
                orderBook.insertOrder(obe);
            }
            else 
            {
                std::cout << "Not enough fund" << std::endl;
            }
        }
        catch (const std::exception&) //Throw exception and warning if it is not possible to create new entry
        {
            std::cout << "MerkelMain::makeOffer Bad input for entry!" << std::endl;
        }  
    }
}

void MerkelMain::makeBid() {
    // Print out instruction
    std::cout << "Make a bid: enter the amount: product, price, and amount. eg: ETH/BTC,200,0.5" << std::endl;
    std::string input;
    // Get user input
    std::getline(std::cin, input);
    std::cout << "You typed " << input << std::endl;
    // Split the user input into string vector
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)     //Check if the input is in correct form
    {
        std::cout << "Bad input for bid" << std::endl;     //Print out warning if input is in incorrect form
    }
    else
    {
        try // Try to create new order book entry out of user input
        {
            OrderBookEntry obe = CSVReader::stringsToBe(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid
            );  // New entry
            //Set different username for the entry so it is distinct from data set entry
            obe.username = "simuser";

            
            //Check if the user has enough fund to place the order
            if (wallet.canFulfillOrder(obe)) 
            {
                std::cout << "Order has been placed" << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Not enough fund" << std::endl;
            }
        }
        catch (const std::exception&) //Throw exception and warning if it is not possible to create new entry
        {
            std::cout << "MerkelMain::makeBid Bad input for entry!" << std::endl;
        }
    }
}

void MerkelMain::checkWallet() {
    //Print out wallet in string form
    std::cout << "Wallet: " << wallet.walletInString() << std::endl;
}


void MerkelMain::goToNext() {

    std::cout << "Going to next time frame. " << std::endl;
    //Match the bids and sales for every products
    for (std::string p : orderBook.getKnownProducts())
    {
        std::vector<OrderBookEntry> sales = orderBook.matchAskBid(p, currentTime);
        std::cout << p << " sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            //Print out every sale that made
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                // update the wallet if sale is from user
                wallet.processSale(sale);
            }
        }
    }
    //Set the timer to next timeframe
    currentTime = orderBook.getNextTime(currentTime);
}

void MerkelMain::exit() {
    std::cout << "Goodbye" << std::endl;
}


//Handles user input and call the proper function
void MerkelMain::processUserInput(int userChoise) {
    if (userChoise == 1) 
    {
        printHelp();
    }
    else if (userChoise == 2) 
    {
        printExchangeRate();
    }
    else if (userChoise == 3) 
    {
        makeOffer();
    }
    else if (userChoise == 4) 
    {
        makeBid();
    }
    else if (userChoise == 5) 
    {
        checkWallet();
    }
    else if (userChoise == 6) 
    {
        goToNext();
    }
    else if (userChoise == 7) 
    {
        exit();
    }
    else 
    {
        printBadOption();
    }

   
}

