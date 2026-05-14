#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;

struct Metrics {
    long long charOps = 0;
};

class StringGenerator {
public:
    explicit StringGenerator(uint32_t seed = 20260511) : rng_(seed) {}

    vector<string> randomArray(int n) {
        vector<string> result;
        result.reserve(n);
        uniform_int_distribution<int> lengthDist(10, 200);
        uniform_int_distribution<int> charDist(0, static_cast<int>(alphabet_.size()) - 1);

        for (int i = 0; i < n; ++i) {
            const int length = lengthDist(rng_);
            string value;
            value.reserve(length);
            for (int j = 0; j < length; ++j) {
                value.push_back(alphabet_[charDist(rng_)]);
            }
            result.push_back(value);
        }
        return result;
    }

    vector<string> reverseSortedArray(int n) {
        vector<string> result = randomArray(n);
        sort(result.begin(), result.end());
        reverse(result.begin(), result.end());
        return result;
    }

    vector<string> almostSortedArray(int n) {
        vector<string> result = randomArray(n);
        sort(result.begin(), result.end());
        if (n < 2) {
            return result;
        }

        uniform_int_distribution<int> indexDist(0, n - 1);
        const int swaps = max(1, n / 100);
        for (int i = 0; i < swaps; ++i) {
            swap(result[indexDist(rng_)], result[indexDist(rng_)]);
        }
        return result;
    }

    vector<string> commonPrefixArray(int n, const string& prefix = "AAAAAAAAAAAAAAAAAAAA") {
        vector<string> result = randomArray(n);
        for (string& value : result) {
            value = prefix + value;
        }
        return result;
    }

private:
    mt19937 rng_;
    const string alphabet_ =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#%:;^&*()-.";
};

class StringSortTester {
public:
    static int compareStrings(const string& lhs, const string& rhs, Metrics& metrics, size_t start = 0) {
        size_t i = start;
        const size_t limit = min(lhs.size(), rhs.size());
        while (i < limit) {
            ++metrics.charOps;
            if (lhs[i] < rhs[i]) {
                return -1;
            }
            if (lhs[i] > rhs[i]) {
                return 1;
            }
            ++i;
        }

        if (lhs.size() == rhs.size()) {
            return 0;
        }
        ++metrics.charOps;
        return lhs.size() < rhs.size() ? -1 : 1;
    }

    static void standardQuickSort(vector<string>& values, int left, int right, Metrics& metrics) {
        if (right <= left) {
            return;
        }

        const string pivot = values[left];
        int less = left;
        int greater = right;
        int i = left + 1;

        while (i <= greater) {
            const int cmp = compareStrings(values[i], pivot, metrics);
            if (cmp < 0) {
                swap(values[less++], values[i++]);
            } else if (cmp > 0) {
                swap(values[i], values[greater--]);
            } else {
                ++i;
            }
        }

        standardQuickSort(values, left, less - 1, metrics);
        standardQuickSort(values, greater + 1, right, metrics);
    }

