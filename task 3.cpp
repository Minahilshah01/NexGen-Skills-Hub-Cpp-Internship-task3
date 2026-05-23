#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
using namespace std;

int strToInt(const string& s) {
    istringstream ss(s);
    int val;
    ss >> val;
    return val;
}

double strToDouble(const string& s) {
    istringstream ss(s);
    double val;
    ss >> val;
    return val;
}

void clearBuffer() {
    cin.ignore(1000, '\n');
}

class MenuItem {
public:
    int    id;
    string name;
    double price;

    MenuItem(int id, string name, double price)
        : id(id), name(name), price(price) {}

    void display() const {
        cout << "  [" << id << "] "
             << left << setw(25) << name
             << "Rs. " << fixed << setprecision(2) << price << "\n";
    }
};

class OrderItem {
public:
    int    menuId;
    string itemName;
    int    quantity;
    double unitPrice;

    OrderItem(int menuId, string itemName, int quantity, double unitPrice)
        : menuId(menuId), itemName(itemName),
          quantity(quantity), unitPrice(unitPrice) {}

    double totalPrice() const {
        return quantity * unitPrice;
    }
};

class Order {
public:
    int               orderId;
    string            customerName;
    string            orderDate;
    vector<OrderItem> items;

    Order(int id, string name, string date)
        : orderId(id), customerName(name), orderDate(date) {}

    void addItem(const OrderItem& item) {
        items.push_back(item);
    }

    double grandTotal() const {
        double total = 0;
        for (int i = 0; i < (int)items.size(); i++)
            total += items[i].totalPrice();
        return total;
    }

    void printBill() const {
        cout << "\n";
        cout << "  ============================================\n";
        cout << "          RESTAURANT ORDER BILL\n";
        cout << "  ============================================\n";
        cout << "  Order ID   : " << orderId      << "\n";
        cout << "  Customer   : " << customerName << "\n";
        cout << "  Date       : " << orderDate    << "\n";
        cout << "  --------------------------------------------\n";
        cout << "  " << left << setw(20) << "Item"
             << setw(6)  << "Qty"
             << setw(10) << "Price"
             << "Total\n";
        cout << "  --------------------------------------------\n";
        for (int i = 0; i < (int)items.size(); i++) {
            cout << "  " << left << setw(20) << items[i].itemName
                 << setw(6)  << items[i].quantity
                 << "Rs." << setw(8) << fixed << setprecision(2) << items[i].unitPrice
                 << "Rs." << items[i].totalPrice() << "\n";
        }
        cout << "  --------------------------------------------\n";
        cout << "  Grand Total : Rs. " << fixed << setprecision(2) << grandTotal() << "\n";
        cout << "  ============================================\n\n";
    }
};

class FileManager {
private:
    string filename;

public:
    FileManager(string fname) : filename(fname) {}

    void saveOrder(const Order& order) {
        ofstream file(filename.c_str(), ios::app);
        if (!file.is_open()) {
            cout << "  [ERROR] Could not open file for saving!\n";
            return;
        }
        file << "ORDER_START\n";
        file << order.orderId      << "\n";
        file << order.customerName << "\n";
        file << order.orderDate    << "\n";
        file << order.items.size() << "\n";
        for (int i = 0; i < (int)order.items.size(); i++) {
            file << order.items[i].menuId   << "|"
                 << order.items[i].itemName  << "|"
                 << order.items[i].quantity  << "|"
                 << order.items[i].unitPrice << "\n";
        }
        file << "ORDER_END\n";
        file.close();
        cout << "  [OK] Order saved successfully!\n";
    }

    vector<Order> loadAllOrders() {
        vector<Order> orders;
        ifstream file(filename.c_str());
        if (!file.is_open())
            return orders;

        string line;
        while (getline(file, line)) {
            if (line == "ORDER_START") {
                int    id, itemCount;
                string name, date;

                file >> id;
                file.ignore(1000, '\n');
                getline(file, name);
                getline(file, date);
                file >> itemCount;
                file.ignore(1000, '\n');

                Order order(id, name, date);

                for (int i = 0; i < itemCount; i++) {
                    string itemLine;
                    getline(file, itemLine);

                    size_t p1 = itemLine.find('|');
                    size_t p2 = itemLine.find('|', p1 + 1);
                    size_t p3 = itemLine.find('|', p2 + 1);

                    int    menuId   = strToInt(itemLine.substr(0, p1));
                    string itemName = itemLine.substr(p1 + 1, p2 - p1 - 1);
                    int    qty      = strToInt(itemLine.substr(p2 + 1, p3 - p2 - 1));
                    double price    = strToDouble(itemLine.substr(p3 + 1));

                    order.addItem(OrderItem(menuId, itemName, qty, price));
                }
                orders.push_back(order);
            }
        }
        file.close();
        return orders;
    }

    void clearAllOrders() {
        ofstream file(filename.c_str(), ios::trunc);
        file.close();
        cout << "  [OK] All order records deleted!\n";
    }

    int getNextOrderId() {
        vector<Order> orders = loadAllOrders();
        if (orders.empty()) return 1;
        return orders[orders.size() - 1].orderId + 1;
    }
};

class Restaurant {
private:
    vector<MenuItem> menu;
    FileManager      fileManager;

