#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, a, l, r, m1 = 0, m2 = 0;
    cin >> n;
    vector<int> num(120, 0);
    for (int i = 0; i < n; i++) {
        cin >> a;
        l = a % 120;
        r = 120 - l;
        if ((a + num[r] >  m1 + m2) && (num[r] > a)){
            m1 = num[r];
            m2 = a;
        }
        if (a > num[l])
            num[l] = a;
    }
    cout << m1 << " " << m2;
    return 0;
}