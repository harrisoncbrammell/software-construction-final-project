/*
Charles Harrison Brammell
COMP 2710: Software Construction - Spring 2025
chb0055@auburn.edu
Final Project: Bank Teller System
Outside Sources: cplusplus.com for getpass() function stackoverflow.com for string parsing
*/

#ifndef BANKSOFT_HPP
#define BANKSOFT_HPP
// Banksoft.hpp
// This is a header file for a banking system software project.
// It contains class definitions for User, Admin, Teller, Client, Account, and Bank.
// The classes are designed to manage user accounts, client information, and bank accounts.

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype> // for isspace
#include <limits>
#include <iostream>
#include <termios.h> // for termios, tcgetattr, tcsetattr
#include <unistd.h>  // for STDIN_FILENO
using namespace std;

namespace banksoft {
#ifdef DEBUG
inline void namespaceFunction() { /**tester function for debugging issues with scope*/
    std::cout << "This is a namespace function" << std::endl;
    return;
}
#endif
///< namespace scope variables
const string CLIENTS_SAVEFILE = "client-info.txt"; ///< savefile for clients
const string USERS_SAVEFILE = "staff.txt"; ///< savefile for users
const string ACCOUNTS_SAVEFILE = "account-info.txt"; ///< savefile for accounts

///< namespace scope functions
#ifdef HIDE_PASSWORD
string getPassword() {
    string pw = getpass("New Password: "); 
    return pw; ///< getpass is a function that hides the password input
}
#endif
#ifndef HIDE_PASSWORD
string getPassword() { //gets password 
    std::cout << "Enter password: ";
    string input;
    std::getline(std::cin,input);
    std::cout << std::endl;
    ///< check if the input is empty and recursivley call the function if so
    if(input.empty()) {
        system("clear");
        std::cout << "Error: Password cannot be empty" << std::endl;
        return getPassword();
    }
    return input;
}
#endif

class Bank;

class User { //COMPLETE
    /** base class for all user accounts on system, inhereited by admin and teller classes */
    bool adminStatus; ///< boolean to check if user is admin
    string password; /** password is not const so it can be changed */
public:
    string username;
    bool authenticate(string pwInput) const {
        return pwInput == password;
    }
    void changePassword() { //COMPLETE
        /** changes the password of the user */
        while (true) {
            string newPassword = getPassword(); // No need to print "New Password"
            if (newPassword == password) {
                std::cout << "Error: Your new password must be different from the old one!" << std::endl;
            } else {
                password = newPassword;
                std::cout << "Password changed successfully!" << std::endl;
                break; // Exit the loop once the password is successfully changed
            }
        }
        return;
    }
    bool isAdmin() const { // Ensure this method is const
        return adminStatus;
    }
    User(string usernameIn, string passwordIn, bool isAdminIn) : username(usernameIn), password(passwordIn), adminStatus(isAdminIn) {} ///< constructor for use by admin and teller classes
    friend class Bank; ///< friend class to allow access to private members such as password
};
///> forward declaration of User class to be used in Bank class
class Account { /** class for all open bank accounts, give the constructor a reference to the client object that owns it */
    long double balance = 0.0; // Changed from int to long double
    const string ownerName;
    const string accountNumber; // Changed from int to string
    const string accountType;
public:
    void deposit(long double amount) { /** deposits money into the account */
        balance += amount;
        return;
    }
    void withdraw(long double amount) { /** takes and withdraws it from the account balance */
        if (amount > balance) {
            std::cout << "Error: Insufficient funds" << std::endl;
        } else if (amount < 0) {
            std::cout << "Error: Cannot withdraw a negative amount" << std::endl;
        } else {
            balance -= amount;
        }
        return;
    }
    string getAccountType() const { /** returns the account type */
        return accountType;
    }
    string getAccountNumber() const { /** returns the account number */
        return accountNumber;
    }
    string getClient() const { /** returns a pointer to the client that owns the account */
        return ownerName;
    }
    long double getBalance() const { /** returns the account balance */
        return balance;
    }
    Account(long double balance, string owner, string accountNumber, string accountType) // Updated accountNumber type
        : balance(balance), ownerName(owner), accountNumber(accountNumber), accountType(accountType) {}
}; 

class Client {
    const string name;
    string address;
    string ssn; // Changed from int to string
    string employer;
    std::vector<Account> accounts;
public:
    void printAccounts(); ///< prints each account in the accounts list by their account number, type, and balance

