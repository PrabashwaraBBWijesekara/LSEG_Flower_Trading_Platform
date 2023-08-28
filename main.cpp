#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iomanip>

// OrderBookEntry class
class OrderBookEntry {
public:
    std::string ID;
    std::string instrument;
    int side;
    double price;
    int quantity;
    std::string orderID;
    OrderBookEntry() {
    }

    OrderBookEntry(std::string ID, std::string instrument, int side, double price, int quantity)
        : ID(ID), instrument(instrument), side(side), price(price), quantity(quantity) {
    }
};

// OrderBook class
class OrderBook {
    public:

    std::vector<OrderBookEntry> orderBook;
    std::vector<OrderBookEntry> buy;
    std::vector<OrderBookEntry> sell;
    OrderBook(){}
  
};

// to sort buy book by highest price
bool compareByPriceAs(const OrderBookEntry a, const OrderBookEntry b) {
    return a.price < b.price;
}

// to sort sell book by lowest price
bool compareByPriceDs(const OrderBookEntry a, const OrderBookEntry b) {
    return a.price > b.price;
}

// to split the string by delimiter
std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
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
    std::ifstream file("ex7.csv");

    //writing file
    std::string file_path = "ex6.csv";
    std::ofstream file_out(file_path, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Could not open the file." << std::endl;
        return 1;
    }
    
    std::string line;

    OrderBook Rose;
    OrderBook Lavender;
    OrderBook Lotus;
    OrderBook Tulip;
    OrderBook Orchid;

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
            std::cout << "Reject" << std::endl;
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

        std::sort(currentOrderBook->buy.begin(), currentOrderBook->buy.end(), compareByPriceDs);
        std::sort(currentOrderBook->sell.begin(), currentOrderBook->sell.end(), compareByPriceAs);



        //matching algorithm
        bool matchFound = false;  // Initialize matchFound as false

        for (OrderBookEntry &buyOrder :currentOrderBook->buy) {
            for (OrderBookEntry &sellOrder :currentOrderBook->sell) {
                if (buyOrder.price >= sellOrder.price) {
                    if (buyOrder.quantity > sellOrder.quantity && sellOrder.quantity != 0) 

                    {
                        int temp =sellOrder.quantity;
                        buyOrder.quantity -= sellOrder.quantity;
                        sellOrder.quantity = 0;
                    
                        //add time
                        std::string timestamp = getTimestamp();
                        


                        std::cout << "pfill_1 "<<buyOrder.ID<<" "<<buyOrder.instrument<<std::endl;
                        std::cout << "filled " <<sellOrder.ID <<" "<<buyOrder.instrument<<std::endl;


                        //print to .csv file
                        file_out <<buyOrder.orderID<<","<<buyOrder.ID<<","<<buyOrder.instrument<<","<<buyOrder.side<<","<< "pfill_1"<<","<<temp<<","<<buyOrder.price<<","<<reject<<","<<timestamp<<std::endl;
                        file_out <<sellOrder.orderID<<","<<sellOrder.ID<<","<<sellOrder.instrument<<","<<sellOrder.side<<","<< "filled "<<","<<temp<<","<<buyOrder.price<<","<<reject<<","<<timestamp<<std::endl;

                        matchFound = true;  // Set matchFound to true when a match occurs
                    } 
                    else if (buyOrder.quantity < sellOrder.quantity && buyOrder.quantity != 0) {

                        int temp =buyOrder.quantity;
                        sellOrder.quantity -= buyOrder.quantity;
                        buyOrder.quantity = 0;
                        //add time
                        std::string timestamp = getTimestamp();
                        
                        
                        
                        
                        std::cout << "pfill_2 " <<sellOrder.ID <<" "<<buyOrder.instrument<<std::endl;
                        std::cout << "filled " <<buyOrder.ID<<" "<<buyOrder.instrument<<std::endl;

                        //print to .csv file
                        file_out <<sellOrder.orderID<<","<<sellOrder.ID<<","<<sellOrder.instrument<<","<<sellOrder.side<<","<< "pfill_2"<<","<<temp<<","<<buyOrder.price<<","<<reject<<","<<timestamp<<std::endl;
                        file_out <<buyOrder.orderID<<","<<buyOrder.ID<<","<<buyOrder.instrument<<","<<buyOrder.side<<","<< "filled"<<","<<temp<<","<<buyOrder.price<<","<<reject<<","<<timestamp<<std::endl;

                        matchFound = true;  // Set matchFound to true when a match occurs
                    } 
                    else if (buyOrder.quantity == sellOrder.quantity && buyOrder.quantity != 0 && sellOrder.quantity != 0) {
                        buyOrder.quantity = 0;
                        sellOrder.quantity = 0;

                        //add time
                        std::string timestamp = getTimestamp();
                        
                        int temp =buyOrder.quantity;


                        std::cout << "filled " <<buyOrder.ID <<" "<<buyOrder.instrument<<std::endl;
                        std::cout << "filled " <<sellOrder.ID <<" "<<buyOrder.instrument<<std::endl;

                        //print to .csv file
                        file_out <<buyOrder.orderID<<","<<buyOrder.ID<<","<<buyOrder.instrument<<","<<buyOrder.side<<","<< "filled"<<","<<temp<<","<<buyOrder.price<<","<<reject<<","<<timestamp<<std::endl;
                        file_out <<buyOrder.orderID<<","<<sellOrder.ID<<","<<sellOrder.instrument<<","<<sellOrder.side<<","<< "filled"<<","<<temp<<","<<buyOrder.price<<","<<reject<<","<<timestamp<<std::endl;

                        matchFound = true;  // Set matchFound to true when a match occurs
                    }
                    
                }
            }
        }

        if (!matchFound) {
        
            //add time
            std::string timestamp = getTimestamp();  

            std::cout<< "New "<< orderBookEntry.ID <<" "<<orderBookEntry.instrument<< std::endl;
            //print to .csv file
            file_out <<orderBookEntry.orderID<<","<<orderBookEntry.ID<<","<<orderBookEntry.instrument<<","<<orderBookEntry.side<<","<< "new"<<","<<orderBookEntry.quantity<<","<<orderBookEntry.price<<","<<reject<<","<<timestamp<<std::endl;
            } 
    }
        //std::cout << line << std::endl;

        file_out.close();
        file.close();
        
        return 0;   

}

        
    
        

