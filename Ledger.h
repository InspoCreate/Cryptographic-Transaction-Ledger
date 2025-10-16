#pragma once

// Replaced <bits/stdc++.h> with necessary standard headers
#include <iostream>
#include <string>
#include <stack>
#include <functional> // For std::hash
#include <stdexcept>

// --- CORE DATA STRUCTURE (The Block) ---
struct Transaction {
    int id;
    std::string sender, receiver;
    double amount;
    size_t prevHash;
    size_t currHash;

    // Constructor to initialize a transaction and calculate its hash immediately
    Transaction(); // Default constructor for potential use (e.g., Genesis Block)
    Transaction(int _id, const std::string& s, const std::string& r, double a, size_t prev)
        : id(_id), sender(s), receiver(r), amount(a), prevHash(prev) {
        
        // Calculate the current hash based on transaction data and previous hash
        std::hash<std::string> hasher;
        std::string data = sender + receiver + std::to_string(amount) + std::to_string(prevHash);
        currHash = hasher(data);
    }
};

// --- LEDGER CLASS (The Chain Manager) ---
class Ledger {
private:
    std::stack<Transaction> transactions;
    int nextId = 1;

public:
    
    void addTransaction(const std::string &sender, const std::string &receiver, double amount);
    bool undoLast();

    
    void printLedger();
    bool verifyLedger();
};
