#include "widget.h"
#include "tetristable.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QRandomGenerator>
// #include <QSettings>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  QString basePath = QCoreApplication::applicationDirPath();
  setWindowTitle("Qtetris");
  setWindowIcon(QIcon(basePath + "/resources/images/logo.png"));
  //  效果不佳  this->setStyleSheet("background-image:
  //    url(/home/gh0s7/project/qt6/Qtetris/resources/images/78355322_p0.jpg);");
  mediaPlayer = new QMediaPlayer(this);
  QAudioOutput *audioOutput = new QAudioOutput;
  audioOutput->setVolume(0.6);
  mediaPlayer->setAudioOutput(audioOutput);
  // mediaPlayer->setMedia(QUrl::fromLocalFile("/home/gh0s7/project/qt6/Qtetris/resources/audio/flower.ogg"));
  // auto *playlist = new QMediaPlaylist();
  // bgm都是自己用开源编曲软件lmms编曲的，原曲来自上海爱莉丝幻乐团qwq
  QStringList audioFiles = {// 广重36号
                            basePath + "/resources/audio/neoexpress.wav",
                            // 宇宙露天咖啡座
                            basePath + "/resources/audio/CafeTerrace.ogg",
                            // 花如幻想一般
                            basePath + "/resources/audio/flower.ogg",
                            // 寻找日本各处之神秘
                            basePath + "/resources/audio/mistery.wav",
                            // 无间之钟
                            basePath + "/resources/audio/clock.wav",
                            // 幻想净琉璃
                            basePath + "/resources/audio/glass.wav"};
  currentIndex = 0;
  mediaPlayer->setSource(QUrl::fromLocalFile(audioFiles[currentIndex]));
  QObject::connect(
      mediaPlayer, &QMediaPlayer::mediaStatusChanged, mediaPlayer, [=]() {
        if (mediaPlayer->mediaStatus() == QMediaPlayer::EndOfMedia) {
          currentIndex = (currentIndex + 1) % audioFiles.size();
          mediaPlayer->setSource(QUrl::fromLocalFile(audioFiles[currentIndex]));
          mediaPlayer->play();
        }
      });
  mediaPlayer->play();
  /* 原qt5写法，相关接口已废弃
    QString audioPath = basePath + "/resources/audio/neoexpress.wav";
    playlist->addMedia(QUrl::fromLocalFile(audioPath));
    audioPath = basePath + "/resources/audio/CafeTerrace.ogg";
    playlist->addMedia(QUrl::fromLocalFile(audioPath));
    audioPath = basePath + "/resources/audio/flower.ogg";
    playlist->addMedia(QUrl::fromLocalFile(audioPath));
    audioPath = basePath + "/resources/audio/mistery.wav";
    playlist->addMedia(QUrl::fromLocalFile(audioPath));
    audioPath = basePath + "/resources/audio/clock.wav";
    playlist->addMedia(QUrl::fromLocalFile(audioPath));
    audioPath = basePath + "/resources/audio/glass.wav";
    playlist->addMedia(QUrl::fromLocalFile(audioPath));

    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    mediaPlayer->setVolume(50);
    mediaPlayer->setPlaylist(playlist);
    mediaPlayer->play();
  */
  m_nextBlockState = (QRandomGenerator::global()->bounded(10)) * 4;
  ui->tableWidget_next->updateBlock(m_nextBlockState);
  connect(ui->pushButton_music, &QPushButton::clicked, this,
          &Widget::onMusicButtonClicked);
  connect(ui->pushButton_start, &QPushButton::clicked, this,
          &Widget::slot_pushButton_start_onClicked);
  connect(ui->pushButton_space, &QPushButton::clicked, this,
          &Widget::slot_pushButton_space_onClicked);
  connect(ui->pushButton_down, &QPushButton::clicked, this,
          &Widget::slot_pushButton_down_onClicked);
  connect(ui->pushButton_left, &QPushButton::clicked, this,
          &Widget::slot_pushButton_left_onClicked);
  connect(ui->pushButton_right, &QPushButton::clicked, this,
          &Widget::slot_pushButton_right_onClicked);
  connect(ui->pushButton_revolve, &QPushButton::clicked, this,
          &Widget::slot_pushButton_revolve_onClicked);
  connect(ui->pushButton_pause, &QPushButton::clicked, this,
          &Widget::slot_pushButton_pause_onClicked);
  connect(ui->pushButton_difficulty, &QPushButton::clicked, this,
          &Widget::slot_pushButton_difficulty_onClicked);
  connect(ui->pushButton_history, &QPushButton::clicked, this,
          &Widget::showHistory);
  connect(ui->tableWidget, &tetristable::signal_out_keyPress_enter, this,
          &Widget::slot_pushButton_start_onClicked);
  connect(ui->tableWidget, &tetristable::signal_out_keyPress_up, this,
          &Widget::slot_pushButton_revolve_onClicked);
  connect(ui->tableWidget, &tetristable::signal_out_keyPress_down, this,
          &Widget::slot_pushButton_down_onClicked);
  connect(ui->tableWidget, &tetristable::signal_out_keyPress_left, this,
          &Widget::slot_pushButton_left_onClicked);
  connect(ui->tableWidget, &tetristable::signal_out_keyPress_right, this,
          &Widget::slot_pushButton_right_onClicked);
  connect(ui->tableWidget, &tetristable::signal_out_keyPress_space, this,
          &Widget::slot_pushButton_space_onClicked);
  connect(ui->tableWidget, &tetristable::signal_out_keyPress_pause, this,
          &Widget::slot_pushButton_pause_onClicked);
  connect(&m_timer, &QTimer::timeout, this, &Widget::slot_timer_timeout);
  mp_tableValue = new quint16[TABLE_ROW]{0};
  mp_tableValue[TABLE_ROW - 1] = 0x01 << (TABLE_COLUMN / 2 - 1);
  init();
}

