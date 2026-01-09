#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cstdio>
using namespace std;

class Account {
private:
    int accNo;
    string name;
    int deposit;
    char type;

public:
    // Default constructor
    Account() : accNo(0), name(""), deposit(0), type(' ') {}

    // Function to create account
    void createAccount() {
        cout << "Enter the account no: ";
        cin >> accNo;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter the account holder name: ";
        getline(cin, name);
        
        cout << "Enter the type of account [C/S]: ";
        cin >> type;
        
        cout << "Enter The Initial amount (>=500 for Saving and >=1000 for current): ";
        cin >> deposit;
        
        // Validate minimum deposit
        if ((type == 'S' || type == 's') && deposit < 500) {
            cout << "Error: Minimum 500 required for Saving account!" << endl;
            deposit = 0;
        } else if ((type == 'C' || type == 'c') && deposit < 1000) {
            cout << "Error: Minimum 1000 required for Current account!" << endl;
            deposit = 0;
        } else {
            cout << "\n\nAccount Created Successfully!" << endl;
        }
    }

    // Function to display account details
    void showAccount() const {
        cout << "Account Number: " << accNo << endl;
        cout << "Account Holder Name: " << name << endl;
        cout << "Type of Account: " << type << endl;
        cout << "Balance: " << deposit << endl;
    }

    // Function to modify account
    void modifyAccount() {
        cout << "Account Number: " << accNo << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Modify Account Holder Name: ";
        getline(cin, name);
        
        cout << "Modify type of Account [C/S]: ";
        cin >> type;
        
        cout << "Modify Balance: ";
        cin >> deposit;
    }

    // Function to deposit amount
    void depositAmount(int amount) {
        deposit += amount;
    }

