#include "testminigame.h"

#include <QMouseEvent>
#include <QRandomGenerator64>

#include "src/game/game_objects/clickableball.h"

TestMinigame::TestMinigame(QGraphicsView* graphics_view, float difficulty)
    : Minigame(graphics_view, difficulty) {
  // Random coefs just for testing the basic game loop
  time_ = qRound(kBasicDuration / (difficulty_ * 1.5f + 1.0f));
  balls_count_ = kBasicBallNumber + qRound(difficulty_ / 0.2f);
  ball_radius_ = qRound(kBasicBallRadius * (1 - difficulty_));
  time_bar_->setVisible(false);
  is_running_ = false;
}

TestMinigame::~TestMinigame() {}

void TestMinigame::Start() {
  graphics_view_->scene()->setBackgroundBrush(
      QBrush(QColor::fromRgb(227, 124, 7)));
  AddBall();
  AnimateTutorial();
}

void TestMinigame::AnimateTutorial() {
  // Random coefs just for testing the basic game loop
  tutorial_label_->setHtml("[TUTORIAL]");
  tutorial_label_->setDefaultTextColor(Qt::white);
  tutorial_label_->setTextWidth(300);
  tutorial_label_->setZValue(100);

  timer_->setInterval(kTutorialDuration);
  connect(timer_, &QTimer::timeout, this, [this] {
    timer_->stop();
    tutorial_label_->setVisible(false);
    time_bar_->setVisible(true);
    StartGame();
  });
  timer_->start();
}

void TestMinigame::StartGame() {
  timer_->setInterval(time_);
  connect(timer_, &QTimer::timeout, this, &TestMinigame::Stop);

  tick_timer_->setInterval(1000 / kFps);
  connect(tick_timer_, &QTimer::timeout, this, &TestMinigame::Tick);

  is_running_ = true;

  timer_->start();
  tick_timer_->start();
}

void TestMinigame::AnimateOutro() {}

void TestMinigame::Tick() {
  if (!is_running_) {
    return;
  }
  timer_->remainingTime();
  time_bar_->SetProgress(1.0f * timer_->remainingTime() / kBasicDuration);

  time_bar_->update();
}

void TestMinigame::AddBall() {
  graphics_view_->scene()->addItem(new ClickableBall(
      graphics_view_, ball_radius_ * 2, ball_radius_ * 2,
      (QRandomGenerator::global()->bounded(graphics_view_->width()) -
       graphics_view_->width() / 2) *
          0.8f,
      (QRandomGenerator::global()->bounded(graphics_view_->height()) -
       graphics_view_->height() / 2) *
          0.8f));
}

void TestMinigame::Stop() {
  is_running_ = false;
  tick_timer_->stop();
  timer_->stop();
  if (balls_count_ == 0) {
    Win();
  } else {
    Lose();
  }
}

void TestMinigame::Win() {
  time_bar_->setVisible(false);
  timer_->setInterval(kOutroDuration);
  graphics_view_->scene()->setBackgroundBrush(QColor::fromRgb(1, 143, 8));
  connect(timer_, &QTimer::timeout, this, [this] {
    timer_->stop();
    graphics_view_->scene()->setBackgroundBrush(Qt::NoBrush);
    emit Passed(points_);
  });
  timer_->start();
}

void TestMinigame::Lose() {
  time_bar_->setVisible(false);
  timer_->setInterval(kOutroDuration);
  graphics_view_->scene()->setBackgroundBrush(QColor::fromRgb(191, 8, 8));
  connect(timer_, &QTimer::timeout, this, [this] {
    timer_->stop();
    graphics_view_->scene()->setBackgroundBrush(Qt::NoBrush);
    emit Failed();
  });
  timer_->start();
}

void TestMinigame::MousePressEvent(QMouseEvent* event) {
  if (!is_running_) {
    return;
  }
  graphics_view_->scene()->setBackgroundBrush(
      QBrush(QColor::fromRgb(200, 0, 0)));
  ClickableBall* clicked_ball =
      dynamic_cast<ClickableBall*>(graphics_view_->itemAt(event->pos()));
  if (clicked_ball != nullptr) {
    graphics_view_->scene()->removeItem(clicked_ball);
    delete clicked_ball;
    balls_count_--;
    if (balls_count_ == 0) {
      // Need to calculate it here as timer_ interval bonus
      // is calculated as 0 in Stop()
      points_ = 100 + timer_->remainingTime() * 10 / timer_->interval();
      Stop();
    } else {
      AddBall();
    }
  }
}

void TestMinigame::MouseReleaseEvent(QMouseEvent*) {
  if (!is_running_) {
    return;
  }
  graphics_view_->scene()->setBackgroundBrush(
      QBrush(QColor::fromRgb(227, 124, 7)));
}

void TestMinigame::MouseMoveEvent(QMouseEvent*) {}

void TestMinigame::KeyPressEvent(QKeyEvent*) {
  if (!is_running_) {
    return;
  }
  graphics_view_->scene()->setBackgroundBrush(
      QBrush(QColor::fromRgb(0, 0, 200)));
}

void TestMinigame::KeyReleaseEvent(QKeyEvent*) {
  if (!is_running_) {
    return;
  }
  graphics_view_->scene()->setBackgroundBrush(
      QBrush(QColor::fromRgb(227, 124, 7)));
}