/*
本代码尝试使用线性代数求解手写数字识别问题
其思路为：
将读入的学习样本，按照标签分类，同一个标签放置进入同一个向量空间。
当新的测试样本进入时，用这是个向量空间分别尝试对测试样本进行线性表出，或者表出一个与之距离最近
的向量。以这个估计向量与测试样本向量之间的距离表示损失。
损失最小的哪一个向量空间即为所求。
然而这个方法好像很失败呀……
*/

#include <windows.h>
#include <bits/stdc++.h>
using namespace std;
#define double_max 100000000.0
#define maxn 60000
#define size 784           // 图片大小
#define step 1             // 学习步长
#define learning_num 60000 // 学习样本数
#define learning_times 50  // 学习代数
#define test_num 100       // 测试样本数

inline void read(int &a) // 快速读入
{
    a = 0;
    char c = getchar();
    while (c < '0' || c > '9')
        c = getchar();
    while (c >= '0' && c <= '9')
    {
        a = (a << 1) + (a << 3) + c - '0';
        c = getchar();
    }
    return;
}

struct Fig // 图片
{
    int l;                       // 标签
    double label[10], vec[size]; // 标签 向量
    void read_fig()              // 读入一个图片
    {
        memset(label, 0, sizeof(label));
        read(l);
        label[l] = 1.0;                // 仅有标签项为1
        for (int i = 0; i < size; i++) // 将图像每一格转化为小于1的值，并转为向量形式
        {
            int x;
            read(x);
            vec[i] = double(x) / 20000; // 图像格灰度值大约在200左右
        }
    }
} train_fig[60000], test_fig[10000]; // 样本集 测试集

struct Matrix // 每一个图像都有一个线性空间
{
    double m[784][10000]; // 用矩阵表示线性空间
    int line, row;        // 行数与列数 行数是图像大小即784，列数是这个向量空间所包含的图像数量
    void init()           // 初始化
    {
        line = 784;
        row = 0;
    }
    void Gauss() // 高斯消元，其实没简化什么计算，因为列最大可能有60000列，行只有700行
    {
        int t = -1; // 消到的列数
        for (int i = 0; i < line; i++)
        {
            if (m[i][0] == 0)
                continue;
            ++t; // 消下一列
            for (int j = i + 1; j < line; j++)
            {
                double x = m[j][t] / m[i][t];
                for (int k = t; k < row; k++)
                    m[j][k] -= x * m[i][k]; // 将矩阵消成行阶梯型
            }
        }
    }
    inline void append(Fig *p) // 传入一个图像列向量
    {
        for (int i = 0; i < line; i++)
            m[i][row] = p->vec[i];
        ++row; // 增加一列
    }
    void print() // 打印向量空间
    {
        for (int i = 0; i < line; i++)
        {
            for (int j = 0; j < row; j++)
                printf("%d ", int(m[i][j]));
            printf("\n");
        }
    }
    double loss;               // 损失值，即这个向量空间可表示与样本向量最接近的估计向量，与样本向量的距离表示
    void loss_function(Fig *p) // 传入一个样本，计算loss值
    {
        /*
        尝试用这个向量空间去拟合样本，即尝试求解一个row*1的w向量，w[j]表示它第j个向量的权值
        对于每一个w[j]的求解，考虑使用梯度下降法
        */
        double w[10000];              // w权值向量，右乘m矩阵得到估计向量
        for (int j = 0; j < row; j++) // 初始化w向量
            w[j] = double(rand() % 10000);
        loss = double_max;
        
        for (int j = 0; j < row; j++) // 对每一列使用梯度下降算法
        {
            double temp_loss;                           // 第j列对应的是w向量的第j位
            for (int ll = 0; ll < learning_times; ll++) // 根据学习次数进行梯度下降
            {
                /* temp_loss= sigma (w[j]*m[i][j] - p->vec[i])^2 */
                temp_loss = 0;
                double dif_w = 0.0;
                for (int i = 0; i < line; i++) // 对于这一列的损失值与偏导
                {
                    temp_loss += (w[j] * m[i][j] - p->vec[i]) * (w[j] * m[i][j] - p->vec[i]); // 计算损失                                               // 计算偏导数
                    dif_w += 2 * (w[j] * m[i][j] - p->vec[i]) * m[i][j];                      // 计算偏导
                }
                //w[j] -= step * dif_w; // 进行梯度下降
            }
            loss += temp_loss; // 增加这个向量的损失值
        }
        loss /= double(row); // 取平均值
    }
} M[10]; // 十个手写数字每一个都有一个向量空间

void Read_data() // 读入所有所需图片
{
    freopen("fig_data1.txt", "r", stdin);
    for (int i = 0; i < learning_num; i++)
    {
        train_fig[i].read_fig();                 // 读入训练数据
        M[train_fig[i].l].append(&train_fig[i]); // 将训练数据加入向量空间
    }
    freopen("CON", "r", stdin);
    freopen("fig_data2.txt", "r", stdin);
    for (int i = 0; i < test_num; i++)
        test_fig[i].read_fig(); // 读入测试数据
    freopen("CON", "r", stdin);
}

int main()
{
    srand(time(NULL));
    for (int k = 0; k <= 9; k++)
        M[k].init(); // 初始化
    Read_data();
    double min_loss = double_max; // 最小损失值
    double rate = 0;              // 正确率
    for (int i = 0; i < test_num; i++)
    {
        double min_loss = double_max;
        int classify = 0;            // 归类为的数字类型
        for (int j = 0; j < 10; j++) // 每种数字都计算loss
        {
            M[j].loss_function(&test_fig[i]);
            if (M[j].loss < min_loss)
            {
                classify = j;
                min_loss = M[j].loss;
            }
            printf("%lf ", M[j].loss);
        }
        cout << endl;
        if (classify == test_fig[i].l)
            rate += 1.0;
        cout << test_fig[i].l << " to " << classify << endl;
    }
    printf("%lf%/n", rate / double(test_num) * 100.0); // 计算正确率
    return 0;
}