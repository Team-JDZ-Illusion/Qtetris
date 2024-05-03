#ifndef WIDGET_H
#define WIDGET_H

#include "block.h"
#include <QMediaPlayer>
#include <QStringList>
#include <QPushButton>
#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QAudioOutput>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();
  enum Difficulty {
      Easy,
      Normal,
      Hard,
      Lunatic
  };
  void init();
  void saveScore(int);
  QString diff2String(Difficulty);

private slots:
  void onMusicButtonClicked();
  void slot_pushButton_start_onClicked();
  void slot_pushButton_space_onClicked();
  void slot_pushButton_down_onClicked();
  void slot_pushButton_left_onClicked();
  void slot_pushButton_right_onClicked();
  void slot_pushButton_revolve_onClicked();
  void slot_pushButton_pause_onClicked();
  void slot_pushButton_difficulty_onClicked();
  void slot_timer_timeout();

private:
  Ui::Widget *ui;
  Difficulty currentDifficulty;
  QMediaPlayer *mediaPlayer;
  QPushButton *playButton;
  int currentIndex;
  quint16 *mp_tableValue;
  activeBlock m_actBlock;
  staticBlock m_staBlock;
  QTimer m_timer;
  int m_nextBlockState;
  void showHistory();
  const QMap<Difficulty, int> difficultySpeed = {
      {Easy, 900},
      {Normal, 700},
      {Hard, 300},
      {Lunatic, 100}
  };
};
#endif // WIDGET_H
