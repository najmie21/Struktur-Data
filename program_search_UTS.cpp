#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <algorithm>

using namespace std;

struct Paper {
    string judul;
    int tahun;
    string penulis;
};

// Hapus tanda kutip di awal dan akhir string
string removeQuotes(string text) {
    if (text.length() >= 2 && text[0] == '"' && text[text.length() - 1] == '"') {
        text = text.substr(1, text.length() - 2);
    }
    return text;
}

bool isAllDigit(const string& str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// Fungsi membaca CSV dengan validasi dan perbaikan tahun
vector<Paper> loadCSV(const string& filename) {
    vector<Paper> data;
    ifstream file(filename.c_str());
    string line;

    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ',')) {
            fields.push_back(removeQuotes(field));
        }

        if (fields.size() >= 8) {
            string judul = fields[5];
            string tahunStr = fields[6];
            string penulis = fields[7];

            // Hapus spasi & .0 dari tahun
            tahunStr.erase(remove(tahunStr.begin(), tahunStr.end(), ' '), tahunStr.end());
            size_t dotPos = tahunStr.find(".0");
            if (dotPos != string::npos) {
                tahunStr = tahunStr.substr(0, dotPos);
            }

            if (!judul.empty() && !tahunStr.empty() && !penulis.empty() && isAllDigit(tahunStr)) {
                Paper p;
                p.judul = judul;
                p.tahun = atoi(tahunStr.c_str()); // Menggunakan atoi
                p.penulis = penulis;
                data.push_back(p);
            }
        }
    }

    return data;
}

// Tampilkan data paper
void tampilkanPaper(const Paper& p) {
    cout << "Judul  : " << p.judul << endl;
    cout << "Tahun  : " << p.tahun << endl;
    cout << "Penulis: " << p.penulis << endl << endl;
}

// Linear Search
void linearSearch(const vector<Paper>& data, const string& key, const string& field) {
    bool found = false;
    for (size_t i = 0; i < data.size(); i++) {
        if ((field == "judul" && data[i].judul.find(key) != string::npos) ||
            (field == "penulis" && data[i].penulis.find(key) != string::npos) ||
            (field == "tahun" && atoi(key.c_str()) == data[i].tahun)) {
            tampilkanPaper(data[i]);
            found = true;
        }
    }
    if (!found) {
        cout << "Data tidak ditemukan." << endl;
    }
}

// Sorting manual berdasarkan field
void sortByField(vector<Paper>& data, const string& field) {
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = i + 1; j < data.size(); j++) {
            bool swap = false;
            if (field == "judul" && data[i].judul > data[j].judul) swap = true;
            if (field == "penulis" && data[i].penulis > data[j].penulis) swap = true;
            if (field == "tahun" && data[i].tahun > data[j].tahun) swap = true;

            if (swap) {
                std::swap(data[i], data[j]);
            }
        }
    }
}

// Function to convert integer to string using stringstream
string intToString(int value) {
    stringstream ss;
    ss << value;
    return ss.str();
}

// Binary Search
void binarySearch(vector<Paper> data, const string& key, const string& field) {
    sortByField(data, field);

    int left = 0, right = data.size() - 1;
    bool found = false;

    while (left <= right) {
        int mid = (left + right) / 2;
        string val;

        if (field == "judul") val = data[mid].judul;
        else if (field == "penulis") val = data[mid].penulis;
        else if (field == "tahun") val = intToString(data[mid].tahun);

        // Pencocokan substring untuk judul dan penulis
        if ((field == "judul" && val.find(key) != string::npos) ||
            (field == "penulis" && val.find(key) != string::npos) ||
            (field == "tahun" && val == key)) {
            tampilkanPaper(data[mid]);
            found = true;
            break;
        }

        // Untuk tahun, kita tidak bisa menggunakan substring, jadi kita harus membandingkan nilai
        if ((field == "judul" && val > key) ||
            (field == "penulis" && val > key) ||
            (field == "tahun" && val > key)) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    if (!found) {
        cout << "Data tidak ditemukan." << endl;
    }
}

// Main menu
int main() {
    string filename = "Dataset_Paper.csv";
    vector<Paper> data = loadCSV(filename);

    if (data.empty()) {
        cout << "Gagal membaca data atau file kosong." << endl;
        return 1;
    }

    int pilihan;
    do {
        cout << "======== MENU PENCARIAN ========" << endl;
        cout << "1. Linear Search" << endl;
        cout << "2. Binary Search" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilihan Anda: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1 || pilihan == 2) {
            string field, key;
            cout << "Cari berdasarkan (judul / penulis / tahun): ";
            getline(cin, field);
            cout << "Masukkan kata kunci: ";
            getline(cin, key);

            cout << "\n========== HASIL PENCARIAN ==========" << endl;
            if (pilihan == 1) {
                linearSearch(data, key, field);
            } else {
                binarySearch(data, key, field);
            }
            cout << "=====================================\n" << endl;

        } else if (pilihan == 3) {
            cout << "Keluar dari program." << endl;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }

    } while (pilihan != 3);

    return 0;
}
