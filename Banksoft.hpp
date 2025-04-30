/*
Charles Harrison Brammell
COMP 2710: Software Construction - Spring 2025
chb0055@auburn.edu
Final Project: Bank Teller System
*/

#ifdef BANKSOFT_HPP
#define BANKSOFT_HPP
// Banksoft.hpp
// This is a header file for a banking system software project.
// It contains class definitions for User, Admin, Teller, Client, Account, and Bank.
// The classes are designed to manage user accounts, client information, and bank accounts.

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class User { // base class for all user accounts on system, inhereited by admin and teller classes
    const string username;
    string password; //password is not const so it can be changed
public:
    bool authenticate(string pwInput) {
        if(pwInput == password) {
            return true;
        } else {
            return false;
        }
    }
    void changePassword(string newPassword) {
        password = newPassword;
        return;
    }
    virtual bool isAdmin(); //virtual function to be overridden by derived classes
    User(string username, string password) : username(username), password(password) {}
};

class Admin : public User { // derived class for admin accounts
public:
    bool isAdmin() {
        return true;
    }
    Admin(string username, string password) : User(username, password) {}
};

class Teller : public User {  //derived class for teller accounts
public:
    bool isAdmin() {
        return false;
    }
    Teller(string username, string password) : User(username, password) {}
};

class Account; // forward declaration of Account class to be used in Client class

class Client {
    const string name;
    const string address;
    const int ssn;
    const string employer;
    std::vector<Account> accounts;
public:
    const string getName() {
        return name;
    }
    const string getAddress() {
        return address;
    }
    const int getSSN() {
        return ssn;
    }
    const string getEmployer() {
        return employer;
    }
    void addAccount() {
        //implement function to add account to client account list
        return;
    }
    Client(string name, string address, int ssn, string employer) : name(name), address(address), ssn(ssn), employer(employer) {}
};

class Account { // class for all open bank accounts, give the constructor a reference to the client object that owns it
    int balance = 0;
    const Client* clientPtr;
    const int accountNumber;
    const string accountType;
public:
    void deposit(int amount) {
        balance += amount;
        return;
    }
    void withdraw(int amount) {
        if(amount > balance) {
            std::cout << "Error :Insufficient funds" << std::endl;
        } else if(amount < 0) {
            std::cout << "Error: Cannot withdraw negative amount" << std::endl;
        } else {
            balance -= amount;
        }
        return;
    }
    string getAccountType() {
        return accountType;
    }
    int getAccountNumber() {
        return accountNumber;
    }
    const Client* getClient() {
        return clientPtr;
    }
    int getBalance() {
        return balance;
    }
    Account(int balance, Client* owner, int accountNumber, string accountType) : balance(balance), clientPtr(owner), accountNumber(accountNumber), accountType(accountType) {}
};

class Bank {
    const string name;
    const int routingNumber;
    std::vector<Client> clients;
    std::vector<User> users;
public:
    void addClient(); //adds a client to the system
    void addUser(); //adds a user to the system
    Client getClient(); //searches list of clients for a client with the given ssn and returns it
    User getUser(); //searches list of users for a user with the given username and returns it
    Bank(string nameInput, int routingNumberInput) : name(nameInput), routingNumber(routingNumberInput) {}
};

#endif // BANKSOFT_HPP
// End of Banksoft.hpp