    void setAddress(string newAddress) { //COMPLETE
        /** sets the address of the client */
        address = newAddress;
        return;
    }
    void setEmployer(string newEmployer) { //COMPLETE
        /** sets the employer of the client */
        employer = newEmployer;
        return;
    }
    string getName() const{
        return name;
    }
    string getAddress() const {
        return address;
    }
    const string getSSN() const { // Updated return type to string
        return ssn;
    }
    void setSSN(string newSSN) { //COMPLETE
        /** sets the ssn of the client */
        ssn = newSSN;
        return;
    }
    string getEmployer() const {
        return employer;
    }
    void addAccount(long double balance, string owner, string accountNumber, string accountType) { //COMPLETE
        // TODO: implement function to add account to client account list
        accounts.push_back(Account(balance, owner, accountNumber, accountType)); ///< adds an account to the clients account list
        return;
    }
    Account* getAccount(string accountNumber) { //COMPLETE
        /** function getAccount(string accountNumber) searches vector accounts for an account with the matching account number and returns it */
        for(int i = 0; i < accounts.size(); i++) {
            if(accounts[i].getAccountNumber() == accountNumber) {
                return &accounts[i];
            }
        }
        std::cout << "Error: Account not found" << std::endl;
        ///< return a pointer to an account object with empty account number
        Account *emptyAccount = new Account(0.0, "", "", ""); // Create a new Account object with empty account number
        return emptyAccount; // Return the empty account object
    }
    Client(string name, string address, string ssn, string employer) // Updated ssn type
        : name(name), address(address), ssn(ssn), employer(employer) {}
};



void Client::printAccounts() { //COMPLETE
    /** prints each account in the accounts list by their account number, type, and balance */
    for (int i = 0; i < accounts.size(); i++) {
        std::cout << "Account " << i + 1 << ": " << accounts[i].getAccountNumber() << ": " << accounts[i].getAccountType() << ": " << accounts[i].getBalance() << std::endl;
    }
    return;
}

class Bank { ///< class for the institution itself operating the software
    const string name;
    const int routingNumber;
    std::vector<User> users;
    std::vector<Client> clients;
    std::vector<Account> accountsBank; ///< vector of all accounts in the bank
    void loadAccounts() { //COMPLETEE ///< this function loops throught each account in the text file then uses getClient to find its owner and adds it the owning client objects account list while also making sure to initialize each accounts client pointer
        int accountCount = 0; ///< loads accounts from a file
        ifstream accountStream(ACCOUNTS_SAVEFILE);
        if (accountStream.peek() == std::ifstream::traits_type::eof()) { ///< checks if the file is empty
            std::cout << "Error: Account file is empty" << std::endl;
            return;
        }
        string line;
        while (getline(accountStream, line)) {
            stringstream ss(line);
            string balanceStr;
            getline(ss, balanceStr, ','); // Read balance until the first comma
            long double balance = std::stold(balanceStr); // Convert balance to long double
            string clientName;
            getline(ss, clientName, ','); // Read client name (owner name) until the next comma
            string accountNumber;
            getline(ss, accountNumber, ','); // Read account number until the next comma
            string accountType;
            getline(ss, accountType, ','); // Read account type until th next comma
            getClient(clientName)->addAccount(balance, clientName, accountNumber, accountType); // Add the account to the client's list of accounts
            accountsBank.push_back(Account(balance, clientName, accountNumber, accountType)); // Add the account to the bank's list of accounts
            accountCount++;
        }
        std::cout << "Loaded " << accountCount << " accounts from file" << std::endl;
        accountStream.close();
    }

    void printAccountsBank() const { ///< prints each account in the accountsBank list by their account number, type, and balance
        for (int i = 0; i < accountsBank.size(); i++) {
            std::cout << "Account " << i + 1 << ": " << accountsBank[i].getAccountNumber() << ": " << accountsBank[i].getAccountType() << ": " << accountsBank[i].getBalance() << std::endl;
        }
        return;
    }