    string getCurrentDate() {
        time_t now = time(0);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&now));
        return string(buf);
    }

    int getIntInput(const string& prompt, int minVal, int maxVal) {
        int val;
        while (true) {
            cout << prompt;
            if (cin >> val && val >= minVal && val <= maxVal) {
                clearBuffer();   
                return val;
            }
            cin.clear();
            clearBuffer();
            cout << "  [ERROR] Invalid input. Enter between "
                 << minVal << " and " << maxVal << ".\n";
        }
    }

    string getStringInput(const string& prompt) {
        string val;
        while (true) {
            cout << prompt;
            getline(cin, val);
            size_t start = val.find_first_not_of(" \t\r\n");
            size_t end   = val.find_last_not_of(" \t\r\n");
            if (start != string::npos) {
                val = val.substr(start, end - start + 1);
                return val;
            }
            cout << "  [ERROR] Name cannot be empty! Try again.\n";
        }
    }

public:
    Restaurant() : fileManager("orders.txt") {
        menu.push_back(MenuItem(1,  "Chicken Burger",    350.00));
        menu.push_back(MenuItem(2,  "Beef Burger",       400.00));
        menu.push_back(MenuItem(3,  "Margherita Pizza",  750.00));
        menu.push_back(MenuItem(4,  "BBQ Chicken Pizza", 900.00));
        menu.push_back(MenuItem(5,  "Pasta Alfredo",     550.00));
        menu.push_back(MenuItem(6,  "Grilled Chicken",   650.00));
        menu.push_back(MenuItem(7,  "French Fries",      200.00));
        menu.push_back(MenuItem(8,  "Coleslaw",          150.00));
        menu.push_back(MenuItem(9,  "Soft Drink",        100.00));
        menu.push_back(MenuItem(10, "Mineral Water",      60.00));
    }

    void displayMenu() {
        cout << "\n";
        cout << "  ============================================\n";
        cout << "               OUR MENU\n";
        cout << "  ============================================\n";
        cout << "  " << left << setw(6) << "No."
             << setw(25) << "Item"
             << "Price\n";
        cout << "  --------------------------------------------\n";
        for (int i = 0; i < (int)menu.size(); i++)
            menu[i].display();
        cout << "  ============================================\n\n";
    }

    void placeOrder() {
        cout << "\n  === PLACE NEW ORDER ===\n";

        string name = getStringInput("  Enter customer name: ");

        int orderId = fileManager.getNextOrderId();
        Order order(orderId, name, getCurrentDate());

        displayMenu();

        bool ordering = true;
        while (ordering) {
            int choice = getIntInput(
                "  Select item number (0 to finish): ", 0, (int)menu.size());

            if (choice == 0) {
                ordering = false;
            } else {
                int qty = getIntInput("  Enter quantity: ", 1, 20);
                MenuItem selected = menu[choice - 1];
                order.addItem(OrderItem(
                    selected.id, selected.name, qty, selected.price));
                cout << "  [OK] " << qty << "x " << selected.name << " added.\n";
            }
        }

        if (order.items.empty()) {
            cout << "  [INFO] No items added. Order cancelled.\n";
            return;
        }

        order.printBill();
        fileManager.saveOrder(order);
    }

    void viewAllOrders() {
        vector<Order> orders = fileManager.loadAllOrders();
        if (orders.empty()) {
            cout << "\n  [INFO] No orders found.\n\n";
            return;
        }
        cout << "\n  === ALL ORDERS ===\n\n";
        for (int i = 0; i < (int)orders.size(); i++)
            orders[i].printBill();
    }

    void searchOrder() {
        int searchId = getIntInput("\n  Enter Order ID to search: ", 1, 99999);
        vector<Order> orders = fileManager.loadAllOrders();
        bool found = false;
        for (int i = 0; i < (int)orders.size(); i++) {
            if (orders[i].orderId == searchId) {
                orders[i].printBill();
                found = true;
                break;
            }
        }
        if (!found)
            cout << "  [INFO] Order #" << searchId << " not found.\n\n";
    }

    void deleteAllOrders() {
        cout << "\n  Are you sure? Delete ALL records? (y/n): ";
        char confirm;
        cin >> confirm;
        clearBuffer();
        if (confirm == 'y' || confirm == 'Y')
            fileManager.clearAllOrders();
        else
            cout << "  [INFO] Deletion cancelled.\n";
    }

    void run() {
        cout << "\n";
        cout << "  ============================================\n";
        cout << "       WELCOME TO BITES & DELIGHTS\n";
        cout << "         Restaurant Order System\n";
        cout << "  ============================================\n";

        int choice;
        do {
            cout << "\n  ========== MAIN MENU ==========\n";
            cout << "  [1] View Menu\n";
            cout << "  [2] Place New Order\n";
            cout << "  [3] View All Orders\n";
            cout << "  [4] Search Order by ID\n";
            cout << "  [5] Delete All Records\n";
            cout << "  [0] Exit\n";
            cout << "  ================================\n";

            choice = getIntInput("  Enter your choice: ", 0, 5);

            switch (choice) {
                case 1: displayMenu();     break;
                case 2: placeOrder();      break;
                case 3: viewAllOrders();   break;
                case 4: searchOrder();     break;
                case 5: deleteAllOrders(); break;
                case 0:
                    cout << "\n  Thank you! Goodbye!\n\n";
                    break;
            }
        } while (choice != 0);
    }
};

int main() {
    Restaurant restaurant;
    restaurant.run();
    return 0;
}
