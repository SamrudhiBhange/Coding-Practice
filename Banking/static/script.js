// API Base URL
const API_BASE_URL = '/cgi-bin/account.cgi';

// Utility Functions
function showNotification(message, type = 'info') {
    const container = document.getElementById('notificationContainer');
    const notification = document.createElement('div');
    notification.className = `notification ${type}`;
    notification.innerHTML = `
        <i class="fas ${type === 'success' ? 'fa-check-circle' : type === 'error' ? 'fa-exclamation-circle' : 'fa-info-circle'}"></i>
        <span>${message}</span>
    `;
    
    container.appendChild(notification);
    
    setTimeout(() => {
        notification.style.opacity = '0';
        notification.style.transform = 'translateX(100%)';
        setTimeout(() => {
            if (notification.parentNode) {
                notification.parentNode.removeChild(notification);
            }
        }, 300);
    }, 5000);
}

function formatCurrency(amount) {
    return new Intl.NumberFormat('en-US', {
        style: 'currency',
        currency: 'USD'
    }).format(amount);
}

// API Functions
async function apiCall(action, data = {}, method = 'GET') {
    try {
        let url = `${API_BASE_URL}?action=${action}`;
        let options = {
            method: method,
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            }
        };

        if (method === 'POST' && Object.keys(data).length > 0) {
            const params = new URLSearchParams();
            Object.keys(data).forEach(key => {
                params.append(key, data[key]);
            });
            options.body = params.toString();
        } else if (method === 'GET' && Object.keys(data).length > 0) {
            const params = new URLSearchParams(data);
            url += '&' + params.toString();
        }

        const response = await fetch(url, options);
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        return await response.json();
    } catch (error) {
        console.error('API Call Error:', error);
        throw error;
    }
}

// Account Functions
async function createAccount(accountData) {
    return await apiCall('create', accountData, 'POST');
}

async function getAllAccounts() {
    return await apiCall('getAll');
}

async function getAccount(accNo) {
    return await apiCall('get', { accNo: accNo });
}

async function depositAmount(accNo, amount) {
    return await apiCall('deposit', { accNo: accNo, amount: amount }, 'POST');
}

async function withdrawAmount(accNo, amount) {
    return await apiCall('withdraw', { accNo: accNo, amount: amount }, 'POST');
}

async function deleteAccount(accNo) {
    return await apiCall('delete', { accNo: accNo }, 'POST');
}

async function modifyAccount(accountData) {
    return await apiCall('modify', accountData, 'POST');
}

async function getStats() {
    return await apiCall('stats');
}

// DOM Functions
function loadDashboard() {
    getStats().then(stats => {
        document.getElementById('totalAccounts').textContent = stats.totalAccounts;
        document.getElementById('totalBalance').textContent = formatCurrency(stats.totalBalance);
        document.getElementById('savingsAccounts').textContent = stats.savingsAccounts;
        document.getElementById('currentAccounts').textContent = stats.currentAccounts;
    }).catch(error => {
        showNotification('Error loading dashboard stats', 'error');
    });
}

function loadAllAccounts() {
    getAllAccounts().then(accounts => {
        const tableBody = document.getElementById('accountsTableBody');
        tableBody.innerHTML = '';
        
        accounts.forEach(account => {
            const row = document.createElement('tr');
            row.innerHTML = `
                <td>${account.accNo}</td>
                <td>${account.name}</td>
                <td>${account.typeString}</td>
                <td>${formatCurrency(account.deposit)}</td>
                <td>
                    <button class="btn btn-small btn-primary" onclick="viewAccount(${account.accNo})">
                        <i class="fas fa-eye"></i>
                    </button>
                    <button class="btn btn-small btn-warning" onclick="editAccount(${account.accNo})">
                        <i class="fas fa-edit"></i>
                    </button>
                    <button class="btn btn-small btn-danger" onclick="deleteAccountPrompt(${account.accNo})">
                        <i class="fas fa-trash"></i>
                    </button>
                </td>
            `;
            tableBody.appendChild(row);
        });
    }).catch(error => {
        showNotification('Error loading accounts', 'error');
    });
}

function viewAccount(accNo) {
    getAccount(accNo).then(account => {
        showNotification(`Account ${account.accNo}: ${account.name} - Balance: ${formatCurrency(account.deposit)}`, 'info');
    });
}

function editAccount(accNo) {
    getAccount(accNo).then(account => {
        // Switch to create section and populate form
        switchSection('create');
        
        document.getElementById('accNo').value = account.accNo;
        document.getElementById('name').value = account.name;
        document.getElementById('type').value = account.type;
        document.getElementById('deposit').value = account.deposit;
        
        showNotification('Account loaded for editing', 'info');
    });
}

function deleteAccountPrompt(accNo) {
    if (confirm(`Are you sure you want to delete account ${accNo}?`)) {
        deleteAccount(accNo).then(response => {
            showNotification(response.message, 'success');
            loadAllAccounts();
            loadDashboard();
        }).catch(error => {
            showNotification('Error deleting account', 'error');
        });
    }
}

