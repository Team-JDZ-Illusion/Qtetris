#include "tetristable.h"
//#include "nextblock.h"
#include <QBrush>
#include <QDebug>
#include <QHeaderView>

tetristable::tetristable(QWidget *parent) : QTableWidget{parent} {
  // 表格的行和列设置
  this->setRowCount(TABLE_ROW);
  this->setColumnCount(TABLE_COLUMN);

  // 关闭滚动条
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // 隐藏表头
  this->horizontalHeader()->setVisible(false);
  this->verticalHeader()->setVisible(false);

  // 禁止选中和编辑单元格
  this->setSelectionMode(QAbstractItemView::NoSelection);
  this->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // 设置网格线可见和样式
  this->setShowGrid(true);
  this->setGridStyle(Qt::DashDotLine);

  // 列宽和行高自动调整
  this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // 初始化表格单元格
  // 获取表格中第一列的宽度
  int cellWidth = this->columnWidth(0);
  for (int i = 0; i < TABLE_ROW; ++i) {
    // 确保行高与列宽相等
    this->setRowHeight(i, cellWidth);
    // 将表格初始化为具有指定行数和列数的网格
    for (int j = 0; j < TABLE_COLUMN; ++j) {
      mp_items[i][j] = new QTableWidgetItem();
      this->setItem(TABLE_ROW - i - 1, TABLE_COLUMN - j - 1, mp_items[i][j]);
    }
  }
}

tetristable::~tetristable() { this->clear(); }

// 更新表格
void tetristable::updateTable(quint16 *data) {
  if (!data)
    return;
//  auto color = generateBrightColor();
  for (int i = 0; i < TABLE_ROW; ++i) {
    // 使用掩码来确定tempValue的哪一位被设置
    quint16 tempVal = data[i];
    for (int j = 0; j < TABLE_COLUMN; ++j) {
      quint16 temp = 0x01;
      temp <<= j;
      temp = tempVal & temp;
      if (temp == 0) {
        mp_items[i][j]->setBackground(QBrush(QColor(255, 255, 255)));
      } else {
        mp_items[i][j]->setBackground(QBrush(QColor(102, 204, 255)));
      //    mp_items[i][j]->setBackground(QBrush(color));
      //    QColor currentColor = mp_items[i][j]->background().color();
      //    if (currentColor == QColor(255, 255, 255)) {  // 检查当前颜色是否为白色
      //        mp_items[i][j]->setBackground(QBrush(color));
      //    }
       }
    }
  }
}

void tetristable::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Up:
    emit signal_out_keyPress_up();
    break;
  case Qt::Key_Left:
    emit signal_out_keyPress_left();
    break;
  case Qt::Key_Right:
    emit signal_out_keyPress_right();
    break;
  case Qt::Key_Down:
    emit signal_out_keyPress_down();
    break;
  case Qt::Key_Space:
    emit signal_out_keyPress_space();
    break;
  case Qt::Key_Enter:
    emit signal_out_keyPress_enter();
      break;
  case Qt::Key_F2:
    emit signal_out_keyPress_pause();
    break;
  default:
    break;
  }
}
