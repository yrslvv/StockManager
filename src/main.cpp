#define _CRT_SECURE_NO_WARNINGS //disabling deprication warning


#include <iostream>
#include <cstdlib>
#include "Menu.h"

#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")

int main() {
    // Provide your actual API key here for Alpha Vantage
    const char* apiKey = std::getenv("API_KEY");

    if (apiKey == nullptr) {
        std::cerr << "API Key not found!" << std::endl;
        return 1;
    }


    // Create an instance of the Menu
    Menu menu(apiKey);
    menu.displayMenu();

    return 0;
}