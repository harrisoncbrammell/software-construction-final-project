/*
Charles Harrison Brammell
COMP 2710: Software Construction - Spring 2025
chb0055@auburn.edu
Final Project: Bank Teller System
*/
#include "Banksoft.hpp"
#include <string>
#include <iostream>
#include <limits> // Required for numeric_limits

using namespace banksoft;
using namespace std;

///< deployment specific variables

///< menu handling login functionality
/// @param b 

inline string getInput(string prompt) {
    string input;
    std::cout << prompt;
    // Clear the input buffer before getline, consuming leftover newline from previous >> operations    
    std::getline(std::cin, input);
    std::cout << std::endl;
    return input;
}

User* loginMenu(Bank* b) {
    while (true) { // Use a loop to handle retries
        char choice;
        std::cout << "=========== Welcome to the " << b->getName() << "! ===========" << std::endl;
        std::cout << "1) Login" << std::endl << "2) Quit" << std::endl << "Please choose an option: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

        switch (choice) {
            case '1': {
                string username = getInput("Enter username: ");
                string password = getPassword(); // No need to print "Enter password"
                // Check if the user is an admin or teller
                User* user = b->getUser(username); // Get the user object from the bank
                if (user->username == "") {
                    system("clear");
                    std::cout << "Invalid username." << std::endl;
                    continue; // Retry the login menu
                }
                if (user->authenticate(password)) {
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
                    continue; // Retry the login menu
                }
            }
            case '2': {
                delete b; // Free the allocated memory for the bank object
                std::cout << "Goodbye!" << std::endl;
                exit(0); // Exit the program
            }
            default: {
                system("clear");
                std::cout << "Invalid option. Please try again." << std::endl;
                continue; // Retry the login menu
            }
        }
    }
}

void acctManMenu(Bank* b, User* u) {
    std::cout << "Teller Terminal System Client and Account Management" << std::endl << "=============================================================" << std::endl;
    std::cout << "1) Add a client" << std::endl << "2) Add an account" << std::endl << "3) Edit Client Information" << std::endl << "4) Manage an account" << std::endl << "5) Save Client and Account Information" << std::endl << "6) Exit" << std::endl << "Please choose an option: " << std::endl;
    char choice;
    std::cin >> choice;
    switch (choice) {
        case '1':{
            system("clear");
            string name = getInput("Enter the name of the new client: ");
            string address = getInput("Enter the address of the new client: ");
            string ssn = getInput("Enter the SSN of the new client: ");
            string employer = getInput("Enter the employer of the new client: ");
            b->addClient(Client(name, address, ssn, employer)); // Call the add client function
            std::cout << "Client added successfully!" << std::endl;
            acctManMenu(b, u); // Call the function to display the account management menu again
            break;
        }
        case '2': {
            system("clear");
            string clientName = getInput("Enter the name of the client to add an account for:   ");
            Client *client = b->getClient(clientName);
            if (client->getName() == "") {
                std::cout << "Client not found." << std::endl;
                acctManMenu(b, u); // Call the function to display the account management menu again
                break;
            } else {
                string accountNumber = getInput("Enter the account number: ");
                string accountType = getInput("Enter the account type: ");
                std::cout << "Enter the initial balance: ";
                long double balance;
                std::cin >> balance;
                client->addAccount(balance, client->getName(), accountNumber, accountType);
                b->addAccountBank(balance, client->getName(), accountNumber, accountType); // Add the account to the client's list of accounts
                std::cout << "Account added successfully!" << std::endl;
                b->saveAccounts();// Save the clients to the file
                acctManMenu(b, u); // Call the function to display the account management menu again
            }
            break;
        }
        case '3': { ///< takes a client name then gets a pointer to the client in b's clients vector and updates its info before 
            system("clear");
            string clientName = getInput("Enter the name of the client to edit: ");
            Client *client = b->getClient(clientName);
            if (client->getName() == "") {
                std::cout << "Client not found." << std::endl;
                acctManMenu(b, u); // Call the function to display the account management menu again
                break;
            }
            string address = getInput("Enter the new address of the client: ");
            string ssn = getInput("Enter the new SSN of the client: ");
            string employer = getInput("Enter the new employer of the client: ");
            client->setAddress(address);
            client->setSSN(ssn);
            client->setEmployer(employer);
            b->saveClients(); // Save the clients to the file
            std::cout << "Client information updated successfully!" << std::endl;
            acctManMenu(b, u); // Call the function to display the account management menu again
        }
            break;
        case '4':{

        }
            break;
        case '5':{
            b->saveClients(); // Save the clients to the file
            b->saveAccounts();
            std::cout << "Client and account information saved successfully!" << std::endl;
            acctManMenu(b, u); // Call the function to display the account management menu again
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
            acctManMenu(b, u); // Recursively call the menu function to allow retry
        }
    }
}

void adminMenu(Bank* b, User* u) {
    std::cout << "Teller Terminal System System Administration" << std::endl << "==================================" << std::endl;
    std::cout << "1) Client and Account Management" << std::endl << "2) Add a branch staff member" << std::endl << "3) Delete a branch staff member" << std::endl << "4) Display branch staff" << std::endl << "5) Change Password" << std::endl << "6) Exit" << std::endl << "Please choose an option: " << std::endl;
    char choice;
    std::cin >> choice;
    switch (choice) {
        case '1':
            acctManMenu(b, u); // Call the function to display the account management menu
            break;
        case '2':{
            system("clear");
            string username = getInput("Enter the username of the new staff member: ");
            string password = getPassword();
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
            string username = getInput("Enter the username of the staff member to delete: ");
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
    Bank* b = new Bank(bankName, bankRoutingNumber);
    User* u = loginMenu(b);
    system("clear");
    if (u->isAdmin()) {
        adminMenu(b, u);
    } else {
        std::cout << "You have teller privileges." << std::endl;
    }
    return 0;
}

