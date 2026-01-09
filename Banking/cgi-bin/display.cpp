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
    string createdDate;
    
public:
    static Account deserialize(const string& data) {
        Account acc;
        stringstream ss(data);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 5) {
            acc.accNo = stoi(tokens[0]);
            acc.name = tokens[1];
            acc.deposit = stod(tokens[2]);
            acc.type = tokens[3][0];
            acc.createdDate = tokens[4];
        }
        
        return acc;
    }
    
    string toHTML() const {
        ostringstream html;
        html << "<tr>";
        html << "<td>" << accNo << "</td>";
        html << "<td>" << name << "</td>";
        html << "<td>" << (type == 'S' || type == 's' ? "Savings" : "Current") << "</td>";
        html << "<td>$" << deposit << "</td>";
        html << "<td>" << createdDate << "</td>";
        html << "</tr>";
        return html.str();
    }
};

int main() {
    cout << "Content-Type: text/html\n\n";
    
    cout << R"(
    <!DOCTYPE html>
    <html>
    <head>
        <title>All Accounts - Bank Management System</title>
        <style>
            body { font-family: Arial, sans-serif; margin: 20px; }
            table { width: 100%; border-collapse: collapse; margin-top: 20px; }
            th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }
            th { background-color: #4CAF50; color: white; }
            tr:nth-child(even) { background-color: #f2f2f2; }
            .header { background: #2c3e50; color: white; padding: 20px; text-align: center; }
            .container { max-width: 1200px; margin: 0 auto; }
            .back-link { display: inline-block; margin: 20px 0; padding: 10px 20px; background: #3498db; color: white; text-decoration: none; }
        </style>
    </head>
    <body>
        <div class="header">
            <h1>Bank Management System</h1>
            <p>All Account Holders</p>
        </div>
        
        <div class="container">
            <a href="/static/index.html" class="back-link">Back to Home</a>
            
            <h2>Account List</h2>
            
            <table>
                <thead>
                    <tr>
                        <th>Account No.</th>
                        <th>Name</th>
                        <th>Type</th>
                        <th>Balance</th>
                        <th>Created Date</th>
                    </tr>
                </thead>
                <tbody>
    )";
    
    // Read and display accounts
    vector<Account> accounts;
    ifstream file("data/accounts.data");
    
    if (!file.is_open()) {
        cout << "<tr><td colspan='5'>No accounts found</td></tr>";
    } else {
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                Account acc = Account::deserialize(line);
                cout << acc.toHTML();
            }
        }
        file.close();
    }
    
    cout << R"(
                </tbody>
            </table>
        </div>
    </body>
    </html>
    )";
    
    return 0;
}