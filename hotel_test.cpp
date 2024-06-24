#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

class Customer {
private:
    static int id;
    int customerId;
    string name;
    double balance;
    string checkInDate;
    string checkOutDate;
public:
    Customer(string n, double b, string cid, string cod)
        : customerId(id++), name(n), balance(b), checkInDate(cid), checkOutDate(cod) {}

    void setName(string n) {
        name = n;
    }
    void setCheckInDate(string date) {
        checkInDate = date;
    }
    void setCheckOutDate(string date) {
        checkOutDate = date;
    }

    string getName() const {
        return name;
    }
    string getCheckInDate() const {
        return checkInDate;
    }
    string getCheckOutDate() const {
        return checkOutDate;
    }
    double getBalance() const {
        return balance;
    }
    int getId() const {
        return customerId;
    }

    void updateBalance(double amount) {
        balance += amount;
    }
    void displayCustomerInfo() const {
        cout << "Customer ID: " << customerId << endl;
        cout << "Name: " << name << endl;
        cout << "Balance: " << fixed << setprecision(2) << balance << endl;
        cout << "Check-in Date: " << checkInDate << endl;
        cout << "Check-out Date: " << checkOutDate << endl;
    }
};

int Customer::id = 1;

class Room {
private:
    static int capacity;
    int roomNumber;
    bool available;
    Customer* customer; // Using pointer for customer

public:
    Room() : available(true), customer(nullptr) {
        roomNumber = capacity;
        capacity++;
    }
    ~Room() { delete customer; } // Destructor to clean up allocated memory

    int getNumber() const { return roomNumber; }
    void setAvailability(bool avail) { available = avail; }
    bool isAvailable() const { return available; }

    void assignCustomer(Customer* cust) {
        if (customer) {
            delete customer; // Clean up existing customer
        }
        customer = cust;
        available = false;
    }

    void releaseRoom() {
        delete customer; // Clean up customer
        customer = nullptr; // Reset to nullptr
        available = true;
    }

    const Customer* getCustomer() const { return customer; }
};

int Room::capacity = 1;



class Hotel {
private:
    static string name;
    static string description;
    static vector<Room> rooms;

public:
    // Setter for name
    void setName(string name) {
        this->name = name;
    }

    // Getter for name
    string getName() {
        return name;
    }

    // Setter for description
    void setDescription(string description) {
        this->description = description;
    }

    // Getter for description
    string getDescription() {
        return description;
    }

    // Method to add a room to the rooms vector
    void addRoom(Room room) {
        rooms.push_back(room);
    }

    // Getter for rooms vector
    vector<Room> getRooms() {
        return rooms;
    }
};

class Accounting {
private:
    string customerName;
    string checkInDate;
    string checkOutDate;
    double amount;
public:
    void generateBill(string customerName, string checkInDate, string checkOutDate, double amount) {
        this->customerName = customerName;
        this->checkInDate = checkInDate;
        this->checkOutDate = checkOutDate;
        this->amount = amount;
    }
    void addBill(vector<Accounting>& bills) {
        bills.push_back(*this);
        ofstream file("bills.db", ios::app);
        if (file.is_open()) {
            file << "Customer Name: " << customerName << "\n"
                << "Check-In Date: " << checkInDate << "\n"
                << "Check-Out Date: " << checkOutDate << "\n"
                << "Amount: $" << amount << "\n\n";
            file.close();
        }
        else {
            cerr << "Unable to open file bills.db";
        }
    }
    void displayBill() const {
        cout << "Customer Name: " << customerName << "\n"
            << "Check-In Date: " << checkInDate << "\n"
            << "Check-Out Date: " << checkOutDate << "\n"
            << "Amount: $" << amount << "\n";
    }
    double getAmount() const {
        return amount;
    }
};

