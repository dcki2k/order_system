#include "myproject.h"
using namespace std;

#include "Table.h"

#include <algorithm>
#include <fstream>
// thư viện dùng lấy time trên hệ thống
#include <ctime> 
#include <chrono> 
// định nghĩa cho cấu trúc Sản phẩm
Product::Product(int id, string name, int price, string type)
		: _id(id), _name(name), _price(price), _type(type) {}
// định nghĩa cho cấu trúc ProductInBill
Product::Product() {}
ProductInBill::ProductInBill(int i, int a) 
	: _id(i), _amount(a) {}


float ProductInBill::getPrice(map<int, Product> p) const {
	return this->_amount * p[this->_id]._price;
}
Product ProductInBill::getProduct(map<int, Product> p) const {
	return p[this->_id];
}

bool ProductInBill::operator==(const ProductInBill& p) {
	return (this->_id == p._id);
}
// định nghĩa cho cấu trúc khách hàng

Customer::Customer(string n, string p, string a) 
	: _name(n), _phoneNumber(p), _address(a) {}
Customer::Customer(){}

void Customer::displayBill(map<int, Product> p) {
	if (this->_pBill.getSize() == 0) {
		cout << "Don hang cua ban trong. Hay them vao gio hang cua ban." << endl;
		return;
	}
	cout << "Don Hang"<<endl;
	this->_pBill.display(p);
	cout << endl;
	Prettytable t({ "Muc luc", "Chi tiet" });
	t.addRow({ "Khach hang" , this->_name});
	float total = this->_pBill.total(p);
	this->_pBill.setFee(total * 0.05);
	t.addRow({ "Phi giao hang" , to_string(this->_pBill.getFee())});
	t.addRow({ "Tong cong" , to_string(total + this->_pBill.getFee()) });
	t.addRow({ "Dia chi giao hang" , this->_address });
	t.addRow({ "So dien thoai" , this->_phoneNumber });
	t.addRow({ "Ngay dat hang" , this->_pBill.getDate() });
	Date d;
	t.addRow({ "Ngay giao hang du kien" , d.nextday(this->_pBill.getDate())});
	t.printTable();
	return;
}
void Customer::displayCart(map<int, Product> p) {
	cout << "Don hang";
	float total = this->_pBill.total(p);
	this->_pBill.setFee(total * 0.05);
	this->_pCart.display(p);
	cout << endl;
	cout << "Total: " << total + this->_pBill.getFee();
}

int Customer::buy() {
	string name, add, phone;
	if (this->_name == "Unknown") {
		cin.ignore();
		cout << "Ban can dien thong tin cua ban: " << endl;
		cout << ">> Name: ";
		getline(cin, name);
		cout << ">> Address: ";
		getline(cin, add);
		cout << ">> Phone number: ";
		getline(cin, phone);
		this->_address = add;
		this->_name = name;
		this->_phoneNumber = phone;
	}
	if (this->_pCart.getSize() == 0) {
		cout << "ERROR. Your cart is empty! " << endl;
		return 1;
	}
	// doan code tim thoi gian cua he thong
	string date;
	auto start = chrono::system_clock::now();
	auto legacyStart = chrono::system_clock::to_time_t(start);
	char tmBuff[30];
	ctime_s(tmBuff, sizeof(tmBuff), &legacyStart);
	date = tmBuff;
	date.pop_back();

	this->_pBill = this->_pCart.processPayment();
	this->_pBill.setDate(date);
	cout << "Thank you! <3" << endl;
	return 0;
}
// dinh nghia cho hoa don

void Bill::display(map<int, Product> p) {
	if (this->_listProducts.size() == 0) {
		cout << "Don hang cua ban trong. Hay them vao gio hang cua ban." << endl;
		return;
	}
	Prettytable table({ "ID", "Name", "Amount", "Price"});
	for (const auto& product : this->_listProducts) {
		int id = product._id;
		table.addRow({to_string(id), p[id]._name, to_string(product._amount),to_string(p[id]._price) + "/" + p[id]._type});
	}
	table.printTable();
	cout << endl;
	return;
}
float Bill::total(map<int, Product> p) const  {
	float sum = 0;
	for (auto ps : this->_listProducts) {
		sum += ps.getPrice(p);
	}
	return sum;
}
int Bill::getSize() const {
	return this->_listProducts.size();
}

void Bill::setFee(float fee) {
	this->_shipmentFee = fee;
}
float Bill::getFee() const {
	return this->_shipmentFee;
}
void Bill::setDate(string d) {
	this->date = d;
	return;
}
string Bill::getDate() const {
	return this->date;
}
// định nghĩa cho loại gio hang

void Cart::addToCart(map<int, Product> p) {
	int id, amt;
	cout << "---Them vao gio hang---" << endl;
	cout << "   >> ID san pham: ";
	cin >> id;
	cout << "   >> So luong: ";
	cin >> amt;
	if (p.find(id) == p.end()){
		cout << "Khong co " << id << " San pham trong cua hang cua chung toi. Vui long thu lai"<<endl;
		return;
	}
	this->_listProducts.push_back(ProductInBill(id, amt));
	cout << endl;
	cout << "Them vao gio hang thanh cong!" << endl;
}
void Cart::deleteFromCart() {
	int id;
	cout << "Nhap ID san pham ban muon xoa: ";
	cin >> id;
	ProductInBill ptemp(0, 0);
	for (const auto& p: this->_listProducts) {
		if (p._id == id) {
			ptemp = p;
		}
	}

	// dùng hàm remove của thư viện algorithm, xóa một sản phẩm ra khỏi danh sách.
	remove(this->_listProducts.begin(), this->_listProducts.end(), ptemp);
	cout << "Xoa thanh cong!" << endl;
	return;
}

