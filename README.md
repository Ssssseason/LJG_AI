# Reversi_AI

## 文件清单

- `include/`
  - `bitboard.hpp` 棋盘类
    - 操作相关函数见main.cpp
    - 评估相关函数见Bitboard中的evaluate()；对黑棋来说，返回值越大越优，白棋反之；评估值相同时可用getRoxannePriority()获取优先级
- `src/`
  - `main.cpp`