#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iomanip>

#include "OrderBookEntry.h"
#include "OrderBook.h"


// to sort buy book by highest price
bool compareByPriceAs(const OrderBookEntry a, const OrderBookEntry b) {
    return a.price < b.price;
}

// to sort sell book by lowest price
bool compareByPriceDs(const OrderBookEntry a, const OrderBookEntry b) {
    return a.price > b.price;
}

std::string getTimestamp() {
    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
    std::tm localTime = *std::localtime(&currentTimeT);
    std::ostringstream timestampStream;
    timestampStream << std::put_time(&localTime, "%Y%m%d-%H%M%S") << "." << std::setfill('0') << std::setw(3) << currentTime.time_since_epoch() / std::chrono::milliseconds(1) % 1000 ;
    std::string timestamp = timestampStream.str();
    return timestamp;
}

int main()
{
    // reading file
    std::ifstream file("Bigdataset.csv");

    //writing file
    std::ofstream file_out("execution_rep.csv", std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Could not open the file." << std::endl;
        return 1;
    }
    
    std::string line;

    OrderBook Rose,Lavender,Lotus,Tulip,Orchid;
    
   int IDint=0;
   
    while (std::getline(file, line)) {

        //rejected reason 
        std::string reject = "NA";

        //iterating through each line
        std::istringstream ss(line);
        std::string cell;
        std::vector<std::string> cells; // Store CSV values for the current line

        
        while (std::getline(ss, cell, ',')) {
            cells.push_back(cell);
        }
        OrderBookEntry orderBookEntry;

        // checking if the line is valid
        std::ostringstream errorMsgStream;
        try
        {
            orderBookEntry.ID = cells[0];
            orderBookEntry.instrument = cells[1];
            orderBookEntry.side = std::stoi(cells[2]);
            orderBookEntry.price = std::stod(cells[4]);
            orderBookEntry.quantity = std::stoi(cells[3]);
        }
        catch(const std::exception& e)
        {
            continue;
        }
        
        // adding the corresponding orderID to the orderBookEntry
        IDint++;
        orderBookEntry.orderID="Ord"+std::to_string(IDint);

        if (cells[0].substr(0, 2) != "aa"){
            reject="Rejected";
            std::string timestamp = getTimestamp();
            file_out <<orderBookEntry.orderID<<","<<"-"<<","<<orderBookEntry.instrument<<","<<orderBookEntry.side<<","<< reject <<","<<orderBookEntry.quantity<<","<<orderBookEntry.price<<","<<"Invalid ID"<<","<<timestamp<<std::endl;
            continue;
        }
        else if ( orderBookEntry.quantity % 10!=0 || orderBookEntry.quantity < 0 || orderBookEntry.quantity > 1000){
            reject="Rejected";
            std::string timestamp = getTimestamp();
            file_out <<orderBookEntry.orderID<<","<<orderBookEntry.ID<<","<<orderBookEntry.instrument<<","<<orderBookEntry.side<<","<< reject <<","<<orderBookEntry.quantity<<","<<orderBookEntry.price<<","<<"Invalid Quantity"<<","<<timestamp<<std::endl;
            continue;
        }
        else if (orderBookEntry.price<0){
            reject="Rejected";
            std::string timestamp = getTimestamp();
            file_out <<orderBookEntry.orderID<<","<<orderBookEntry.ID<<","<<orderBookEntry.instrument<<","<<orderBookEntry.side<<","<< reject <<","<<orderBookEntry.quantity<<","<<orderBookEntry.price<<","<<"Invalid Price"<<","<<timestamp<<std::endl;
            continue;
        }

        // selecting current orderbook
        OrderBook *currentOrderBook = nullptr;
        if(cells[1] == "Rose"){
            currentOrderBook = &Rose;  
        }
        else if(cells[1] == "Lavender"){
            currentOrderBook = &Lavender;
        }
        else if(cells[1] == "Lotus"){
            currentOrderBook = &Lotus;
        }
        else if(cells[1] == "Tulip"){
            currentOrderBook = &Tulip;
        }
        else if(cells[1] == "Orchid"){
            currentOrderBook = &Orchid;
        }
        else{
            std::string timestamp = getTimestamp();
            file_out <<orderBookEntry.orderID<<","<<orderBookEntry.ID<<","<<"-"<<","<<orderBookEntry.side<<","<< "Rejected"<<","<<orderBookEntry.quantity<<","<<orderBookEntry.price<<","<<"Invalid Instrument"<<","<<timestamp<<std::endl;
            continue;
        }
       
        currentOrderBook->orderBook.push_back(orderBookEntry); 
        
        if (orderBookEntry.side == 1) {
            currentOrderBook->buy.push_back(orderBookEntry);
        } else if (orderBookEntry.side == 2) {
            currentOrderBook->sell.push_back(orderBookEntry);
        }
        else{reject="Rejected";
            std::string timestamp = getTimestamp();
            file_out <<orderBookEntry.orderID<<","<<orderBookEntry.ID<<","<<orderBookEntry.instrument<<","<<" "<<","<< reject <<","<<orderBookEntry.quantity<<","<<orderBookEntry.price<<","<<"Invalid Side"<<","<<timestamp<<std::endl;
            continue;}

        //sorting the orderbook
        if(orderBookEntry.side==1){
            std::sort(currentOrderBook->buy.begin(), currentOrderBook->buy.end(), compareByPriceDs);
        }
        else if(orderBookEntry.side==2){
            std::sort(currentOrderBook->sell.begin(), currentOrderBook->sell.end(), compareByPriceAs);
        }

        //matching algorithm
        bool matchFound = false;  // Initialize matchFound as false

        for (int i = 0; i < currentOrderBook->buy.size(); ++i) {
            for (int j = 0; j < currentOrderBook->sell.size(); ++j) {
                if (currentOrderBook->buy[i].price >= currentOrderBook->sell[j].price) {
                    if (currentOrderBook->buy[i].quantity > currentOrderBook->sell[j].quantity && currentOrderBook->sell[j].quantity != 0 ) 

                    {
                        int temp =currentOrderBook->sell[j].quantity;
                        currentOrderBook->buy[i].quantity -= currentOrderBook->sell[j].quantity;
                        currentOrderBook->sell[j].quantity = 0;
                    
                        //add time
                        std::string timestamp = getTimestamp();
                        
                        //print to .csv file
                        if(stoi(currentOrderBook->buy[i].orderID.substr(3,-1))>stoi(currentOrderBook->sell[j].orderID.substr(3,-1))){
                            file_out <<currentOrderBook->buy[i].orderID<<","<<currentOrderBook->buy[i].ID<<","<<currentOrderBook->buy[i].instrument<<","<<currentOrderBook->buy[i].side<<","<< "PFill"<<","<<temp<<","<<currentOrderBook->sell[i].price<<","<<reject<<","<<timestamp<<std::endl;
                            file_out <<currentOrderBook->sell[j].orderID<<","<<currentOrderBook->sell[j].ID<<","<<currentOrderBook->sell[j].instrument<<","<<currentOrderBook->sell[j].side<<","<< "Fill"<<","<<temp<<","<<currentOrderBook->sell[i].price<<","<<reject<<","<<timestamp<<std::endl;
                        }
                        else{
                            file_out <<currentOrderBook->sell[j].orderID<<","<<currentOrderBook->sell[j].ID<<","<<currentOrderBook->sell[j].instrument<<","<<currentOrderBook->sell[j].side<<","<< "Fill"<<","<<temp<<","<<currentOrderBook->buy[i].price<<","<<reject<<","<<timestamp<<std::endl;
                            file_out <<currentOrderBook->buy[i].orderID<<","<<currentOrderBook->buy[i].ID<<","<<currentOrderBook->buy[i].instrument<<","<<currentOrderBook->buy[i].side<<","<< "PFill"<<","<<temp<<","<<currentOrderBook->buy[i].price<<","<<reject<<","<<timestamp<<std::endl;
                        }
                        
                        
                        //erase the order from the orderbook
                        currentOrderBook->sell.erase(currentOrderBook->sell.begin()+j);
                        
                        matchFound = true;  // Set matchFound to true when a match occurs
                    } 
                    else if (currentOrderBook->buy[i].quantity < currentOrderBook->sell[j].quantity && currentOrderBook->buy[i].quantity != 0 ) {

                        int temp =currentOrderBook->buy[i].quantity;
                        currentOrderBook->sell[j].quantity -= currentOrderBook->buy[i].quantity;
                        currentOrderBook->buy[i].quantity = 0;
                        
                        //add time
                        std::string timestamp = getTimestamp();
                        
                        //print to .csv file
                        if(stoi(currentOrderBook->buy[i].orderID.substr(3,-1))>stoi(currentOrderBook->sell[j].orderID.substr(3,-1))){
                            file_out <<currentOrderBook->buy[i].orderID<<","<<currentOrderBook->buy[i].ID<<","<<currentOrderBook->buy[i].instrument<<","<<currentOrderBook->buy[i].side<<","<< "Fill"<<","<<temp<<","<<currentOrderBook->sell[i].price<<","<<reject<<","<<timestamp<<std::endl;
                            file_out <<currentOrderBook->sell[j].orderID<<","<<currentOrderBook->sell[j].ID<<","<<currentOrderBook->sell[j].instrument<<","<<currentOrderBook->sell[j].side<<","<< "PFill"<<","<<temp<<","<<currentOrderBook->sell[i].price<<","<<reject<<","<<timestamp<<std::endl;
                        }
                        else{
                            file_out <<currentOrderBook->sell[j].orderID<<","<<currentOrderBook->sell[j].ID<<","<<currentOrderBook->sell[j].instrument<<","<<currentOrderBook->sell[j].side<<","<< "PFill"<<","<<temp<<","<<currentOrderBook->buy[i].price<<","<<reject<<","<<timestamp<<std::endl;
                            file_out <<currentOrderBook->buy[i].orderID<<","<<currentOrderBook->buy[i].ID<<","<<currentOrderBook->buy[i].instrument<<","<<currentOrderBook->buy[i].side<<","<< "Fill"<<","<<temp<<","<<currentOrderBook->buy[i].price<<","<<reject<<","<<timestamp<<std::endl;
                        }

                        //erase the order from the orderbook
                        currentOrderBook->buy.erase(currentOrderBook->buy.begin()+i);

                        matchFound = true;  // Set matchFound to true when a match occurs
                    } 
                    else if (currentOrderBook->buy[i].quantity == currentOrderBook->sell[j].quantity && currentOrderBook->buy[i].quantity != 0) {
                        

                        //add time
                        std::string timestamp = getTimestamp();
                        
                        int temp =currentOrderBook->buy[i].quantity;

                        //print to .csv file
                        if(stoi(currentOrderBook->buy[i].orderID.substr(3,-1))>stoi(currentOrderBook->sell[j].orderID.substr(3,-1))){
                            file_out <<currentOrderBook->buy[i].orderID<<","<<currentOrderBook->buy[i].ID<<","<<currentOrderBook->buy[i].instrument<<","<<currentOrderBook->buy[i].side<<","<< "Fill"<<","<<temp<<","<<currentOrderBook->sell[i].price<<","<<reject<<","<<timestamp<<std::endl;
                            file_out <<currentOrderBook->sell[j].orderID<<","<<currentOrderBook->sell[j].ID<<","<<currentOrderBook->sell[j].instrument<<","<<currentOrderBook->sell[j].side<<","<< "Fill"<<","<<temp<<","<<currentOrderBook->sell[i].price<<","<<reject<<","<<timestamp<<std::endl;
                        }
                        else{
                            file_out <<currentOrderBook->sell[j].orderID<<","<<currentOrderBook->sell[j].ID<<","<<currentOrderBook->sell[j].instrument<<","<<currentOrderBook->sell[j].side<<","<< "Fill"<<","<<temp<<","<<currentOrderBook->buy[i].price<<","<<reject<<","<<timestamp<<std::endl;
                            file_out <<currentOrderBook->buy[i].orderID<<","<<currentOrderBook->buy[i].ID<<","<<currentOrderBook->buy[i].instrument<<","<<currentOrderBook->buy[i].side<<","<< "Fill"<<","<<temp<<","<<currentOrderBook->buy[i].price<<","<<reject<<","<<timestamp<<std::endl;
                        }
                        currentOrderBook->buy[i].quantity = 0;
                        currentOrderBook->sell[j].quantity = 0;
                        
                        //erase the order from the orderbook
                        currentOrderBook->buy.erase(currentOrderBook->buy.begin()+i);
                        currentOrderBook->sell.erase(currentOrderBook->sell.begin()+j);
                        
                        matchFound = true;  // Set matchFound to true when a match occurs
                    }
                    
                }
            }
        }

        if (!matchFound) {
        
            //add time
            std::string timestamp = getTimestamp();  

            //print to .csv file
            file_out <<orderBookEntry.orderID<<","<<orderBookEntry.ID<<","<<orderBookEntry.instrument<<","<<orderBookEntry.side<<","<< "New"<<","<<orderBookEntry.quantity<<","<<orderBookEntry.price<<","<<reject<<","<<timestamp<<std::endl;
            } 
    }
        //std::cout << line << std::endl;

        file_out.close();
        file.close();
        
        return 0;   

}

        
    
        