Bill Cart::processPayment() const {
	return static_cast<Bill>(*this);
}

void Cart::display(map<int, Product> p) {
	Bill::display(p);
	cout << endl;
	float total = this->total(p);
	this->setFee(total * 0.05);
	cout << "Shipment fee (estimate): " << this->getFee() << endl;
	cout << "Total: " << total + this->getFee() << endl;
	return;
}


// định nghĩa cho lớp Cửa hàng:
void Store::getProductsFromFile(string filename) {
	fstream myfile(filename, ios::in);
	// nếu dữ liệu từ file không hợp lệ thì tạm dừng toàn bộ chương trình
	if (myfile.fail()) {
		cout << "Something wrong with data in file: " << filename << endl;
		// Hàm ngắt chương trình
		abort();
	}
	// lấy dữ liệu từ file
	while (!myfile.eof()) {
		string id, name, price, type;
		getline(myfile, id, ';');
		getline(myfile, name, ';');
		getline(myfile, price, ';');
		getline(myfile, type, ';');
		if (id != "\0") {
			// lưu vào danh sách
			this->_products[stoi(id)] = Product(stoi(id), name, stof(price), type);
		}
	}
	// read data
	myfile.close();
}
void Store::main() {
	string filename = "products.txt";
	Customer currCustomer;
	this->getProductsFromFile(filename);

	cout << "---Chao mung ban den voi cua hang cua chung toi---" << endl;
	int option, option2, isErr;
	while (true) {
		// main menu
		cout << "Ban dang tim kiem cai gi? " << endl;
		cout << ">> [1] Xem tat ca san pham" << endl;
		cout << ">> [2] Quan ly gio hang cua ban" << endl;
		cout << ">> [3] Xem don hang cuoi cung cua ban" << endl;

		cout << ">> Nhap su lua chon cua ban: ";
		cin >> option;
		cout << "**************************\n\n";
		switch (option) {
		case 1:
			this->display();
			cin.get();
			break;
		case 2:
			// sub menu
			cout << "---Gio hang cua ban--- " << endl;
			cout << ">> [1] Them mot san pham vao gio hang cua ban" << endl;
			cout << ">> [2] Xoa san pham khoi gio hang cua ban" << endl;
			cout << ">> [3] Xem" << endl;
			cout << ">> [4] Dat hang" << endl;
			cout << ">> [any] Thoat" << endl;
			cout << "Nhap su lua chon cua ban: ";
			cin >> option2;
			cout << "**************************\n\n";
			switch (option2) {
			case 1:
				while (true) {
					currCustomer._pCart.addToCart(this->_products);
					cout << endl;
					cout << "Do you want to continue? " << endl;
					cout << "[1] Yes" << endl;
					cout << "[0] No" << endl;
					int op;
					cin >> op;
					if (op == 0) {
						break;
					}
				}
				break;
			case 2:
				currCustomer._pCart.deleteFromCart();
				break;
			case 3:
				currCustomer._pCart.display(this->_products);
				break;
			case 4:
				isErr = currCustomer.buy();
				if (isErr) {
					cout << "Truoc tien ban can them mot so san pham vao gio hang cua minh." << endl;
					break;
				}
				currCustomer.displayBill(this->_products);
			default:
				break;
			}
			break;
		case 3:
			currCustomer.displayBill(this->_products);
		default:
			break;
		}

	}
	cin.get();
	return;
}

void Store::display() {
	cout << "----Danh sach san pham----" << endl;
	Prettytable table({ "ID", "Name", "Price" });
	for (auto pr : this->_products) {
		table.addRow({ to_string(pr.first), pr.second._name,to_string(pr.second._price) + "/" + pr.second._type });
	}
	table.printTable();
	return;
}






int Date::nhuan(int y) {
	return ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0);
}
int Date::songaytrongthang(int m, int y) {
	switch (m)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12: {
		return 31;
		break;
	}
	case 2: {
		if (nhuan(y)) {
			return 29;
		}
		return 28;
	}
	case 4:
	case 6:
	case 9:
	case 11: {
		return 30;
	}
	}
}

string Date::nextday(string systemTime) {
	map<string, int> month = {
		{"Jan", 1},
		{"Feb", 2},
		{"Mar", 3},
		{"Apr", 4},
		{"May", 5},
		{"Jun", 6},
		{"Jul", 7},
		{"Aug", 8},
		{"Sep", 9},
		{"Oct", 10},
		{"Nov", 11},
		{"Dec", 12},
	};
	int y = stoi(systemTime.substr(19, 5));
	int m = month[systemTime.substr(4, 3)];
	int d = stoi(systemTime.substr(8, 2));
	int ny = y, nm = m, nd = d;
	if (y > 0 && m > 0 && m < 13 && d > 0 && d <= (songaytrongthang(m, y))) {
		nd = d + 1;
		if (m != 12 && d == songaytrongthang(m, y)) {
			nd = 1;
			nm = m + 1;
		}
		else if (m == 12 && d == songaytrongthang(m, y)) {
			nd = 1;
			ny = y + 1;
			nm = 1;
		}
		else if (m == 2) {
			if (nhuan(y)) {
				if (d == 29) {
					nd = 1;
					nm = m + 1;
				}
			}
			else {
				if (d == 28) {
					nd = 1;
					nm = m + 1;
				}
			}
		}
	}
	return to_string(nd) + "/" + to_string(nm) + "/" + to_string(ny);
}