class Reception {
private:
    vector<Room> availableRooms;
    vector<Room> bookedRooms;
    vector<Accounting> bills;
    vector<Customer> customers;
    Hotel hotel;
public:
    void loadRoomsAndBookings(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                int roomNumber;
                string status, customerName, dateIn, dateOut;
                iss >> roomNumber >> status >> customerName >> dateIn >> dateOut;

                Room room;
                Customer* customer = new Customer(customerName, 0, dateIn, dateOut);
                room.assignCustomer(customer);
                room.setAvailability(status == "available");
                if (status == "available") {
                    availableRooms.push_back(room);
                }
                else {
                    bookedRooms.push_back(room);
                }
            }
            file.close();
        }
        else {
            cerr << "Unable to open file: " << filename << endl;
        }
    }

    void loadCustomersAndBills(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                int id, room;
                string name, surname;
                double balance, billAmount;
                iss >> id >> name >> surname >> room >> balance >> billAmount;

                Customer customer(name, balance, "", "");
                customers.push_back(customer);

                Accounting bill;
                bill.generateBill(name, "", "", billAmount);
                bills.push_back(bill);
            }
            file.close();
        }
        else {
            cerr << "Unable to open file: " << filename << endl;
        }
    }

    void bookCustomer(Room& room, const string& name, double balance, const string& checkInDate, const string& checkOutDate) {
        cout << "1";
        Customer* customer = new Customer(name, balance, checkInDate, checkOutDate);
        room.assignCustomer(customer);
        vector <Room> availableRooms = this->hotel.getRooms();
        bookedRooms.push_back(room);
        cout << "broniruu";
        auto it = find_if(availableRooms.begin(), availableRooms.end(), [&](const Room& r) { return r.getNumber() == room.getNumber(); });
        if (it != availableRooms.end()) {
            cout << "zanayto";
            availableRooms.erase(it);
        }
        cout << "Uspeshno";

        Accounting bill;
        bill.generateBill(name, checkInDate, checkOutDate, balance);
        bills.push_back(bill);
    }

    void checkInCustomer(Room& room, const string& name, double balance, const string& checkInDate, const string& checkOutDate) {
        bookCustomer(room, name, balance, checkInDate, checkOutDate);
    }

    void checkOutCustomer(Room& room) {
        auto it = find_if(bookedRooms.begin(), bookedRooms.end(), [&](const Room& r) { return r.getNumber() == room.getNumber(); });
        if (it != bookedRooms.end()) {
            room.releaseRoom();
            availableRooms.push_back(room);
            bookedRooms.erase(it);
        }
    }


    vector<Room> getAvailableRooms(Hotel hotel) const { return availableRooms; }
    vector<Room> getBookedRooms() const { return bookedRooms; }

    void chargeCustomer(Customer& customer) {
        auto it = find_if(bills.begin(), bills.end(), [&](const Accounting& bill) { return bill.getAmount() == customer.getBalance(); });
        if (it != bills.end()) {
            double amount = it->getAmount();
            customer.updateBalance(-amount);
        }
    }

    void saveRoomsAndBookings(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& room : availableRooms) {
                file << room.getNumber() << " available " << " " << " " << endl;
            }
            for (const auto& room : bookedRooms) {
                const Customer* customer = room.getCustomer(); // Get the pointer to const Customer
                file << room.getNumber() << " booked " << customer->getName() << " " << customer->getCheckInDate() << " " << customer->getCheckOutDate() << endl;
            }
            file.close();
        }
        else {
            cerr << "Unable to open file: " << filename << endl;
        }
    }

    void saveCustomersAndBills(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& customer : customers) {
                file << customer.getName() << " " << customer.getCheckInDate() << " " << customer.getCheckOutDate() << " " << customer.getBalance() << endl;
            }
            for (const auto& bill : bills) {
                file << bill.getAmount() << endl;
            }
            file.close();
        }
        else {
            cerr << "Unable to open file: " << filename << endl;
        }
    }
};

class HotelSystem {
private:
    Hotel hotel;
    Accounting accounting;
    vector<Room> availableRooms;
    Reception reception;
public:
    HotelSystem() {
        for (int i = 1; i < 1000; i++) {
            Room room;
            this->hotel.addRoom(room);
        }
        readInitialStateFromFile();
    }

