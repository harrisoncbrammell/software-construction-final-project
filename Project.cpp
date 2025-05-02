/*
Project.cpp
Charles Harrison Brammell
COMP 2710: Software Construction - Spring 2025
chb0055@auburn.edu
Final Project: Bank Teller System
Outside Sources: cplusplus.com and stackoverflow.com for string parsing help and getPassword() function with asterisk masking using termios.
*/
#include "Banksoft.hpp"
#include <string>
#include <iostream>
#include <limits>

using namespace banksoft;
using namespace std;

void press_any_key(void);

inline string getInput(string prompt) {
    string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    std::cout << std::endl;
    if (input.empty()) { // check to make sure input is not empty
        return getInput("Error: Input cannot be empty\n" + prompt); // Recursively call the function until valid input is received
    } else {
        return input;
    }
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
                string username = getInput("\nEnter username: ");
                string password = getPassword();
                // Check if the user is an admin or teller
                User* user = b->getUser(username); // Get the user object from the bank
                if (user->username.empty()) { // Check if the username is valid
                    #ifdef LOUD
                    system("clear");
                    #endif
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
                    #ifdef LOUD
                    system("clear");
                    #endif
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
#ifdef LOUD
                system("clear");
#endif
                std::cout << "Invalid option. Please try again." << std::endl;
                continue; // Retry the login menu
            }
        }
    }
}

