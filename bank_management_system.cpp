#include <iostream.h>  // Use <iostream.h> instead of <iostream>
#include <fstream.h>   // Use <fstream.h> instead of <fstream>
#include <conio.h>     // Required for Turbo C++
#include <stdio.h>     // For remove(), rename()
#include <string.h>    // For string functions

class BankAccount {
private:
    int accountNumber;
    char name[50];
    float balance;

public:
    void createAccount();  
    void showAccount() const;  
    void deposit(float);  
    void withdraw(float);  
    int getAccountNumber() const { return accountNumber; }  
};

// Create a new account
void BankAccount::createAccount() {
    cout << "\nEnter Account Number: ";
    cin >> accountNumber;
    cout << "Enter Account Holder Name: ";
    cin.ignore();
    gets(name);  // Turbo C++ does not support getline(), use gets()
    cout << "Enter Initial Deposit Amount: ";
    cin >> balance;
    cout << "\nAccount Created Successfully!\n";
}

// Display account details
void BankAccount::showAccount() const {
    cout << "\nAccount Number: " << accountNumber;
    cout << "\nAccount Holder: " << name;
    cout << "\nBalance: Rs. " << balance << endl;
}

// Deposit money
void BankAccount::deposit(float amount) {
    balance += amount;
    cout << "\nAmount Deposited Successfully! New Balance: Rs. " << balance << endl;
}

// Withdraw money
void BankAccount::withdraw(float amount) {
    if (amount > balance) {
        cout << "\nInsufficient Balance!\n";
    } else {
        balance -= amount;
        cout << "\nAmount Withdrawn Successfully! Remaining Balance: Rs. " << balance << endl;
    }
}

// Write account to file
void writeAccount() {
    BankAccount acc;
    ofstream outFile("bank.dat", ios::binary | ios::app);
    acc.createAccount();
    outFile.write((char *)&acc, sizeof(BankAccount));
    outFile.close();
}

// Display all accounts
void displayAll() {
    BankAccount acc;
    ifstream inFile("bank.dat", ios::binary);
    if (!inFile) {
        cout << "\nNo Account Records Found!\n";
        return;
    }
    cout << "\n--- All Bank Accounts ---\n";
    while (inFile.read((char *)&acc, sizeof(BankAccount))) {
        acc.showAccount();
        cout << "-------------------------\n";
    }
    inFile.close();
}

// Search account
void searchAccount(int num) {
    BankAccount acc;
    ifstream inFile("bank.dat", ios::binary);
    int found = 0;  // Turbo C++ does not support bool, use int
    while (inFile.read((char *)&acc, sizeof(BankAccount))) {
        if (acc.getAccountNumber() == num) {
            cout << "\n--- Account Details ---\n";
            acc.showAccount();
            found = 1;
            break;
        }
    }
    inFile.close();
    if (!found) cout << "\nAccount Not Found!\n";
}

// Deposit or Withdraw money
void updateBalance(int num, int isDeposit) {  // Turbo C++ does not support bool
    BankAccount acc;
    fstream file("bank.dat", ios::binary | ios::in | ios::out);
    int found = 0;
    while (!file.eof()) {
        int pos = file.tellg();
        if (file.read((char *)&acc, sizeof(BankAccount))) {
            if (acc.getAccountNumber() == num) {
                float amount;
                cout << "\nEnter Amount: ";
                cin >> amount;
                if (isDeposit) {
                    acc.deposit(amount);
                } else {
                    acc.withdraw(amount);
                }
                file.seekp(pos);
                file.write((char *)&acc, sizeof(BankAccount));
                found = 1;
                break;
            }
        }
    }
    file.close();
    if (!found) cout << "\nAccount Not Found!\n";
}

// Delete account
void deleteAccount(int num) {
    BankAccount acc;
    ifstream inFile("bank.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    int found = 0;

    while (inFile.read((char *)&acc, sizeof(BankAccount))) {
        if (acc.getAccountNumber() != num) {
            outFile.write((char *)&acc, sizeof(BankAccount));
        } else {
            found = 1;
        }
    }
    inFile.close();
    outFile.close();
    remove("bank.dat");
    rename("temp.dat", "bank.dat");

    if (found) {
        cout << "\nAccount Deleted Successfully!\n";
    } else {
        cout << "\nAccount Not Found!\n";
    }
}

// Main function
int main() {
    int choice, accNum;
    clrscr();  // Required in Turbo C++
    
    do {
        cout << "\n===== Bank Management System =====";
        cout << "\n1. Create Account";
        cout << "\n2. Display All Accounts";
        cout << "\n3. Search Account";
        cout << "\n4. Deposit Money";
        cout << "\n5. Withdraw Money";
        cout << "\n6. Delete Account";
        cout << "\n7. Exit";
        cout << "\nEnter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: writeAccount(); break;
            case 2: displayAll(); break;
            case 3: 
                cout << "\nEnter Account Number: ";
                cin >> accNum;
                searchAccount(accNum);
                break;
            case 4: 
                cout << "\nEnter Account Number: ";
                cin >> accNum;
                updateBalance(accNum, 1);
                break;
            case 5: 
                cout << "\nEnter Account Number: ";
                cin >> accNum;
                updateBalance(accNum, 0);
                break;
            case 6: 
                cout << "\nEnter Account Number: ";
                cin >> accNum;
                deleteAccount(accNum);
                break;
            case 7: cout << "\nExiting Program...\n"; break;
            default: cout << "\nInvalid Choice! Try Again.\n";
        }
        getch();  // Pause screen
        clrscr(); // Clear screen for next iteration
    } while (choice != 7);
    
    return 0;
}
