#include "MD5.h"
#include <iostream>
#ifdef _WIN32 // Код для Windows
#include <windows.h>
#else // Код для Unix-подобных систем
#include <csignal>
#endif
int main(int argc, char *argv[]) {
#ifdef _WIN32
    SetConsoleCtrlHandler(ConsoleHandler, TRUE); // for Windows systems
#else
    signal(SIGINT, signal_h); // for Unix systems
#endif

    if (argc < 2 || argc > 3){
        std::cerr << "Incorrect number of arguments" << std::endl;
        return 1;
    }

    if (argc == 2 && std::string(argv[1]) != "resume"){
        std::cerr << "Incorrect arguments" << std::endl;
        return 2;
    }

    if (argc == 2 && std::string(argv[1]) == "resume") {
        std::cout << "Resuming work...." << std::endl;
        load_state(current_prefix);
    } else {
        hash = argv[1];
        for(char c: hash){
            if((c < '0' || c > '9') && (tolower(c) < 'a' || tolower(c) > 'f')){
                std::cout << "Incorrect hash" << std::endl;
                return 3;
            }
        }
        charset = load(argv[2]);
        current_position.clear();
        current_position.resize(1,0);
    }
    run();
    return 0;
}
