#include <bits/stdc++.h>
using namespace std;
int main()
{
    int s[100];
    s[0] = 0;
    s[1] = 1;
    s[2] = 2;
    int *c = s;
    cout << ++*c;
    return 0;
}