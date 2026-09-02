// Banking System
// Built for the CodeAlpha C++ internship task.
// Models a simple bank with Customer, Account and Transaction classes.
// Supports deposits, withdrawals, transfers between accounts, and keeps
// a transaction history per account.

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <cstring>

using namespace std;

// ---------- Transaction ----------
// Represents a single record of money movement on an account.
// Every deposit, withdrawal or transfer creates one of these so the
// account can keep a full history.
class Transaction {
public:
    string type;        // "Deposit", "Withdrawal", "Transfer In", "Transfer Out"
    double amount;
    double balanceAfter; // account balance right after this transaction
    string timestamp;

    Transaction(string type, double amount, double balanceAfter)
        : type(type), amount(amount), balanceAfter(balanceAfter) {
        timestamp = currentTime();
    }

    // grabs the current system time as a readable string
    static string currentTime() {
        time_t now = time(0);
        char buf[26];
        // ctime() adds a trailing newline, strip it off
        strncpy(buf, ctime(&now), 25);
        buf[24] = '\0';
        return string(buf);
    }

    void print() const {
        cout << "  [" << timestamp << "] " << left << setw(14) << type
             << " Amount: " << fixed << setprecision(2) << setw(10) << amount
             << " Balance after: " << balanceAfter << "\n";
    }
};

// ---------- Account ----------
// Holds the balance and transaction history for one bank account.
// Deposit/withdraw logic lives here so an Account always keeps itself
// consistent (balance can't go negative, every change is logged).
class Account {
public:
    int accountNumber;
    string ownerName;
    double balance;
    vector<Transaction> history;

    Account(int accountNumber, string ownerName, double openingBalance = 0.0)
        : accountNumber(accountNumber), ownerName(ownerName), balance(openingBalance) {}

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Deposit amount must be positive.\n";
            return;
        }
        balance += amount;
        history.push_back(Transaction("Deposit", amount, balance));
        cout << "Deposited " << fixed << setprecision(2) << amount
             << ". New balance: " << balance << "\n";
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Withdrawal amount must be positive.\n";
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient funds. Current balance: " << fixed
                 << setprecision(2) << balance << "\n";
            return false;
        }
        balance -= amount;
        history.push_back(Transaction("Withdrawal", amount, balance));
        cout << "Withdrew " << fixed << setprecision(2) << amount
             << ". New balance: " << balance << "\n";
        return true;
    }

    // used internally by transfers so both sides log the right label
    void recordTransferOut(double amount) {
        balance -= amount;
        history.push_back(Transaction("Transfer Out", amount, balance));
    }

    void recordTransferIn(double amount) {
        balance += amount;
        history.push_back(Transaction("Transfer In", amount, balance));
    }

    void displayInfo() const {
        cout << "\n--- Account Info ---\n";
        cout << "Account Number: " << accountNumber << "\n";
        cout << "Owner: " << ownerName << "\n";
        cout << "Balance: " << fixed << setprecision(2) << balance << "\n";
    }

    // shows the most recent transactions, newest first
    void showRecentTransactions(int count = 5) const {
        cout << "\n--- Recent Transactions (Account " << accountNumber << ") ---\n";
        if (history.empty()) {
            cout << "  No transactions yet.\n";
            return;
        }
        int shown = 0;
        for (int i = (int)history.size() - 1; i >= 0 && shown < count; i--, shown++) {
            history[i].print();
        }
    }
};

// ---------- Customer ----------
// A customer can hold multiple accounts (e.g. savings and current).
// This class is mostly a container that links a person to their accounts.
class Customer {
public:
    int customerId;
    string name;
    string email;
    vector<Account> accounts;

    Customer(int customerId, string name, string email)
        : customerId(customerId), name(name), email(email) {}

    Account* findAccount(int accountNumber) {
        for (auto& acc : accounts) {
            if (acc.accountNumber == accountNumber)
                return &acc;
        }
        return nullptr;
    }

    void displayProfile() const {
        cout << "\nCustomer #" << customerId << " - " << name << " (" << email << ")\n";
        cout << "Accounts: " << accounts.size() << "\n";
        for (const auto& acc : accounts) {
            cout << "  - Account " << acc.accountNumber << ": "
                 << fixed << setprecision(2) << acc.balance << "\n";
        }
    }
};

// ---------- Bank ----------
// Top-level manager: keeps track of every customer and account, and is
// the only place that knows how to look an account up by number, which
// is what makes transfers between different customers possible.
class Bank {
private:
    vector<Customer> customers;
    int nextCustomerId = 1;
    int nextAccountNumber = 1001;

public:
    Customer* createCustomer(string name, string email) {
        customers.emplace_back(nextCustomerId++, name, email);
        cout << "Created customer #" << customers.back().customerId
             << " (" << name << ")\n";
        return &customers.back();
    }

