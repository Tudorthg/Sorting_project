#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm> // for std::swap
#include <string>

using namespace std;
using namespace std::chrono;

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}


void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        swap(arr[min_idx], arr[i]);
    }
}
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
        }
    }
}

void logToCSV(string alg, int size, long long ms) {
    ofstream file("benchmark_results.csv", ios::app);//appends the results to already writen ones
    file.seekp(0, ios::end);
    if (file.tellp() == 0) file << "Algorithm,Elements,Time_MS\n";
    file << alg << "," << size << "," << ms << "\n";
    file.close();
}

int main() {
    ifstream f("numbers.txt");
    if (!f) { cerr << "File numbers.txt not found!"; return 1; }

    vector<int> data;
    int val;
    while (f >> val) data.push_back(val);
    f.close();

    int n = data.size();
    cout << "Loaded " << n << " elements.\n\n";

    vector<int> vMerge = data;
    auto start = high_resolution_clock::now();
    mergeSort(vMerge, 0, n - 1);
    auto end = high_resolution_clock::now();
    long long tMerge = duration_cast<milliseconds>(end - start).count();
    cout << "Merge Sort: " << tMerge << " ms\n";
    logToCSV("MergeSort", n, tMerge);

    vector<int> vSelect = data;
    start = high_resolution_clock::now();
    selectionSort(vSelect);
    end = high_resolution_clock::now();
    long long tSelect = duration_cast<milliseconds>(end - start).count();
    cout << "Selection Sort: " << tSelect << " ms\n";
    logToCSV("SelectionSort", n, tSelect);


    vector<int> vBubble = data;
    start = high_resolution_clock::now();
    bubbleSort(vBubble);
    end = high_resolution_clock::now();
    long long tBubble = duration_cast<milliseconds>(end - start).count();
    cout << "Bubble Sort: " << tBubble << " ms\n";
    logToCSV("BubbleSort", n, tBubble);

    cout << "\nResults appended to benchmark_results.csv" << endl;
    return 0;
}