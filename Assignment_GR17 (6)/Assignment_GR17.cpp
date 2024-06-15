#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <stack>
#include <sstream>


using namespace std;

struct MenuItem {
    string code;
    string name;
    string category;
    double price;
};

struct OrderItem {
    string item;
    int quantity;
    double price;
    string username;
    string orderTime;
    string dineOption; 
};

struct Order {
    string customerName;
    string dineOption;
    string foodDetails;
    int quantity;
    double price;
    string orderTime;
    time_t parsedOrderTime;
    double totalPrice; 

    bool operator<(const Order &other) const {
        return parsedOrderTime < other.parsedOrderTime;
    }
};



class SearchItem {
public:
    static vector<MenuItem> searchByPriceRange(const vector<MenuItem>& menu, double minPrice, double maxPrice) {
        vector<MenuItem> result;
        for (const auto& item : menu) {
            if (item.price >= minPrice && item.price <= maxPrice) {
                result.push_back(item);
            }
        }
        return result;
    }

    static vector<MenuItem> searchByCategory(const vector<MenuItem>& menu, const string& category) {
        vector<MenuItem> result;
        for (const auto& item : menu) {
            if (item.category == category) {
                result.push_back(item);
            }
        }
        return result;
    }

    // Overloaded search function for searching by name
    static vector<MenuItem> searchByName(const vector<MenuItem>& menu, const string& name) {
        vector<MenuItem> result;
        for (const auto& item : menu) {
            if (item.name == name) {
                result.push_back(item);
            }
        }
        return result;
    }

    // Overloaded search function for searching by code
    static vector<MenuItem> searchByCode(const vector<MenuItem>& menu, const string& code) {
        vector<MenuItem> result;
        for (const auto& item : menu) {
            if (item.code == code) {
                result.push_back(item);
            }
        }
        return result;
    }
};

class Algorithm {
public:
    static void bubbleSortMenuByName(vector<MenuItem>& menu) {
        int n = menu.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (menu[j].name > menu[j + 1].name) {
                    swap(menu[j], menu[j + 1]);
                }
            }
        }
    }

    static void bubbleSortMenuByPrice(vector<MenuItem>& menu) {
    int n = menu.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (menu[j].price < menu[j + 1].price) {  // Change comparison to <
                swap(menu[j], menu[j + 1]);
            }
        }
    }
}

};

// Base class
class User {
protected:
    string username;
    string password;
public:
    User(const string& u, const string& p) : username(u), password(p) {}

    // Virtual login function
    virtual bool login(const string& u, const string& p) {
        return username == u && password == p;
    }

    friend void displayUserDetails(const User& user);
};

void displayUserDetails(const User& user) {
    cout << "Username: " << user.username << endl;
}

// Derived class for Customer
class Customer : public User {
public:
    Customer(const string& u, const string& p) : User(u, p) {}

    // Overridden login function
    bool login(const string& u, const string& p) override {
        if (User::login(u, p)) {
            cout << "Customer login successful" << endl;
            return true;
        }
        cout << "Customer login failed" << endl;
        return false;
    }
};

// Derived class for Admin
class Admin : public User {
public:
    Admin(const string& u, const string& p) : User(u, p) {}

    bool login(const string& u, const string& p) override {
        if (User::login(u, p)) {
            cout << "Admin login successful" << endl;
            return true;
        }
        cout << "Admin login failed" << endl;
        return false;
    }
    
    MenuItem getMenuItemDetails(const string& code, const map<string, MenuItem>& menuMap) {
    if (menuMap.find(code) != menuMap.end()) {
        return menuMap.at(code);
    } else {
        // Return a default MenuItem or throw an exception if needed
        return MenuItem(); // Replace with appropriate handling as per your design
    }
}
    

    void addMenuItem(vector<MenuItem>& menu, const MenuItem& item) {
        menu.push_back(item);
    }

    void removeMenuItem(vector<MenuItem>& menu, const string& code) {
        auto it = remove_if(menu.begin(), menu.end(), [&](const MenuItem& item) { return item.code == code; });
        menu.erase(it, menu.end());
    }

