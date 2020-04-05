#include "testminigame.h"

#include <QMouseEvent>
#include <QRandomGenerator64>

#include "src/game/game_objects/clickableball.h"

TestMiniGame::TestMiniGame(QGraphicsView* graphics_view, qreal difficulty)
    : MiniGame(graphics_view, difficulty) {
  // Random coefs just for testing the basic game loop
  time_ = qRound(kBasicDuration / (difficulty_ * 1.5 + 1.0));
  balls_count_ = kBasicBallNumber + qRound((difficulty_ / 0.2));
  ball_raduis_ = qRound(kBasicBallRadius * (1 - difficulty_));
  time_bar_->setVisible(false);
  is_running_ = false;
}

TestMiniGame::~TestMiniGame() {}

void TestMiniGame::Start() {
  graphics_view_->scene()->setBackgroundBrush(
      QBrush(QColor::fromRgb(227, 124, 7)));
  AddBall();
  AnimateTutorial();
}

void TestMiniGame::AnimateTutorial() {
  // Random coefs just for testing the basic game loop
  tutorial_label_->setHtml("[TUTORIAL]");
  tutorial_label_->setDefaultTextColor(Qt::white);
  tutorial_label_->setTextWidth(300);
  graphics_view_->scene()->addItem(tutorial_label_);

  timer_->setInterval(kTutorialDuration);
  connect(timer_, &QTimer::timeout, this, [=] {
    timer_->stop();
    tutorial_label_->setVisible(false);
    time_bar_->setVisible(true);
    StartGame();
  });
  timer_->start();
}

void TestMiniGame::StartGame() {
  timer_->setInterval(kBasicDuration);
  connect(timer_, &QTimer::timeout, this, &TestMiniGame::Stop);

  tick_timer_->setInterval(1000 / kFps);
  connect(tick_timer_, &QTimer::timeout, this, &TestMiniGame::Tick);

  is_running_ = true;

  timer_->start();
  tick_timer_->start();
}

void TestMiniGame::AnimateOutro() {}

void TestMiniGame::Tick() {
  if (!is_running_) {
    return;
  }
  time_bar_->SetProgress(1.0 * timer_->remainingTime() / kBasicDuration);

  time_bar_->update();
}

void TestMiniGame::AddBall() {
  graphics_view_->scene()->addItem(new ClickableBall(
      graphics_view_, ball_raduis_ * 2, ball_raduis_ * 2,
      (QRandomGenerator::global()->bounded(graphics_view_->width()) -
       graphics_view_->width() / 2) *
          0.8,
      (QRandomGenerator::global()->bounded(graphics_view_->height()) -
       graphics_view_->height() / 2) *
          0.8));
}

void TestMiniGame::Stop() {
  is_running_ = false;
  tick_timer_->stop();
  timer_->stop();
  if (balls_count_ == 0) {
    Win();
  } else {
    Lose();
  }
}

void TestMiniGame::Win() {
  time_bar_->setVisible(false);
  timer_->setInterval(kOutroDuration);
  graphics_view_->scene()->setBackgroundBrush(QColor::fromRgb(1, 143, 8));
  connect(timer_, &QTimer::timeout, this, [=] {
    timer_->stop();
    graphics_view_->scene()->setBackgroundBrush(Qt::NoBrush);
    emit Passed(points_);
  });
  timer_->start();
}

void TestMiniGame::Lose() {
  time_bar_->setVisible(false);
  timer_->setInterval(kOutroDuration);
  graphics_view_->scene()->setBackgroundBrush(QColor::fromRgb(191, 8, 8));
  connect(timer_, &QTimer::timeout, this, [=] {
    timer_->stop();
    graphics_view_->scene()->setBackgroundBrush(Qt::NoBrush);
    emit Failed();
  });
  timer_->start();
}

void TestMiniGame::MousePressEvent(QMouseEvent* event) {
  if (!is_running_) {
    return;
  }
  graphics_view_->scene()->setBackgroundBrush(
      QBrush(QColor::fromRgb(200, 0, 0)));
  ClickableBall* ball_at =
      dynamic_cast<ClickableBall*>(graphics_view_->itemAt(event->pos()));
  if (ball_at != nullptr) {
    graphics_view_->scene()->removeItem(ball_at);
    delete ball_at;
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

void TestMiniGame::MouseReleaseEvent(QMouseEvent*) {
  if (!is_running_) {
    return;
  }
  graphics_view_->scene()->setBackgroundBrush(
      QBrush(QColor::fromRgb(227, 124, 7)));
}

void TestMiniGame::MouseMoveEvent(QMouseEvent*) {}

void TestMiniGame::KeyPressEvent(QKeyEvent*) {
  if (!is_running_) {
    return;
  }
  graphics_view_->scene()->setBackgroundBrush(
      QBrush(QColor::fromRgb(0, 0, 200)));
}

void TestMiniGame::KeyReleaseEvent(QKeyEvent*) {
  if (!is_running_) {
    return;
  }
  graphics_view_->scene()->setBackgroundBrush(
      QBrush(QColor::fromRgb(227, 124, 7)));
}