#include <iostream>
#include <string>
using namespace std;

class Part {
    int id;
    string name;
    string brand;
    float price;
    int qty;
    char grade;
    static int cnt;
public:
    void setData(int id, string name, string brand, float price, int qty, char grade);
    void showData();
    int getId();
    static int getCount();
    friend void showValue(Part &p);
};

// static data member is defined outside the class
int Part::cnt = 0;

void Part::setData(int id, string name, string brand, float price, int qty, char grade) {
    this->id = id;
    this->name = name;
    this->brand = brand;
    this->price = price;
    this->qty = qty;
    this->grade = grade;
    cnt++;
}

void Part::showData() {
    cout << id << "\t" << name << "\t" << brand << "\t" << price << "\t" << qty << "\t" << grade << endl;
}

inline int Part::getId() {
    return id;
}

int Part::getCount() {
    return cnt;
}

// friend can see private data of Part
void showValue(Part &p) {
    cout << "Part: " << p.name << " (" << p.brand << ")" << endl;
    cout << "Stock value = " << p.price * p.qty << endl;
}

void takeInput(int &id, string &name, string &brand, float &price, int &qty, char &grade) {
    cout << "Enter part id: ";
    cin >> id;
    while (id <= 0) {
        cout << "Id must be positive, enter again: ";
        cin >> id;
    }
    cin.ignore();
    cout << "Enter part name: ";
    getline(cin, name);
    cout << "Enter brand/model: ";
    getline(cin, brand);
    cout << "Enter price: ";
    cin >> price;
    while (price <= 0) {
        cout << "Price must be more than 0, enter again: ";
        cin >> price;
    }
    cout << "Enter quantity: ";
    cin >> qty;
    while (qty < 0) {
        cout << "Quantity cannot be negative, enter again: ";
        cin >> qty;
    }
    cout << "Enter grade (A/B/C): ";
    cin >> grade;
    while (grade != 'A' && grade != 'B' && grade != 'C') {
        cout << "Grade must be A, B or C, enter again: ";
        cin >> grade;
    }
}

// returns -1 if not found
int findIndex(Part *p, int id) {
    int pos = -1;
    int i;
    for (i = 0; i < Part::getCount(); i++) {
        if ((p + i)->getId() == id) {
            pos = i;
        }
    }
    return pos;
}

int main() {
    Part arr[50];
    // pointer to first object of array
    Part *ptr = arr;
    int choice, id, qty, flag, i, pos;
    string name, brand;
    float price;
    char grade;

    do {
        cout << "\n--- Repair Parts Ledger ---" << endl;
        cout << "1. Add part" << endl;
        cout << "2. Display all parts" << endl;
        cout << "3. Search part by id" << endl;
        cout << "4. Show stock value of a part" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            if (Part::getCount() == 50) {
                cout << "Ledger is full" << endl;
                break;
            }
            takeInput(id, name, brand, price, qty, grade);
            // check if id already exists
            flag = 0;
            for (i = 0; i < Part::getCount(); i++) {
                if ((ptr + i)->getId() == id) {
                    flag = 1;
                }
            }
            if (flag == 1) {
                cout << "Error: this part id already exists" << endl;
            } else {
                (ptr + Part::getCount())->setData(id, name, brand, price, qty, grade);
                cout << "Part added. Total parts = " << Part::getCount() << endl;
            }
            break;
        case 2:
            if (Part::getCount() == 0) {
                cout << "No records to show" << endl;
            } else {
                cout << "ID\tName\tBrand\tPrice\tQty\tGrade" << endl;
                for (i = 0; i < Part::getCount(); i++) {
                    (ptr + i)->showData();
                }
            }
            break;
        case 3:
            cout << "Enter id to search: ";
            cin >> id;
            pos = findIndex(arr, id);
            if (pos == -1) {
                cout << "Part not found" << endl;
            } else {
                cout << "ID\tName\tBrand\tPrice\tQty\tGrade" << endl;
                arr[pos].showData();
            }
            break;
        case 4:
            cout << "Enter id: ";
            cin >> id;
            pos = findIndex(arr, id);
            if (pos == -1) {
                cout << "Part not found" << endl;
            } else {
                showValue(arr[pos]);
            }
            break;
        case 5:
            cout << "Bye" << endl;
            break;
        default:
            cout << "Wrong choice, try again" << endl;
        }
    } while (choice != 5);

    return 0;
}
