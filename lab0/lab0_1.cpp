#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

bool comp(int a, int b) {
    if (a > b) {
        return true;
    } else {
        return false;
    }
}

int main() {
    int N;
    cin >> N;
    vector<int> num(N);

    for (int i = 0; i < N; i++) {
        cin >> num[i];
    }

    sort(num.begin(), num.end(), comp);

    int a, b = 0, c = -1;

    for (int i = 0; i < N; i++) {
        if (num[i] % 33 == 0) {
            a = num[i];
            c = i;
            break;
        }
    }

    if (c == -1) {
        cout << "Нет чисел, кратных 33" << endl;
        return 0;
    }

    for (int i = 0; i < N; i++) {
        if ((num[i] - a) % 2 == 0 && i != c) {
            b = num[i];
            break;
        }
    }

    if (b == 0) {
        cout << "Нет чисел, разность которых чётна" << endl;
        return 0;
    } else {
        cout << a << " " << b;
        return 0;
    }
}