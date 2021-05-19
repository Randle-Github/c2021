#include "chessboard.cpp"
#include "head.cpp"

struct Node;
int fight(Node *a, Node *b);

struct Node //棋手，不同策略
{
    double weight[10];
    //权重: 0五子相连 1四子两空 2四子一空 3三子两空 4三子一空 5两子两空 6两子一空
    //规定5恒为1.0
} node[total_num];

double get_score(int num, int blocks, Node *player) //连线数 封锁数 玩家
{
    if (num >= 5)
    {
        return player->weight[0]; //五子连线
    }
    else if (num == 4)
    {
        if (!blocks)
            return player->weight[1]; //四子两空
        if (blocks == 1)
            return player->weight[2]; //四子一空
    }
    else if (num == 3)
    {
        if (!blocks)
            return player->weight[3]; //三子两空
        if (blocks == 1)
            return player->weight[4]; //三子一空
    }
    else if (num == 2)
    {
        if (!blocks)
            return player->weight[5]; //两子两空
        if (blocks == 1)
            return player->weight[6]; //两子一空
    }
    return 0;
}

double take_step_value(int x, int y, int condition, Node *player, int step, double cut_num) //2 max 1 min
{                                                                                           //计算落子获得分数:坐标 黑白 弈者 步数，奇为自己，偶为对手
    B.board[x][y] = condition;                                                              //落子
    double score = 0, new_score, attain_score = double(INT_MAX);                            //最终分数 暂时分数 下一步最优分数
    if (condition == 1)
        attain_score *= -1; // min层初始化希望下一层得到最大值
    int k1, k2, blocks;     //能走的步数 封堵数
    blocks = 0;
    k1 = B.judge(x, y, condition, 0); //上下
    k2 = B.judge(x, y, condition, 1);
    if (x - k1 == -1 || B.board[x - k1][y] == 3 - condition)
        blocks++;
    if (x + k2 == size || B.board[x + k1][y] == 3 - condition)
        blocks++;
    score += get_score(k1 + k2 - 1, blocks, player);
    blocks = 0;
    k1 = B.judge(x, y, condition, 2); //左右
    k2 = B.judge(x, y, condition, 3);
    if (y - k1 == -1 || B.board[x][y - k1] == 3 - condition)
        blocks++;
    if (y + k2 == size || B.board[x][y + k2] == 3 - condition)
        blocks++;
    score += get_score(k1 + k2 - 1, blocks, player);
    blocks = 0;
    k1 = B.judge(x, y, condition, 4); //右上
    k2 = B.judge(x, y, condition, 5);
    if (x - k1 == -1 || y + k1 == size || B.board[x - k1][y + k1] == 3 - condition)
        blocks++;
    if (x + k2 == size || y - k2 == -1 || B.board[x + k2][y - k2] == 3 - condition)
        blocks++;
    score += get_score(k1 + k2 - 1, blocks, player);
    blocks = 0;
    k2 = B.judge(x, y, condition, 6); //右下
    k1 = B.judge(x, y, condition, 7);
    if (x + k1 == size || y + k1 == size || B.board[x + k1][y + k1] == 3 - condition)
        blocks++;
    if (x - k2 == -1 || y - k2 == -1 || B.board[x - k2][y - k2] == 3 - condition)
        blocks++;
    score += get_score(k1 + k2 - 1, blocks, player);
    if (condition == 1) // min层需要取负处理
        score *= -1;
    if (step < consider_step) //仍需考虑下一步
    {
        for (int i = max(0, previousx - consider_range); i < min(size, previousx + consider_range); i++)
            for (int j = max(0, previousy - consider_range); j < min(size, previousy + consider_range); j++)
            {
                if (!B.board[i][j])
                {                       //以自己的思维模拟对方出棋
                    if (condition == 2) //max层
                    {
                        new_score = take_step_value(i, j, 1, player, step + 1, attain_score);
                        if (new_score < attain_score) //取下一层的最小值
                            attain_score = new_score;
                        if (score + attain_score < cut_num)
                        {                      // alpha-beta剪枝，min层
                            B.board[x][y] = 0; //清除这一步
                            return 0;          //alpha-beta cut
                        }
                    }
                    else // min层
                    {
                        new_score = take_step_value(i, j, 1, player, step + 1, attain_score);
                        if (new_score > attain_score) //取下一层的最大值
                            attain_score = new_score;
                        if (score + attain_score > cut_num)
                        {                      // alpha-beta剪枝，min层
                            B.board[x][y] = 0; //清除这一步
                            return 0;          //alpha-beta cut
                        }
                    }
                }
            }
    }
    else
        attain_score = 0;
    B.board[x][y] = 0; //清除这一步
    return score + attain_score;
}