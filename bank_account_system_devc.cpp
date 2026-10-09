#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

// Transaction Structure
struct Transaction {
    string date;
    string type;      // "Deposit" or "Withdraw"
    double amount;
    double balanceAfter;
};

// BankAccount Class Definition
class BankAccount {
private:
    string accountNumber;
    string accountHolder;
    double balance;
    vector<Transaction> records;
    string recordsFile;
    string balanceFile;

    // Helper function to convert number to string (C++98 compatible)
    string doubleToString(double num) {
        ostringstream oss;
        oss << fixed << setprecision(2) << num;
        return oss.str();
    }

    string intToString(int num) {
        ostringstream oss;
        oss << num;
        return oss.str();
    }

public:
    // Constructor
    BankAccount(string accNum = "", string accHolder = "", double initialBalance = 0.0) {
        accountNumber = accNum;
        accountHolder = accHolder;
        balance = initialBalance;
        recordsFile = "transactions.txt";
        balanceFile = "balance.txt";
        loadBalance();
    }

    // Get current date and time
    string getCurrentDateTime() {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        string dateStr = intToString(timeinfo->tm_mday) + "/" + 
                        intToString(timeinfo->tm_mon + 1) + "/" + 
                        intToString(timeinfo->tm_year + 1900) + " " +
                        intToString(timeinfo->tm_hour) + ":" + 
                        intToString(timeinfo->tm_min) + ":" + 
                        intToString(timeinfo->tm_sec);
        return dateStr;
    }

    // Deposit Money Operation
    bool deposit(double amount) {
        // Input validation
        if (amount <= 0) {
            cout << "\n[ERROR] Invalid deposit amount! Amount must be positive." << endl;
            return false;
        }

        balance += amount;
        
        // Create transaction record
        Transaction trans;
        trans.date = getCurrentDateTime();
        trans.type = "Deposit";
        trans.amount = amount;
        trans.balanceAfter = balance;
        records.push_back(trans);

        // Save to files
        saveBalance();
        saveTransaction(trans);

        cout << "\n[SUCCESS] Deposit Successful!" << endl;
        cout << "Amount Deposited: Rs. " << fixed << setprecision(2) << amount << endl;
        cout << "Current Balance: Rs. " << balance << endl;
        return true;
    }

    // Withdraw Money Operation
    bool withdraw(double amount) {
        // Input validation
        if (amount <= 0) {
            cout << "\n[ERROR] Invalid withdrawal amount! Amount must be positive." << endl;
            return false;
        }

        // Check sufficient balance
        if (amount > balance) {
            cout << "\n[ERROR] Insufficient Funds!" << endl;
            cout << "Available Balance: Rs. " << fixed << setprecision(2) << balance << endl;
            cout << "Requested Amount: Rs. " << amount << endl;
            return false;
        }

        balance -= amount;

        // Create transaction record
        Transaction trans;
        trans.date = getCurrentDateTime();
        trans.type = "Withdraw";
        trans.amount = amount;
        trans.balanceAfter = balance;
        records.push_back(trans);

        // Save to files
        saveBalance();
        saveTransaction(trans);

        cout << "\n[SUCCESS] Withdrawal Successful!" << endl;
        cout << "Amount Withdrawn: Rs. " << fixed << setprecision(2) << amount << endl;
        cout << "Current Balance: Rs. " << balance << endl;
        return true;
    }

    // Check Account Balance
    void checkBalance() {
        cout << "\n" << string(50, '=') << endl;
        cout << "          ACCOUNT BALANCE" << endl;
        cout << string(50, '=') << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Current Balance: Rs. " << fixed << setprecision(2) << balance << endl;
        cout << string(50, '=') << endl;
    }

