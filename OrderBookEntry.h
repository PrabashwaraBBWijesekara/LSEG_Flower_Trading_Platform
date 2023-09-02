#pragma once
#include <string>

class OrderBookEntry {
public:
    std::string ID;
    std::string instrument;
    int side;
    double price;
    int quantity;
    std::string orderID;

    OrderBookEntry();
    OrderBookEntry(std::string ID, std::string instrument, int side, double price, int quantity);
};
