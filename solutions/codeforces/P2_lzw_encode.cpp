#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);
    if (!s.empty() && s.back() == '\r') {
        s.pop_back();
    }

    unordered_map<string, int> code;
    code.reserve(20000);
    for (int i = 0; i < 128; ++i) {
        code[string(1, static_cast<char>(i))] = i;
    }

    int nextCode = 128;
    string current;
    vector<int> result;
    result.reserve(s.size());

    for (char ch : s) {
        string extended = current + ch;
        if (code.find(extended) != code.end()) {
            current = extended;
        } else {
            result.push_back(code[current]);
            code[extended] = nextCode++;
            current = string(1, ch);
        }
    }
    if (!current.empty()) {
        result.push_back(code[current]);
    }

    cout << result.size() << '\n';
    for (size_t i = 0; i < result.size(); ++i) {
        if (i) {
            cout << ' ';
        }
        cout << result[i];
    }
    cout << '\n';

    return 0;
}
