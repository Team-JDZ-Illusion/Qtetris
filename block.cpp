#include "block.h"
#include "tetristable.h"
#include <QDebug>
#define BIT_CLEAR (0b0000001111111111)

// 存储当前活动方块的值和限制
activeBlock::activeBlock() {
  mp_blockValue = new quint16[TABLE_ROW]{0};
  mp_limitValue = new quint16[TABLE_ROW]{0};
  for (int i = 0; i < TABLE_ROW; ++i) {
    mp_blockValue[i] = 0x00;
    mp_limitValue[i] = 0x00;
  }
}

activeBlock::~activeBlock() {
  delete[] mp_blockValue;
  mp_blockValue = nullptr;
  delete[] mp_limitValue;
  mp_limitValue = nullptr;
}

void activeBlock::init() {
  for (int i = 0; i < TABLE_ROW; ++i) {
    mp_blockValue[i] = 0x0000;
    mp_limitValue[i] = 0x0000;
  }
}

void activeBlock::updateNewBlock(int state) {
  m_pos.x = TABLE_COLUMN / 2 - 1;
  m_pos.y = TABLE_ROW - 1;
  m_pos.blockState = state;
  updateBlockValue();
}

bool activeBlock::blockDown() {
  if (checkPosition(m_pos.x, m_pos.y - 1, m_pos.blockState)) {
    m_pos.y -= 1;
    updateBlockValue();
    return true;
  } else {
    return false;
  }
}

void activeBlock::blockLeft() {
  if (checkPosition(m_pos.x + 1, m_pos.y, m_pos.blockState)) {
    m_pos.x += 1;
    updateBlockValue();
  }
}

void activeBlock::blockRight() {
  if (checkPosition(m_pos.x - 1, m_pos.y, m_pos.blockState)) {
    m_pos.x -= 1;
    updateBlockValue();
  }
}

void activeBlock::blockRevolve() {
  int rem = (m_pos.blockState + 1) % 4;
  int state = 0;
  if (rem == 0)
    state = -3;
  else
    state = 1;
  qDebug() << QString("x=%1,y=%2,state=%3,in=%4")
                  .arg(m_pos.x)
                  .arg(m_pos.y)
                  .arg(m_pos.blockState)
                  .arg(m_pos.blockState + state);
  if (checkPosition(m_pos.x, m_pos.y, m_pos.blockState + state)) {
    m_pos.blockState = m_pos.blockState + state;
    updateBlockValue();
  }
}

quint16 *activeBlock::blockValue() { return mp_blockValue; }

void activeBlock::updateLimit(quint16 *limitTable) {
  for (int i = 0; i < TABLE_ROW; ++i) {
    mp_limitValue[i] = limitTable[i];
  }
}

void activeBlock::printValue() {
  for (int i = 0; i < TABLE_ROW; ++i) {
    qDebug() << QString("%1").arg(mp_blockValue[i], 10, 2);
  }
}

void activeBlock::updateBlockValue() {
  int xMove = m_pos.x - (TABLE_COLUMN / 2 - 1);
  for (int i = 0; i < TABLE_ROW; ++i) {
    if (i <= m_pos.y + 2 && i >= m_pos.y - 2) {
      int yMove = i - m_pos.y + 2;
      if (xMove >= 0) {
        mp_blockValue[i] = blockTable[m_pos.blockState][yMove] << (xMove);
      } else {
        mp_blockValue[i] = blockTable[m_pos.blockState][yMove] >> (-xMove);
      }
    } else {
      mp_blockValue[i] = 0x0;
    }
  }
}

