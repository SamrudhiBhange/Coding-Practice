#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace std;

// Account class
class Account {
private:
    int accNo;
    string name;
    double deposit;
    char type;
    string createdDate;
    
public:
    Account() : accNo(0), name(""), deposit(0), type(' ') {}
    
    Account(int no, string n, double d, char t) : 
        accNo(no), name(n), deposit(d), type(t) {
        setCreatedDate();
    }
    
    void setCreatedDate() {
        time_t now = time(0);
        char* dt = ctime(&now);
        createdDate = string(dt);
        // Remove newline
        if (!createdDate.empty() && createdDate[createdDate.length()-1] == '\n') {
            createdDate.erase(createdDate.length()-1);
        }
    }
    
    int getAccountNo() const { return accNo; }
    string getName() const { return name; }
    double getDeposit() const { return deposit; }
    char getType() const { return type; }
    string getTypeString() const {
        if (type == 'S' || type == 's') return "Savings";
        else if (type == 'C' || type == 'c') return "Current";
        return "Unknown";
    }
    string getCreatedDate() const { return createdDate; }
    
    void setName(const string& n) { name = n; }
    void setDeposit(double d) { deposit = d; }
    void setType(char t) { type = t; }
    
    void depositAmount(double amount) { deposit += amount; }
    bool withdrawAmount(double amount) {
        if (amount <= deposit) {
            deposit -= amount;
            return true;
        }
        return false;
    }
    
    string serialize() const {
        ostringstream oss;
        oss << accNo << "|" << name << "|" << deposit << "|" 
            << type << "|" << createdDate;
        return oss.str();
    }
    
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
    
    string toJSON() const {
        ostringstream json;
        json << "{";
        json << "\"accNo\":" << accNo << ",";
        json << "\"name\":\"" << name << "\",";
        json << "\"deposit\":" << deposit << ",";
        json << "\"type\":\"" << type << "\",";
        json << "\"typeString\":\"" << getTypeString() << "\",";
        json << "\"createdDate\":\"" << createdDate << "\"";
        json << "}";
        return json.str();
    }
};

// File operations
vector<Account> readAccountsFromFile(const string& filename = "data/accounts.data") {
    vector<Account> accounts;
    ifstream file(filename);
    
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                accounts.push_back(Account::deserialize(line));
            }
        }
        file.close();
    }
    
    return accounts;
}

void writeAccountsToFile(const vector<Account>& accounts, const string& filename = "data/accounts.data") {
    ofstream file(filename);
    
    if (file.is_open()) {
        for (const auto& acc : accounts) {
            file << acc.serialize() << endl;
        }
        file.close();
    }
}

// Utility functions
string getPostData() {
    string postData;
    char* contentLength = getenv("CONTENT_LENGTH");
    
    if (contentLength != NULL) {
        int length = atoi(contentLength);
        if (length > 0) {
            char* buffer = new char[length + 1];
            cin.read(buffer, length);
            buffer[length] = '\0';
            postData = buffer;
            delete[] buffer;
        }
    }
    return postData;
}

string getQueryParam(const string& param) {
    char* query = getenv("QUERY_STRING");
    if (query == NULL) return "";
    
    // FIXED LINE: Removed extra closing parenthesis
    string queryStr = query;
    
    size_t start = queryStr.find(param + "=");
    if (start == string::npos) return "";
    
    start += param.length() + 1;
    size_t end = queryStr.find("&", start);
    if (end == string::npos) end = queryStr.length();
    
    string value = queryStr.substr(start, end - start);
    
    // URL decode
    string result;
    for (size_t i = 0; i < value.length(); i++) {
        if (value[i] == '+') {
            result += ' ';
        } else if (value[i] == '%' && i + 2 < value.length()) {
            string hex = value.substr(i + 1, 2);
            char ch = static_cast<char>(strtol(hex.c_str(), NULL, 16));
            result += ch;
            i += 2;
        } else {
            result += value[i];
        }
    }
    
    return result;
}

void sendJSONResponse(const string& json) {
    cout << "Content-Type: application/json\n\n";
    cout << json;
}

void sendHTMLResponse(const string& html) {
    cout << "Content-Type: text/html\n\n";
    cout << html;
}

void sendError(const string& message) {
    string json = "{\"status\":\"error\",\"message\":\"" + message + "\"}";
    sendJSONResponse(json);
}

void sendSuccess(const string& message, const string& data = "{}") {
    string json = "{\"status\":\"success\",\"message\":\"" + message + "\",\"data\":" + data + "}";
    sendJSONResponse(json);
}