    static void standardMergeSort(vector<string>& values, vector<string>& buffer, int left, int right, Metrics& metrics) {
        if (right - left <= 1) {
            return;
        }

        const int middle = left + (right - left) / 2;
        standardMergeSort(values, buffer, left, middle, metrics);
        standardMergeSort(values, buffer, middle, right, metrics);

        int i = left;
        int j = middle;
        int pos = left;
        while (i < middle && j < right) {
            if (compareStrings(values[i], values[j], metrics) <= 0) {
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

    static void stringMergeSort(vector<string>& values, vector<string>& buffer, int left, int right, Metrics& metrics) {
        if (right - left <= 1) {
            return;
        }

        const int middle = left + (right - left) / 2;
        stringMergeSort(values, buffer, left, middle, metrics);
        stringMergeSort(values, buffer, middle, right, metrics);

        int i = left;
        int j = middle;
        int pos = left;
        while (i < middle && j < right) {
            const size_t knownLcp = commonPrefix(values[i], values[j], metrics);
            if (compareStrings(values[i], values[j], metrics, knownLcp) <= 0) {
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

    static void stringQuickSort(vector<string>& values, int left, int right, int depth, Metrics& metrics) {
        if (right <= left) {
            return;
        }

        const int pivot = charAt(values[left], depth, metrics);
        int less = left;
        int greater = right;
        int i = left + 1;

        while (i <= greater) {
            const int current = charAt(values[i], depth, metrics);
            if (current < pivot) {
                swap(values[less++], values[i++]);
            } else if (current > pivot) {
                swap(values[i], values[greater--]);
            } else {
                ++i;
            }
        }

        stringQuickSort(values, left, less - 1, depth, metrics);
        if (pivot >= 0) {
            stringQuickSort(values, less, greater, depth + 1, metrics);
        }
        stringQuickSort(values, greater + 1, right, depth, metrics);
    }

    static void msdRadixSort(vector<string>& values, vector<string>& buffer, int left, int right, int depth, Metrics& metrics, bool switchToQuick) {
        if (right - left <= 1) {
            return;
        }
        if (switchToQuick && right - left < 74) {
            stringQuickSort(values, left, right - 1, depth, metrics);
            return;
        }

        constexpr int alphabet = 256;
        vector<int> frequency(alphabet + 1, 0);
        for (int i = left; i < right; ++i) {
            ++frequency[charAt(values[i], depth, metrics) + 1];
        }

        vector<int> start(alphabet + 2, 0);
        for (int key = 0; key <= alphabet; ++key) {
            start[key + 1] = start[key] + frequency[key];
        }

        vector<int> next = start;
        for (int i = left; i < right; ++i) {
            const int key = charAt(values[i], depth, metrics) + 1;
            buffer[next[key]++] = values[i];
        }
        for (int i = 0; i < right - left; ++i) {
            values[left + i].swap(buffer[i]);
        }

        for (int key = 1; key <= alphabet; ++key) {
            const int from = left + start[key];
            const int to = from + frequency[key];
            msdRadixSort(values, buffer, from, to, depth + 1, metrics, switchToQuick);
        }
    }

private:
    static int charAt(const string& value, int depth, Metrics& metrics) {
        ++metrics.charOps;
        if (depth >= static_cast<int>(value.size())) {
            return -1;
        }
        return static_cast<unsigned char>(value[depth]);
    }

    static size_t commonPrefix(const string& lhs, const string& rhs, Metrics& metrics) {
        size_t i = 0;
        const size_t limit = min(lhs.size(), rhs.size());
        while (i < limit) {
            ++metrics.charOps;
            if (lhs[i] != rhs[i]) {
                break;
            }
            ++i;
        }
        return i;
    }
};

struct Measurement {
    long long micros = 0;
    long long charOps = 0;
    bool sorted = false;
};

template <class Runner>
Measurement measure(const vector<string>& input, int repetitions, Runner runner) {
    long long totalMicros = 0;
    long long totalCharOps = 0;
    bool allSorted = true;

    for (int rep = 0; rep < repetitions; ++rep) {
        vector<string> values = input;
        Metrics metrics;
        const auto start = chrono::steady_clock::now();
        runner(values, metrics);
        const auto finish = chrono::steady_clock::now();

        totalMicros += chrono::duration_cast<chrono::microseconds>(finish - start).count();
        totalCharOps += metrics.charOps;
        allSorted = allSorted && is_sorted(values.begin(), values.end());
    }

    return Measurement{totalMicros / repetitions, totalCharOps / repetitions, allSorted};
}

int main(int argc, char** argv) {
    const bool quickMode = argc > 1 && string(argv[1]) == "--quick";
    const int maxN = quickMode ? 1000 : 3000;
    const int step = quickMode ? 300 : 100;
    const int repetitions = quickMode ? 2 : 5;

    StringGenerator generator;
    vector<pair<string, vector<string>>> datasets = {
        {"random", generator.randomArray(maxN)},
        {"reverse_sorted", generator.reverseSortedArray(maxN)},
        {"almost_sorted", generator.almostSortedArray(maxN)},
        {"common_prefix", generator.commonPrefixArray(maxN)},
    };

    cout << "dataset,size,algorithm,time_us,char_ops,sorted\n";
    for (const auto& dataset : datasets) {
        const string& datasetName = dataset.first;
        const vector<string>& fullData = dataset.second;
        for (int n = 100; n <= maxN; n += step) {
            vector<string> input(fullData.begin(), fullData.begin() + n);

            auto runAndPrint = [&](const string& algorithm, auto runner) {
                const Measurement measurement = measure(input, repetitions, runner);
                cout << datasetName << ','
                     << n << ','
                     << algorithm << ','
                     << measurement.micros << ','
                     << measurement.charOps << ','
                     << (measurement.sorted ? "yes" : "no") << '\n';
            };

            runAndPrint("standard_quick_sort", [](vector<string>& values, Metrics& metrics) {
                if (!values.empty()) {
                    StringSortTester::standardQuickSort(values, 0, static_cast<int>(values.size()) - 1, metrics);
                }
            });
            runAndPrint("standard_merge_sort", [](vector<string>& values, Metrics& metrics) {
                vector<string> buffer(values.size());
                StringSortTester::standardMergeSort(values, buffer, 0, static_cast<int>(values.size()), metrics);
            });
            runAndPrint("string_quick_sort", [](vector<string>& values, Metrics& metrics) {
                if (!values.empty()) {
                    StringSortTester::stringQuickSort(values, 0, static_cast<int>(values.size()) - 1, 0, metrics);
                }
            });
            runAndPrint("string_merge_sort", [](vector<string>& values, Metrics& metrics) {
                vector<string> buffer(values.size());
                StringSortTester::stringMergeSort(values, buffer, 0, static_cast<int>(values.size()), metrics);
            });
            runAndPrint("msd_radix_sort", [](vector<string>& values, Metrics& metrics) {
                vector<string> buffer(max<size_t>(1, values.size()));
                StringSortTester::msdRadixSort(values, buffer, 0, static_cast<int>(values.size()), 0, metrics, false);
            });
            runAndPrint("msd_radix_quick_sort", [](vector<string>& values, Metrics& metrics) {
                vector<string> buffer(max<size_t>(1, values.size()));
                StringSortTester::msdRadixSort(values, buffer, 0, static_cast<int>(values.size()), 0, metrics, true);
            });
        }
    }

    return 0;
}
