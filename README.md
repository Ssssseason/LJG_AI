# Reversi_AI

C++实现的AI五子棋，主要包括：
- 使用curl实现的网络请求处理
- 标准蒙特卡洛树算法
- Alpha-Beta pruning Minimax
- Iterative Minimax （每次三层迭代，充分利用时间）

**性能**
MCTS < alpha-beta pruning minimax < iterative minimax

## 使用方法
p1_engine 和 p2_engine用于包装ai引擎，里面需要做类型和格式的调整用以适应不同的ai引擎的写法

在main里面修改p1 p2的Role，改变先后手

- minmax引擎传入估值函数(类型double (*)(Bitboard b))和搜索深度
- mcts引擎传入搜索深度和时间控制类实例
	- 搜索深度：大于0则使用minmax_search（调试中）；否则使用random_search
	- 时间控制：限制迭代次数
- human手动测试用

**Alpha-Reversi**中为去年代码，eigen3为代码中使用的矩阵库，core中为核心代码，battlemain.cpp用于测试我们算法性能。

**oldyear**中为Botzone排行榜中第二名代码，测试用。

**v1.0**为用于提交的经过整理的代码

***

## 文件清单

- `include/`
  - `bitboard.hpp` 棋盘类
    - 操作相关函数见main.cpp
    - 评估相关函数见Bitboard中的evaluate()；对黑棋来说，返回值越大越优，白棋反之；评估值相同时可用getRoxannePriority()获取优先级
- `src/`
  - `main.cpp`

****
## 接口

#### `Role`枚举

  `BLACK`, `WHITE`


#### `Bitboard`类

##### 非静态成员变量

  - `uint64_t black, white`

    分别代表黑白方现有棋子

    索引如下

    00 01 02 03 04 05 06 07 
    08 09 10 11 12 13 14 15 
    16 17 18 19 20 21 22 23 
    24 25 26 27 28 29 30 31 
    32 33 34 35 36 37 38 39 
    40 41 42 43 44 45 46 47 
    48 49 50 51 52 53 54 55 
    56 57 58 59 60 61 62 63

##### 非静态成员函数

  - `void takeAction(Role player, uint64_t action)`

    在action置位处，player落子，改变棋盘状态；action必须只有1位为1

  - `uint64_t getActions(Role player)`

    返回player当前可下位置，uint64_t对应位置为1

  - `bool hasEnded()`

    true则棋局结束

  - `pair<int, int> getPiceces()`

    返回黑白拥有棋子的个数

  - `void printBoard()`

     打印棋盘

##### 静态成员函数

- `void init()`

  使用前需要进行初始化


- `void printPawn(uint64_t pawn)`

  输入64位整数，打印出对应棋盘落子

- `int getRoxannePriority(uint64_t action)`

  当评估函数返回值相同时，调用该函数获取优先级；传入只有1位为1的action；优先级1-5越低越好；为0则有问题（落子在初始位置）

- `int scanForward(uint64_t bits)`

  返回bits的最低有效位

- `double evaluateCombine(const Bitboard &b)`

  返回评估值；目前只有这一个评估函数，其余评估函数参数相同，名字均以evaluate开头

#### `Timer`类
##### 成员变量
- `chrono::system_clock::time_point start`

  起始时间
- `int seconds`

  限定的时间间隔
#### 成员函数
- `Timer(int seconds)`

  构造函数，初始化seconds为传入seconds，初始化start为当前时间

- `double getTimeLeft()`

  返回以秒为单位的剩余时间

- `double getTimePassed()`

  返回以秒为单位的已经流逝的时间

