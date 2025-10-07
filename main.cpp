/*
 * Author: Charles Phillips
 * Creation Date: 05/07/2025 @ 8:45 AM EST
 * Last Revision: 10/07/2025 @ 12:18 PM EST
 * 
 * Restaurant Management System
 * ----------------------------
 * This program simulates a simple restaurant order management system.
 * It allows the user to:
 *   - Place orders for a table with up to 4 guests.
 *   - Track table status: seated, completed, or paid.
 *   - Complete orders before allowing payment.
 *   - Calculate subtotal, tax, and tip.
 *   - Confirm and record payment, generating a receipt file.
 *   - Close the restaurant only when all orders are completed and paid.
 *
 * Features:
 *   - Table capacity handling (up to 4 per table)
 *   - Itemized entree menu with prices
 *   - Status-aware menu options
 *   - Receipt output to a .txt file with unique transaction ID
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>
#include <vector>
#include <array>
#include <random>
#include <limits>

using namespace std;

const int TABLE_QTY = 4;
const int TABLE_CAPACITY = 4;
const double TAX_RATE = 0.10;
const double TIP_RATE = 0.20;

enum Entrees { RAW_FISH, EGGS, HAM, BISC, TOAST };

const array<string, 5> entreeNames = {"Raw Fish", "Eggs", "Ham", "Biscuits", "Toast"};
const array<int, 5> entreePrices = {35, 45, 38, 38, 38};

struct Table {
    int capacity = TABLE_CAPACITY;
    int seatedGuests = 0;
};

struct Order {
    vector<Entrees> items;
    bool isCompleted = false;
    bool isPaid = false;
};

map<int, Table> tables;
map<int, Order> orders;

bool allOrdersPaidAndComplete() {
    for (const auto& [tableId, order] : orders) {
        if (!order.isCompleted || !order.isPaid)
            return false;
    }
    return true;
}

void initializeTables() {
    for (int i = 1; i <= TABLE_QTY; ++i)
        tables[i] = Table();
}

void showMenu() {
    cout << "--- Menu ---\n";
    for (size_t i = 0; i < entreeNames.size(); ++i)
        cout << (i + 1) << ". " << entreeNames[i] << " - $" << entreePrices[i] << "\n";
}

int checkNum(int min, int max, const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        cin >> val;
        if (cin.fail() || val < min || val > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
        } else {
            return val;
        }
    }
}

void placeOrder() {
    int tableId = checkNum(1, TABLE_QTY, "Enter table number (1-" + to_string(TABLE_QTY) + "): ");
    Table& table = tables[tableId];

    int availableSeats = TABLE_CAPACITY - table.seatedGuests;
    if (availableSeats <= 0) {
        cout << "Sorry! Table " << tableId << " is full.\n";
        return;
    }
    // I got creative with this logic to make it prettier and more fun :)
    if (availableSeats <= 2) {
        cout << "\nAct quickly! ";
        cout << "Only " << availableSeats << " seat" << (availableSeats == 1 ? "" : "s") << " left at this table.\n" << endl;
    } else {
        cout << "\nNotice:\n";
        cout << "There " << (availableSeats == 1 ? "is" : "are") << " " 
             << availableSeats << " seat" << (availableSeats == 1 ? "" : "s") 
             << " available at this table.\n" << endl;
    }

    int guests = checkNum(1, availableSeats, "Enter number of guests to seat: ");
    table.seatedGuests += guests;

    showMenu();
    vector<Entrees> items;
    for (int i = 0; i < guests; ++i) {
        int choice = checkNum(1, entreeNames.size(), "Guest " + to_string(i + 1) + ", enter item number: ");
        items.push_back(static_cast<Entrees>(choice - 1));
    }

    orders[tableId].items.insert(orders[tableId].items.end(), items.begin(), items.end());
    cout << "Order placed for table " << tableId << " successfully.\n";
}

void checkTableStatus() {
    for (int tableId = 1; tableId <= TABLE_QTY; ++tableId) {
        if (orders.count(tableId)) {
            cout << "Table #" << tableId << " status: ";
            const Order& order = orders[tableId];
            cout << (!order.isCompleted ? "awaiting completion"
                  : !order.isPaid      ? "awaiting payment"
                                       : "all done") << endl;
        }
    }
}

int checkOrderAndTableStatus(const string& promptMessage) {
    if (!allOrdersPaidAndComplete()) {
        checkTableStatus();
        return checkNum(1, TABLE_QTY, promptMessage);
    } else {
        cout << "No pending orders / all have been completed and paid.\n";
        return -1;
    }
}

void completeOrder() {
    int tableId = checkOrderAndTableStatus("Enter table number to complete order: ");
    if (tableId == -1) return;

    if (!orders.count(tableId)) {
        cerr << "No order found for Table " << tableId << ".\n";
        return;
    }

    orders[tableId].isCompleted = true;
    cout << "Order for table " << tableId << ": "
         << "*marked as complete"
         << "*awaiting payment.\n" << endl;
}

void payForOrder() {
    int tableId = checkOrderAndTableStatus("Enter table number to pay: ");
    if (tableId == -1) return;

    if (!orders.count(tableId)) {
        cerr << "No order found for Table " << tableId << ".\n";
        return;
    }

    Order& order = orders[tableId];

    //Prevent payment if the order isn't completed yet
    if (!order.isCompleted) {
        cerr << "Order for Table " << tableId << " is not completed yet!" << endl;
        cerr << "Please complete the order before payment.\n" << endl;
        return;
    }

    int subtotal = 0;
    for (Entrees item : order.items)
        subtotal += entreePrices[item];

    double tax = subtotal * TAX_RATE;
    double tip = subtotal * TIP_RATE;
    double total = subtotal + tax + tip;

    cout << fixed << setprecision(2);
    cout << "Subtotal: $" << subtotal << "\n";
    cout << "Tax: $" << tax << "\n";
    cout << "Tip: $" << tip << "\n";
    cout << "Total: $" << total << "\n";

    char confirm;
    cout << "Confirm payment? (y/n): ";
    cin >> confirm;

    if (tolower(confirm) == 'y') {
        order.isPaid = true;
        tables[tableId].seatedGuests = 0;

        int transId = rand() % 9000 + 1000; // random 4-digit ID (1000–9999)
        string filename = "Transaction#" + to_string(transId) + ".txt";
        ofstream out(filename);
        out << "*** RECEIPT FOR TABLE " << tableId << " ***\n";
        out << "-------------------------\n";
        for (Entrees item : order.items)
            out << entreeNames[item] << " - $" << entreePrices[item] << "\n";
        out << "-------------------------\n";
        out << fixed << setprecision(2);
        out << "Subtotal: $" << subtotal << "\n";
        out << "Tip (20%): $" << tip << "\n";
        out << "Tax (10%): $" << tax << "\n";
        out << "Total: $" << total << "\n";
        out.close();

        cout << "Payment successful. Receipt saved to '" << filename << "'.\n";
    } else {
        cout << "Payment cancelled.\n";
    }
}

void showMenuOptions() {
    cout << "\n--- MESSIJOE'S MAIN MENU ---\n";
    cout << "1. Enter Order\n";

    if (!(orders.empty()) && !allOrdersPaidAndComplete()) 
    {
        cout << "2. Complete Order\n";
        cout << "3. Calculate and Pay Bill\n";
    }
    if (!(orders.empty()) && allOrdersPaidAndComplete())
        cout << "4. Close the Restaurant\n";
}

int main() {
    initializeTables();
    bool inService = true;

    while (inService) {
        showMenuOptions();
        int choice = checkNum(1, 4, "Choose an option: ");

        switch (choice) {
            case 1:
                placeOrder();
                break;
            case 2:
                if (!(orders.empty()) && !allOrdersPaidAndComplete()) {
                    completeOrder();
                } else {
                    cout << "No orders available to complete.\n";
                }
                break;
            case 3:
                if (!(orders.empty()) && !allOrdersPaidAndComplete()) {
                    payForOrder();
                } else {
                    cout << "No unpaid orders available.\n";
                }
                break;
            case 4:
                if (!(orders.empty()) && allOrdersPaidAndComplete()) {
                    inService = false;
                    cout << "Goodbye!\n";
                } else {
                    cout << "Cannot close — orders still pending.\n";
                }
                break;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
