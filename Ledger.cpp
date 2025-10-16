#include "Ledger.h"
#include <vector>
#include <algorithm>

using namespace std;

Transaction::Transaction() 
    : id(0), amount(0.0), prevHash(0), currHash(0) {}


void Ledger::addTransaction(const string &sender, const string &receiver, double amount) {
    // Determine the previous hash: 0 for the first transaction (Genesis), otherwise top()'s hash
    size_t prev = transactions.empty() ? 0 : transactions.top().currHash;
    
    // Create and push the new transaction onto the stack
    transactions.push(Transaction(nextId++, sender, receiver, amount, prev));
    cout << "\n[SUCCESS] Transaction #" << transactions.top().id << " added.\n";
}

bool Ledger::undoLast() {
    if (transactions.empty()) {
        cout << "\n[INFO] Cannot undo: Ledger is already empty.\n";
        return false;
    }
    transactions.pop();
    cout << "\n[SUCCESS] Last transaction undone.\n";
    return true;
}

void Ledger::printLedger() {
    if (transactions.empty()) { 
        cout << "\n[INFO] Ledger empty.\n"; 
        return; 
    } 

    // Use a vector to temporarily hold and reverse the order for display (oldest to newest)
    stack<Transaction> tmp = transactions;
    vector<Transaction> orderedTx;
    
    while (!tmp.empty()) {
        orderedTx.push_back(tmp.top());
        tmp.pop();
    }
    
    cout << "\n---- CRYPTOGRAPHIC LEDGER STATE ----\n";
    
    // Print in reverse order to show the chain history from oldest (bottom) to newest (top)
    for (int i = orderedTx.size() - 1; i >= 0; --i) {
        const Transaction& t = orderedTx[i];
        cout << "ID: " << t.id 
             << " | " << t.sender << " -> " << t.receiver 
             << " | Amount: " << t.amount 
             << " | PrevHash: " << t.prevHash 
             << " | CurrHash: " << t.currHash << "\n";
    }
    cout << "------------------------------------\n";
}

bool Ledger::verifyLedger() {
    if (transactions.empty()) {
        cout << "\n[INFO] Ledger is empty, therefore verified.\n";
        return true;
    }

    // Since we need to iterate from oldest to newest for verification, we transfer the stack
    stack<Transaction> tempStack = transactions;
    vector<Transaction> blocks;

    while (!tempStack.empty()) {
        blocks.push_back(tempStack.top());
        tempStack.pop();
    }

    // Verification starts from the second to last element in the reversed vector (index size-2)
    // because we compare each current block to its immediate predecessor.
    for (size_t i = blocks.size(); i > 0; --i) {
        const Transaction& current = blocks[i-1]; // Current block being checked
        size_t expectedPrevHash = (i == blocks.size()) ? 0 : blocks[i].currHash; // The block linked before it

        // Recompute the hash of the current transaction
        std::hash<std::string> hasher;
        std::string data = current.sender + current.receiver + 
                           std::to_string(current.amount) + std::to_string(expectedPrevHash);
        size_t recomputedHash = hasher(data);

        // Check 1: Does the stored current hash match the recomputed hash? (Internal integrity)
        if (recomputedHash != current.currHash) {
            cout << "\n[FAIL] Internal Hash Mismatch on Transaction ID #" << current.id << ".\n";
            return false;
        }

        // Check 2: Does the stored previous hash match the hash of the actual previous block? (Chain integrity)
        if (current.prevHash != expectedPrevHash) {
            if (i < blocks.size()) { 
                const Transaction& previous = blocks[i];
                if (current.prevHash != previous.currHash) {
                    cout << "\n[FAIL] Chain Link Broken on Transaction ID #" << current.id 
                         << ". The expected previous hash does not match the block's stored previous hash.\n";
                    return false;
                }
            }
        }
    }

    cout << "\n[SUCCESS] The entire ledger chain is verified and cryptographically sound.\n";
    return true;
}
