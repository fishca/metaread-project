#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: metaread <config_file> [export_path]" << std::endl;
        return 1;
    }

    std::string configFile = argv[1];
    std::string exportPath = (argc > 2) ? argv[2] : "export";



    return 0;
}