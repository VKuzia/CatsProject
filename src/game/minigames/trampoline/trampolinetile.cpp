#include "trampolinetile.h"

#include <QGraphicsColorizeEffect>

TrampolineTile::TrampolineTile(GameView* game_view, QSizeF size, qreal x,
                               qreal y)
    : GameObject(game_view, size, x, y) {
  setOpacity(kNoFocusOpacity_);

  animation_.setTargetObject(this);
  animation_.setPropertyName("animationProgress");
  animation_.setDuration(kScaleAnimationDuration_);
  animation_.setStartValue(0);
  animation_.setEndValue(1);
}

void TrampolineTile::Activate() {
  animation_.setDirection(QPropertyAnimation::Direction::Forward);
  animation_.start();
}

void TrampolineTile::Deactivate(bool is_path_correct) {
  QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect();
  if (is_path_correct) {
    effect->setColor(kPassColor_);
  } else {
    effect->setColor(kFailColor_);
  }
  effect->setStrength(kEffectStrength_);
  setGraphicsEffect(effect);

  animation_.setDirection(QPropertyAnimation::Direction::Backward);
  animation_.start();
}

bool TrampolineTile::CheckPath(const TrampolinePath& path_item_, QPointF start,
                               QPointF finish) {
  bool result = false;
  qreal path_width =
      path_item_.path().boundingRect().width() + 1;  // To prevent zero division
  qreal path_height = path_item_.path().boundingRect().height() + 1;
  Vector2D shift(finish.x() - start.x(), finish.y() - start.y());
  switch (direction_) {
    case SwipeDirection::kUp:
      if (path_height / path_width > kMinimalPathRectRatio_ &&
          shift.y() < -kMinimalSwipeLength_ * game_view_->GetPixelsInMeter()) {
        result = true;
      }
      break;
    case SwipeDirection::kDown:
      if (path_height / path_width > kMinimalPathRectRatio_ &&
          shift.y() > kMinimalSwipeLength_ * game_view_->GetPixelsInMeter()) {
        result = true;
      }
      break;
    case SwipeDirection::kLeft:
      if (path_width / path_height > kMinimalPathRectRatio_ &&
          shift.x() < -kMinimalSwipeLength_ * game_view_->GetPixelsInMeter()) {
        result = true;
      }
      break;
    case SwipeDirection::kRight:
      if (path_width / path_height > kMinimalPathRectRatio_ &&
          shift.x() > kMinimalSwipeLength_ * game_view_->GetPixelsInMeter()) {
        result = true;
      }
      break;
  }
  return result;
}

void TrampolineTile::SetDirection(TrampolineTile::SwipeDirection direction) {
  direction_ = direction;
  setGraphicsEffect(nullptr);
  switch (direction_) {
    case SwipeDirection::kUp:
      setPixmap(LoadPixmap("trampoline/up.png"));
      break;
    case SwipeDirection::kDown:
      setPixmap(LoadPixmap("trampoline/down.png"));
      break;
    case SwipeDirection::kLeft:
      setPixmap(LoadPixmap("trampoline/left.png"));
      break;
    case SwipeDirection::kRight:
      setPixmap(LoadPixmap("trampoline/right.png"));
      break;
  }
}

qreal TrampolineTile::GetAnimationProgress() const {
  return animation_progress_;
}

void TrampolineTile::SetAnimationProgress(qreal progress) {
  animation_progress_ = progress;
  setOpacity(kNoFocusOpacity_ + (1 - kNoFocusOpacity_) * animation_progress_);
  setScale(1 + kScaleAnimationAmount_ * progress);
}
