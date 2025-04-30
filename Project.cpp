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

///< menu handling login functionality
/// @param b 
User* loginMenu(Bank* b) {
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
            User* user = b->getUser(username); // Get the user object from the bank
            if (user->username == "") {
                system("clear");
                std::cout << "Invalid username." << std::endl;
                loginMenu(b); // Recursively call the menu function to allow retry
            }
            if(user->authenticate(password)) {
                if (user->isAdmin()) {
                    // Admin login
                    std::cout << "Welcome, Admin!" << std::endl;
                    return user;
                } else {
                    // Teller login
                    std::cout << "Welcome, Teller!" << std::endl;
                    return user;
                }
            } else {
                system("clear");
                std::cout << "Invalid password." << std::endl;
                loginMenu(b); // Recursively call the menu function to allow retry
            }
            break;
        }
        case '2':
            delete b; // Free the allocated memory for the bank object
            std::cout << "Goodbye!" << std::endl;
            exit(0); // Exit the program
            break;
        default:
            system("clear");
            std::cout << "Invalid option. Please try again." << std::endl;
            loginMenu(b); // Recursively call the menu function to allow retry
    }
}

void adminMenu(Bank* b, User* u) {
    std::cout << "Teller Terminal System System Administration" << std::endl << "==================================" << std::endl;
    std::cout << "1) Client and Account Managment" << std::endl << "2) Add a brach staff member" << std::endl << "3) Delete a branch staff member" << std::endl << "4) Display branch staff" << std::endl << "5) Change Password" << std::endl << "6) Exit" << std::endl << "Please choose an option: " << std::endl;
    char choice;
    std::cin >> choice;
    switch (choice) {
        case '1':
            // Call the client and account management function
            break;
        case '2':{
            ///< get the username and password and type for the new user then adds it to b
            system("clear");
            std::cout << "Enter the username of the new staff member: ";
            string username;
            std::cin >> username;
            std::cout << "Enter the password of the new staff member: ";
            string password;
            password = getPassword();
            std::cout << "Is this user an admin? (y/n): ";
            char isAdminChoice;
            std::cin >> isAdminChoice;
            bool isAdmin = (isAdminChoice == 'y' || isAdminChoice == 'Y');
            b->addUser(username, password, isAdmin); // Call the add user function
            std::cout << "User " << username << " added successfully!" << std::endl;
            b->saveUsers(); // Save the users to the file   
            break;
        }
        case '3':{
            system("clear");
            std::cout << "Enter the username of the staff member to delete: ";
            string username;
            std::cin >> username;
            b->deleteUser(username); // Call the delete user function
            break;
        }
        case '4':{
            system("clear");
            b->printUsersBank();
            adminMenu(b, u); // Call the function to display branch staff
            break;
        }
        case '5':{
            system("clear");
            u->changePassword(); // Call the change password function
            b->saveUsers(); // Save the users to the file
            break;
        }
        case '6':{
            delete b; // Free the allocated memory for the bank object
            std::cout << "Exiting..." << std::endl;
            exit(0);
        }
        default:{
            system("clear");
            std::cout << "Invalid option. Please try again." << std::endl;
            adminMenu(b, u); // Recursively call the menu function to allow retry
        }
    }
    system("clear");
    adminMenu(b, u); // Call the function to display the admin menu again
}

int main(){
    #ifdef DEBUG

    #endif
    const string bankName = "Tiger Branch of Auburn Bank";
    const int bankRoutingNumber = 123456789;
    ///< create the bank object on the heap
    Bank* b = new Bank(bankName, bankRoutingNumber);
    ///< initial menu
    User* u = loginMenu(b);
    system("clear");
    if (u->isAdmin()) {
        adminMenu(b, u);
    } else {
        std::cout << "You have teller privileges." << std::endl;
    }
     

    return 0;
}