// Navigation
function switchSection(sectionId) {
    // Hide all sections
    document.querySelectorAll('.section').forEach(section => {
        section.classList.remove('active');
    });
    
    // Show selected section
    document.getElementById(sectionId).classList.add('active');
    
    // Update navigation links
    document.querySelectorAll('.nav-link').forEach(link => {
        link.classList.remove('active');
        if (link.getAttribute('href') === `#${sectionId}`) {
            link.classList.add('active');
        }
    });
    
    // Load data for section
    if (sectionId === 'dashboard') {
        loadDashboard();
    } else if (sectionId === 'accounts') {
        loadAllAccounts();
    }
}

// Tab Switching
function switchTab(tabId) {
    document.querySelectorAll('.tab-btn').forEach(btn => {
        btn.classList.remove('active');
    });
    
    document.querySelectorAll('.tab-content').forEach(content => {
        content.classList.remove('active');
    });
    
    document.querySelector(`[data-tab="${tabId}"]`).classList.add('active');
    document.getElementById(tabId).classList.add('active');
}

// Event Listeners
document.addEventListener('DOMContentLoaded', function() {
    // Navigation
    document.querySelectorAll('.nav-link').forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            const sectionId = this.getAttribute('href').substring(1);
            switchSection(sectionId);
        });
    });
    
    // Tab buttons
    document.querySelectorAll('.tab-btn').forEach(btn => {
        btn.addEventListener('click', function() {
            const tabId = this.getAttribute('data-tab');
            switchTab(tabId);
        });
    });
    
    // Create Account Form
    document.getElementById('createAccountForm').addEventListener('submit', function(e) {
        e.preventDefault();
        
        const accountData = {
            accNo: document.getElementById('accNo').value,
            name: document.getElementById('name').value,
            type: document.getElementById('type').value,
            deposit: document.getElementById('deposit').value
        };
        
        createAccount(accountData).then(response => {
            showNotification(response.message, 'success');
            this.reset();
            loadDashboard();
            switchSection('accounts');
        }).catch(error => {
            showNotification(error.message || 'Error creating account', 'error');
        });
    });
    
    // Deposit Form
    document.getElementById('depositForm').addEventListener('submit', function(e) {
        e.preventDefault();
        
        const accNo = document.getElementById('depositAccNo').value;
        const amount = document.getElementById('depositAmount').value;
        
        depositAmount(accNo, amount).then(response => {
            showNotification(response.message, 'success');
            this.reset();
            loadDashboard();
        }).catch(error => {
            showNotification(error.message || 'Error processing deposit', 'error');
        });
    });
    
    // Withdraw Form
    document.getElementById('withdrawForm').addEventListener('submit', function(e) {
        e.preventDefault();
        
        const accNo = document.getElementById('withdrawAccNo').value;
        const amount = document.getElementById('withdrawAmount').value;
        
        withdrawAmount(accNo, amount).then(response => {
            showNotification(response.message, 'success');
            this.reset();
            loadDashboard();
        }).catch(error => {
            showNotification(error.message || 'Error processing withdrawal', 'error');
        });
    });
    
    // Search Button
    document.getElementById('searchBtn').addEventListener('click', function() {
        const accNo = document.getElementById('searchInput').value;
        
        if (!accNo) {
            showNotification('Please enter an account number', 'error');
            return;
        }
        
        getAccount(accNo).then(account => {
            const resultsDiv = document.getElementById('searchResults');
            resultsDiv.innerHTML = `
                <div class="account-card">
                    <h3>Account Found</h3>
                    <p><strong>Account Number:</strong> ${account.accNo}</p>
                    <p><strong>Account Holder:</strong> ${account.name}</p>
                    <p><strong>Account Type:</strong> ${account.typeString}</p>
                    <p><strong>Current Balance:</strong> ${formatCurrency(account.deposit)}</p>
                    <p><strong>Created Date:</strong> ${account.createdDate}</p>
                </div>
            `;
        }).catch(error => {
            document.getElementById('searchResults').innerHTML = `
                <div class="error-message">
                    <i class="fas fa-exclamation-triangle"></i>
                    Account not found
                </div>
            `;
        });
    });
    
    // Refresh Accounts Button
    document.getElementById('refreshAccounts').addEventListener('click', loadAllAccounts);
    
    // Account type validation
    document.getElementById('type').addEventListener('change', function() {
        const depositHint = document.getElementById('depositHint');
        if (this.value === 'S') {
            depositHint.textContent = 'Minimum deposit: $500';
        } else if (this.value === 'C') {
            depositHint.textContent = 'Minimum deposit: $1000';
        } else {
            depositHint.textContent = '';
        }
    });
    
    // Load initial data
    loadDashboard();
    loadAllAccounts();
});