Widget::~Widget() {
  delete ui;
  delete mediaPlayer;
  delete[] mp_tableValue;
  mp_tableValue = nullptr;
}

void Widget::init() {
  for (int i = 0; i < TABLE_ROW; i++) {
    mp_tableValue[i] = 0x0000;
  }
  m_actBlock.init();
  m_staBlock.init();
  ui->lineEdit_point->setText(QString::number(0));
}

void Widget::onMusicButtonClicked() {
  if (mediaPlayer->isPlaying() == true) {
    mediaPlayer->pause();
    ui->pushButton_music->setText("Continue");
  } else {
    mediaPlayer->play();
    ui->pushButton_music->setText("Music");
  }
}

void Widget::slot_pushButton_start_onClicked() {
  if (m_timer.isActive()) {
    m_timer.stop();
    ui->pushButton_start->setText(tr("Start"));
  } else {
    init();
    m_actBlock.updateNewBlock(m_nextBlockState);
    m_nextBlockState = (QRandomGenerator::global()->bounded(10)) * 4;
    ui->tableWidget_next->updateBlock(m_nextBlockState);
    staticBlock::mixBlockTable(mp_tableValue, m_actBlock.blockValue(),
                               m_staBlock.blockValue());
    ui->tableWidget->updateTable(mp_tableValue);
    ui->pushButton_start->setText(tr("Over"));
    m_timer.start(difficultySpeed[currentDifficulty]);
  }
  ui->tableWidget->setFocus();
}

void Widget::slot_pushButton_difficulty_onClicked() {
  switch (currentDifficulty) {
  case Easy:
    currentDifficulty = Normal;
    ui->pushButton_difficulty->setText("Normal");
    break;
  case Normal:
    currentDifficulty = Hard;
    ui->pushButton_difficulty->setText("Hard");
    break;
  case Hard:
    currentDifficulty = Lunatic;
    ui->pushButton_difficulty->setText("Lunatic");
    break;
  case Lunatic:
    currentDifficulty = Easy;
    ui->pushButton_difficulty->setText("Easy");
    break;
  }
}
void Widget::slot_pushButton_space_onClicked() {
  for (int i = 0; i < 10; ++i) {
    slot_pushButton_down_onClicked();
  }
  if (!m_timer.isActive()) {
    slot_pushButton_start_onClicked();
    return;
  }
  bool nret = m_actBlock.blockDown();
  if (!nret) {
    nret = m_staBlock.appendBlock(m_actBlock.blockValue());
    ui->lineEdit_point->setText(QString::number(m_staBlock.point()));
    if (nret) {
      m_timer.stop();
      saveScore(m_staBlock.getPoint());
      QMessageBox::information(NULL, tr("Tip"), tr("Game Over"));
    }
  }
}

