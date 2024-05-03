#ifndef TETRISTABLE_H
#define TETRISTABLE_H

#include <QKeyEvent>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>
const int TABLE_COLUMN = 10;
const int TABLE_ROW = 20;

class tetristable : public QTableWidget {
  // Qt标准宏,支持 Qt 的信号和槽机制、国际化等特性
  Q_OBJECT
signals:
  void signal_out_keyPress_up();
  void signal_out_keyPress_space();
  void signal_out_keyPress_down();
  void signal_out_keyPress_left();
  void signal_out_keyPress_right();
  void signal_out_keyPress_enter();
  void signal_out_keyPress_pause();

public:
  explicit tetristable(QWidget *parent = nullptr);
  ~tetristable();
  void updateTable(quint16 *data);

  // 处理键盘事件
private:
  void keyPressEvent(QKeyEvent *event);

  // 显示与更新游戏状态
private:
  QTableWidgetItem *mp_items[TABLE_ROW][TABLE_COLUMN];
};

#endif // TETRISTABLE_H
