#include "plateminigame.h"

#include <QMouseEvent>
#include <QRandomGenerator64>
#include <limits>

PlateMinigame::PlateMinigame(GameView* game_view, qreal difficulty,
                             qreal pixels_in_meter)
    : Minigame(game_view, difficulty, pixels_in_meter) {}

void PlateMinigame::SetUp() {
  background_->SetUp(game_view_, "plate/plate.png");
  game_view_->scene()->addItem(background_);

  SetUpLabel(kTutorialText_);

  AddBall();
}

void PlateMinigame::SetUpParameters() {
  // Random coefs just for testing the basic game loop
  time_ = qRound(kBasicDuration / (difficulty_ * 1.5 + 1.0));
  balls_count_ = kBasicBallNumber + qRound(difficulty_ / 0.2);
  ball_radius_ = qRound(kBasicBallRadius * (1 - difficulty_));
}
void PlateMinigame::Start() { AnimateTutorial(); }

void PlateMinigame::AnimateTutorial() {
  QTimer::singleShot(kTutorialDuration, this, [this] { StartGame(); });
}

void PlateMinigame::StartGame() {
  time_bar_->setVisible(true);
  tutorial_label_->setVisible(false);
  time_bar_->Launch(time_);
  QTimer::singleShot(time_, this, [this] {
    if (is_running_) {
      Stop(MinigameStatus::kFailed);
    }
  });
  tick_timer_.setInterval(1000 / kFps);
  connect(&tick_timer_, &QTimer::timeout, this, &PlateMinigame::Tick);

  is_running_ = true;

  tick_timer_.start();
}

void PlateMinigame::Tick() {}

void PlateMinigame::AddBall() {
  QPointF center = GetRandomBallCenter();
  ClickableBall* ball =
      new ClickableBall(game_view_, ball_radius_ * 2, ball_radius_ * 2, center);
  ball->SetUp();
  connect(ball, &ClickableBall::Clicked, this, &PlateMinigame::DeleteBall);
  current_ball_ = ball;
  game_view_->scene()->addItem(ball);
}

void PlateMinigame::DeleteBall() {
  if (current_ball_ == nullptr || !is_running_) {
    return;
  }
  game_view_->scene()->removeItem(current_ball_);
  delete current_ball_;
  current_ball_ = nullptr;
  balls_count_--;
  if (balls_count_ == 0) {
    time_left_ = time_ - time_bar_->GetCurrentTime();
    Stop(MinigameStatus::kPassed);
  } else {
    AddBall();
  }
}

QPointF PlateMinigame::GetRandomBallCenter() const {
  // Scene's (0,0) point is in its centre.
  // That's why we subtract a half of width(height)
  // Then scale for center being inside but the edges
  qreal x = (QRandomGenerator::global()->bounded(game_view_->width()) -
             game_view_->width() / 2) *
            kCenterRegionFactor;
  qreal y = (QRandomGenerator::global()->bounded(game_view_->height()) -
             game_view_->height() / 2) *
            kCenterRegionFactor;
  x /= game_view_->GetPixelsInMeter();
  y /= game_view_->GetPixelsInMeter();
  return QPointF(x, y);
}

void PlateMinigame::Stop(MinigameStatus status) {
  is_running_ = false;
  tick_timer_.stop();
  time_bar_->setVisible(false);
  switch (status) {
    case MinigameStatus::kPassed:
      score_ = 100 + time_left_ / 50;
      Win();
      break;
    case MinigameStatus::kFailed:
      Lose();
      break;
  }
}
