#include "nextblock.h"
#include "block.h"
#include <QBrush>
#include <QDebug>
#include <QHeaderView>

nextblock::nextblock(QWidget *parent) : QTableWidget{parent} {
  this->setRowCount(5);
  this->setColumnCount(5);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->horizontalHeader()->setVisible(false);
  this->verticalHeader()->setVisible(false);
  this->setSelectionMode(QAbstractItemView::NoSelection);
  this->setEditTriggers(QAbstractItemView::NoEditTriggers);
  this->setShowGrid(true);
  this->setGridStyle(Qt::SolidLine);
  // 根据QTableWidget的大小自动调整列宽和行高
  this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  for (int column = 0; column < this->columnCount(); ++column) {
    this->setColumnWidth(column, this->width() / this->columnCount());
  }
  this->resizeColumnsToContents();
  int cellWidth = this->columnWidth(0);
  for (int i = 0; i < 5; ++i) {
    this->setRowHeight(i, cellWidth);
    for (int j = 0; j < 5; ++j) {
      mp_items[i][j] = new QTableWidgetItem();
      this->setItem(5 - i - 1, 5 - j - 1, mp_items[i][j]);
    }
  }
}

nextblock::~nextblock() { this->clear(); }

void nextblock::updateBlock(int state) {
  quint16 temp[5]{0};
  for (int i = 0; i < 5; ++i) {
    temp[i] = blockTable[state][i] >> 2;
  }
  updateTable(temp);
}

/*QColor generateBrightColor() {
    int red = QRandomGenerator::global()->bounded(256);   // 0-255
    int green = QRandomGenerator::global()->bounded(256); // 0-255
    int blue = QRandomGenerator::global()->bounded(256);  // 0-255

    // 转换为 HSV 来增加鲜艳度
    QColor color(red, green, blue);
    color = color.toHsv();
    color.setHsv(color.hue(), 255, 255); // 设置最大饱和度和最大亮度

    return color.toRgb(); // 转回 RGB
}
*/
void nextblock::updateTable(quint16 *data) {
  if (!data)
    return;
//  auto color = generateBrightColor();
  for (int i = 0; i < 5; ++i) {
    // 使用掩码来确定tempValue的哪一位被设置
    quint16 tempVal = data[i];
    for (int j = 0; j < 5; ++j) {
      quint16 temp = 0x01;
      temp <<= j;
      temp = tempVal & temp;
      if (temp == 0) {
        mp_items[i][j]->setBackground(QBrush(QColor(255, 255, 255)));
      } else {
       mp_items[i][j]->setBackground(QBrush(QColor(102, 204, 255)));
       // mp_items[i][j]->setBackground(QBrush(color));
      }
    }
  }
}


