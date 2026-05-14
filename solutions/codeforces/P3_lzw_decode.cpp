#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k = 0;
    cin >> k;

    vector<int> codes(k);
    for (int i = 0; i < k; ++i) {
        cin >> codes[i];
    }

    vector<string> dictionary;
    dictionary.reserve(20000);
    for (int i = 0; i < 128; ++i) {
        dictionary.push_back(string(1, static_cast<char>(i)));
    }

    string previous = dictionary[codes[0]];
    string answer = previous;

    for (int i = 1; i < k; ++i) {
        string entry;
        if (codes[i] < static_cast<int>(dictionary.size())) {
            entry = dictionary[codes[i]];
        } else {
            entry = previous + previous.front();
        }

        answer += entry;
        dictionary.push_back(previous + entry.front());
        previous = entry;
    }

    cout << answer << '\n';

    return 0;
}
