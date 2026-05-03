#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <string>
#include <iomanip>
#include <filesystem>

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

// ─────────────────────────────────────────────
//  ALGORITMI DE SORTARE
// ─────────────────────────────────────────────

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else               arr[k++] = R[j++];
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
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx]) min_idx = j;
        swap(arr[min_idx], arr[i]);
    }
}

void bubbleSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
}

// ─────────────────────────────────────────────
//  UTILITARE
// ─────────────────────────────────────────────

vector<int> readNumbers(const string& filepath) {
    vector<int> data;
    ifstream f(filepath);
    if (!f) return data;
    int val;
    while (f >> val) data.push_back(val);
    return data;
}

void writeSorted(const string& outFile, const string& srcName,
                 const string& algorithm, const vector<int>& sorted) {
    ofstream f(outFile, ios::app);
    f << "# Fisier: " << srcName
      << " | Algoritm: " << algorithm
      << " | Elemente: " << sorted.size() << "\n";
    for (size_t i = 0; i < sorted.size(); i++) {
        f << sorted[i];
        if (i + 1 < sorted.size()) f << " ";
    }
    f << "\n\n";
}

void logCSV(const string& csvFile, bool writeHeader,
            const string& srcName, const string& algorithm,
            int elements, long long microseconds) {
    ofstream f(csvFile, ios::app);
    if (writeHeader)
        f << "Fisier,Algoritm,Elemente,Timp_us,Timp_ms\n";
    f << srcName << ","
      << algorithm << ","
      << elements << ","
      << microseconds << ","
      << fixed << setprecision(3) << microseconds / 1000.0 << "\n";
}

void runAndLog(const string& algName,
               vector<int> data,
               const string& srcName,
               const string& sortedFile,
               const string& csvFile,
               bool csvHeader) {

    auto start = high_resolution_clock::now();

    if      (algName == "MergeSort")     mergeSort(data, 0, (int)data.size() - 1);
    else if (algName == "SelectionSort") selectionSort(data);
    else if (algName == "BubbleSort")    bubbleSort(data);

    auto end = high_resolution_clock::now();
    long long us = duration_cast<microseconds>(end - start).count();

    cout << "    " << left << setw(15) << algName
         << "-> " << setw(10) << us << " us"
         << "  (" << fixed << setprecision(3) << us / 1000.0 << " ms)\n";

    writeSorted(sortedFile, srcName, algName, data);
    logCSV(csvFile, csvHeader, srcName, algName, (int)data.size(), us);
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────

int main(int argc, char* argv[]) {

    // Folderul de input: implicit "numbers", sau dat ca argument
    // Exemplu: ./mainsort               -> citeste din ./numbers/
    //          ./mainsort date_mari      -> citeste din ./date_mari/
    string inputFolder = (argc > 1) ? argv[1] : "numbers";

    const string SORTED_FILE   = "sorted_numbers.txt";
    const string BENCHMARK_CSV = "benchmark.csv";

    if (!fs::exists(inputFolder) || !fs::is_directory(inputFolder)) {
        cerr << "[EROARE] Folderul '" << inputFolder << "' nu exista!\n";
        cerr << "Creaza un folder 'numbers/' si pune fisiere .txt cu numere in el.\n";
        return 1;
    }

    // Colectează toate fișierele .txt din folder
    vector<string> files;
    for (const auto& entry : fs::directory_iterator(inputFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
            files.push_back(entry.path().string());
    }

    if (files.empty()) {
        cerr << "[EROARE] Niciun fisier .txt gasit in '" << inputFolder << "'!\n";
        return 1;
    }

    sort(files.begin(), files.end());

    remove(SORTED_FILE.c_str());
    remove(BENCHMARK_CSV.c_str());

    cout << "================================================\n";
    cout << "   SORTING BENCHMARK  -  3 Algoritmi\n";
    cout << "================================================\n";
    cout << "  Folder : " << inputFolder << "\n";
    cout << "  Fisiere: " << files.size() << "\n\n";

    bool firstCSV = true;

    for (const auto& filepath : files) {
        string srcName = fs::path(filepath).filename().string();
        vector<int> data = readNumbers(filepath);

        if (data.empty()) {
            cout << "  [SKIP] " << srcName << " - fisier gol sau invalid.\n\n";
            continue;
        }

        cout << "[ " << srcName << " ]  " << data.size() << " elemente\n";

        runAndLog("MergeSort",     data, srcName, SORTED_FILE, BENCHMARK_CSV, firstCSV);
        firstCSV = false;
        runAndLog("SelectionSort", data, srcName, SORTED_FILE, BENCHMARK_CSV, false);
        runAndLog("BubbleSort",    data, srcName, SORTED_FILE, BENCHMARK_CSV, false);

        cout << "\n";
    }

    cout << "================================================\n";
    cout << "  Rezultate sortate -> " << SORTED_FILE  << "\n";
    cout << "  Benchmark         -> " << BENCHMARK_CSV << "\n";

    return 0;
}