    void loadClients() { //COMPLETE ///< loads clients from a file
        int clientCount = 0; ///< loads clients from a file
        ifstream clientStream(CLIENTS_SAVEFILE);
        if (clientStream.peek() == std::ifstream::traits_type::eof()) { ///< checks if the file is empty
            std::cout << "Error: Client file is empty" << std::endl;
            return;
        }
        string line;
        while (getline(clientStream, line)) {
            stringstream ss(line);
            string name;
            getline(ss, name, ','); // Read name until the first comma
            string address;
            getline(ss, address, ','); // Read address until the next comma
            string ssn;
            getline(ss, ssn, ','); // Read SSN as a string
            string employer;
            getline(ss, employer, ','); // Read employer until the next comma
            clients.push_back(Client(name, address, ssn, employer));
            clientCount++;
        }
        std::cout << "Loaded " << clientCount << " clients from file" << std::endl;
        clientStream.close();
    }
    void printClients() const{ // COMPLETE ///< prints each client in the clients list by their name, address, ssn, and employer
        for (int i = 0; i < clients.size(); i++) {
            std::cout << "Client " << i + 1 << ": " << clients[i].getName() << ": " << clients[i].getAddress() << ": " << clients[i].getSSN() << ": " << clients[i].getEmployer() << std::endl;
        }
        return;
    }
    void loadUsers() { //COMPLETE
        int userCount = 0; ///< loads users from a file
        ifstream userStream(USERS_SAVEFILE);
        if(userStream.peek() == std::ifstream::traits_type::eof()) { ///< checks if the file is empty
            std::cout << "Error: User file is empty" << std::endl;
            return;
        }
        string line;
        while(getline(userStream, line)) {
            stringstream ss(line);
            string accountType;
            getline(ss, accountType, ',');
            string username;
            getline(ss, username, ',');
            string password; 
            getline(ss, password, ',');
            if(accountType == "admin"){
                users.push_back(User(username, password, true));
                userCount++;
            } else {
                users.push_back(User(username, password, false));
                userCount++;
            }
        }
        std::cout << "Loaded " << userCount << " users from file" << std::endl;
        userStream.close();
        return;
    }
public:
    Account* getAccountBank(string accountNumber) { //COMPLETE
        /** function getAccountBank(string accountNumber) searches vector accountsBank for an account with the matching account number and returns it */
        for(int i = 0; i < accountsBank.size(); i++) {
            if(accountsBank[i].getAccountNumber() == accountNumber) {
                return &accountsBank[i];
            }
        }
        std::cout << "Error: Account not found" << std::endl;
        ///< return a pointer to an account object with empty account number
        Account *emptyAccount = new Account(0.0, "", "", ""); // Create a new Account object with empty account number
        return emptyAccount; // Return the empty account object
    }
    //this function prints each user in the users list by their username, password, and account type
    void printUsers() const { //COMPLETE
        for(int i = 0; i < users.size(); i++) {
            std::cout << "User " << i+1 << ": " << users[i].username << ": " << (users[i].isAdmin() ? "admin" : "teller") << std::endl;
        }
        return;
    }

    ///< this function saves the users to the staff.txt file
    void saveUsers() { //COMPLETE
        ofstream userStream(USERS_SAVEFILE);
        for(int i = 0; i < users.size(); i++) {
            userStream << (users[i].isAdmin() ? "admin" : "teller") << "," << users[i].username << "," << users[i].password << endl;
        }
        userStream.close();
        return;
    }
    ///< this function saves the clients to the client-info.txt file
    void saveClients() {
        ofstream clientStream(CLIENTS_SAVEFILE);
        for (int i = 0; i < clients.size(); i++) {
            clientStream << clients[i].getName() << "," 
                         << clients[i].getAddress() << "," 
                         << clients[i].getSSN() << "," 
                         << clients[i].getEmployer() << endl;
        }
        clientStream.close();
    }
    ///< this function saves the accounts to the account-info.txt file
    void saveAccounts() { //COMPLETE
        ofstream accountStream(ACCOUNTS_SAVEFILE);
        for(int i = 0; i < accountsBank.size(); i++) {
            accountStream << accountsBank[i].getBalance() << "," << accountsBank[i].getClient() << "," << accountsBank[i].getAccountNumber() << "," << accountsBank[i].getAccountType() << endl;
        }
        accountStream.close();
        return;
    }

