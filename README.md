# Banksoft - Bank Teller System

[![C++](https://img.shields.io/badge/C%2B%2B-14-blue.svg)](https://isocpp.org/)

A simple command-line bank teller system implemented in C++. Allows for managing staff (admins/tellers), clients, and client accounts.

## Features

*   User authentication (Admin/Teller roles)
*   Password management (change password, optional password hiding)
*   Client management (add, edit client details)
*   Account management (add account, deposit, withdraw)
*   Data persistence via text files

## Compilation

This project uses a `makefile` for easy compilation. Ensure you have `g++` installed.

*   **Compile all versions:**
    ```bash
    make
    ```
    This creates three executables:
    *   `main.elf`: Production version with password hiding enabled.
    *   `main_showpw.elf`: Version without password hiding (shows typed password).
    *   `main_debug.elf`: Debug version with extra logging and debugging symbols.

*   **Compile and run production version:**
    ```bash
    make run_prod
    ```

*   **Compile and run debug version:**
    ```bash
    make run_debug
    ```

*   **Compile and run show password version:**
    ```bash
    make run_showpw
    ```

*   **Clean build files:**
    ```bash
    make clean
    ```

## Usage

Run the desired executable from your terminal:

*   **Production:**
    ```bash
    ./main.elf
    ```
*   **Show Password:**
    ```bash
    ./main_showpw.elf
    ```
*   **Debug:**
    ```bash
    ./main_debug.elf
    ```

Follow the on-screen prompts to log in and navigate the menus.

## Data Files

The application requires three data files in the same directory as the executable. Ensure these files exist, even if empty, before running the application for the first time.

1.  **`staff.txt`**
    *   Stores user login information (admins and tellers).
    *   **Format:** Comma-Separated Values (CSV)
    *   **Columns:** `accountType,username,password`
    *   **Example:**
        ```csv
        admin,harrison,chb12345
        teller,john,abc123
        ```
    *   `accountType` must be either `admin` or `teller`.

2.  **`client-info.txt`**
    *   Stores client profile information.
    *   **Format:** Comma-Separated Values (CSV)
    *   **Columns:** `name,address,ssn,employer,annualIncome`
    *   **Example:**
        ```csv
        Charles Brammell,123 Oak St,111-22-333,Tech Corp,85000
        Eleanor Vance,456 Pine Ave,222-33-444,City Hospital,62000
        ```

3.  **`account-info.txt`**
    *   Stores bank account details linked to clients.
    *   **Format:** Comma-Separated Values (CSV)
    *   **Columns:** `balance,ownerName,accountType,accountNumber`
    *   **Example:**
        ```csv
        5000,Charles Brammell,Checking,123456789
        15000,Charles Brammell,Savings,987654321
        ```
    *   `ownerName` must match a `name` in `client-info.txt`.
    *   `balance` should be a numeric value (long double).

**Important:** The application reads these files on startup and saves them on exit or when explicitly saved through the menu. Ensure the files have the correct permissions for reading and writing.

*readme generated using ai
