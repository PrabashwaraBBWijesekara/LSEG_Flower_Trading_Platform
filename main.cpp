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
    timestampStream << std::put_time(&localTime, "%Y%m%d-%H%M%S") << "." << std::setfill('0') << std::setw(4) << currentTime.time_since_epoch() / std::chrono::milliseconds(1) % 1000 ;
    std::string timestamp = timestampStream.str();
    return timestamp;
}

int main()
{
    // reading file
    std::ifstream file("ex7_1.csv");

    //writing file
    std::string file_path = "ex6.csv";
    std::ofstream file_out(file_path, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Could not open the file." << std::endl;
        return 1;
    }
    
    std::string line;
    std::cout<<"cdmnjrvnh"<<std::endl;

    OrderBook Rose;
    OrderBook Lavender;
    OrderBook Lotus;
    OrderBook Tulip;
    OrderBook Orchid;
   
    while (std::getline(file, line)) {

        //time settings
        


        std::istringstream ss(line);
        std::string cell;
        std::vector<std::string> cells; // Store CSV values for the current line

        while (std::getline(ss, cell, ',')) {
            cells.push_back(cell);
        }
        OrderBookEntry orderBookEntry;
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
            std::cerr << "Invalid input" << '\n';
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
            file_out <<orderBookEntry.ID<<","<<" "<<","<<orderBookEntry.side<<","<< "pfill_2"<<","<<orderBookEntry.quantity<<","<<orderBookEntry.price<<std::endl;
            continue;

        }
        currentOrderBook->orderBook.push_back(orderBookEntry); 
        
        
         if (orderBookEntry.side == 1) {
            currentOrderBook->buy.push_back(orderBookEntry);
        } else if (orderBookEntry.side == 2) {
            currentOrderBook->sell.push_back(orderBookEntry);
        }

        std::sort(currentOrderBook->buy.begin(), currentOrderBook->buy.end(), compareByPriceDs);
        std::sort(currentOrderBook->sell.begin(), currentOrderBook->sell.end(), compareByPriceAs);



        //matching algorithm
        bool matchFound = false;  // Initialize matchFound as false

        for (OrderBookEntry &buyOrder :currentOrderBook->buy) {
            for (OrderBookEntry &sellOrder :currentOrderBook->sell) {
                if (buyOrder.price >= sellOrder.price) {
                    if (buyOrder.quantity > sellOrder.quantity && sellOrder.quantity != 0) 
                    {
                        buyOrder.quantity -= sellOrder.quantity;
                        sellOrder.quantity = 0;
                    
                        //add time
                        std::string timestamp = getTimestamp();
                        


                        std::cout << "pfill_1 "<<buyOrder.ID<<" "<<buyOrder.instrument<<std::endl;
                        std::cout << "filled " <<sellOrder.ID <<" "<<buyOrder.instrument<<std::endl;


                        //print to .csv file
                        file_out <<timestamp<<","<<buyOrder.ID<<","<<buyOrder.instrument<<","<<buyOrder.side<<","<< "pfill_1"<<","<<orderBookEntry.quantity<<","<<buyOrder.price<<std::endl;
                        file_out <<timestamp<<","<<buyOrder.ID<<","<<buyOrder.instrument<<","<<buyOrder.side<<","<< "filled "<<","<<orderBookEntry.quantity<<","<<buyOrder.price<<std::endl;

                        matchFound = true;  // Set matchFound to true when a match occurs
                    } 
                    else if (buyOrder.quantity < sellOrder.quantity && buyOrder.quantity != 0) {
                        sellOrder.quantity -= buyOrder.quantity;
                        buyOrder.quantity = 0;
                        //add time
                        std::string timestamp = getTimestamp();
                        
                        
                        
                        
                        std::cout << "pfill_2 " <<sellOrder.ID <<" "<<buyOrder.instrument<<std::endl;
                        std::cout << "filled " <<buyOrder.ID<<" "<<buyOrder.instrument<<std::endl;

                        //print to .csv file
                        file_out <<timestamp<<","<<buyOrder.ID<<","<<buyOrder.instrument<<","<<buyOrder.side<<","<< "pfill_2"<<","<<orderBookEntry.quantity<<","<<buyOrder.price<<std::endl;
                        file_out <<timestamp<<","<<buyOrder.ID<<","<<buyOrder.instrument<<","<<buyOrder.side<<","<< "filled"<<","<<orderBookEntry.quantity<<","<<buyOrder.price<<std::endl;

                        matchFound = true;  // Set matchFound to true when a match occurs
                    } 
                    else if (buyOrder.quantity == sellOrder.quantity && buyOrder.quantity != 0 && sellOrder.quantity != 0) {
                        buyOrder.quantity = 0;
                        sellOrder.quantity = 0;

                        //add time
                        std::string timestamp = getTimestamp();
                        



                        std::cout << "filled " <<buyOrder.ID <<" "<<buyOrder.instrument<<std::endl;
                        std::cout << "filled " <<sellOrder.ID <<" "<<buyOrder.instrument<<std::endl;

                        //print to .csv file
                        file_out <<timestamp<<","<<buyOrder.ID<<","<<buyOrder.instrument<<","<<buyOrder.side<<","<< "filled"<<","<<orderBookEntry.quantity<<","<<buyOrder.price<<std::endl;
                        file_out <<timestamp<<","<<buyOrder.ID<<","<<buyOrder.instrument<<","<<buyOrder.side<<","<< "filled"<<","<<orderBookEntry.quantity<<","<<buyOrder.price<<std::endl;

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
            file_out <<timestamp<<","<<orderBookEntry.ID<<","<<orderBookEntry.instrument<<","<<orderBookEntry.side<<","<< "new"<<","<<orderBookEntry.quantity<<","<<orderBookEntry.price<<std::endl;
            } 
    }
        //std::cout << line << std::endl;

        file_out.close();
        file.close();
        
        return 0;   

}

        
    
        

