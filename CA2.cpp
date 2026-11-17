#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

class Part {
    int id;
    char name[30];
    char brand[30];
    float price;
    int qty;
    char grade;
    static int cnt;
public:
    Part();
    Part(int id, const char name[], const char brand[], float price, int qty, char grade);
    Part(const Part &p);
    ~Part();
    void setPriceQty(float price, int qty);
    void showData();
    int getId();
    static int getCount();
    static void setCount(int c);
    friend void showValue(Part &p);
};

int Part::cnt = 0;

Part::Part() {
    id = 0;
    strcpy(name, "none");
    strcpy(brand, "none");
    price = 0;
    qty = 0;
    grade = 'C';
    cout << "[Default constructor called]" << endl;
}

Part::Part(int id, const char name[], const char brand[], float price, int qty, char grade) {
    this->id = id;
    strcpy(this->name, name);
    strcpy(this->brand, brand);
    this->price = price;
    this->qty = qty;
    this->grade = grade;
    cout << "[Parameterized constructor called]" << endl;
}

Part::Part(const Part &p) {
    id = p.id;
    strcpy(name, p.name);
    strcpy(brand, p.brand);
    price = p.price;
    qty = p.qty;
    grade = p.grade;
    cout << "[Copy constructor called]" << endl;
}

Part::~Part() {
    cout << "[Destructor called for id " << id << "]" << endl;
}

void Part::setPriceQty(float price, int qty) {
    this->price = price;
    this->qty = qty;
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

void Part::setCount(int c) {
    cnt = c;
}

// friend can see private data of Part
void showValue(Part &p) {
    cout << "Part: " << p.name << " (" << p.brand << ")" << endl;
    cout << "Stock value = " << p.price * p.qty << endl;
}

const char fname[] = "./data/records.dat";

void takeInput(int &id, char name[], char brand[], float &price, int &qty, char &grade) {
    cout << "Enter part id: ";
    cin >> id;
    while (id <= 0) {
        cout << "Id must be positive, enter again: ";
        cin >> id;
    }
    cin.ignore();
    cout << "Enter part name: ";
    cin.getline(name, 30);
    cout << "Enter brand/model: ";
    cin.getline(brand, 30);
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

// file size divided by record size gives number of records
int countRecords() {
    ifstream file(fname, ios::binary);
    int size = 0;
    if (file) {
        file.seekg(0, ios::end);
        size = (int)file.tellg();
    }
    file.close();
    return size / sizeof(Part);
}

// returns record number (0,1,2..) or -1 if not found
int findPos(int id) {
    ifstream file(fname, ios::binary);
    Part temp;
    int pos = -1;
    int i = 0;
    while (file.read((char *)&temp, sizeof(temp))) {
        if (temp.getId() == id) {
            pos = i;
        }
        i++;
    }
    file.close();
    return pos;
}

void addPart() {
    int id, qty, pos;
    char name[30], brand[30], grade;
    float price;
    takeInput(id, name, brand, price, qty, grade);
    pos = findPos(id);
    if (pos != -1) {
        cout << "Error: this part id already exists" << endl;
    } else {
        Part p(id, name, brand, price, qty, grade);
        // opening file in binary mode, app adds at the end
        ofstream file(fname, ios::binary | ios::app);
        if (!file) {
            cout << "Cannot open file. Is the data folder created?" << endl;
        } else {
            file.write((char *)&p, sizeof(p));
            file.close();
            Part::setCount(Part::getCount() + 1);
            cout << "Part saved in file. Total parts = " << Part::getCount() << endl;
        }
    }
}

void displayAll() {
    ifstream file(fname, ios::binary);
    Part temp;
    if (!file || Part::getCount() == 0) {
        cout << "No records to show" << endl;
    } else {
        cout << "ID\tName\tBrand\tPrice\tQty\tGrade" << endl;
        while (file.read((char *)&temp, sizeof(temp))) {
            temp.showData();
        }
    }
    file.close();
}

void searchPart() {
    int id, pos;
    cout << "Enter id to search: ";
    cin >> id;
    pos = findPos(id);
    if (pos == -1) {
        cout << "Part not found" << endl;
    } else {
        ifstream file(fname, ios::binary);
        Part temp;
        // jump straight to the record: record number * size of one record
        file.seekg(pos * sizeof(Part), ios::beg);
        file.read((char *)&temp, sizeof(temp));
        cout << "ID\tName\tBrand\tPrice\tQty\tGrade" << endl;
        temp.showData();
        file.close();
    }
}

void updatePart() {
    int id, pos, qty;
    float price;
    cout << "Enter id to update: ";
    cin >> id;
    pos = findPos(id);
    if (pos == -1) {
        cout << "Part not found" << endl;
    } else {
        fstream file(fname, ios::binary | ios::in | ios::out);
        Part temp;
        file.seekg(pos * sizeof(Part), ios::beg);
        file.read((char *)&temp, sizeof(temp));
        Part old(temp);
        cout << "Enter new price: ";
        cin >> price;
        while (price <= 0) {
            cout << "Price must be more than 0, enter again: ";
            cin >> price;
        }
        cout << "Enter new quantity: ";
        cin >> qty;
        while (qty < 0) {
            cout << "Quantity cannot be negative, enter again: ";
            cin >> qty;
        }
        temp.setPriceQty(price, qty);
        // go back to the same record and overwrite it
        file.seekp(pos * sizeof(Part), ios::beg);
        file.write((char *)&temp, sizeof(temp));
        file.close();
        cout << "Old record:" << endl;
        old.showData();
        cout << "New record:" << endl;
        temp.showData();
    }
}

void stockValue() {
    int id, pos;
    cout << "Enter id: ";
    cin >> id;
    pos = findPos(id);
    if (pos == -1) {
        cout << "Part not found" << endl;
    } else {
        ifstream file(fname, ios::binary);
        Part temp;
        file.seekg(pos * sizeof(Part), ios::beg);
        file.read((char *)&temp, sizeof(temp));
        showValue(temp);
        file.close();
    }
}

int main() {
    int choice;
    Part::setCount(countRecords());
    cout << "Records already in file = " << Part::getCount() << endl;

    do {
        cout << "\n--- Repair Parts Ledger ---" << endl;
        cout << "1. Add part" << endl;
        cout << "2. Display all parts" << endl;
        cout << "3. Search part by id" << endl;
        cout << "4. Update price and quantity" << endl;
        cout << "5. Show stock value of a part" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addPart();
            break;
        case 2:
            displayAll();
            break;
        case 3:
            searchPart();
            break;
        case 4:
            updatePart();
            break;
        case 5:
            stockValue();
            break;
        case 6:
            cout << "Bye" << endl;
            break;
        default:
            cout << "Wrong choice, try again" << endl;
        }
    } while (choice != 6);

    return 0;
}
