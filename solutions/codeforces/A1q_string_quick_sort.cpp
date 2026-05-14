#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

int charAt(const string& value, int depth) {
    if (depth >= static_cast<int>(value.size())) {
        return -1;
    }
    return static_cast<unsigned char>(value[depth]);
}

void stringQuickSort(vector<string>& values, int left, int right, int depth) {
    if (right <= left) {
        return;
    }

    const int pivot = charAt(values[left], depth);
    int less = left;
    int greater = right;
    int i = left + 1;

    while (i <= greater) {
        const int current = charAt(values[i], depth);
        if (current < pivot) {
            swap(values[less++], values[i++]);
        } else if (current > pivot) {
            swap(values[i], values[greater--]);
        } else {
            ++i;
        }
    }

    stringQuickSort(values, left, less - 1, depth);
    if (pivot >= 0) {
        stringQuickSort(values, less, greater, depth + 1);
    }
    stringQuickSort(values, greater + 1, right, depth);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 0;
    if (!(cin >> n)) {
        return 0;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<string> values(n);
    for (int i = 0; i < n; ++i) {
        getline(cin, values[i]);
        if (!values[i].empty() && values[i].back() == '\r') {
            values[i].pop_back();
        }
    }

    if (!values.empty()) {
        stringQuickSort(values, 0, static_cast<int>(values.size()) - 1, 0);
    }

    for (const string& value : values) {
        cout << value << '\n';
    }

    return 0;
}
