#ifndef NEXTBLOCK_H
#define NEXTBLOCK_H

#include <QObject>
#include <QTableWidget>
#include <QWidget>
//#include <QRandomGenerator>

class nextblock : public QTableWidget {
  Q_OBJECT
public:
  explicit nextblock(QWidget *parent = nullptr);
  ~nextblock();
  // 下一个方块
  void updateBlock(int state);

private:
  void updateTable(quint16 *data);
  QTableWidgetItem *mp_items[5][5];
};
//QColor generateBrightColor();

#endif // NEXTBLOCK_H
