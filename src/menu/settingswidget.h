#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QDir>
#include <QWidget>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget {
  Q_OBJECT

 public:
  explicit SettingsWidget(QWidget* parent = nullptr);
  ~SettingsWidget() override;

  void Load();
  QSize GetResolution() const;
  QString GetLanguage() const;
  QString GetPlayerName() const;

 signals:
  void MainMenu();
  void ResolutionChanged();
  void LanguageChanged();
  void PlayerNameChanged();

 public slots:
  void ReturnToMainMenu();

  void ChangeSound();
  void ChangeVolume();
  void ChangeLanguage();
  void ChangeUserName();
  void ChangeResolution();

 private:
  const QString kPathToSettings = QDir::currentPath() + "/data/settings/";
  const QVector<QSize> kResolutions_ = {
      {1024, 576},  {1280, 720},  {1366, 768},  {1600, 900},
      {1920, 1080}, {2560, 1440}, {3200, 1800}, {3840, 2160}};
  const QVector<QString> kLanguages_ = {"english"};

  bool volume_on_ = true;
  int volume_ = 1;
  QString player_name_ = "Player";
  QSize resolution_ = {1024, 576};

  Ui::SettingsWidget* ui_;

  void Save() const;

  void resizeEvent(QResizeEvent* event) override;
};

#endif  // SETTINGSWIDGET_H
