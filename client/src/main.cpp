#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include "../include/ApiClient.h"

using namespace std;

#define NOMINMAX 

void printSeparator() {
    cout << "\n" << string(50, '=') << "\n";
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
}

string getStringInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int getIntInput(const string& prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter a number: ";
        clearInputBuffer();
    }
    clearInputBuffer(); // Clear the newline after reading the number
    return value;
}

bool getUserConfirmation(const string& message) {
    cout << "\n" << message << " (y/n): ";
    string input;
    getline(cin, input);
    return (input == "y" || input == "Y" || input == "yes" || input == "Yes");
}

void showMenu() {
    cout << "\n========== REST API CLIENT MENU ==========\n";
    cout << "1. Get all users\n";
    cout << "2. Add a new user\n";
    cout << "3. Update a user\n";
    cout << "4. Delete a user\n";
    cout << "5. Exit\n";
    cout << "==========================================\n";
}

void handleGetAllUsers(ApiClient& client) {
    printSeparator();
    cout << "FETCHING ALL USERS\n";
    cout << "Sending GET request to /users...\n";
    
    string response = client.getAllUsers();
    cout << "\nServer Response:\n" << response << endl;
}

void handleAddUser(ApiClient& client) {
    printSeparator();
    cout << "ADD NEW USER\n";
    
    string name = getStringInput("Enter user name: ");
    while (name.empty()) {
        cout << "Name cannot be empty!\n";
        name = getStringInput("Enter user name: ");
    }
    
    string email = getStringInput("Enter user email: ");
    while (email.empty() || email.find('@') == string::npos) {
        cout << "Please enter a valid email address!\n";
        email = getStringInput("Enter user email: ");
    }
    
    cout << "\nData to send:\n";
    cout << "Name: " << name << "\n";
    cout << "Email: " << email << "\n";
    
    if (getUserConfirmation("Confirm adding this user?")) {
        cout << "\nSending POST request to /users...\n";
        string response = client.addUser(name, email);
        cout << "\nServer Response:\n" << response << endl;
    } else {
        cout << "User addition cancelled.\n";
    }
}

void handleUpdateUser(ApiClient& client) {
    printSeparator();
    cout << "UPDATE USER\n";
    
    // First show current users
    cout << "Current users:\n";
    string allUsers = client.getAllUsers();
    cout << allUsers << "\n\n";
    
    int userId = getIntInput("Enter user ID to update: ");
    
    string name = getStringInput("Enter new user name: ");
    while (name.empty()) {
        cout << "Name cannot be empty!\n";
        name = getStringInput("Enter new user name: ");
    }
    
    string email = getStringInput("Enter new user email: ");
    while (email.empty() || email.find('@') == string::npos) {
        cout << "Please enter a valid email address!\n";
        email = getStringInput("Enter new user email: ");
    }
    
    cout << "\nData to update:\n";
    cout << "ID: " << userId << "\n";
    cout << "Name: " << name << "\n";
    cout << "Email: " << email << "\n";
    
    if (getUserConfirmation("Confirm updating this user?")) {
        cout << "\nSending PUT request to /users/" << userId << "...\n";
        string response = client.updateUser(userId, name, email);
        cout << "\nServer Response:\n" << response << endl;
    } else {
        cout << "User update cancelled.\n";
    }
}

void handleDeleteUser(ApiClient& client) {
    printSeparator();
    cout << "DELETE USER\n";
    
    // First show current users
    cout << "Current users:\n";
    string allUsers = client.getAllUsers();
    cout << allUsers << "\n\n";
    
    int userId = getIntInput("Enter user ID to delete: ");
    
    cout << "\nYou are about to delete user with ID: " << userId << "\n";
    
    if (getUserConfirmation("Are you sure you want to delete this user?")) {
        cout << "\nSending DELETE request to /users/" << userId << "...\n";
        string response = client.deleteUser(userId);
        cout << "\nServer Response:\n" << response << endl;
    } else {
        cout << "User deletion cancelled.\n";
    }
}

int main() {
    cout << "========================================\n";
    cout << "       REST API CLIENT APPLICATION     \n";
    cout << "========================================\n";
    cout << "This application connects to a Node.js REST API server.\n";
    cout << "Make sure the server is running on localhost:3000\n";
    cout << "========================================\n";
    
    if (!getUserConfirmation("Ready to initialize API client?")) {
        cout << "Exiting...\n";
        return 0;
    }
    
    cout << "\nInitializing API client...\n";
    
    ApiClient client("http://localhost:3000");
    
    if (!client.initialize()) {
        cerr << "Failed to initialize API client!\n";
        cout << "Please make sure libcurl is properly installed.\n";
        cout << "Press Enter to exit...";
        cin.get();
        return 1;
    }
    
    cout << "✓ API client initialized successfully!\n";
    
    int choice;
    do {
        showMenu();
        choice = getIntInput("Enter your choice (1-5): ");
        
        switch (choice) {
            case 1:
                handleGetAllUsers(client);
                break;
            case 2:
                handleAddUser(client);
                break;
            case 3:
                handleUpdateUser(client);
                break;
            case 4:
                handleDeleteUser(client);
                break;
            case 5:
                cout << "Thank you for using the REST API Client!\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1-5.\n";
        }
        
        if (choice != 5) {
            cout << "\nPress Enter to return to menu...";
            cin.get();
        }
        
    } while (choice != 5);
    
    return 0;
}