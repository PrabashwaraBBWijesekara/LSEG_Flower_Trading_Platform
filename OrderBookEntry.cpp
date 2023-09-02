#include "OrderBookEntry.h"

OrderBookEntry::OrderBookEntry() {
   
}

OrderBookEntry::OrderBookEntry(std::string ID, std::string instrument, int side, double price, int quantity)
    : ID(ID), instrument(instrument), side(side), price(price), quantity(quantity) {
    
}
