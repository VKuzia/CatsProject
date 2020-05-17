#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QDir>
#include <QGraphicsView>

class Minigame;

class GameView : public QGraphicsView {
  Q_OBJECT

 public:
  explicit GameView(QWidget* parent = nullptr);
  ~GameView() override = default;

  void SetUp(QSize resolution);

  void SetMinigame(Minigame* current_minigame);

  void SetPixelsInMeter(qreal pixels_in_meter);
  qreal GetPixelsInMeter() const;

  QString GetPathToMinigameImages() const;

 private:
  const QString kPathToMinigameImages_ =
      QDir::currentPath() + "/data/images/minigames/";
  const int32_t kBasicWidth_ = 1024;

  Minigame* current_minigame_ = nullptr;
  qreal scale_ = 1;
  qreal pixels_in_meter_ = 0;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
};

#endif  // GAMEVIEW_H
