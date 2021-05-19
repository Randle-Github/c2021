#include "functions.cpp"
#include "game_start.cpp"
int main()
{
    srand(time(NULL));
    node[1].weight[0] = 10000;
    node[1].weight[1] = 304.75;
    node[1].weight[2] = 78.75;
    node[1].weight[3] = 34.75;
    node[1].weight[4] = 9.25;
    node[1].weight[5] = 3;
    node[1].weight[6] = 1;
    game_start();
    return 0;
}