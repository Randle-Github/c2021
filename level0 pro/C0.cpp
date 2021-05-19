#include <bits/stdc++.h>
#include <ctime>
using namespace std;
#define mod (1 + total_num / 2) * total_num / 4
const int total_num = 500;
inline const int Russian_roulette() // decide the parents with probability
{
    int chosen = int(long(rand()) * long(rand()) % long(mod));
    //distribute the probability with the rank as 1+2+3+4 etc
    int _down = 1, _up = total_num / 2;
    int mid = (_up + _down) / 2;
    while (mid > _down)
    {
        if ((1 + mid) * mid / 2 < chosen)
        {
            _down = mid;
            mid = (mid + _up) / 2;
            continue;
        }
        else if ((1 + mid) * mid / 2 > chosen)
        {
            _up = mid;
            mid = (mid + _down) / 2;
            continue;
        }
        else
            return total_num / 2 - mid;
    }
    return total_num / 2 - mid;
}
int main()
{
    srand(time(NULL));
    for (int i = 1; i <= 150; ++i)
        cout << Russian_roulette() << endl;
    //cout << 50 * 51 / 2;
    return 0;
}