    // Function to withdraw amount
    void withdrawAmount(int amount) {
        if (amount <= deposit) {
            deposit -= amount;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    }

    // Function to display account in report format
    void report() const {
        cout << accNo << "\t" << name << "\t" << type << "\t" << deposit << endl;
    }

    // Getters
    int getAccountNo() const { return accNo; }
    string getAccountHolderName() const { return name; }
    char getAccountType() const { return type; }
    int getDeposit() const { return deposit; }

    // Friend function for file operations
    friend ofstream& operator<<(ofstream& ofs, const Account& acc);
    friend ifstream& operator>>(ifstream& ifs, Account& acc);
};

// Overloaded output operator for file writing
ofstream& operator<<(ofstream& ofs, const Account& acc) {
    ofs << acc.accNo << endl;
    ofs << acc.name << endl;
    ofs << acc.type << endl;
    ofs << acc.deposit << endl;
    return ofs;
}

// Overloaded input operator for file reading
ifstream& operator>>(ifstream& ifs, Account& acc) {
    ifs >> acc.accNo;
    ifs.ignore();
    getline(ifs, acc.name);
    ifs >> acc.type;
    ifs >> acc.deposit;
    return ifs;
}

void intro() {
    cout << "\t\t\t\t**********************" << endl;
    cout << "\t\t\t\tBANK MANAGEMENT SYSTEM" << endl;
    cout << "\t\t\t\t**********************" << endl;
    cout << "\t\t\t\tBrought To You By:" << endl;
    cout << "\t\t\t\tprojectworlds.in" << endl;
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void writeAccount() {
    Account acc;
    acc.createAccount();
    
    ofstream outFile("accounts.data", ios::app);
    if (outFile.is_open()) {
        outFile << acc;
        outFile.close();
        cout << "Account saved successfully!" << endl;
    } else {
        cout << "Error opening file!" << endl;
    }
}

void displayAll() {
    ifstream inFile("accounts.data");
    if (!inFile) {
        cout << "No records to display" << endl;
        return;
    }
    
    cout << "\n\t\t\t\tACCOUNT HOLDER LIST" << endl;
    cout << "======================================================" << endl;
    cout << "A/c no.\t\tNAME\t\tType\t\tBalance" << endl;
    cout << "======================================================" << endl;
    
    Account acc;
    while (inFile >> acc) {
        acc.report();
    }
    inFile.close();
}

void displaySp(int num) {
    ifstream inFile("accounts.data");
    if (!inFile) {
        cout << "No records to search" << endl;
        return;
    }
    
    Account acc;
    bool found = false;
    
    while (inFile >> acc) {
        if (acc.getAccountNo() == num) {
            cout << "\nYour Account Details:" << endl;
            cout << "=====================" << endl;
            acc.showAccount();
            found = true;
            break;
        }
    }
    inFile.close();
    
    if (!found) {
        cout << "No existing record with account number: " << num << endl;
    }
}

void depositWithdraw(int num, int option) {
    vector<Account> accounts;
    Account acc;
    bool found = false;
    
    // Read all accounts
    ifstream inFile("accounts.data");
    if (!inFile) {
        cout << "No records to search" << endl;
        return;
    }
    
    while (inFile >> acc) {
        accounts.push_back(acc);
    }
    inFile.close();
    
    // Modify the specific account
    for (auto& account : accounts) {
        if (account.getAccountNo() == num) {
            found = true;
            if (option == 1) { // Deposit
                int amount;
                cout << "Enter the amount to deposit: ";
                cin >> amount;
                account.depositAmount(amount);
                cout << "Amount deposited successfully!" << endl;
            } else if (option == 2) { // Withdraw
                int amount;
                cout << "Enter the amount to withdraw: ";
                cin >> amount;
                account.withdrawAmount(amount);
                cout << "Amount withdrawn successfully!" << endl;
            }
            break;
        }
    }
    
    if (!found) {
        cout << "Account not found!" << endl;
        return;
    }
    
    // Write all accounts back to file
    ofstream outFile("accounts.data");
    if (outFile.is_open()) {
        for (const auto& account : accounts) {
            outFile << account;
        }
        outFile.close();
    }
}

void deleteAccount(int num) {
    vector<Account> accounts;
    Account acc;
    
    // Read all accounts
    ifstream inFile("accounts.data");
    if (!inFile) {
        cout << "No records to delete from" << endl;
        return;
    }
    
    while (inFile >> acc) {
        if (acc.getAccountNo() != num) {
            accounts.push_back(acc);
        }
    }
    inFile.close();
    
    // Remove the old file and write new one
    remove("accounts.data");
    
    ofstream outFile("accounts.data");
    if (outFile.is_open()) {
        for (const auto& account : accounts) {
            outFile << account;
        }
        outFile.close();
        cout << "Account deleted successfully!" << endl;
    }
}

void modifyAccount(int num) {
    vector<Account> accounts;
    Account acc;
    bool found = false;
    
    // Read all accounts
    ifstream inFile("accounts.data");
    if (!inFile) {
        cout << "No records to modify" << endl;
        return;
    }
    
    while (inFile >> acc) {
        accounts.push_back(acc);
    }
    inFile.close();
    
    // Modify the specific account
    for (auto& account : accounts) {
        if (account.getAccountNo() == num) {
            found = true;
            cout << "\nCurrent Account Details:" << endl;
            account.showAccount();
            cout << "\nEnter new details:" << endl;
            account.modifyAccount();
            cout << "Account modified successfully!" << endl;
            break;
        }
    }
    
    if (!found) {
        cout << "Account not found!" << endl;
        return;
    }
    
    // Write all accounts back to file
    remove("accounts.data");
    ofstream outFile("accounts.data");
    if (outFile.is_open()) {
        for (const auto& account : accounts) {
            outFile << account;
        }
        outFile.close();
    }
}

int main() {
    int ch;
    int num;
    
    intro();
    
    do {
        // system("cls"); // Uncomment for Windows
        // system("clear"); // Uncomment for Linux/Mac
        
        cout << "\n\tMAIN MENU" << endl;
        cout << "\t1. NEW ACCOUNT" << endl;
        cout << "\t2. DEPOSIT AMOUNT" << endl;
        cout << "\t3. WITHDRAW AMOUNT" << endl;
        cout << "\t4. BALANCE ENQUIRY" << endl;
        cout << "\t5. ALL ACCOUNT HOLDER LIST" << endl;
        cout << "\t6. CLOSE AN ACCOUNT" << endl;
        cout << "\t7. MODIFY AN ACCOUNT" << endl;
        cout << "\t8. EXIT" << endl;
        cout << "\tSelect Your Option (1-8): ";
        cin >> ch;
        
        // system("cls"); // Uncomment for Windows
        // system("clear"); // Uncomment for Linux/Mac
        
        switch(ch) {
            case 1:
                writeAccount();
                break;
            case 2:
                cout << "\tEnter The account No.: ";
                cin >> num;
                depositWithdraw(num, 1);
                break;
            case 3:
                cout << "\tEnter The account No.: ";
                cin >> num;
                depositWithdraw(num, 2);
                break;
            case 4:
                cout << "\tEnter The account No.: ";
                cin >> num;
                displaySp(num);
                break;
            case 5:
                displayAll();
                break;
            case 6:
                cout << "\tEnter The account No.: ";
                cin >> num;
                deleteAccount(num);
                break;
            case 7:
                cout << "\tEnter The account No.: ";
                cin >> num;
                modifyAccount(num);
                break;
            case 8:
                cout << "\tThanks for using bank management system" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
        
        if (ch != 8) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
        
    } while(ch != 8);
    
    return 0;
}