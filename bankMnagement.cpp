#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Account {
private:
    int accountNumber;
    string name;
    double balance;

public:
    Account() {}
    Account(int accNo, string accName, double accBalance) {
        accountNumber = accNo;
        name = accName;
        balance = accBalance;
    }

    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cin.ignore();
        cout << "Enter Account Holder Name: ";
        getline(cin, name);
        cout << "Enter Initial Balance: ";
        cin >> balance;
    }

    void showAccount() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << name << endl;
        cout << "Balance: " << balance << endl;
    }

    void deposit(double amount) {
        balance += amount;
    }

    void withdraw(double amount) {
        if (balance >= amount)
            balance -= amount;
        else
            cout << "Insufficient balance!\n";
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }
};

void writeAccount() {
    Account acc;
    acc.createAccount();

    ofstream outFile("bank.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();
    cout << "\nAccount Created Successfully!\n";
}

void displayAccount(int accNo) {
    Account acc;
    bool found = false;

    ifstream inFile("bank.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be opened!\n";
        return;
    }

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == accNo) {
            acc.showAccount();
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found)
        cout << "Account Not Found!\n";
}

void depositWithdraw(int accNo, int option) {
    Account acc;
    bool found = false;

    fstream file("bank.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "File could not be opened!\n";
        return;
    }

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account)) && !found) {
        if (acc.getAccountNumber() == accNo) {
            acc.showAccount();
            double amount;
            cout << (option == 1 ? "Enter amount to deposit: " : "Enter amount to withdraw: ");
            cin >> amount;

            if (option == 1)
                acc.deposit(amount);
            else
                acc.withdraw(amount);

            int pos = (-1) * static_cast<int>(sizeof(acc));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            cout << "\nTransaction Successful!\n";
            found = true;
        }
    }
    file.close();

    if (!found)
        cout << "Account Not Found!\n";
}

void displayAll() {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be opened!\n";
        return;
    }

    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        acc.showAccount();
        cout << "---------------------------\n";
    }
    inFile.close();
}

void intro() {
    cout << "\n\n\tBank Management System";
    cout << "\n\tBy Veda Kimbahune\n";
    cout << "\t----------------------\n";
}

int main() {
    int choice;
    int accNo;

    intro();

    do {
        cout << "\n\nMain Menu\n";
        cout << "1. Create New Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Display Account Details\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Exit\n";
        cout << "Enter your choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1:
                writeAccount();
                break;
            case 2:
                cout << "Enter Account Number: ";
                cin >> accNo;
                depositWithdraw(accNo, 1);
                break;
            case 3:
                cout << "Enter Account Number: ";
                cin >> accNo;
                depositWithdraw(accNo, 2);
                break;
            case 4:
                cout << "Enter Account Number: ";
                cin >> accNo;
                displayAccount(accNo);
                break;
            case 5:
                displayAll();
                break;
            case 6:
                cout << "Thanks for using Bank Management System!\n";
                break;
            default:
                cout << "Invalid Choice!\n";
        }

    } while (choice != 6);

    return 0;
}
