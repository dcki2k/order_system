#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
class Prettytable {
private:
    // tên cửa hàng:
    vector<string> m_head;
    // lưu trữ các giá trị trong bảng:
    vector<vector<string>> m_row;
    // lưu trữ độ dài tối đa của mỗi hàng, mỗi cột. Được sử dụng khi in ra bảng.
    vector<int> sizeCol;
    // chieu dai cua bang
    int length = 0;
    //tìm chiều dài:
    inline void findSize() {
        int size = sizeCol.size();
        for (int r = 0; r < size; r++) { // cot
            length += sizeCol[r];
        }
        length = length + 6 * m_head.size() + (m_head.size() - 2);
    }
    // in một số dòng, trang trí bảng.
    inline void line(int notEnd = 0) {
        cout << "+";
        for (int i = 0; i < length; i++) {
            cout << "-";
        }
        cout << "+";
        cout << endl;
        // nếu nó là cuối bảng,không cần in ra"|" "
        if (!notEnd)
            cout << "|";
        return;
    }
public:
    Prettytable(vector<string> yourHeader) {
        // init tên cửa hàng:
        for (auto&& i : yourHeader) {
            m_head.push_back(i);
            sizeCol.push_back(i.length());
        }
    }
    // trừ khi row.size () == m_head.size (), bảng sẽ không được in ra.
    void addRow(vector<string> row) {
        m_row.push_back(row);
        int size = row.size();
        for (int i = 0; i < size; i++) {
            int newSize = row[i].length();
            if (newSize > sizeCol[i]) {
                sizeCol[i] = newSize;
            }
        }
    }
    inline void printTable() {
        findSize();
        line();
        // in đầu
        for (int i = 0; i < m_head.size(); i++) {
            cout << "  " << left << setw(sizeCol[i] + 3) << m_head[i] << "| ";
            // căn chỉnh 6 khoảng trắng cho mỗi cột, | <3> giá trị <3> |. Chỉ cần đặt nó ở giữa
        }
        cout << endl;
        //giá trị in:
        for (int m = 0; m < m_row.size(); m++) {
            line();
            for (int i = 0; i < m_head.size(); i++) {
                cout << "  " << left << setw(sizeCol[i] + 3) << m_row[m][i] << "| ";
            }
            cout << endl;
        }
        line(1);
        cout << endl;
        //đặt lại độ dài trong trường hợp gọi hàm này hai lần:
        length = 0;
        return;
    }
    
};
ostream& operator<<(ostream& stream, Prettytable& table) {
    table.printTable();
    return stream;
}

