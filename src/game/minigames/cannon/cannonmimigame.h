#ifndef CANNONMIMIGAME_H
#define CANNONMIMIGAME_H
#include <QGraphicsEllipseItem>
#include <QRandomGenerator>
#include <QSet>

#include "src/game/minigame.h"
#include "src/game/minigames/cannon/cannoncat.h"
#include "src/game/minigames/cannon/cannonsatus.h"
#include "src/game/minigames/cannon/cannonsausage.h"

class CannonMinigame : public Minigame {
  Q_OBJECT

 public:
  CannonMinigame(GameView* game_view, qreal difficulty,
                 qreal pixels_in_meter = 576 / 10.0);
  ~CannonMinigame() override = default;

  void Start() override;

 public slots:
  void SausageWasCaught();

 private:
  const QString kTutorialText_ =
      "Use A and D keys to set power and angle.\nCatch as many sausages as you "
      "can!";

  const qreal kCatWidth = 1.5;
  const qreal kCatHeight = 2.1;
  const qreal kCatY = 3;
  const qreal kCatX = -6;

  const qreal kCannonWidth = 6;
  const qreal kCannonHeight = 5;
  const qreal kCannonY = 3;
  const qreal KCannonX = -6;

  const qreal kSpeedometerX = 7;
  const qreal kSpeedometerY = -3;

  const qreal kStatusStartX = -7;
  const qreal kStatusStartY = -4;
  const qreal kStatusWidth = 1;
  const qreal kStatusHeight = 1;
  const qreal KStatusDeltaX = 1.5;

  const qreal kPowerDelta = 0.001;
  const qreal kAngleDelta = M_PI / 200;
  const qreal kMaxPower = 0.12;

  const QPoint kSausageXBoders = {-6, 6};

  const qreal KTutorialHeight = -1.5;

  const qreal KSausageRadius = 1.2;

  const qreal kFloorHeight = 3.2;

  // Parameters of quadratic equation - (ax^2 + bx + c) generation
  qreal sausage_a_param = 0;
  qreal sausage_b_param = 0;

  qreal angle_ = M_PI / 2;
  qreal power_ = 0;
  bool angle_increases_ = true;
  bool power_increases_ = true;
  bool is_cat_flying_ = false;
  bool params_choosen_angle_ = false;
  bool params_choosen_power_ = false;

  int32_t sausage_count_ = 0;
  int32_t number_to_win_ = 0;
  int32_t current_score_ = 0;

  GameObject* cannon_ = nullptr;
  GameObject* speedometer_ = nullptr;
  GameObject* arrow_ = nullptr;
  CannonCat* cat_ = nullptr;
  QSet<CannonSausage*> sausages_;

  QVector<CannonStatus*> status_bar_;

  void LaunchSausage();
  void SetUp() override;
  void SetUpParameters() override;

  void AnimateTutorial() override;
  void StartGame() override;

  void Tick() override;
  void ChangeParameters();

  void Stop(MinigameStatus) override;

  void KeyPressEvent(QKeyEvent* event) override;
};

#endif  // CANNONMIMIGAME_H
