#ifndef BLOCK_H
#define BLOCK_H

#include <QObject>

const int blockState = 40;
const int blockCheck = 5;
// 设置形状
const quint16 blockTable[blockState][blockCheck] = {
    {0b0000000000, 0b0000100000, 0b0000110000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000011000, 0b0000110000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0000011000, 0b0000001000, 0b0000000000},
    {0b0000000000, 0b0000011000, 0b0000110000, 0b0000000000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0000011000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000111000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0000110000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0000111000, 0b0000000000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000110000, 0b0000110000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000110000, 0b0000110000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000110000, 0b0000110000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000110000, 0b0000110000, 0b0000000000},
    {0b0000010000, 0b0000010000, 0b0000110000, 0b0000000000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0001110000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000011000, 0b0000010000, 0b0000010000},
    {0b0000000000, 0b0000010000, 0b0000011100, 0b0000000000, 0b0000000000},
    {0b0000010000, 0b0000010000, 0b0000010000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0001111000, 0b0000000000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0000010000, 0b0000010000, 0b0000010000},
    {0b0000000000, 0b0000000000, 0b0000111100, 0b0000000000, 0b0000000000},
    {0b0000000000, 0b0000001000, 0b0000011000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000110000, 0b0000011000, 0b0000000000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0000110000, 0b0000100000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000110000, 0b0000011000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000110000, 0b0000110000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000110000, 0b0000110000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000110000, 0b0000110000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000110000, 0b0000110000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0000110000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000111000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0000011000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0000111000, 0b0000000000, 0b0000000000},
    {0b0000010000, 0b0000010000, 0b0000011000, 0b0000000000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0001110000, 0b0000000000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0000110000, 0b0000010000, 0b0000010000},
    {0b0000000000, 0b0000000000, 0b0000011100, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000010000, 0b0000010000, 0b0000010000, 0b0000010000},
    {0b0000000000, 0b0000000000, 0b0000111100, 0b0000000000, 0b0000000000},
    {0b0000010000, 0b0000010000, 0b0000010000, 0b0000010000, 0b0000000000},
    {0b0000000000, 0b0000000000, 0b0001111000, 0b0000000000, 0b0000000000}};

// 记录位置状态
struct blockPosition {
  int x, y, blockState;
};

// 管理当前活动块
class activeBlock {
public:
  activeBlock();
  ~activeBlock();
  void init();
  void updateNewBlock(int state);
  bool blockDown();
  void blockLeft();
  void blockRight();
  void blockRevolve();
  quint16 *blockValue();
  void updateLimit(quint16 *limitTable);
  void printValue();

private:
  void updateBlockValue();
  bool checkPosition(int x, int y, int state);

private:
  quint16 *mp_blockValue;
  quint16 *mp_limitValue;
  blockPosition m_pos;
};

// 管理已经下落固定的方块
class staticBlock {
public:
  staticBlock();
  ~staticBlock();
  void init();
  quint16 *blockValue();
  bool appendBlock(quint16 *blockValue);
  int point();
  static void mixBlockTable(quint16 *srcTable, quint16 *actTable,
                            quint16 *staTable);
  int getPoint(){return m_point;};

private:
  quint16 *mp_blockValue;
  int m_point;
};

#endif // BLOCK_H