// Main CGI function
int main() {
    // Get request method
    char* method = getenv("REQUEST_METHOD");
    if (!method) {
        sendError("No request method");
        return 1;
    }
    
    string requestMethod = method;
    string action = getQueryParam("action");
    
    if (action == "create") {
        if (requestMethod == "POST") {
            string postData = getPostData();
            // For CGI, we'll parse from query string
            int accNo = atoi(getQueryParam("accNo").c_str());
            string name = getQueryParam("name");
            double deposit = atof(getQueryParam("deposit").c_str());
            char type = getQueryParam("type")[0];
            
            // Validate
            if (name.empty() || accNo <= 0 || deposit < 0) {
                sendError("Invalid input");
                return 1;
            }
            
            if ((type == 'S' || type == 's') && deposit < 500) {
                sendError("Minimum deposit for Savings account is 500");
                return 1;
            }
            
            if ((type == 'C' || type == 'c') && deposit < 1000) {
                sendError("Minimum deposit for Current account is 1000");
                return 1;
            }
            
            // Read existing accounts
            vector<Account> accounts = readAccountsFromFile();
            
            // Check if account exists
            for (const auto& acc : accounts) {
                if (acc.getAccountNo() == accNo) {
                    sendError("Account number already exists");
                    return 1;
                }
            }
            
            // Create new account
            Account newAcc(accNo, name, deposit, type);
            accounts.push_back(newAcc);
            
            // Save to file
            writeAccountsToFile(accounts);
            
            sendSuccess("Account created successfully", newAcc.toJSON());
        } else {
            sendError("POST method required");
        }
    }
    else if (action == "getAll") {
        vector<Account> accounts = readAccountsFromFile();
        
        ostringstream json;
        json << "[";
        for (size_t i = 0; i < accounts.size(); i++) {
            json << accounts[i].toJSON();
            if (i < accounts.size() - 1) json << ",";
        }
        json << "]";
        
        sendJSONResponse(json.str());
    }
    else if (action == "get") {
        int accNo = atoi(getQueryParam("accNo").c_str());
        
        vector<Account> accounts = readAccountsFromFile();
        bool found = false;
        
        for (const auto& acc : accounts) {
            if (acc.getAccountNo() == accNo) {
                sendJSONResponse(acc.toJSON());
                found = true;
                break;
            }
        }
        
        if (!found) {
            sendError("Account not found");
        }
    }
    else if (action == "deposit") {
        if (requestMethod == "POST") {
            int accNo = atoi(getQueryParam("accNo").c_str());
            double amount = atof(getQueryParam("amount").c_str());
            
            if (amount <= 0) {
                sendError("Invalid amount");
                return 1;
            }
            
            vector<Account> accounts = readAccountsFromFile();
            bool found = false;
            
            for (auto& acc : accounts) {
                if (acc.getAccountNo() == accNo) {
                    acc.depositAmount(amount);
                    found = true;
                    break;
                }
            }
            
            if (found) {
                writeAccountsToFile(accounts);
                sendSuccess("Deposit successful");
            } else {
                sendError("Account not found");
            }
        } else {
            sendError("POST method required");
        }
    }
    else if (action == "withdraw") {
        if (requestMethod == "POST") {
            int accNo = atoi(getQueryParam("accNo").c_str());
            double amount = atof(getQueryParam("amount").c_str());
            
            if (amount <= 0) {
                sendError("Invalid amount");
                return 1;
            }
            
            vector<Account> accounts = readAccountsFromFile();
            bool found = false;
            bool success = false;
            
            for (auto& acc : accounts) {
                if (acc.getAccountNo() == accNo) {
                    found = true;
                    success = acc.withdrawAmount(amount);
                    break;
                }
            }
            
            if (!found) {
                sendError("Account not found");
            } else if (!success) {
                sendError("Insufficient balance");
            } else {
                writeAccountsToFile(accounts);
                sendSuccess("Withdrawal successful");
            }
        } else {
            sendError("POST method required");
        }
    }
    else if (action == "delete") {
        if (requestMethod == "POST") {
            int accNo = atoi(getQueryParam("accNo").c_str());
            
            vector<Account> accounts = readAccountsFromFile();
            vector<Account> newAccounts;
            bool found = false;
            
            for (const auto& acc : accounts) {
                if (acc.getAccountNo() == accNo) {
                    found = true;
                } else {
                    newAccounts.push_back(acc);
                }
            }
            
            if (found) {
                writeAccountsToFile(newAccounts);
                sendSuccess("Account deleted successfully");
            } else {
                sendError("Account not found");
            }
        } else {
            sendError("POST method required");
        }
    }
    else if (action == "modify") {
        if (requestMethod == "POST") {
            int accNo = atoi(getQueryParam("accNo").c_str());
            string name = getQueryParam("name");
            char type = getQueryParam("type")[0];
            double deposit = atof(getQueryParam("deposit").c_str());
            
            vector<Account> accounts = readAccountsFromFile();
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
            
            if (found) {
                writeAccountsToFile(accounts);
                sendSuccess("Account modified successfully");
            } else {
                sendError("Account not found");
            }
        } else {
            sendError("POST method required");
        }
    }
    else if (action == "stats") {
        vector<Account> accounts = readAccountsFromFile();
        
        int totalAccounts = accounts.size();
        double totalBalance = 0;
        int savingsCount = 0;
        int currentCount = 0;
        
        for (const auto& acc : accounts) {
            totalBalance += acc.getDeposit();
            if (acc.getType() == 'S' || acc.getType() == 's') {
                savingsCount++;
            } else {
                currentCount++;
            }
        }
        
        ostringstream json;
        json << "{";
        json << "\"totalAccounts\":" << totalAccounts << ",";
        json << "\"totalBalance\":" << totalBalance << ",";
        json << "\"savingsAccounts\":" << savingsCount << ",";
        json << "\"currentAccounts\":" << currentCount;
        json << "}";
        
        sendJSONResponse(json.str());
    }
    else if (action == "ping") {
        sendSuccess("Server is running");
    }
    else {
        // Default response - API documentation
        string html = R"(
     <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Bank Management System</title>
    <link rel="stylesheet" href="css/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
</head>
<body>
    <div class="container">
        <header class="header">
            <div class="logo">
                <i class="fas fa-university"></i>
                <h1>Bank Management System</h1>
            </div>
            <nav class="nav">
                <a href="#dashboard" class="nav-link active">Dashboard</a>
                <a href="#create" class="nav-link">Create Account</a>
                <a href="#accounts" class="nav-link">All Accounts</a>
                <a href="#transaction" class="nav-link">Transactions</a>
                <a href="#search" class="nav-link">Search</a>
            </nav>
        </header>

        <main class="main-content">
            <!-- Dashboard Section -->
            <section id="dashboard" class="section active">
                <h2><i class="fas fa-tachometer-alt"></i> Dashboard</h2>
                <div class="stats-grid">
                    <div class="stat-card">
                        <div class="stat-icon">
                            <i class="fas fa-users"></i>
                        </div>
                        <div class="stat-info">
                            <h3 id="totalAccounts">0</h3>
                            <p>Total Accounts</p>
                        </div>
                    </div>
                    <div class="stat-card">
                        <div class="stat-icon">
                            <i class="fas fa-money-bill-wave"></i>
                        </div>
                        <div class="stat-info">
                            <h3 id="totalBalance">$0</h3>
                            <p>Total Balance</p>
                        </div>
                    </div>
                    <div class="stat-card">
                        <div class="stat-icon">
                            <i class="fas fa-piggy-bank"></i>
                        </div>
                        <div class="stat-info">
                            <h3 id="savingsAccounts">0</h3>
                            <p>Savings Accounts</p>
                        </div>
                    </div>
                    <div class="stat-card">
                        <div class="stat-icon">
                            <i class="fas fa-briefcase"></i>
                        </div>
                        <div class="stat-info">
                            <h3 id="currentAccounts">0</h3>
                            <p>Current Accounts</p>
                        </div>
                    </div>
                </div>
                
                <div class="recent-activity">
                    <h3>Recent Activity</h3>
                    <div class="activity-list" id="recentActivity">
                        <p>No recent activity</p>
                    </div>
                </div>
            </section>

            <!-- Create Account Section -->
            <section id="create" class="section">
                <h2><i class="fas fa-user-plus"></i> Create New Account</h2>
                <form id="createAccountForm" class="form">
                    <div class="form-group">
                        <label for="accNo"><i class="fas fa-id-card"></i> Account Number</label>
                        <input type="number" id="accNo" name="accNo" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="name"><i class="fas fa-user"></i> Account Holder Name</label>
                        <input type="text" id="name" name="name" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="type"><i class="fas fa-credit-card"></i> Account Type</label>
                        <select id="type" name="type" required>
                            <option value="">Select Type</option>
                            <option value="S">Savings (Minimum $500)</option>
                            <option value="C">Current (Minimum $1000)</option>
                        </select>
                    </div>
                    
                    <div class="form-group">
                        <label for="deposit"><i class="fas fa-money-bill"></i> Initial Deposit</label>
                        <input type="number" id="deposit" name="deposit" required>
                        <small class="hint" id="depositHint"></small>
                    </div>
                    
                    <button type="submit" class="btn btn-primary">
                        <i class="fas fa-check-circle"></i> Create Account
                    </button>
                </form>
            </section>

            <!-- All Accounts Section -->
            <section id="accounts" class="section">
                <h2><i class="fas fa-users"></i> All Accounts</h2>
                <div class="table-container">
                    <table id="accountsTable">
                        <thead>
                            <tr>
                                <th>Account No.</th>
                                <th>Name</th>
                                <th>Type</th>
                                <th>Balance</th>
                                <th>Actions</th>
                            </tr>
                        </thead>
                        <tbody id="accountsTableBody">
                            <!-- Accounts will be loaded here -->
                        </tbody>
                    </table>
                </div>
                <button id="refreshAccounts" class="btn btn-secondary">
                    <i class="fas fa-sync-alt"></i> Refresh
                </button>
            </section>

            <!-- Transaction Section -->
            <section id="transaction" class="section">
                <h2><i class="fas fa-exchange-alt"></i> Transactions</h2>
                <div class="transaction-tabs">
                    <button class="tab-btn active" data-tab="deposit">Deposit</button>
                    <button class="tab-btn" data-tab="withdraw">Withdraw</button>
                    <button class="tab-btn" data-tab="transfer">Transfer</button>
                </div>
                
                <!-- Deposit Tab -->
                <div id="deposit" class="tab-content active">
                    <form id="depositForm" class="form">
                        <div class="form-group">
                            <label for="depositAccNo">Account Number</label>
                            <input type="number" id="depositAccNo" name="accNo" required>
                        </div>
                        
                        <div class="form-group">
                            <label for="depositAmount">Amount</label>
                            <input type="number" id="depositAmount" name="amount" required min="1">
                        </div>
                        
                        <button type="submit" class="btn btn-success">
                            <i class="fas fa-money-bill-wave"></i> Deposit
                        </button>
                    </form>
                </div>
                
                <!-- Withdraw Tab -->
                <div id="withdraw" class="tab-content">
                    <form id="withdrawForm" class="form">
                        <div class="form-group">
                            <label for="withdrawAccNo">Account Number</label>
                            <input type="number" id="withdrawAccNo" name="accNo" required>
                        </div>
                        
                        <div class="form-group">
                            <label for="withdrawAmount">Amount</label>
                            <input type="number" id="withdrawAmount" name="amount" required min="1">
                        </div>
                        
                        <button type="submit" class="btn btn-warning">
                            <i class="fas fa-hand-holding-usd"></i> Withdraw
                        </button>
                    </form>
                </div>
                
                <!-- Transfer Tab -->
                <div id="transfer" class="tab-content">
                    <form id="transferForm" class="form">
                        <div class="form-group">
                            <label for="fromAccount">From Account</label>
                            <input type="number" id="fromAccount" name="fromAccount" required>
                        </div>
                        
                        <div class="form-group">
                            <label for="toAccount">To Account</label>
                            <input type="number" id="toAccount" name="toAccount" required>
                        </div>
                        
                        <div class="form-group">
                            <label for="transferAmount">Amount</label>
                            <input type="number" id="transferAmount" name="amount" required min="1">
                        </div>
                        
                        <button type="submit" class="btn btn-info">
                            <i class="fas fa-exchange-alt"></i> Transfer
                        </button>
                    </form>
                </div>
            </section>

            <!-- Search Section -->
            <section id="search" class="section">
                <h2><i class="fas fa-search"></i> Search Account</h2>
                <div class="search-container">
                    <div class="form-group">
                        <input type="number" id="searchInput" placeholder="Enter account number">
                        <button id="searchBtn" class="btn btn-primary">
                            <i class="fas fa-search"></i> Search
                        </button>
                    </div>
                    
                    <div id="searchResults" class="search-results">
                        <!-- Search results will appear here -->
                    </div>
                </div>
            </section>
        </main>

        <footer class="footer">
            <p>&copy; 2024 Bank Management System | Developed with <i class="fas fa-heart"></i></p>
        </footer>

        <!-- Notification Container -->
        <div id="notificationContainer"></div>
    </div>

    <script src="js/script.js"></script>
</body>
</html>
        )";
        
        sendHTMLResponse(html);
    }
    
    return 0;
}