bool activeBlock::checkPosition(int x, int y, int state) {

  int xMove = x - (TABLE_COLUMN / 2 - 1);
  if (xMove >= 0) {
    for (int i = 0; i < blockCheck; i++) {
      quint16 oldv = blockTable[state][i] << xMove;
      if (oldv >= (quint16)0x01 << TABLE_COLUMN)
        return false;
    }    // 检测左边界
  } else // 右移
  {
    for (int i = 0; i < blockCheck; i++) {
      xMove = -xMove;
      quint16 oldv = blockTable[state][i];
      if ((oldv & (1 << (xMove - 1))) != 0)
        return false;
    } // 检测右边界
  }

  for (int i = 0; i < blockCheck; i++) {
    quint16 oldv = blockTable[state][i];
    // qDebug()<<i<<QString("0b%1").arg(oldv, 16, 2, QLatin1Char('0'));
    if (oldv > 0) {
      if (y - (blockCheck / 2 - i) < 0)
        return false;
    } // 检测底边界
  }
  xMove = x - (TABLE_COLUMN / 2 - 1);
  for (int i = 0; i < TABLE_ROW; i++) {
    // qDebug()<<i<<m_pos.y;
    if (mp_limitValue[i] != 0) {
      for (int j = 0; j < blockCheck; j++) {
        int ty = y + (j - blockCheck / 2);
        if (ty == i) {
          quint16 temp = blockTable[state][j];
          if (xMove >= 0)
            temp = (BIT_CLEAR) & (temp << (xMove));
          else
            temp = (BIT_CLEAR) & (temp >> (-xMove));
          qDebug()
              << QString("test 0b%1 0b%2 i=%3,j=%4,xMove=%5,x=%6,y=%7,state=%8")
                     .arg(temp, 16, 2, QLatin1Char('0'))
                     .arg(mp_limitValue[i], 16, 2, QLatin1Char('0'))
                     .arg(i)
                     .arg(j)
                     .arg(xMove)
                     .arg(x)
                     .arg(ty)
                     .arg(state);
          if ((temp & mp_limitValue[i]) != 0) {
            qDebug()
                << QString(
                       "test 0b%1 0b%2 i=%3,j=%4,xMove=%5,x=%6,y=%7,state=%8")
                       .arg(temp, 16, 2, QLatin1Char('0'))
                       .arg(mp_limitValue[i], 16, 2, QLatin1Char('0'))
                       .arg(i)
                       .arg(j)
                       .arg(xMove)
                       .arg(x)
                       .arg(ty)
                       .arg(state);
            return false;
          }
        }
      }
    }
  }

  return true;
}

staticBlock::staticBlock() { mp_blockValue = new quint16[TABLE_ROW]{0}; }

staticBlock::~staticBlock() {
  delete[] mp_blockValue;
  mp_blockValue = nullptr;
}

void staticBlock::init() {
  for (int i = 0; i < TABLE_ROW; ++i) {
    mp_blockValue[i] = 0x0000;
  }
  m_point = 0;
}

quint16 *staticBlock::blockValue() { return mp_blockValue; }

bool staticBlock::appendBlock(quint16 *blockVal) {
  for (int i = 0; i < TABLE_ROW; ++i) {
    if (blockVal[i] == 0)
      continue;
    mp_blockValue[i] = mp_blockValue[i] | blockVal[i];
  }
  int flag = 0;
  for (int i = 0; i < TABLE_ROW;) {
    if (mp_blockValue[i] >= (BIT_CLEAR)) {
      for (int j = i; j < TABLE_ROW; ++j) {
        if (j < TABLE_ROW - 1)
          mp_blockValue[j] = mp_blockValue[j + 1];
        else
          mp_blockValue[j] = 0x0000;
      }
      switch(flag){
      case 0:
          m_point += 100;
          break;
      case 1:
          m_point += 100;
          break;
      case 2:
          m_point += 200;
          break;
      case 3:
          m_point += 400;
          break;
      default:
          break;
      }
      flag++;
    } else{
      ++i;
      flag = 0;
    }
  }
  if (mp_blockValue[TABLE_ROW - 1] != 0)
    return true;
  return false;
}

int staticBlock::point() { return m_point; }

void staticBlock::mixBlockTable(quint16 *srcTable, quint16 *activeTable,
                                quint16 *staticTable) {
  for (int i = 0; i < TABLE_ROW; ++i) {
    srcTable[i] = activeTable[i] | staticTable[i];
  }
}
