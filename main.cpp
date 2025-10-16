#include "Ledger.h"
#include <limits> 

using namespace std;

void printMenu() {
    cout << "\n\n--- Cryptographic Ledger CLI ---\n";
    cout << "1. Add New Transaction\n";
    cout << "2. View Ledger State\n";
    cout << "3. Verify Ledger Integrity\n";
    cout << "4. Undo Last Transaction\n";
    cout << "5. Exit\n";
    cout << "Enter choice: ";
}

void getTransactionInput(string& sender, string& receiver, double& amount) {
    // Clear any leftover input from previous runs
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "Enter Sender Name: ";
    getline(cin, sender);
    cout << "Enter Receiver Name: ";
    getline(cin, receiver);
    
    cout << "Enter Amount (e.g., 100.50): ";
    // Basic input validation for amount
    while (!(cin >> amount) || amount <= 0) {
        cout << "Invalid amount. Please enter a positive number: ";
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
    }
}

int main() {
    // Initialize the Ledger
    Ledger ledger;
    int choice = 0;

    // Add some initial transactions for demonstration
    cout << "--- Initializing Demonstration Data ---\n";
    ledger.addTransaction("Alice", "Bob", 50.0);
    ledger.addTransaction("Bob", "Charlie", 12.5);
    ledger.addTransaction("Charlie", "Alice", 100.0);
    ledger.verifyLedger();

    while (choice != 5) {
        printMenu();
        if (!(cin >> choice)) {
            // Handle non-integer input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n[ERROR] Invalid input. Please enter a number.\n";
            continue;
        }

        string sender, receiver;
        double amount;
        
        switch (choice) {
            case 1:
                getTransactionInput(sender, receiver, amount);
                ledger.addTransaction(sender, receiver, amount);
                break;
            case 2:
                ledger.printLedger();
                break;
            case 3:
                ledger.verifyLedger();
                break;
            case 4:
                ledger.undoLast();
                break;
            case 5:
                cout << "Exiting Ledger CLI. Goodbye!\n";
                break;
            default:
                cout << "\n[ERROR] Invalid choice. Please select from 1-5.\n";
        }
    }

    return 0;
}
