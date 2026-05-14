#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

int lcpCompare(const string& lhs, const string& rhs, size_t knownLcp = 0) {
    size_t i = knownLcp;
    const size_t limit = min(lhs.size(), rhs.size());
    while (i < limit && lhs[i] == rhs[i]) {
        ++i;
    }

    if (i == lhs.size() && i == rhs.size()) {
        return 0;
    }
    if (i == lhs.size()) {
        return -1;
    }
    if (i == rhs.size()) {
        return 1;
    }
    return static_cast<unsigned char>(lhs[i]) < static_cast<unsigned char>(rhs[i]) ? -1 : 1;
}

void mergeSort(vector<string>& values, vector<string>& buffer, int left, int right) {
    if (right - left <= 1) {
        return;
    }

    const int middle = left + (right - left) / 2;
    mergeSort(values, buffer, left, middle);
    mergeSort(values, buffer, middle, right);

    int i = left;
    int j = middle;
    int pos = left;

    while (i < middle && j < right) {
        if (lcpCompare(values[i], values[j]) <= 0) {
            buffer[pos++] = values[i++];
        } else {
            buffer[pos++] = values[j++];
        }
    }
    while (i < middle) {
        buffer[pos++] = values[i++];
    }
    while (j < right) {
        buffer[pos++] = values[j++];
    }

    for (int k = left; k < right; ++k) {
        values[k].swap(buffer[k]);
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

    vector<string> buffer(n);
    mergeSort(values, buffer, 0, n);

    for (const string& value : values) {
        cout << value << '\n';
    }

    return 0;
}