void Widget::slot_pushButton_down_onClicked() {
  if (!m_timer.isActive()) {
    slot_pushButton_start_onClicked();
    return;
  }
  bool nret = m_actBlock.blockDown();
  if (!nret) {
    nret = m_staBlock.appendBlock(m_actBlock.blockValue());
    ui->lineEdit_point->setText(QString::number(m_staBlock.point()));
    if (nret) {
      m_timer.stop();
      saveScore(m_staBlock.getPoint());
      QMessageBox::information(NULL, tr("Tip"), tr("Game Over"));
    }
    m_actBlock.updateLimit(m_staBlock.blockValue());
    m_actBlock.updateNewBlock(m_nextBlockState);
    m_nextBlockState = (QRandomGenerator::global()->bounded(10)) * 4;
    ui->tableWidget_next->updateBlock(m_nextBlockState);
  }
  staticBlock::mixBlockTable((quint16 *)mp_tableValue,
                             (quint16 *)m_actBlock.blockValue(),
                             (quint16 *)m_staBlock.blockValue());
  ui->tableWidget->updateTable(mp_tableValue);
  ui->tableWidget->setFocus();
}

void Widget::slot_pushButton_left_onClicked() {
  m_actBlock.blockLeft();
  staticBlock::mixBlockTable((quint16 *)mp_tableValue,
                             (quint16 *)m_actBlock.blockValue(),
                             (quint16 *)m_staBlock.blockValue());
  ui->tableWidget->updateTable(mp_tableValue);
}

void Widget::slot_pushButton_right_onClicked() {
  m_actBlock.blockRight();
  staticBlock::mixBlockTable((quint16 *)mp_tableValue,
                             (quint16 *)m_actBlock.blockValue(),
                             (quint16 *)m_staBlock.blockValue());
  ui->tableWidget->updateTable(mp_tableValue);
}

void Widget::slot_pushButton_revolve_onClicked() {
  m_actBlock.blockRevolve();
  staticBlock::mixBlockTable((quint16 *)mp_tableValue,
                             (quint16 *)m_actBlock.blockValue(),
                             (quint16 *)m_staBlock.blockValue());
  ui->tableWidget->updateTable(mp_tableValue);
}

void Widget::slot_timer_timeout() { slot_pushButton_down_onClicked(); }

void Widget::slot_pushButton_pause_onClicked() {
  if (m_timer.isActive()) {
    m_timer.stop();
    ui->pushButton_pause->setText("Game Continue");
  } else {
    m_timer.start(difficultySpeed[currentDifficulty]);
    ui->pushButton_pause->setText("Pause");
  }
}

QString Widget::diff2String(Difficulty diff) {
  switch (diff) {
  case Easy:
    return "Easy";
  case Normal:
    return "Normal";
  case Hard:
    return "Hard";
  case Lunatic:
    return "Lunatic";
  default:
    return "Normal";
  }
}

void Widget::saveScore(int score) {
  QFile file("ScoresHistory.txt");
  if (file.open(QIODevice::Append | QIODevice::Text)) {
    QTextStream out(&file);
    QString diff = diff2String(currentDifficulty);
    out << "Unkown  " << diff << "  " << score << "\n";
    file.close();
  }
}

void Widget::showHistory() {
  QDialog *dialog = new QDialog(this);
  dialog->setWindowTitle("Game History");

  QVBoxLayout *layout = new QVBoxLayout(dialog);

  QTextEdit *textEdit = new QTextEdit;
  textEdit->setReadOnly(true);

  QFile file("ScoresHistory.txt");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    textEdit->setText(in.readAll());
    file.close();
  }

  textEdit->append("\nP.S. To set your name, please edit ScoresHistory.txt");

  layout->addWidget(textEdit);

  dialog->setLayout(layout);
  dialog->resize(400, 300);
  dialog->exec();
}
