#include <iostream>
#include <fstream>
#include <string>

class OrderBookEntry {
public:
    std::string ID;
    std::string instrument;
    int side;
    double price;
    int quantity;

    OrderBookEntry(std::string ID, std::string instrument, int side, double price, int quantity)
        : ID(ID), instrument(instrument), side(side), price(price), quantity(quantity) {
    }
};


// class OrderBook{
//     OrderBook("ex7.csv"){};
// };



int main()
{
    std::ifstream file("ex7.csv");
    if (!file.is_open()) {
        std::cerr << "Could not open the file." << std::endl;
        return 1;
    }
    std::string line;

    std::getline(file, line);

    std::cout << line << std::endl;
    
    file.close();

    return 0;
}
