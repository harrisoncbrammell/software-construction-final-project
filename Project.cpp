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

void press_any_key(void);

inline string getInput(string prompt) {
    string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    std::cout << std::endl;
    // check to make sure input is not empty
    if (input.empty()) {
        return getInput("Error: Input cannot be empty\n"+prompt); // Recursively call the function until valid input is received
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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
    switch (choice) {
        case '1': {
            system("clear");
            string name = getInput("Enter the name of the new client: ");
            string address = getInput("Enter the address of the new client: ");
            string ssn = getInput("Enter the SSN of the new client: ");
            string employer = getInput("Enter the employer of the new client: ");
            b->addClient(Client(name, address, ssn, employer)); // Call the add client function
            std::cout << "Client added successfully!" << std::endl;
            break;
        }
        case '2': {
            system("clear");
            string clientName = getInput("Enter the name of the client to add an account for:   ");
            Client *client = b->getClient(clientName);
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
        case '3': { ///< takes a client name then gets a pointer to the client in b's clients vector and updates its info before 
            system("clear");
            string clientName = getInput("Enter the name of the client to edit: ");
            Client *client = b->getClient(clientName);
            if (client->getName() == "") {
                std::cout << "Client not found." << std::endl;
                break;
            }
            string address = getInput("Enter the new address of the client: ");
            string ssn = getInput("Enter the new SSN of the client: ");
            string employer = getInput("Enter the new employer of the client: ");
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
            std::cout << "Client information updated successfully!" << std::endl;
            break;
        }
        case '4': {
            string clientName = getInput("Enter the name of the client to manage accounts for: ");
            string accountNumber = getInput("Enter the account number to manage: ");
            Client* client = b->getClient(clientName);
            if (client == nullptr || client->getName() == "") {
                std::cout << "Client not found." << std::endl;
                break;
            }
            Account* accountClient = client->getAccount(accountNumber);
            Account* accountBank = b->getAccountBank(accountNumber);
            if (accountClient == nullptr || accountClient->getAccountNumber() == "") {
                std::cout << "Account not found." << std::endl;
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
                        break; // Added break
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
                        break; // Exit the inner switch
                    }
                    default: {
                        cout << "Error: Invalid option. Please try again." << std::endl;
                        break; // Break for the default case of the inner switch
                    }
                } // end inner switch
            } // end while loop
            break; // Break for case '4' of the outer switch
        } // end case 4
        case '5':{
            b->saveClients(); // Save the clients to the file
            b->saveAccounts();
            std::cout << "Client and Account information saved." << std::endl;
            break; // Added break
        }
        case '6':{
            delete b; // Free the allocated memory for the bank object
            std::cout << "Exiting..." << std::endl;
            exit(0);
        }
        default:{ // This is the default for the outer switch
            system("clear");
            std::cout << "Invalid option. Please try again." << std::endl;
            break; // Added break
        }
    }
    acctManMenu(b, u); // Call the function to display the account management menu again
}

void tellerMenu(Bank* b, User* u) {
    cout << "=== Teller Terminal System Branch Staff ===\n==================================================================\n1) Client and Account Management\n2) Change password\n3) Exit\nPlease choose an option: ";
    //switch statement to handle menu options
    char choice;
    std::cin >> choice;
    switch (choice) {
        case '1':{
            system("clear");
            acctManMenu(b, u); // Call the function to display the account management menu
            break;
        }
        case '2':{
            system("clear");
            u->changePassword(); // Call the change password function
            b->saveUsers(); // Save the users to the file
            break;
        }
        case '3':{
            delete b; // Free the allocated memory for the bank object
            std::cout << "Exiting..." << std::endl;
            exit(0);
        }
        default:{
            std::cout << "Invalid option. Please try again." << std::endl;
            tellerMenu(b, u); // Recursively call the menu function to allow retry
        }
    }
}

void adminMenu(Bank* b, User* u) {
    press_any_key();
    system("clear");
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
            std::cout << "Delete a user User Name: " << username << std::endl;
            std::cout << "1) Confirm" << std::endl << "2) Cancel" << std::endl << "Please choose an option: ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
            if (confirm != '1') {
                std::cout << "User deletion cancelled." << std::endl;
                adminMenu(b, u); // Return to the admin menu
                break; // Exit the case
            }
            b->deleteUser(username); // Call the delete user function
            break;
        }
        case '4':{
            system("clear");
            b->printUsersBank();
            bool quiet = true;
            press_any_key();
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
            std::cout << "Invalid option. Please try again." << std::endl;
            press_any_key(); // Wait for user input before clearing the screen
            adminMenu(b, u); // Recursively call the menu function to allow retry
        }
    }
    press_any_key();
    adminMenu(b, u); // Call the function to display the admin menu again
}

void press_any_key(void) {
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wait for Enter key
}

int main(){
#ifdef DEBUG

#endif
    const string bankName = "Tiger Branch of Auburn Bank";
    const int bankRoutingNumber = 123456789;
    Bank* b = new Bank(bankName, bankRoutingNumber);
    press_any_key();
    system("clear");
    User* u = loginMenu(b);
    system("clear");
    if (u->isAdmin()) {
        adminMenu(b, u);
    } else {
        std::cout << "You have teller privileges." << std::endl;
    }
    return 0;
}

