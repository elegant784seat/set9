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

void msdRadixQuickSort(vector<string>& values, vector<string>& buffer, int left, int right, int depth) {
    if (right - left <= 1) {
        return;
    }
    if (right - left < 74) {
        stringQuickSort(values, left, right - 1, depth);
        return;
    }

    constexpr int alphabet = 256;
    vector<int> frequency(alphabet + 1, 0);
    for (int i = left; i < right; ++i) {
        const int key = charAt(values[i], depth) + 1;
        ++frequency[key];
    }

    vector<int> start(alphabet + 2, 0);
    for (int key = 0; key <= alphabet; ++key) {
        start[key + 1] = start[key] + frequency[key];
    }

    vector<int> next = start;
    for (int i = left; i < right; ++i) {
        const int key = charAt(values[i], depth) + 1;
        buffer[next[key]++] = values[i];
    }

    for (int i = 0; i < right - left; ++i) {
        values[left + i].swap(buffer[i]);
    }

    for (int key = 1; key <= alphabet; ++key) {
        const int from = left + start[key];
        const int to = from + frequency[key];
        if (to - from > 1) {
            msdRadixQuickSort(values, buffer, from, to, depth + 1);
        }
    }
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

    vector<string> buffer(max(1, n));
    msdRadixQuickSort(values, buffer, 0, n, 0);

    for (const string& value : values) {
        cout << value << '\n';
    }

    return 0;
}