    Account* createAccount(Customer* customer, double openingBalance = 0.0) {
        if (!customer) return nullptr;
        customer->accounts.emplace_back(nextAccountNumber++, customer->name, openingBalance);
        cout << "Opened account " << customer->accounts.back().accountNumber
             << " for " << customer->name << "\n";
        return &customer->accounts.back();
    }

    // searches every customer's accounts for a matching account number
    Account* findAccountByNumber(int accountNumber) {
        for (auto& cust : customers) {
            Account* acc = cust.findAccount(accountNumber);
            if (acc) return acc;
        }
        return nullptr;
    }

    Customer* findCustomerById(int customerId) {
        for (auto& cust : customers) {
            if (cust.customerId == customerId)
                return &cust;
        }
        return nullptr;
    }

    // moves money from one account to another, only if the source
    // account actually has enough funds
    bool transfer(int fromAccNum, int toAccNum, double amount) {
        Account* from = findAccountByNumber(fromAccNum);
        Account* to = findAccountByNumber(toAccNum);

        if (!from || !to) {
            cout << "One or both account numbers were not found.\n";
            return false;
        }
        if (amount <= 0) {
            cout << "Transfer amount must be positive.\n";
            return false;
        }
        if (amount > from->balance) {
            cout << "Insufficient funds for transfer.\n";
            return false;
        }

        from->recordTransferOut(amount);
        to->recordTransferIn(amount);
        cout << "Transferred " << fixed << setprecision(2) << amount
             << " from account " << fromAccNum << " to account " << toAccNum << "\n";
        return true;
    }

    void listCustomers() const {
        cout << "\n--- All Customers ---\n";
        for (const auto& cust : customers) {
            cust.displayProfile();
        }
    }
};

// ---------- Console menu ----------
// Small interactive loop so the system can be tried out by hand.
void showMenu() {
    cout << "\n===== Banking System =====\n";
    cout << "1. Create customer\n";
    cout << "2. Open account for customer\n";
    cout << "3. Deposit\n";
    cout << "4. Withdraw\n";
    cout << "5. Transfer funds\n";
    cout << "6. View account info\n";
    cout << "7. View recent transactions\n";
    cout << "8. List all customers\n";
    cout << "0. Exit\n";
    cout << "Choice: ";
}

int main() {
    Bank bank;

    // seed a couple of customers/accounts so the menu has something to
    // work with right away, without needing manual setup every run
    Customer* alice = bank.createCustomer("Alice Johnson", "alice@example.com");
    bank.createAccount(alice, 500.0);
    Customer* bob = bank.createCustomer("Bob Smith", "bob@example.com");
    bank.createAccount(bob, 200.0);

    int choice;
    do {
        showMenu();
        cin >> choice;

        if (choice == 1) {
            string name, email;
            cout << "Name: "; cin.ignore(); getline(cin, name);
            cout << "Email: "; getline(cin, email);
            bank.createCustomer(name, email);

        } else if (choice == 2) {
            int custId;
            double opening;
            cout << "Customer ID: "; cin >> custId;
            cout << "Opening balance: "; cin >> opening;
            Customer* cust = bank.findCustomerById(custId);
            if (cust) bank.createAccount(cust, opening);
            else cout << "Customer not found.\n";

        } else if (choice == 3) {
            int accNum; double amount;
            cout << "Account number: "; cin >> accNum;
            cout << "Amount: "; cin >> amount;
            Account* acc = bank.findAccountByNumber(accNum);
            if (acc) acc->deposit(amount);
            else cout << "Account not found.\n";

        } else if (choice == 4) {
            int accNum; double amount;
            cout << "Account number: "; cin >> accNum;
            cout << "Amount: "; cin >> amount;
            Account* acc = bank.findAccountByNumber(accNum);
            if (acc) acc->withdraw(amount);
            else cout << "Account not found.\n";

        } else if (choice == 5) {
            int fromAcc, toAcc; double amount;
            cout << "From account: "; cin >> fromAcc;
            cout << "To account: "; cin >> toAcc;
            cout << "Amount: "; cin >> amount;
            bank.transfer(fromAcc, toAcc, amount);

        } else if (choice == 6) {
            int accNum;
            cout << "Account number: "; cin >> accNum;
            Account* acc = bank.findAccountByNumber(accNum);
            if (acc) acc->displayInfo();
            else cout << "Account not found.\n";

        } else if (choice == 7) {
            int accNum;
            cout << "Account number: "; cin >> accNum;
            Account* acc = bank.findAccountByNumber(accNum);
            if (acc) acc->showRecentTransactions();
            else cout << "Account not found.\n";

        } else if (choice == 8) {
            bank.listCustomers();

        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    cout << "Goodbye.\n";
    return 0;
}