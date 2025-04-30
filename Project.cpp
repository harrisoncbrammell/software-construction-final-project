/*
Charles Harrison Brammell
COMP 2710: Software Construction - Spring 2025
chb0055@auburn.edu
Final Project: Bank Teller System
*/
#include "Banksoft.hpp"
#include <string>
#include <iostream>

using namespace banksoft;
using namespace std;

///< deployment specific variables
void loginMenu(Bank* b) {
    char choice;
    std::cout << "=========== Welcome to the " << b->getName() << "! ===========" << std::endl;
    std::cout << "1) Login" << std::endl << "2) Quit" << std::endl << "Please choose an option: ";
    std::cin >> choice;
    switch (choice) {
        case '1': {
            string username;
            string password;
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            password = getPassword();
            // Check if the user is an admin or teller
           User user = b->getUser(username); // Get the user object from the bank
            if (user.username == "") {
                system("clear");
                std::cout << "Invalid username." << std::endl;
                loginMenu(b); // Recursively call the menu function to allow retry
                return; // Exit the function after retrying
            }
            if(user.authenticate(password)) {
                if (user.isAdmin()) {
                    // Admin login
                    std::cout << "Welcome, Admin!" << std::endl;
                    // Admin functionalities here
                } else {
                    // Teller login
                    std::cout << "Welcome, Teller!" << std::endl;
                    // Teller functionalities here
                }
            } else {
                system("clear");
                std::cout << "Invalid password." << std::endl;
                loginMenu(b); // Recursively call the menu function to allow retry
                return; // Exit the function after retrying
            }
            break;
        }
        case '2':
            delete b; // Free the allocated memory for the bank object
            std::cout << "Goodbye!" << std::endl;
            break;
        default:
            system("clear");
            std::cout << "Invalid option. Please try again." << std::endl;
            loginMenu(b); // Recursively call the menu function to allow retry
    }
}
int main(){
    #ifdef DEBUG

    #endif
    const string bankName = "Tiger Branch of Auburn Bank";
    const int bankRoutingNumber = 123456789;
    ///< create the bank object on the heap
    Bank* b = new Bank(bankName, bankRoutingNumber);
    ///< initial menu
    loginMenu(b);

     

    return 0;
}

