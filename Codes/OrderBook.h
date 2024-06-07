#pragma once
#include <vector>
#include "OrderBookEntry.h" 

class OrderBook {
public:
    std::vector<OrderBookEntry> orderBook;
    std::vector<OrderBookEntry> buy;
    std::vector<OrderBookEntry> sell;

    OrderBook();
};