    // View Transaction Records
    void viewRecords() {
        cout << "\n" << string(80, '=') << endl;
        cout << "              TRANSACTION HISTORY" << endl;
        cout << string(80, '=') << endl;

        if (records.empty()) {
            cout << "No transactions found!" << endl;
            cout << string(80, '=') << endl;
            return;
        }

        cout << left << setw(20) << "Date/Time" 
             << setw(15) << "Type" 
             << setw(15) << "Amount" 
             << setw(15) << "Balance After" << endl;
        cout << string(80, '-') << endl;

        // Use traditional for loop (C++98 compatible)
        for (int i = 0; i < records.size(); i++) {
            cout << left << setw(20) << records[i].date
                 << setw(15) << records[i].type
                 << setw(15) << "Rs. " + doubleToString(records[i].amount)
                 << setw(15) << "Rs. " + doubleToString(records[i].balanceAfter) << endl;
        }
        cout << string(80, '=') << endl;
    }

    // Save balance to file
    void saveBalance() {
        ofstream outFile(balanceFile.c_str());
        if (outFile.is_open()) {
            outFile << accountNumber << "\n"
                    << accountHolder << "\n"
                    << fixed << setprecision(2) << balance;
            outFile.close();
        }
    }

    // Load balance from file
    void loadBalance() {
        ifstream inFile(balanceFile.c_str());
        if (inFile.is_open()) {
            inFile >> accountNumber >> accountHolder >> balance;
            inFile.close();
        }
    }

    // Save single transaction to file
    void saveTransaction(const Transaction& trans) {
        ofstream outFile(recordsFile.c_str(), ios::app);
        if (outFile.is_open()) {
            outFile << trans.date << " | "
                    << trans.type << " | "
                    << fixed << setprecision(2) << trans.amount << " | "
                    << trans.balanceAfter << "\n";
            outFile.close();
        }
    }

    // Get balance
    double getBalance() const {
        return balance;
    }
};

// Function to display menu
void displayMenu() {
    cout << "\n" << string(50, '=') << endl;
    cout << "       BANK ACCOUNT MANAGEMENT SYSTEM" << endl;
    cout << string(50, '=') << endl;
    cout << "1. Deposit Money" << endl;
    cout << "2. Withdraw Money" << endl;
    cout << "3. Check Balance" << endl;
    cout << "4. View Transaction Records" << endl;
    cout << "5. Exit" << endl;
    cout << string(50, '=') << endl;
    cout << "Enter your choice (1-5): ";
}

// Input validation function
int getValidChoice() {
    int choice;
    while (!(cin >> choice) || choice < 1 || choice > 5) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "[ERROR] Invalid choice! Please enter a number between 1 and 5: ";
    }
    cin.ignore(10000, '\n');
    return choice;
}

// Get valid amount input
double getValidAmount(string operation) {
    double amount;
    cout << "Enter amount to " << operation << " (Rs.): ";
    while (!(cin >> amount) || amount < 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "[ERROR] Invalid amount! Please enter a positive number: ";
    }
    cin.ignore(10000, '\n');
    return amount;
}

// Main Program
int main() {
    // Initialize BankAccount object
    BankAccount account("ACC001", "Customer", 5000.0);

    cout << "\n" << string(50, '=') << endl;
    cout << "    WELCOME TO BANK ACCOUNT SYSTEM" << endl;
    cout << string(50, '=') << endl;
    cout << "Account Number: ACC001" << endl;
    cout << "Initial Balance: Rs. 5000.00" << endl;

    int choice;
    bool exitFlag = false;

    // Main program loop
    while (!exitFlag) {
        displayMenu();
        choice = getValidChoice();

        switch (choice) {
            case 1: {
                // Deposit operation
                double depositAmount = getValidAmount("deposit");
                account.deposit(depositAmount);
                break;
            }
            case 2: {
                // Withdraw operation
                double withdrawAmount = getValidAmount("withdraw");
                account.withdraw(withdrawAmount);
                break;
            }
            case 3: {
                // Check balance
                account.checkBalance();
                break;
            }
            case 4: {
                // View records
                account.viewRecords();
                break;
            }
            case 5: {
                // Exit
                cout << "\n[INFO] Closing all files..." << endl;
                cout << "[INFO] Thank you for using Bank Account System!" << endl;
                cout << "[INFO] Program terminated successfully." << endl;
                cout << string(50, '=') << endl;
                exitFlag = true;
                break;
            }
            default: {
                cout << "\n[ERROR] Invalid Choice!" << endl;
                break;
            }
        }
    }

    return 0;
}
