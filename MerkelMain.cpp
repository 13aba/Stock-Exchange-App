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
    currentTime = orderBook.getEarliestTime();
    wallet.insertCurrency("BTC", 10);
    while (true) 
    {
        printMenu();
        input = getUserInput();
        processUserInput(input);
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
    try
    {
        userChoise = std::stoi(line);
    }
    catch (const std::exception& e)
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
    for (std::string const s : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << s << std::endl;
        std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, s, currentTime);
        std::cout << "Ask orders " << askEntries.size() << std::endl;

        std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, s, currentTime);
        std::cout << "Bid orders " << bidEntries.size() << std::endl;

        double min = orderBook.getLowestPrice(askEntries);
        double max = orderBook.getHighestPrice(bidEntries);
        std::cout << "Max bid price: " << max << std::endl;
        std::cout << "Min ask price: " << min << std::endl;
        std::cout << "Quoted Spread: " << orderBook.getQuotedSpread(min, max) << "%" << std::endl;
    }
}

void MerkelMain::makeOffer() {
    std::cout << "Make an ask: enter the amount: product, price, and amount. eg: ETH/BTC,200,0.5" << std::endl;
    std::string input;

    std::getline(std::cin, input);
    std::cout << "You typed " << input << std::endl;

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "Bad input for ask" << std::endl;
    }
    else 
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToBe(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask
            );
            obe.username = "simuser";
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
        catch (const std::exception&)
        {
            std::cout << "MerkelMain::makeOffer Bad input for entry!" << std::endl;
        }  
    }
}

void MerkelMain::makeBid() {
    std::cout << "Make a bid: enter the amount: product, price, and amount. eg: ETH/BTC,200,0.5" << std::endl;
    std::string input;

    std::getline(std::cin, input);
    std::cout << "You typed " << input << std::endl;

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "Bad input for bid" << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToBe(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid
            );
            obe.username = "simuser";
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
        catch (const std::exception&)
        {
            std::cout << "MerkelMain::makeBid Bad input for entry!" << std::endl;
        }
    }
}

void MerkelMain::checkWallet() {
    std::cout << "Wallet: " << wallet.walletInString() << std::endl;
}


void MerkelMain::goToNext() {

    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAskBid(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
    }

    currentTime = orderBook.getNextTime(currentTime);
    currentTime = orderBook.getNextTime(currentTime);
}

void MerkelMain::exit() {
    std::cout << "Goodbye" << std::endl;
}

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

