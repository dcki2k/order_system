#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;


struct Product {
	// thông tin sản phẩm
	int _id;
	string _name;
	int _price;
	string _type;
	// các hàm khởi tạo
	Product(int id, string name, int price, string type);
	Product();
};

struct ProductInBill {
	int _id;
	int _amount;


	ProductInBill(int i, int a);
	// trả về giá của một mặt hàng có id = _id;
	float getPrice(map<int, Product> p) const;
	// trả về thông tin của mặt hàng có id = _id;
	Product getProduct(map<int, Product> p) const;
	// overload toán tử == để sử dụng các hàm trong thư viện STL
	bool operator==(const ProductInBill& p);
};


class Bill {
protected:
	// lưu danh sách các sản phẩm trong hóa đơn
	vector<ProductInBill> _listProducts;
	string date = "Unknown";
private:
	float _shipmentFee = 0;
public:
	// hàm in hóa đơn ra màn hình
	virtual void display (map<int, Product> p);
	// trả về tổng tiền mặt hàng (chưa tính tiền ship)
	float total(map<int, Product> p) const;
	// trả về số lượng mặt hàng trong bill
	int getSize() const;
	// định giá ship cho hóa đơn, ở đây để dễ dàng thì lấy total*0.05
	void setFee(float fee);
	// trả về phí ship
	float getFee() const;
	// đặt và trả về thời gian đặt hàng
	void setDate(string s);
	string getDate() const;
};

class Cart : public Bill {
public:
	// thêm mới một sản phẩm từ danh sách p vào giỏ hàng
	void addToCart(map<int, Product> p);
	// xóa một mặt hàng khỏi giỏ hàng
	void deleteFromCart();
	// hiện mặt hàng có trong giỏ hàng
	void display(map<int, Product> p) override;
	// chuyển giỏ hàng thành bill (tức là người dùng đã xác nhận mua hàng ở trong giỏ hàng)
	Bill processPayment() const;
};

struct Customer {
	string _name = "Unknown";
	string _phoneNumber = "Unknown";
	string _address = "Unknown";
	// giỏ hàng
	Cart _pCart;
	// hóa đơn
	Bill _pBill;


	Customer(string n, string p, string a);
	Customer();
	// người dùng xác nhận mua hàng thì cần có thêm thông tin cá nhân
	// nếu giỏ hàng trống thì in ra thông báo và trả về 1, đơn hàng chưa được thực thi. Ok thì trả về 0
	int buy();
	// in giỏ hàng và tổng tiền tạm thời
	void displayCart(map<int, Product> p);
	// in hóa đơn và tiền cần phải thanh toán
	void displayBill(map<int, Product> p);
};

class Store {
private:
	// lưu danh sách tất cả sản phẩm trong cửa hàng
	map<int, Product> _products;
public:
	// lấy thông tin từ file
	/*
	Định dạng file yêu cầu:

	
	mỗi dòng sẽ chứa thông tin một sản phẩm, mỗi thuộc tính ngăn cách bởi dấu ';', cuối dòng buộc phải có dấu ';'

	định dạng: id;name;price;type

	vd: 1010;Nuoc Giat;150000;Tui;
	*/
	void getProductsFromFile(string fileName);

	// quản lý các hoạt động trong cửa hàng
	void main();
	// in danh sách sản phẩm trong cửa hàng
	void display();
};

class Date {
private:
	int nhuan(int y);
	int songaytrongthang(int m, int y);
public:
	string nextday(string systemTime);
};