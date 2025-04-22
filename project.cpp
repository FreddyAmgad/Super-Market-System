#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

// Forward declarations
class User;
class Product;
class SupermarketSystem;

// User class to handle user information
class User {
private:
    string username;
    string password;
    double balance;
    int bonusPoints;

public:
    User(string uname = "", string pwd = "", double bal = 0.0, int points = 0)
        : username(uname), password(pwd), balance(bal), bonusPoints(points) {}

    // Getters
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    double getBalance() const { return balance; }
    int getBonusPoints() const { return bonusPoints; }

    // Setters
    void setBalance(double newBalance) { balance = newBalance; }
    void addBonusPoints(int points) { bonusPoints += points; }

    // Save user data to file
    void saveToFile(ofstream& outFile) const {
        outFile << username << " " << password << " " << balance << " " << bonusPoints << "\n";
    }

    // Load user data from file
    void loadFromFile(ifstream& inFile) {
        inFile >> username >> password >> balance >> bonusPoints;
    }

    // Display user info
    void display() const {
        cout << "Username: " << username << "\n";
        cout << "Balance: $" << fixed << setprecision(2) << balance << "\n";
        cout << "Bonus Points: " << bonusPoints << "\n";
    }
};

// Product class to handle product information
class Product {
private:
    string name;
    string category;
    double price;
    int stock;

public:
    Product(string n = "", string cat = "", double p = 0.0, int s = 0)
        : name(n), category(cat), price(p), stock(s) {}

    // Getters
    string getName() const { return name; }
    string getCategory() const { return category; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    // Setters
    void setStock(int newStock) { stock = newStock; }

    // Display product info
    void display() const {
        cout << left << setw(20) << name 
             << setw(15) << category 
             << "$" << setw(10) << fixed << setprecision(2) << price 
             << setw(5) << stock << "\n";
    }
};

// Supermarket system class
class SupermarketSystem {
private:
    vector<User> users;
    vector<Product> products;
    User* currentUser;

    // Initialize products
    void initializeProducts() {
        // Clothing
        products.emplace_back("T-Shirt", "Clothing", 19.99, 50);
        products.emplace_back("Jeans", "Clothing", 49.99, 30);
        products.emplace_back("Jacket", "Clothing", 79.99, 20);

        // Food
        products.emplace_back("Bread", "Food", 3.99, 100);
        products.emplace_back("Milk", "Food", 2.49, 80);
        products.emplace_back("Cheese", "Food", 5.99, 60);

        // Vegetables
        products.emplace_back("Tomato", "Vegetable", 1.99, 120);
        products.emplace_back("Potato", "Vegetable", 0.99, 150);
        products.emplace_back("Carrot", "Vegetable", 1.49, 100);
    }

    // Load users from file
    void loadUsers() {
        ifstream inFile("datafile.txt");
        if (inFile) {
            User user;
            while (inFile) {
                user.loadFromFile(inFile);
                if (!user.getUsername().empty()) {
                    users.push_back(user);
                }
            }
        }
        inFile.close();
    }

    // Save users to file
    void saveUsers() {
        ofstream outFile("datafile.txt");
        for (const auto& user : users) {
            user.saveToFile(outFile);
        }
        outFile.close();
    }

    // Check if user exists
    bool userExists(const string& username) {
        for (const auto& user : users) {
            if (user.getUsername() == username) {
                return true;
            }
        }
        return false;
    }

    // Find user by username
    User* findUser(const string& username) {
        for (auto& user : users) {
            if (user.getUsername() == username) {
                return &user;
            }
        }
        return nullptr;
    }

    // User login
    bool login() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        User* user = findUser(username);
        if (user && user->getPassword() == password) {
            currentUser = user;
            return true;
        }
        return false;
    }

