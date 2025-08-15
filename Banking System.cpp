#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
using namespace std;

// Transaction class
class Transaction {
public:
    string type;      // Deposit, Withdrawal, Transfer
    double amount;
    string date;

    Transaction(string t, double a) {
        type = t;
        amount = a;
        time_t now = time(0);
        date = ctime(&now);
        date.pop_back(); // remove newline
    }

    void display() const {
        cout << setw(12) << type << " | "
             << setw(8) << amount << " | "
             << date << endl;
    }
};

// Account class
class Account {
private:
    int accountNumber;
    double balance;
    vector<Transaction> history;

public:
    Account(int accNo, double initialBalance = 0.0) {
        accountNumber = accNo;
        balance = initialBalance;
    }

    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount!" << endl;
            return;
        }
        balance += amount;
        history.emplace_back("Deposit", amount);
        cout << "Deposited: " << amount << endl;
    }

    void withdraw(double amount) {
        if (amount <= 0 || amount > balance) {
            cout << "Invalid withdrawal amount!" << endl;
            return;
        }
        balance -= amount;
        history.emplace_back("Withdraw", amount);
        cout << "Withdrew: " << amount << endl;
    }

    bool transfer(Account &toAccount, double amount) {
        if (amount <= 0 || amount > balance) {
            cout << "Invalid transfer amount!" << endl;
            return false;
        }
        balance -= amount;
        toAccount.balance += amount;
        history.emplace_back("TransferOut", amount);
        toAccount.history.emplace_back("TransferIn", amount);
        cout << "Transferred " << amount << " to account " << toAccount.getAccountNumber() << endl;
        return true;
    }

    void displayAccountInfo() const {
        cout << "\n--- Account Info ---" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: " << balance << endl;
    }

    void displayTransactionHistory() const {
        cout << "\n--- Transaction History ---" << endl;
        cout << setw(12) << "Type" << " | "
             << setw(8) << "Amount" << " | "
             << "Date" << endl;
        cout << "---------------------------------------------" << endl;
        for (const auto &t : history) {
            t.display();
        }
    }
};

// Customer class
class Customer {
private:
    string name;
    int customerID;
    vector<Account> accounts;

public:
    Customer(string n, int id) {
        name = n;
        customerID = id;
    }

    void createAccount(int accountNumber, double initialBalance = 0.0) {
        accounts.emplace_back(accountNumber, initialBalance);
        cout << "Account " << accountNumber << " created for " << name << endl;
    }

    Account* getAccount(int accountNumber) {
        for (auto &acc : accounts) {
            if (acc.getAccountNumber() == accountNumber) {
                return &acc;
            }
        }
        return nullptr;
    }

    void displayCustomerInfo() const {
        cout << "\nCustomer Name: " << name << endl;
        cout << "Customer ID: " << customerID << endl;
        cout << "Accounts: ";
        for (const auto &acc : accounts) {
            cout << acc.getAccountNumber() << " ";
        }
        cout << endl;
    }
};

int main() {
    // Create customers
    Customer cust1("Alice", 101);
    Customer cust2("Bob", 102);

    // Create accounts
    cust1.createAccount(1001, 5000);
    cust2.createAccount(2001, 3000);

    // Transactions
    Account* acc1 = cust1.getAccount(1001);
    Account* acc2 = cust2.getAccount(2001);

    if (acc1 && acc2) {
        acc1->deposit(2000);
        acc1->withdraw(1500);
        acc1->transfer(*acc2, 1000);
    }

    // Display account info & history
    cust1.displayCustomerInfo();
    if (acc1) {
        acc1->displayAccountInfo();
        acc1->displayTransactionHistory();
    }

    cust2.displayCustomerInfo();
    if (acc2) {
        acc2->displayAccountInfo();
        acc2->displayTransactionHistory();
    }

    return 0;
}
