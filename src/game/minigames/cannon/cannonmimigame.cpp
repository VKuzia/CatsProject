#include "cannonmimigame.h"

#include <QKeyEvent>

Cannonmimigame::Cannonmimigame(GameView* game_view, qreal difficulty,
                               qreal pixels_in_meter)
    : Minigame(game_view, difficulty, pixels_in_meter) {}

void Cannonmimigame::Start() { AnimateTutorial(); }

void Cannonmimigame::SetUp() {
  background_->SetUp(game_view_, "cannon/circus.png");
  game_view_->scene()->addItem(background_);

  cannon_ = new Cannon(game_view_, kCatWidth, kCatHeight, -6, kCatY);
  cannon_->SetUp();
  game_view_->scene()->addItem(cannon_);

  cat_ = new Cannoncat(game_view_, kCatWidth, kCatHeight, -6, kCatY);
  cat_->SetUp();
  game_view_->scene()->addItem(cat_);

  cannon_ =
      new Cannon(game_view_, 2.5 * kCatWidth, 1.5 * kCatHeight, -6, kCatY);
  cannon_->SetUp();
  game_view_->scene()->addItem(cannon_);

  arrow_ = new Arrow(game_view_, kCatWidth, kCatHeight, 6, -kCatY);
  arrow_->SetUp();
  game_view_->scene()->addItem(arrow_);

  speedometer_ = new Speedometer(game_view_, kCatWidth, kCatHeight, 7, -kCatY);
  speedometer_->SetUp();
  game_view_->scene()->addItem(speedometer_);

  SetUpLabel();
  time_bar_->setVisible(false);
  sausage_timer_.setInterval(sausage_launch_period_);
  for (int i = 0; i < sausage_count_; i++) {
    LaunchSausage();
  }
  for (int i = 0; i < number_to_win_; i++) {
    No* no = new No(game_view_, 1, 1, -7 + 1.5 * i, -kCatY - 1.5);
    no->SetUp();
    not_caught_.insert(no);
    game_view_->scene()->addItem(no);
  }
  for (int i = 0; i < number_to_win_; i++) {
    Yes* yes = new Yes(game_view_, 1, 1, -7 + 1.5 * i, -kCatY - 1.5);
    yes->SetUp();
    caught_.insert(yes);
    yes->setVisible(false);
    game_view_->scene()->addItem(yes);
  }
}

void Cannonmimigame::SetUpLabel() {
  tutorial_label_->setHtml("[TUTORIAL]");
  tutorial_label_->setDefaultTextColor(Qt::black);
  tutorial_label_->setTextWidth(300);
  tutorial_label_->setZValue(100);
  tutorial_label_->setPos(0, 0);
  tutorial_label_->setVisible(false);
}

void Cannonmimigame::AnimateTutorial() {
  tutorial_label_->setVisible(true);
  QTimer::singleShot(kTutorialDuration, [this] {
    tutorial_label_->setVisible(false);
    StartGame();
  });
}

void Cannonmimigame::StartGame() {
  tick_timer_.setInterval(1000 / kFps);
  connect(&tick_timer_, &QTimer::timeout, this, &Cannonmimigame::Tick);

  is_running_ = true;

  tick_timer_.start();
  sausage_timer_.start();
}

void Cannonmimigame::AnimateOutro() {}

void Cannonmimigame::Tick() {
  if (!is_running_) {
    return;
  }
  if (!params_choosen_power_) {
    if (power_increases_) {
      power_ += 0.001;
      arrow_->setRotation(arrow_->rotation() + M_PI * (power_ / 0.12));
      if (power_ >= 0.12) {
        power_increases_ = false;
      }
    } else {
      power_ -= 0.001;
      arrow_->setRotation(arrow_->rotation() - M_PI * (power_ / 0.12));
      if (power_ <= 0) {
        power_increases_ = true;
      }
    }
  }
  if (!params_choosen_angle_) {
    if (angle_increases_) {
      angle_ += M_PI / 200;
      cat_->setRotation(cat_->rotation() - angle_);
      cannon_->setRotation(cat_->rotation() - angle_);
      if (angle_ > M_PI / 2) {
        angle_increases_ = false;
      }
    } else {
      angle_ -= M_PI / 200;
      cat_->setRotation(cat_->rotation() + angle_);
      cannon_->setRotation(cat_->rotation() + angle_);
      if (angle_ <= 0) {
        angle_increases_ = true;
      }
    }
  }

  if (cat_flight) {
    cat_->Update();
    if (cat_->was_caught_last_tick_ && cat_->GetCaught() <= number_to_win_) {
      (*(not_caught_.begin() + cat_->GetCaught() - 1))->setVisible(false);
      (*(caught_.begin() + cat_->GetCaught() - 1))->setVisible(true);
    }
  } else if (params_choosen_angle_ && params_choosen_power_) {
    cat_->SetAngle(angle_);
    cat_->SetPower(power_);
    cat_flight = true;
  }
  if (cat_->GetY() >= kFloorHeight) {
    cat_->SetFallen(true);
    if (cat_->GetCaught() >= number_to_win_) {
      Stop(Status::kPass);
    } else {
      Stop(Status::kFail);
    }
  }
  for (auto item : sausages_) {
    item->Update();
  }
}