    // User registration
    void registerUser() {
        string username, password;
        double initialBalance;

        cout << "Enter new username: ";
        cin >> username;

        if (userExists(username)) {
            cout << "Username already exists!\n";
            return;
        }

        cout << "Enter password: ";
        cin >> password;

        cout << "Enter initial balance: ";
        while (!(cin >> initialBalance) || initialBalance < 0) {
            cout << "Invalid amount. Please enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        users.emplace_back(username, password, initialBalance, 0);
        saveUsers();
        cout << "Registration successful!\n";
    }

    // Display products by category
    void displayProductsByCategory(const string& category) {
        cout << "\n--- " << category << " ---\n";
        cout << left << setw(20) << "Name" << setw(15) << "Category" 
             << setw(10) << "Price" << setw(5) << "Stock\n";
        cout << string(50, '-') << "\n";

        for (const auto& product : products) {
            if (product.getCategory() == category) {
                product.display();
            }
        }
    }

    // Display all products
    void displayAllProducts() {
        cout << "\n--- All Products ---\n";
        cout << left << setw(20) << "Name" << setw(15) << "Category" 
             << setw(10) << "Price" << setw(5) << "Stock\n";
        cout << string(50, '-') << "\n";

        for (const auto& product : products) {
            product.display();
        }
    }

    // Find product by name
    Product* findProduct(const string& name) {
        for (auto& product : products) {
            if (product.getName() == name) {
                return &product;
            }
        }
        return nullptr;
    }

    // Purchase product
    void purchaseProduct() {
        string productName;
        int quantity;

        displayAllProducts();
        cout << "\nEnter product name to purchase: ";
        cin.ignore();
        getline(cin, productName);

        Product* product = findProduct(productName);
        if (!product) {
            cout << "Product not found!\n";
            return;
        }

        cout << "Enter quantity: ";
        while (!(cin >> quantity) || quantity <= 0) {
            cout << "Invalid quantity. Please enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (quantity > product->getStock()) {
            cout << "Not enough stock available!\n";
            return;
        }

        double totalCost = product->getPrice() * quantity;
        if (totalCost > currentUser->getBalance()) {
            cout << "Insufficient balance!\n";
            return;
        }

        // Process purchase
        currentUser->setBalance(currentUser->getBalance() - totalCost);
        product->setStock(product->getStock() - quantity);
        
        // Add bonus points (1 point per $10 spent)
        int pointsEarned = static_cast<int>(totalCost / 10);
        currentUser->addBonusPoints(pointsEarned);

        cout << "\nPurchase successful!\n";
        cout << "Total cost: $" << fixed << setprecision(2) << totalCost << "\n";
        cout << "Points earned: " << pointsEarned << "\n";
        cout << "Remaining balance: $" << currentUser->getBalance() << "\n";
        cout << "Total bonus points: " << currentUser->getBonusPoints() << "\n";

        saveUsers();
    }

    // Display user menu
    void userMenu() {
        int choice;
        do {
            cout << "\n=== Supermarket System ===\n";
            cout << "1. View Products\n";
            cout << "2. View Clothing\n";
            cout << "3. View Food\n";
            cout << "4. View Vegetables\n";
            cout << "5. Purchase Product\n";
            cout << "6. View Account Info\n";
            cout << "7. Logout\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    displayAllProducts();
                    break;
                case 2:
                    displayProductsByCategory("Clothing");
                    break;
                case 3:
                    displayProductsByCategory("Food");
                    break;
                case 4:
                    displayProductsByCategory("Vegetable");
                    break;
                case 5:
                    purchaseProduct();
                    break;
                case 6:
                    currentUser->display();
                    break;
                case 7:
                    cout << "Logging out...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 7);
    }

public:
    SupermarketSystem() : currentUser(nullptr) {
        initializeProducts();
        loadUsers();
    }

    // Main menu
    void run() {
        int choice;
        do {
            cout << "\n=== Supermarket System ===\n";
            cout << "1. Login\n";
            cout << "2. Register\n";
            cout << "3. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    if (login()) {
                        cout << "Login successful!\n";
                        userMenu();
                    } else {
                        cout << "Invalid username or password!\n";
                    }
                    break;
                case 2:
                    registerUser();
                    break;
                case 3:
                    cout << "Exiting system...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 3);
    }
};

int main() {
    SupermarketSystem system;
    system.run();
    return 0;
}