    void editMenuItem(vector<MenuItem>& menu, map<string, MenuItem>& menuMap, const string& code) {
        for (auto& item : menu) {
            if (item.code == code) {
                cout << "Editing item with code: " << code << endl;
                cout << "Enter new name (or press enter to keep current): ";
                string newName;
                cin.ignore();
                getline(cin, newName);
                if (!newName.empty()) {
                    item.name = newName;
                }

                cout << "Enter new category (or press enter to keep current): ";
                string newCategory;
                getline(cin, newCategory);
                if (!newCategory.empty()) {
                    item.category = newCategory;
                }

                cout << "Enter new price (or press enter to keep current): ";
                string priceStr;
                getline(cin, priceStr);
                if (!priceStr.empty()) {
                    item.price = stod(priceStr);
                }

                // Update the menu map
                menuMap[item.code] = item;

                cout << "Item updated successfully." << endl;
                break;
            }
        }
    }

    void saveMenuToFile(const vector<MenuItem>& menu) {
        ofstream menuFile("menu.txt");
        if (!menuFile) {
            cerr << "Failed to open menu.txt" << endl;
            return;
        }

        for (const auto& item : menu) {
            menuFile << item.code << "," << item.name << "," << item.category << "," << item.price << endl;
        }

        menuFile.close();
        cout << "Menu saved to menu.txt" << endl;
    }

    friend void displayAdminDetails(const Admin& admin);
};


void displayAdminDetails(const Admin& admin) {
    cout << "Admin Username: " << admin.username << endl;
}

class FastFoodOrderingSystem {
private:
    vector<MenuItem> menu;
    map<string, MenuItem> menuMap;
    vector<MenuItem> originalMenu;
    vector<OrderItem> orders;
    stack<string> orderHistory; // Stack to store order history

    void loadMenu() {
        ifstream menuFile("menu.txt");
        if (!menuFile) {
            cerr << "Failed to open menu.txt" << endl;
            exit(1);
        }

        string line;
        while (getline(menuFile, line)) {
            MenuItem item;
            size_t pos = 0;
            pos = line.find(',');
            item.code = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(',');
            item.name = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(',');
            item.category = line.substr(0, pos);
            line.erase(0, pos + 1);

            item.price = stod(line);

            menu.push_back(item);
            originalMenu.push_back(item);
            menuMap[item.code] = item;
        }

        menuFile.close();
    }

    void displayMenu(const vector<MenuItem>& menu) {
        cout << "-----------------------------------------------------------------------------"
             << endl;
        cout << "Food code | Food name                          | Food type       | Food Price" << endl;
        cout << "-----------------------------------------------------------------------------"
             << endl;

        for (const auto& item : menu) {
            cout << setw(9) << left << item.code << " | "
                 << setw(33) << left << item.name << " | "
                 << setw(14) << left << item.category << " | RM "
                 << fixed << setprecision(2) << item.price << endl;
        }

        cout << "-----------------------------------------------------------------------------" << endl;
    }

    void displayOrderSummary() 
	{
    	system("cls");
		cout << "------------------------------------------" << endl;
        cout << "Receipt:" << endl;
        cout << "------------------------------------------" << endl;
        for (const auto& order : orders) 
		{
            cout << setw(25) << left << order.item << " x"
                 << setw(5) << right << order.quantity << "  RM "
                 << fixed << setprecision(2) << order.price << endl;
        }
        cout << "------------------------------------------" << endl;
        cout << "Total Price: \t\t\tRM " << calculateTotalPrice() << endl;
        cout << "------------------------------------------\n" << endl;

    }
    
        
    