void Cannonmimigame::SetUpParameters() {
  sausage_launch_period_ = 100;
  sausage_a_param = QRandomGenerator().global()->bounded(100) * 1.0 / 100;
  sausage_b_param = QRandomGenerator().global()->bounded(100) * 1.0 / 100;
  int32_t difficulty_level = qFloor(difficulty_ / 0.1);
  switch (difficulty_level) {
    case 1:
      sausage_count_ = 7;
      number_to_win_ = 2;
      break;
    case 2:
      sausage_count_ = 7;
      number_to_win_ = 3;
      break;
    case 3:
      sausage_count_ = 6;
      number_to_win_ = 3;
      break;
    case 4:
      sausage_count_ = 6;
      number_to_win_ = 4;
      break;
    case 5:
      sausage_count_ = 5;
      number_to_win_ = 4;
      break;
    case 6:
      sausage_count_ = 5;
      number_to_win_ = 4;
      break;
    case 7:
      sausage_count_ = 4;
      number_to_win_ = 4;
      break;
    case 8:
      sausage_count_ = 4;
      number_to_win_ = 4;
      break;
    case 9:
      sausage_count_ = 4;
      number_to_win_ = 4;
      break;
    default:
      sausage_count_ = 3;
      number_to_win_ = 3;
      break;
  }
}

void Cannonmimigame::Stop(Status status) {
  is_running_ = false;
  tick_timer_.stop();
  sausage_timer_.stop();
  time_bar_->setVisible(false);
  if (status == Status::kPass) {
    score_ = 100;
    Win();
  }
  if (status == Status::kFail) {
    Lose();
  }
}

void Cannonmimigame::Win() {
  game_view_->scene()->setBackgroundBrush(kWinBackgroundBrush);
  QTimer::singleShot(kOutroDuration, this, [this] {
    game_view_->scene()->setBackgroundBrush(kEmptyBackgroundBrush);
    emit Passed(score_);
  });
}

void Cannonmimigame::Lose() {
  game_view_->scene()->setBackgroundBrush(kLoseBackgroundBrush);
  QTimer::singleShot(kOutroDuration, this, [this] {
    game_view_->scene()->setBackgroundBrush(kEmptyBackgroundBrush);
    emit Failed();
  });
}

void Cannonmimigame::KeyPressEvent(QKeyEvent* event) {
  if (!is_running_) {
    return;
  }
  if (event->key() == Qt::Key_A) {
    params_choosen_angle_ = true;
  } else if (event->key() == Qt::Key_D) {
    params_choosen_power_ = true;
  } else if (event->key() == Qt::Key_Space) {
    params_choosen_angle_ = true;
    params_choosen_power_ = true;
  }
}

void Cannonmimigame::LaunchSausage() {
  if (sausages_.size() >= sausage_count_) {
    return;
  }

  qreal SausageX = QRandomGenerator().global()->bounded(-6, 6) +
                   QRandomGenerator().global()->bounded(100) * 1.0 / 100;

  Sausage* sausage =
      new Sausage(game_view_, KSausageRadius * 2, KSausageRadius * 2, SausageX,
                  sausage_a_param * SausageX * SausageX / 2 +
                      sausage_b_param * SausageX - 3);
  if (sausages_.size() % 2 == 0) {
    sausage->move_down = false;
  }
  sausages_.insert(sausage);
  sausage->SetUp();
  game_view_->scene()->addItem(sausage);
}
