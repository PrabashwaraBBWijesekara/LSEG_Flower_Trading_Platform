#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

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

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    std::ifstream file("ex7.csv");
    if (!file.is_open()) {
        std::cerr << "Could not open the file." << std::endl;
        return 1;
    }
    std::string line;
    std::cout<<"cdmnjrvnh"<<std::endl;

    std::getline(file, line);

    std::cout << line << std::endl;

    std::vector<std::string> fields = split(line, ',');

    for (const std::string &field : fields) {
        std::cout << field << std::endl;
    }

    file.close();

    return 0;
}
