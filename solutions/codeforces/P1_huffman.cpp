#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Node {
    long long frequency = 0;
    int order = 0;
    int symbol = -1;
    int left = -1;
    int right = -1;
};

struct ByFrequency {
    const vector<Node>* nodes = nullptr;

    bool operator()(int lhs, int rhs) const {
        const Node& a = (*nodes)[lhs];
        const Node& b = (*nodes)[rhs];
        if (a.frequency != b.frequency) {
            return a.frequency > b.frequency;
        }
        return a.order > b.order;
    }
};

void buildCodes(const vector<Node>& nodes, int vertex, string current, array<string, 26>& codes) {
    if (nodes[vertex].symbol != -1) {
        if (current.empty()) {
            current = "0";
        }
        codes[nodes[vertex].symbol - 'a'] = current;
        return;
    }

    buildCodes(nodes, nodes[vertex].left, current + '0', codes);
    buildCodes(nodes, nodes[vertex].right, current + '1', codes);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    array<long long, 26> frequency{};
    for (char ch : s) {
        ++frequency[ch - 'a'];
    }

    vector<Node> nodes;
    nodes.reserve(2 * 26);
    priority_queue<int, vector<int>, ByFrequency> heap((ByFrequency{&nodes}));

    int order = 0;
    for (int i = 0; i < 26; ++i) {
        if (frequency[i] == 0) {
            continue;
        }
        nodes.push_back(Node{frequency[i], order++, static_cast<int>('a' + i), -1, -1});
        heap.push(static_cast<int>(nodes.size()) - 1);
    }

    while (heap.size() > 1) {
        const int left = heap.top();
        heap.pop();
        const int right = heap.top();
        heap.pop();
        nodes.push_back(Node{
            nodes[left].frequency + nodes[right].frequency,
            order++,
            -1,
            left,
            right,
        });
        heap.push(static_cast<int>(nodes.size()) - 1);
    }

    array<string, 26> codes{};
    buildCodes(nodes, heap.top(), "", codes);

    long long encodedSize = 0;
    int distinct = 0;
    for (int i = 0; i < 26; ++i) {
        if (frequency[i] != 0) {
            ++distinct;
            encodedSize += frequency[i] * static_cast<long long>(codes[i].size());
        }
    }

    cout << distinct << ' ' << encodedSize << '\n';
    for (int i = 0; i < 26; ++i) {
        if (frequency[i] != 0) {
            cout << static_cast<char>('a' + i) << ": " << codes[i] << '\n';
        }
    }
    for (char ch : s) {
        cout << codes[ch - 'a'];
    }
    cout << '\n';

    return 0;
}
