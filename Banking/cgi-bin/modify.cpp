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
    string getName() const { return name; }
    double getDeposit() const { return deposit; }
    char getType() const { return type; }
    
    void setName(const string& n) { name = n; }
    void setDeposit(double d) { deposit = d; }
    void setType(char t) { type = t; }
    
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
    
    // Check request method
    char* method = getenv("REQUEST_METHOD");
    if (!method || string(method) != "POST") {
        cout << "<h1>Error: POST method required</h1>";
        return 1;
    }
    
    // Read POST data
    string postData;
    char* contentLength = getenv("CONTENT_LENGTH");
    if (contentLength) {
        int length = atoi(contentLength);
        if (length > 0) {
            char* buffer = new char[length + 1];
            cin.read(buffer, length);
            buffer[length] = '\0';
            postData = buffer;
            delete[] buffer;
        }
    }
    
    // Parse POST data (simple parsing)
    int accNo = 0;
    string name;
    char type = ' ';
    double deposit = 0;
    
    // Simple parsing - in production use proper parsing
    size_t pos;
    
    pos = postData.find("accNo=");
    if (pos != string::npos) {
        pos += 6;
        size_t end = postData.find("&", pos);
        if (end == string::npos) end = postData.length();
        string value = postData.substr(pos, end - pos);
        accNo = stoi(value);
    }
    
    pos = postData.find("name=");
    if (pos != string::npos) {
        pos += 5;
        size_t end = postData.find("&", pos);
        if (end == string::npos) end = postData.length();
        name = postData.substr(pos, end - pos);
        // URL decode
        for (size_t i = 0; i < name.length(); i++) {
            if (name[i] == '+') name[i] = ' ';
        }
    }
    
    pos = postData.find("type=");
    if (pos != string::npos) {
        pos += 5;
        size_t end = postData.find("&", pos);
        if (end == string::npos) end = postData.length();
        string value = postData.substr(pos, end - pos);
        if (!value.empty()) type = value[0];
    }
    
    pos = postData.find("deposit=");
    if (pos != string::npos) {
        pos += 8;
        size_t end = postData.find("&", pos);
        if (end == string::npos) end = postData.length();
        string value = postData.substr(pos, end - pos);
        deposit = stod(value);
    }
    
    if (accNo == 0 || name.empty() || type == ' ' || deposit < 0) {
        cout << "<h1>Error: Invalid input</h1>";
        return 1;
    }
    
    // Validate minimum deposit
    if ((type == 'S' || type == 's') && deposit < 500) {
        cout << "<h1>Error: Minimum deposit for Savings account is 500</h1>";
        return 1;
    }
    
    if ((type == 'C' || type == 'c') && deposit < 1000) {
        cout << "<h1>Error: Minimum deposit for Current account is 1000</h1>";
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
    
    // Find and modify account
    bool found = false;
    for (auto& acc : accounts) {
        if (acc.getAccountNo() == accNo) {
            acc.setName(name);
            acc.setType(type);
            acc.setDeposit(deposit);
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "<h1>Error: Account not found</h1>";
        return 1;
    }
    
    // Write back to file
    ofstream outFile("data/accounts.data");
    if (outFile.is_open()) {
        for (const auto& acc : accounts) {
            outFile << acc.serialize() << endl;
        }
        outFile.close();
    }
    
    cout << "<h1>Account Modified Successfully</h1>";
    cout << "<p>Account " << accNo << " has been updated.</p>";
    cout << "<a href='/static/index.html'>Back to Home</a>";
    
    return 0;
}