	void recordOrder(const string& customerName, const string& dineOption) {
    ofstream orderFile("orders.txt", ios::app);
    if (!orderFile) {
        cerr << "Failed to open orders.txt" << endl;
        return;
    }

    auto now = chrono::system_clock::now();
    time_t orderTime = chrono::system_clock::to_time_t(now);

    // Write each order to file
    for (const auto& order : orders) {
        orderFile << customerName << "," << dineOption << "," << order.item << "," << order.quantity << "," << order.price << "," << ctime(&orderTime);
        // Add order details to order history
        orderHistory.push(order.item + " x " + to_string(order.quantity) + " RM " + to_string(order.price));
    }

    // Calculate total price after writing orders to file
    double totalPrice = calculateTotalPrice();


    orderFile.close();
}

void searchOrderByCustomerName() {
    ifstream orderFile("history.txt");
    if (!orderFile) {
        cerr << "Failed to open history.txt" << endl;
        return;
    }

    unordered_map<string, vector<string>> orderMap;
    string line;

    // Build the hash map
    while (getline(orderFile, line)) {
        stringstream ss(line);
        string customerName, dineOption, foodDetails, orderTime;
        int quantity;
        double price;

        getline(ss, customerName, ',');
        getline(ss, dineOption, ',');
        getline(ss, foodDetails, ',');
        ss >> quantity;
        ss.ignore(); // Ignore the comma after quantity
        ss >> price;
        ss.ignore(); // Ignore the comma after price
        getline(ss, orderTime);

        string orderDetails = dineOption + "," + foodDetails + "," + to_string(quantity) + "," + to_string(price) + "," + orderTime;
        orderMap[customerName].push_back(orderDetails);
    }

    orderFile.close();

    // Prompt admin to enter customer name
    string searchName;
    cout << "Enter customer name to search: ";
    cin.ignore();
    getline(cin, searchName);

    if (orderMap.find(searchName) != orderMap.end()) {
        cout << "Order History for " << searchName << ":" << endl;
        cout << "----------------------------------------------------------------------------------------------------------------" << endl;
        cout << setw(20) << left << "Dine Option" << setw(30) << left << "Food Details" << setw(10) << left << "Quantity" << setw(10) << left << "Price" << "Order Time" << endl;

        for (const auto& order : orderMap[searchName]) {
            stringstream ss(order);
            string dineOption, foodDetails, orderTime;
            int quantity;
            double price;

            getline(ss, dineOption, ',');
            getline(ss, foodDetails, ',');
            ss >> quantity;
            ss.ignore(); // Ignore the comma after quantity
            ss >> price;
            ss.ignore(); // Ignore the comma after price
            getline(ss, orderTime);

            cout << setw(20) << left << (dineOption == "t" ? "Take away" : "Dine in") << setw(30) << left << foodDetails << setw(10) << left << quantity << setw(10) << left << fixed << setprecision(2) << price << orderTime << endl;
        }
        cout << "----------------------------------------------------------------------------------------------------------------" << endl;
    } else {
        cout << "No order history found for customer: " << searchName << endl;
    }
}


void merge(vector<Order> &arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Order> L(n1);
    vector<Order> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<Order> &arr, int left, int right) {
    if (left >= right)
        return;
    
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void viewOrderHistory() {
    ifstream orderFile("history.txt");
    if (!orderFile) {
        cerr << "Failed to open history.txt" << endl;
        return;
    }

    vector<Order> orders;
    string line;

    while (getline(orderFile, line)) {
        stringstream ss(line);
        Order order;

        getline(ss, order.customerName, ',');
        getline(ss, order.dineOption, ',');
        getline(ss, order.foodDetails, ',');
        ss >> order.quantity;
        ss.ignore(); // Ignore the comma after quantity
        ss >> order.price;
        ss.ignore(); // Ignore the comma after price
        getline(ss, order.orderTime);

        // Calculate total price for the order
        order.totalPrice = order.quantity * order.price;

        orders.push_back(order);
    }
    orderFile.close();

    cout << "Order History:" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << setw(20) << left << "Customer name" << setw(15) << left << "Dine Option" << setw(30) << left << "Food Details" << setw(10) << left << "Quantity" << setw(10) << left << "Price" << setw(15) << left << "Total Price" << "Order Time" << endl;

    for (const auto &order : orders) {
        cout << setw(20) << left << order.customerName
             << setw(15) << left << (order.dineOption == "t" ? "Take away" : "Dine in")
             << setw(30) << left << order.foodDetails
             << setw(10) << left << order.quantity
             << setw(10) << left << fixed << setprecision(2) << order.price
             << setw(15) << left << fixed << setprecision(2) << order.totalPrice
             << order.orderTime << endl;
    }

    cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;

    while (true) {
        cout << "Choose an option:" << endl;
        cout << "1. Search order by customer name" << endl;
        cout << "2. Sort orders by latest order time" << endl;
        cout << "3. Back" << endl;
        cout << "Enter your choice: ";
        int adminChoice;
        cin >> adminChoice;

        if (adminChoice == 1) {
            searchOrderByCustomerName(); // Assuming this function is defined elsewhere
        } else if (adminChoice == 2) {
        	system("cls");
            mergeSort(orders, 0, orders.size() - 1);
            cout << "Sorted Order History:" << endl;
            cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
            cout << setw(20) << left << "Customer name" 
			<< setw(15) << left << "Dine Option" 
			<< setw(30) << left << "Food Details" 
			<< setw(10) << left << "Quantity" 
			<< setw(10) << left << "Price" 
			<< setw(15) << left << "Total Price" << "Order Time" << endl;

            for (const auto &order : orders) {
                cout << setw(20) << left << order.customerName
                     << setw(15) << left << (order.dineOption == "t" ? "Take away" : "Dine in")
                     << setw(30) << left << order.foodDetails
                     << setw(10) << left << order.quantity
                     << setw(10) << left << fixed << setprecision(2) << order.price
                     << setw(15) << left << fixed << setprecision(2) << order.totalPrice
                     << order.orderTime << endl;
            }
            cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
        } else if (adminChoice == 3) {
            break;
        } else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }
}


    void simulateLoading() {
        cout << "Processing";
        
        for (int i = 0; i < 3; ++i) {
            cout << ".";
            this_thread::sleep_for(chrono::milliseconds(500));
        }
        cout << endl;
    }
    
    
    	

    void userRegistration() {
        system("cls");
        cout << "******************************************" << endl;
        cout << "                Register page              " << endl;
        cout << "******************************************" << endl;

        string username, password, confirmPassword;

        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;
        cout << "Confirm your password: ";
        cin >> confirmPassword;

        if (password == confirmPassword) {
            ofstream userFile("users.txt", ios::app);
            if (!userFile) {
                cerr << "Failed to open users.txt" << endl;
                return;
            }

            userFile << username << "," << password << endl;
            userFile.close();

            // Log registration details with time and date
            ofstream registerLogFile("register.txt", ios::app);
            if (!registerLogFile) {
                cerr << "Failed to open register.txt" << endl;
                return;
            }

            auto now = chrono::system_clock::now();
            time_t now_c = chrono::system_clock::to_time_t(now);
            registerLogFile << "Username: " << username << ", Date: " << ctime(&now_c);

            registerLogFile.close();

            simulateLoading();
            cout << "Register successful........." << endl;
        } else {
            cout << "Passwords do not match. Please try again." << endl;
        }
    }

    bool userLogin() {
        system("cls");
        cout << "******************************************" << endl;
        cout << "                Login page                " << endl;
        cout << "******************************************" << endl;

        string username, password;
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;

        ifstream userFile("users.txt");
        if (!userFile) {
            cerr << "Failed to open users.txt" << endl;
            return false;
        }

        string line;
        while (getline(userFile, line)) {
            size_t pos = line.find(',');
            string fileUsername = line.substr(0, pos);
            string filePassword = line.substr(pos + 1);

            if (username == fileUsername && password == filePassword) {
                userFile.close();
                simulateLoading();
                system("cls");
                cout << "Login successful......" << endl;
                return true;
            }
        }

        userFile.close();
        simulateLoading();
        cout << "Login failed. Please check your username and password." << endl;
        return false;
    }

    double calculateTotalPrice() {
        double totalPrice = 0.0;
        for (size_t i = 0; i < orders.size(); i++) {
            totalPrice += orders[i].price * orders[i].quantity;
        }
        return totalPrice;
    }

public:
    void startOrdering() {
        loadMenu();

        while (true) {
            system("cls");
            cout << "*******************************************" << endl;
            cout << "  Welcome to Fast Food Ordering system  " << endl;
            cout << "*******************************************" << endl;
            cout << "1. Customer login" << endl;
            cout << "2. Customer Registration" << endl;
            cout << "3. Admin Mode" << endl;
            cout << "4. Exit" << endl;
            cout << "Please enter your choice: ";
            int choice;
            cin >> choice;

            if (choice == 2) {
                userRegistration();
            } else if (choice == 1) {
                if (userLogin()) {
                string customerName;  // Declare customerName variable
                cout << "Enter your name: ";
                cin.ignore();
                getline(cin, customerName);  // Capture the customer's name
                
                    string dineOption;
                    cout << "Do you want to (e = Eat in  t = Take away): ";
                    cin >> dineOption;
                    while (dineOption != "e" && dineOption != "E" && dineOption != "t" && dineOption != "T") {
                    cout << "Invalid choice! Please type again: ";
                    cin >> dineOption;
                }
                    system("cls");

                    cout << "****************************************************************************" << endl;
                    cout << "                           Welcome to PS Fast Food!                         " << endl;
                    cout << "****************************************************************************" << endl;

                    while (true) 
					{
                        displayMenu(menu);
                        cout << "Choose an option:" << endl;
                        cout << "1. Sort by menu" << endl;
                        cout << "2. Search by menu" << endl;
                        cout << "3. Place order" << endl;
                        cout << "4. Exit" << endl;
                        cout << "Enter your choice: ";
                        int menuChoice;
                        cin >> menuChoice;

						
						if(menuChoice == 1)
						{		system("cls");
								cout << "1. Sort menu by name" << endl;
	                        	cout << "2. Sort menu by price" << endl;
	                        	
	                        	cout << "Enter your choice: ";
	                        	int userchoice;
	                        	cin >> userchoice;
	                        	if (userchoice == 1) 
								{
		                            Algorithm::bubbleSortMenuByName(menu);
		                            displayMenu(menu);
		                            menu = originalMenu;
		                        } 
								else if (userchoice == 2) 
								{
		                            Algorithm::bubbleSortMenuByPrice(menu);
		                            displayMenu(menu);
		                            menu = originalMenu;
								}
						}
                        else if(menuChoice == 2)
                        {
                        	system("cls");
                        	cout << "1. Search food by price range" << endl;
                        	cout << "2. Search food by food type" << endl;
                        	
                        	cout << "Enter your choice: ";
	                        int userchoice;
	                        cin >> userchoice;
	                        
                        	if (userchoice == 1) 
							{
	                            double minPrice, maxPrice;
	                            cout << "Enter minimum price: ";
	                            cin >> minPrice;
	                            cout << "Enter maximum price: ";
	                            cin >> maxPrice;
	                            
	                            while (true) 
								{
		                            	vector<MenuItem> results = SearchItem::searchByPriceRange(menu, minPrice, maxPrice);
		                            	displayMenu(results);
		                            	
		                            	cout << "1. Continue searching" << endl;
		                				cout << "2. Back to menu" << endl;
		                				cout << "Enter your choice: ";
		                				int searchChoice;
		                				cin >> searchChoice;

				                		if (searchChoice == 1) {
				                            cout << "Enter minimum price: ";
				                            cin >> minPrice;
				                            cout << "Enter maximum price: ";
				                            cin >> maxPrice;
				                		} 
										else if (searchChoice == 2) 
										{
				                			system("cls");
				                    		break;
				               			}
									   	else 
									   	{
			                    			cout << "Invalid choice! Please try again." << endl;
			                			}
		                		}
		                } 
						else if (menuChoice == 2) 
						{
            				string category;
            				cout << "Enter food type (burger/pizza/cake/snack): ";
            				cin >> category;
            				while (true) 
							{
                				vector<MenuItem> results = SearchItem::searchByCategory(menu, category);
                				displayMenu(results);

                				cout << "1. Continue searching" << endl;
                				cout << "2. Back to menu" << endl;
                				cout << "Enter your choice: ";
                				int searchChoice;
                				cin >> searchChoice;

                		if (searchChoice == 1) 
						{
                    		cout << "Enter food type (burger/pizza/cake/snack): ";
                    		cin >> category;
                		} 
						else if (searchChoice == 2)
						{
                			system("cls");
                    	break;
               			} 
						else 
						{
                    		cout << "Invalid choice! Please try again." << endl;
                		}
            		}
        			
				}
						}  
						else if (menuChoice == 3) 
						{
                            while (true) 
							{
                                system("cls");
                                menu = originalMenu;
                                displayMenu(menu);
                                string itemCode;
                                string BeverageCode;
                                int quantity;

                                cout << "Enter the food code you wish to order: ";
                                cin >> itemCode;

                                auto it = menuMap.find(itemCode);
                            	if (it != menuMap.end())
								{
                                    MenuItem item = menuMap[itemCode];
                                    
                                    cout << "----------------------------" << endl;
                                    cout << item.name << endl;
                                    cout << "----------------------------" << endl;

                                    cout << "Enter the quantity: ";
                                    cin >> quantity;

                                    OrderItem orderItem = { item.name, quantity, item.price };
                                    orders.push_back(orderItem);

                                    cout << "Would you like to add a beverage? (yes/no): ";
                                    string addBeverage;
                                    cin >> addBeverage;
																			
                                    if (addBeverage == "yes" || addBeverage == "YES") 
									{    
										cout << "***************************" << endl;
                                        cout << "D01    Tea         RM 2.00     " << endl;
                                        cout << "D02    Coffee      RM 3.00     " << endl;
                                        cout << "***************************" << endl;
                         
                                        cout << "Enter the beverage code: ";
                                        cin >> BeverageCode;
                                        cout << "Enter the quantity: ";
                                        cin >> quantity;

                                        double beveragePrice = 0.0;
                                        if (BeverageCode == "D01") 
										{
                                            beveragePrice = 2.00;
                                        } 
										else if (BeverageCode == "D02") 
										{
                                            beveragePrice = 3.00;
                                        } 
										else 
										{
                                            cout << "Invalid beverage code!" << endl;
                                        }

                                        if (beveragePrice > 0.0) {
                                            OrderItem beverageOrder = { (BeverageCode == "D01" ? "Tea" : "Coffee"), quantity, beveragePrice };
                                            orders.push_back(beverageOrder);
                                        }
                                    }
                                } 
								

                                cout << "Would you like to order another item? (yes/no): ";
                                string moreItems;
                                cin >> moreItems;
                                
                                
                                if (moreItems == "no") 
								{
									displayOrderSummary();
		                            recordOrder(customerName, dineOption);
		                            break; // Exit after showing receipt
                                }
                                
                                else 
								{
                                    cout << "Invalid item code!" << endl;
                                }
                                
                            }

                            
                        } 
						else if (menuChoice == 4) 
						{
                            break;
                        }
                    }
                }
            } 
			else if (choice == 3) 
			{
                // Admin Mode
                Admin admin("ps" , "111"); // Example admin credentials
                string adminUsername, adminPassword;
                cout << "Enter admin username: ";
                cin >> adminUsername;
                cout << "Enter admin password: ";
                cin >> adminPassword;

                if (admin.login(adminUsername, adminPassword)) {system("cls");
                    while (true) {
                        cout << "********************************" << endl;
                        cout << "          Admin Menu            " << endl;
                        cout << "********************************" << endl;
                        cout << "1. Add Menu Item" << endl;
                        cout << "2. Edit Item" << endl;
                        cout << "3. Remove Menu Item" << endl;
                        cout << "4. View Menu" << endl;
                        cout << "5. View Order History" << endl;
                        cout << "6. Logout" << endl;
                        cout << "Enter your choice: ";
                        int adminChoice;
                        cin >> adminChoice;

                        if (adminChoice == 1) {
                            MenuItem newItem;
                            cout << "Enter food code: ";
                            cin >> newItem.code;
                            cout << "Enter food name: ";
                            cin.ignore();
                            getline(cin, newItem.name);
                            cout << "Enter food category: ";
                            getline(cin, newItem.category);
                            cout << "Enter food price: ";
                            cin >> newItem.price;
                            admin.addMenuItem(menu, newItem);
                            menuMap[newItem.code] = newItem;
                            admin.saveMenuToFile(menu);
                        } else if (adminChoice == 2) {
                            cout << "Enter the code of the item to edit: ";
                            string editCode;
                            cin >> editCode;

                            // Get item details using Admin class method
                            MenuItem itemToEdit = admin.getMenuItemDetails(editCode, menuMap);

                            // Check if the item exists
                            if (itemToEdit.code == "") 
							{
                                cout << "Item with code " << editCode << " not found." << endl;
                            } 
							else 
							{
                                cout << "********************************" << endl;
                                cout << "  Current details of item " << editCode << ":" << endl;
                                cout << "********************************" << endl;
                                cout << "Name: " << itemToEdit.name << endl;
                                cout << "Category: " << itemToEdit.category << endl;
                                cout << "Price: RM " << fixed << setprecision(2) << itemToEdit.price << endl;

                                // Proceed to edit the item (your existing editMenuItem function call)
                                admin.editMenuItem(menu, menuMap, editCode);
                                admin.saveMenuToFile(menu);

                                cout << "Item " << editCode << " has been updated." << endl;
                            }
                        } 
						else if (adminChoice == 3) 
						{
                            cout << "Enter the code of the item to remove: ";
                            string removeCode;
                            cin >> removeCode;
                            admin.removeMenuItem(menu, removeCode);
                            admin.saveMenuToFile(menu);
                        } 
						else if (adminChoice == 4) 
						{
                        	system("cls");
                            displayMenu(menu);
                        } 
						else if (adminChoice == 5) 
						{
                        	system("cls");
                        	viewOrderHistory();  
                        } 
						else if (adminChoice == 6) 
						{
                            break;
                        }
                    }
                }
            } 
			else if (choice == 4)
			{
                break;
            }
        }
    }
};

int main() 
{
    FastFoodOrderingSystem system;
    system.startOrdering();
    return 0;
}