void acctManMenu(Bank* b, User* u) {
    std::cout << "Teller Terminal System Client and Account Management" << std::endl
              << "=============================================================" << std::endl;
    std::cout << "1) Add a client" << std::endl
              << "2) Add an account" << std::endl
              << "3) Edit Client Information" << std::endl
              << "4) Manage an account" << std::endl
              << "5) Save Client and Account Information" << std::endl
              << "6) Exit" << std::endl
              << "Please choose an option: ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
    switch (choice) {
        case '1': {
#ifdef LOUD
            system("clear");
#endif
            string name = getInput("Enter the name of the new client: ");
            string address = getInput("Enter the address of the new client: ");
            string ssn = getInput("Enter the SSN of the new client: ");
            string employer = getInput("Enter the employer of the new client: ");
            string income = getInput("Enter the annual income of the new client: "); // New input
            b->addClient(Client(name, address, ssn, employer, income)); // Updated constructor
            std::cout << "Client added successfully!" << std::endl;
            break;
        }
        case '2': {
#ifdef LOUD
            system("clear");
#endif
            string clientName = getInput("Enter the name of the client to add an account for:   ");
            Client* client = b->getClient(clientName);
            if (client->getName() == "") {
                std::cout << "Client not found." << std::endl;
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
            }
            break;
        }
        case '3': {
#ifdef LOUD
            system("clear");
#endif
            string clientName = getInput("Enter the name of the client to edit: ");
            Client* client = b->getClient(clientName);
            if (client->getName() == "") {
                std::cout << "Client not found." << std::endl;
                break;
            }
            string address = getInput("Enter the new address of the client: ");
            string ssn = getInput("Enter the new SSN of the client: ");
            string employer = getInput("Enter the new employer of the client: ");
            string income = getInput("Enter the new annual income of the client: "); // New input
            cout << "Client " << client->getName() << "'s information will be updated\n1) Confirm\n2) Cancel\nPlease choose an option:    ";
            char confirm;
            std::cin >> confirm;
            if (confirm != '1') {
                std::cout << "Client information update cancelled." << std::endl;
                break;
            }
            client->setAddress(address);
            client->setSSN(ssn);
            client->setEmployer(employer);
            client->setAnnualIncome(income); // Update annual income
            std::cout << "Client information updated successfully!" << std::endl;
            break;
        }
        case '4': {
            string clientName = getInput("Enter the name of the client to manage accounts for: ");
            Client* client = b->getClient(clientName);
            if (client == nullptr || client->getName() == "") {
                std::cout << "Client not found." << std::endl;
                break;
            }

            // Display client's accounts
            std::cout << "\nAccounts for " << client->getName() << ":" << std::endl;
            client->printAccounts(); // Call the function to print accounts

            string accountNumber = getInput("\nEnter the account number to manage: "); // Ask for account number after listing them
            Account* accountClient = client->getAccount(accountNumber);
            Account* accountBank = b->getAccountBank(accountNumber);
            if (accountClient == nullptr || accountClient->getAccountNumber() == "") {
                std::cout << "Account not found for this client." << std::endl; // Adjusted error message
                break;
            }

            // Manage account menu
            bool manageAccountLoop = true;
            while (manageAccountLoop) {
                std::cout << "\nManage account " << accountNumber << " for " << clientName << std::endl;
                std::cout << "Current Balance: " << accountClient->getBalance() << std::endl;
                std::cout << "1) Deposit" << std::endl;
                std::cout << "2) Withdraw" << std::endl;
                std::cout << "3) Cancel" << std::endl;
                std::cout << "Please choose an option: ";
                char option;
                std::cin >> option; // Read the option
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

                switch (option) {
                    case '1': { // Deposit
                        std::cout << "Enter the amount to deposit: ";
                        long double amount;
                        std::cin >> amount;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
                        if (amount <= 0) {
                            std::cout << "Error: Deposit amount must be greater than zero." << std::endl;
                        } else {
                            accountClient->deposit(amount);
                            accountBank->deposit(amount);
                            std::cout << "Deposit successful. New balance: " << accountClient->getBalance() << std::endl;
                        }
                        break;
                    }
                    case '2': { // Withdraw
                        std::cout << "Enter the amount to withdraw: ";
                        long double amount;
                        std::cin >> amount;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
                        if (amount <= 0) {
                            std::cout << "Error: Withdrawal amount must be greater than zero." << std::endl;
                        } else if (amount > accountClient->getBalance()) {
                            std::cout << "Error: Insufficient funds." << std::endl;
                        } else {
                            accountClient->withdraw(amount);
                            accountBank->withdraw(amount);
                            std::cout << "Withdrawal successful. New balance: " << accountClient->getBalance() << std::endl;
                        }
                        break;
                    }
                    case '3': { // Cancel
                        std::cout << "Returning to account management menu..." << std::endl;
                        manageAccountLoop = false; // Set flag to exit loop
                        break;
                    }
                    default: {
                        cout << "Error: Invalid option. Please try again." << std::endl;
                        break;
                    }
                }
            }
            break;
        }
        case '5': {
            b->saveClients(); // Save the clients to the file
            b->saveAccounts();
            std::cout << "Client and Account information saved." << std::endl;
            break;
        }
        case '6': {
            delete b; // Free the allocated memory for the bank object
            std::cout << "Exiting..." << std::endl;
            exit(0);
        }
        default: {
#ifdef LOUD
            system("clear");
#endif
            std::cout << "Invalid option. Please try again." << std::endl;
            break;
        }
    }
    acctManMenu(b, u); // Call the function to display the account management menu again
}

void tellerMenu(Bank* b, User* u) {
    while (true) { // Use a loop instead of recursion
        cout << "=== Teller Terminal System Branch Staff ===\n==================================================================\n1) Client and Account Management\n2) Change password\n3) Exit\nPlease choose an option: ";
        //switch statement to handle menu options
        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer after reading choice

        switch (choice) {
            case '1':{
                #ifdef LOUD
                system("clear");
                #endif
                acctManMenu(b, u); // Call the function to display the account management menu
                // acctManMenu now returns here, loop will redisplay tellerMenu
                break; // Break from switch, loop continues
            }
            case '2':{
                #ifdef LOUD
                system("clear");
                #endif
                u->changePassword(); // Call the change password function
                b->saveUsers(); // Save the users to the file
                press_any_key(); // Pause after successful change
                #ifdef LOUD
                system("clear");
                #endif
                break; // Break from switch, loop continues
            }
            case '3':{
                delete b; // Free the allocated memory for the bank object
                std::cout << "Exiting..." << std::endl;
                exit(0); // Exit the program
            }
            default:{
                #ifdef LOUD
                system("clear");
                #endif
                std::cout << "Invalid option. Please try again." << std::endl;
                press_any_key(); // Pause after invalid input
                #ifdef LOUD
                system("clear");
                #endif
                // tellerMenu(b, u); // Remove recursive call
                break; // Break from switch, loop continues
            }
        }
        // Loop will automatically redisplay the menu unless exited
    }
}

