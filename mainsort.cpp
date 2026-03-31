#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
using namespace std;

void selectionSort(vector<int> &v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = i + 1; j < v.size(); j++) {
            if (v[j] < v[i]) {
                swap(v[i], v[j]);
            }
        }
    }
}
void bubblesort(vector<int>& v) {
    int n = static_cast<int>(v.size());
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]);
            }
        }
    }
}

int main() {
    ifstream f("numbers.txt");
    int x;
    vector<int> v;

    while (f >> x) {
        v.push_back(x);
    }
    f.close();

    int n = static_cast<int>(v.size());
    auto start = chrono::high_resolution_clock::now();
    selectionSort(v);
    //bubblesort(v);
    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    ofstream timeFile("sort_time.txt");
    timeFile << "Sort execution time: " << duration.count() << " ms" << endl;
    timeFile.close();

    ofstream g("numbers.txt");
    if (!g) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }

    for (int i = 0; i < n; i++) {
        g << v[i] <<" ";
    }
    g.close();

    return 0;
}
