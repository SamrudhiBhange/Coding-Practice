#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Account {
private:
    int accNo;
    string name;
    double deposit;
    char type;
    
public:
    int getAccountNo() const { return accNo; }
    
    static Account deserialize(const string& data) {
        Account acc;
        stringstream ss(data);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 4) {
            acc.accNo = stoi(tokens[0]);
            acc.name = tokens[1];
            acc.deposit = stod(tokens[2]);
            acc.type = tokens[3][0];
        }
        
        return acc;
    }
    
    string serialize() const {
        ostringstream oss;
        oss << accNo << "|" << name << "|" << deposit << "|" << type;
        return oss.str();
    }
};

int main() {
    cout << "Content-Type: text/html\n\n";
    
    // Get account number from query string
    char* query = getenv("QUERY_STRING");
    if (!query) {
        cout << "<h1>Error: No query string</h1>";
        return 1;
    }
    
    string queryStr = query;
    int accNo = 0;
    
    // Parse query string
    size_t accPos = queryStr.find("accNo=");
    if (accPos != string::npos) {
        accPos += 6;
        size_t accEnd = queryStr.find("&", accPos);
        if (accEnd == string::npos) accEnd = queryStr.length();
        string accStr = queryStr.substr(accPos, accEnd - accPos);
        accNo = stoi(accStr);
    }
    
    if (accNo == 0) {
        cout << "<h1>Error: Invalid account number</h1>";
        return 1;
    }
    
    // Read accounts
    vector<Account> accounts;
    ifstream file("data/accounts.data");
    
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                accounts.push_back(Account::deserialize(line));
            }
        }
        file.close();
    }
    
    // Delete account
    vector<Account> newAccounts;
    bool found = false;
    
    for (const auto& acc : accounts) {
        if (acc.getAccountNo() == accNo) {
            found = true;
        } else {
            newAccounts.push_back(acc);
        }
    }
    
    if (!found) {
        cout << "<h1>Error: Account not found</h1>";
        return 1;
    }
    
    // Write back to file
    ofstream outFile("data/accounts.data");
    if (outFile.is_open()) {
        for (const auto& acc : newAccounts) {
            outFile << acc.serialize() << endl;
        }
        outFile.close();
    }
    
    cout << "<h1>Account Deleted Successfully</h1>";
    cout << "<p>Account " << accNo << " has been deleted.</p>";
    cout << "<a href='/static/index.html'>Back to Home</a>";
    
    return 0;
}