void adminMenu(Bank* b, User* u) {
    while (true) { // Use a loop instead of recursion
        #ifdef LOUD
        system("clear");
        #endif
        std::cout << "Teller Terminal System System Administration" << std::endl << "==================================" << std::endl;
        std::cout << "1) Client and Account Management" << std::endl << "2) Add a branch staff member" << std::endl << "3) Delete a branch staff member" << std::endl << "4) Display branch staff" << std::endl << "5) Change Password" << std::endl << "6) Exit" << std::endl << "Please choose an option: "; // Removed extra endl
        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer after reading choice

        switch (choice) {
            case '1':{
                #ifdef LOUD
                system("clear");
                #endif
                acctManMenu(b, u); // Call the function to display the account management menu
                // Returns here, loop will redisplay admin menu
                break; // Break from switch, loop continues
            }
            case '2':{
                #ifdef LOUD
                system("clear");
                #endif
                string username = getInput("Enter the username of the new staff member: ");
                string password = getPassword();
                std::cout << "Is this user an admin? (y/n): ";
                char isAdminChoice;
                std::cin >> isAdminChoice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
                bool isAdmin = (isAdminChoice == 'y' || isAdminChoice == 'Y');
                b->addUser(username, password, isAdmin); // Call the add user function
                // Assuming addUser prints success/failure
                b->saveUsers(); // Save the users to the file
                press_any_key(); // Pause after operation
                break; // Break from switch, loop continues
            }
            case '3':{
                #ifdef LOUD
                system("clear");
                #endif
                string username = getInput("Enter the username of the staff member to delete: ");
                std::cout << "Delete a user User Name: " << username << std::endl;
                std::cout << "1) Confirm" << std::endl << "2) Cancel" << std::endl << "Please choose an option: ";
                char confirm;
                std::cin >> confirm;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
                if (confirm != '1') {
                    std::cout << "User deletion cancelled." << std::endl;
                } else {
                    b->deleteUser(username); // Call the delete user function
                    // Assuming deleteUser prints success/failure
                }
                press_any_key(); // Pause after operation
                break; // Break from switch, loop continues
            }
            case '4':{
                #ifdef LOUD
                system("clear");
                #endif
                b->printUsersBank();
                press_any_key(); // Pause after displaying
                break; // Break from switch, loop continues
            }
            case '5':{
                #ifdef LOUD
                system("clear");
                #endif
                u->changePassword(); // Call the change password function
                b->saveUsers(); // Save the users to the file
                press_any_key(); // Pause after successful change
                break; // Break from switch, loop continues
            }
            case '6':{
                delete b; // Free the allocated memory for the bank object
                std::cout << "Exiting..." << std::endl;
                exit(0); // Exit the program
            }
            default:{
                #ifdef LOUD
                system("clear");
                #endif
                std::cout << "Invalid option. Please try again." << std::endl;
                press_any_key(); // Wait for user input before clearing the screen
                // adminMenu(b, u); // Remove recursive call
                break; // Break from switch, loop continues
            }
        }
        // press_any_key(); // Remove this - handled within cases or at start of loop
        // #ifdef LOUD
        // system("clear");
        // #endif
        // adminMenu(b, u); // Remove recursive call
    } // End of while loop
}

void press_any_key(void) {
    std::cout << "Press Enter to continue..." << std::endl;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Original line - might consume needed input if buffer isn't empty
    std::cin.get(); // Waits specifically for the Enter key press
}

int main(){
#ifdef DEBUG

#endif
    const string bankName = "Tiger Branch of Auburn Bank";
    const int bankRoutingNumber = 123456789;
    Bank* b = new Bank(bankName, bankRoutingNumber);
#ifdef LOUD
    press_any_key();
    system("clear");
#endif
    User* u = loginMenu(b);
#ifdef LOUD
    system("clear");
#endif
    if (u->isAdmin()) {
        adminMenu(b, u);
    } else {
        tellerMenu(b, u);
    }
    return 0;
}