    void addAccountBank(long double balance, string owner, string accountNumber, string accountType) { //COMPLETE
        /** adds an account to the bank */
        accountsBank.push_back(Account(balance, owner, accountNumber, accountType));
        saveAccounts(); ///< saves the accounts to the file
        return;
    }

    void deleteUser(string username) { //COMPLETE
        ///< checks is user exhists in the system and returns if not found does nothing if found
        if(this->getUser(username)->username == "") {
            std::cout << "Error: User not found" << std::endl;
            return;
        }
        /** deletes a user from the system */
        for(int i = 0; i < users.size(); i++) {
            if(users[i].username == username) {
                users.erase(users.begin() + i);
                std::cout << "User " << username << " deleted" << std::endl;
            }
        }
        this->saveUsers();
        return;
    }
    void printUsersBank() const { //COMPLETE  //< this function prints each username and whether they are an admin or teller on a new line then returns
        for(int i = 0; i < users.size(); i++) {
            std::cout << "User " << i+1 << ": " << users[i].username << ": " << (users[i].isAdmin() ? "admin" : "teller") << std::endl;
        }
        return;
    }
    void addClient(const Client& client){ ///<adds a client to the system
        clients.push_back(client);
        saveClients(); ///< saves the clients to the file
        return;
    } 
    void addUser(string username, string password, bool isAdmin) { ///<adds a user to the system
        ///< checks if the user already exists in the system
        ///< check if user with given username already exists
        for(int i = 0; i < users.size(); i++) {
            if(users[i].username == username) {
                std::cout << "Error: User already exists" << std::endl;
                return;
            }
        }
        /** adds a user to the system */
        this->users.push_back(User(username, password, isAdmin));
        saveUsers(); ///< saves the users to the file
        return;
    }
    string getName() const { ///< returns the name of the bank
        return name;
    }
    int getRoutingNumber() const { ///< returns the routing number of the bank
        return routingNumber;
    }
    Client* getClient(string clientName) {
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].getName() == clientName) {
                return &clients[i]; // Return a pointer to the existing client
            }
        }
        std::cout << "Error: Client not found" << std::endl;
        static Client defaultClient("", "", "", ""); // Create a default client object
        return &defaultClient; // Return nullptr if the client is not found
    }
    User* getUser(string username) {    ///COMPLETE
        ///< function getUser(string username) searches vector users for a user with the matching username and returns it
        for(int i = 0; i < users.size(); i++) {
            if(users[i].username == username) {
                return &users[i];
            }
        }
        std::cout << "Error: User not found" << std::endl;
        ///< return a user object with empty username
        //return a pointer to a user object with empty username
        User *emptyUser = new User("", "", false); // Create a new User object with empty username
        return emptyUser; // Return the empty user object
    }

    #ifdef DEBUG 
    static void staticFunction() {
        std::cout << "This is a static function" << std::endl;
        return;
    }
    #endif
    Bank(string nameInput, int routingNumberInput) : name(nameInput), routingNumber(routingNumberInput) {
        loadUsers(); ///< loads users from a file
        loadClients(); ///< loads clients from a file
        loadAccounts(); ///< loads accounts from a file

        #ifdef DEBUG ///< code to run when DEBUG is defined
        //printUsers();
        //printClients();
        //printAccountsBank();
        #endif
        return;
    }
    ///< Destructor
    ~Bank() {
        saveUsers(); ///< saves users to a file
        saveClients(); ///< saves clients to a file
        saveAccounts(); ///< saves accounts to a file
        std::cout << "Data Successfully Saved" << std::endl;
        return;
    }
};

}

#endif // BANKSOFT_HPP
// End of Banksoft.hpp