    void readInitialStateFromFile() {
        // Read initial state from db and initialize the system
    }

    void displayHotelDescription() {
        // Display hotel description
    }

    void checkInCustomer() {
        string name, checkInDate, checkOutDate;
        double balance;
        int roomNumber;

        cout << "Enter customer name: ";
        cin >> name;
        cout << "Enter balance: ";
        cin >> balance;
        cout << "Enter check-in date: ";
        cin >> checkInDate;
        cout << "Enter check-out date: ";
        cin >> checkOutDate;
        cout << "Enter room number: ";
        cin >> roomNumber;

        // Find the room by number
        Room* roomToBook = nullptr;
        
        for (auto& room : Hotel::getRooms()) {
            cout << "room get number";
            if (room.getNumber() == roomNumber) {
                roomToBook = &room;
                break;
            }
        }
        cout << "da";
        if (roomToBook) {
            reception.bookCustomer(*roomToBook, name, balance, checkInDate, checkOutDate);
            cout << "Customer checked-in successfully!" << endl;
        }
        else {
            cout << "Room number " << roomNumber << " not found or not available." << endl;
        }
    }

    void checkOutCustomer() {
        int roomNumber;
        cout << "Enter room number to check-out: ";
        cin >> roomNumber;

        // Find the room by number
        Room* roomToCheckOut = nullptr;
        for (auto& room : reception.getBookedRooms()) {
            if (room.getNumber() == roomNumber) {
                roomToCheckOut = &room;
                break;
            }
        }

        if (roomToCheckOut) {
            reception.checkOutCustomer(*roomToCheckOut);
            cout << "Customer checked-out successfully!" << endl;
        }
        else {
            cout << "Room number " << roomNumber << " not found or not booked." << endl;
        }
    }

    void viewAvailableRooms() {
        cout << "Available Rooms:" << endl;
        for (const auto& room : this->hotel.getRooms()) {
            if (room.isAvailable()) {
                cout << "Room " << room.getNumber() << endl;
            }
        }
    }


    void viewBookedRooms() {
        cout << "Booked Rooms:" << endl;
        for (const auto& room : this->hotel.getRooms()) {
            if (!room.isAvailable()) {
                cout << "Room " << room.getNumber() << endl;
            }
        }
    }

    // void viewBillingSystem() {
    //     reception.displayBillingSystem();
    // }

    void saveToDatabase() {
        ofstream file("rooms.db");
        if (!file) {
            cerr << "Error opening file" << endl;
            return;
        }

        for (const auto& room : hotel.getRooms()) {
            file << room.getNumber() << ": " << (room.isAvailable() ? "available" : "booked") << endl;
        }

        file.close();
    }

    void otherFeatures() {
        // Если чего еще придумаете
        cout << "Ты молодец!" << endl;
    }

    void menu() {
        int choice;
        do {
            cout << "Hotel Management System Menu" << endl;
            cout << "1. Display Hotel Description" << endl;
            cout << "2. Check-in Customer" << endl;
            cout << "3. Check-out Customer" << endl;
            cout << "4. View Available Rooms" << endl;
            cout << "5. View Booked Rooms" << endl;
            cout << "6. View Billing System" << endl;
            cout << "7. Save to Database" << endl;
            cout << "8. Inshalla не обосремся" << endl;
            cout << "9. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                displayHotelDescription();
                break;
            case 2:
                checkInCustomer();
                break;
            case 3:
                checkOutCustomer();
                break;
            case 4:
                viewAvailableRooms();
                break;
            case 5:
                viewBookedRooms();
                break;
                // case 6:
                //     viewBillingSystem();
                //     break;
            case 7:
                saveToDatabase();
                break;
            case 8:
                otherFeatures();
                break;
            case 9:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 9);
    }
};

int main() {

    HotelSystem hotelSystem;
    hotelSystem.menu();



    return 0;
}