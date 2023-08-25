#include <iostream>


    std::vector<std::string> fields = split(line, ',');

    for (const std::string &field : fields) {
        std::cout << field << std::endl;
    }

int main()
{

    return 0;
}
