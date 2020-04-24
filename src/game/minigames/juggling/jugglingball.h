#ifndef JUGGLINGBALL_H
#define JUGGLINGBALL_H

#include "src/game/gameobject.h"

class JugglingBall : public GameObject {
 public:
  JugglingBall(GameView* graphics_view, qreal width, qreal height, qreal x = 0,
               qreal y = 0, qreal floor_y_ = 0);
  ~JugglingBall() override;
  void SetUp() override;

  void Update() override;

  void SetCaught(bool is_caught);
  qreal GetRadius() const;

 private:
  static const qreal kZValue;

  qreal radius_;
  qreal floor_y_;
  bool is_caught_ = false;
};

#endif  // JUGGLINGBALL_H
