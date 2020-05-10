#include "settingswidget.h"
#include <QFile>
#include <QTextStream>

#include "mainwindow.h"
#include "ui_settingswidget.h"


SettingsWidget::SettingsWidget(QWidget* parent)
    : QWidget(parent), ui_(new Ui::SettingsWidget) {
  ui_->setupUi(this);
  ui_->ui_language_combo_box_->addItem("english");
  ui_->ui_language_combo_box_->addItem("русский");
  ui_->ui_language_combo_box_->addItem("беларуская");
  ui_->ui_resolution_combo_box_->addItem("1024×576");
  ui_->ui_resolution_combo_box_->addItem("1280×800");
  ui_->ui_resolution_combo_box_->addItem("1920×1080");
  ui_->ui_resolution_combo_box_->addItem("2048×1100");

  resolutions_[0].setWidth(1024);
  resolutions_[0].setHeight(576);
  resolutions_[1].setWidth(1280);
  resolutions_[1].setHeight(800);
  resolutions_[2].setWidth(1920);
  resolutions_[2].setHeight(1080);
  resolutions_[3].setWidth(2048);
  resolutions_[3].setHeight(1100);

  Load();
}

SettingsWidget::~SettingsWidget() { delete ui_; }

void SettingsWidget::ReturnToMainMenu() {
    Save();
    emit MainMenu();
}

void SettingsWidget::Save() {
    QFile file(kPathToSettings + "basic_settings.txt");
    try {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw std::runtime_error("Error, imposible to save changes!");
        }
    }
    catch(std::exception const& exception) {
        QTextStream error(stdout);
        error << exception.what() << "\n";
    }

    QTextStream save(&file);
    save.setCodec("UTF-8");

//    save << resolution_.width() << " " << resolution_.height() << "\n";

    save << volume_off_ << "\n";

    volume_ = ui_->ui_volume_->value();
    save << volume_ << "\n";

//    save << current_resolution_index_ << "\n";

    save << ui_->ui_resolution_combo_box_->currentIndex() << "\n";

    current_language_index_ = ui_->ui_language_combo_box_->currentIndex();
    save << current_language_index_ << "\n";

    user_name_ = ui_->ui_user_line_edit_->text();
    save << user_name_ << "\n";

    file.close();
}

void SettingsWidget::Load() {
    QFile file(kPathToSettings + "basic_settings.txt");
    try {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw std::runtime_error("Error, imposible to load changes!");
        }
    }
    catch(std::exception const& exception) {
        QTextStream error(stdout);
        error << exception.what() << "\n";
    }

    QTextStream load(&file);
    load.setCodec("UTF-8");

//    QString resolution_line = load.readLine();
//    QStringList resolution_pair = resolution_line.split(' ');
//    resolution_.setWidth(resolution_pair[0].toInt());
//    resolution_.setHeight(resolution_pair[1].toInt());

    QString volume_off_line = load.readLine();
    int64_t volume_off_int_ = volume_off_line.toInt();
    volume_off_ = (volume_off_int_ == 0 ? false : true);
    ui_->ui_sound_check_box_->setChecked(volume_off_);

    QString volume_line = load.readLine();
    volume_ = volume_line.toInt();
    ui_->ui_volume_->setValue(volume_);

//    QString current_resolution_index_line = load.readLine();
//    current_resolution_index_ = current_resolution_index_line.toInt();
//    ui_->ui_resolution_combo_box_->setCurrentIndex(current_resolution_index_);

    QString current_resolution_line = load.readLine();
    ui_->ui_resolution_combo_box_->setCurrentIndex(
                current_resolution_line.toInt());

    QString current_language_index_line = load.readLine();
    current_language_index_ = current_language_index_line.toInt();
    ui_->ui_language_combo_box_->setCurrentIndex(current_language_index_);

    user_name_ = load.readLine();
    ui_->ui_user_line_edit_->setText(user_name_);
}

QSize SettingsWidget::GetResolution() const {
//    return resolution_;
    return resolutions_[ui_->ui_resolution_combo_box_->currentIndex()];
}

void SettingsWidget::ChangeSound() {
    if (ui_->ui_sound_check_box_->isChecked()) {
        ui_->ui_sound_check_box_->setText("Off");
        volume_off_ = true;
        return;
    }
    ui_->ui_sound_check_box_->setText("On");
    volume_off_ = false;
}

void SettingsWidget::ChangeResolution() {
//    switch (ui_->ui_resolution_combo_box_->currentIndex()) {
//    case 0:
//        resolution_.setWidth(1024);
//        resolution_.setHeight(576);
//        break;
//    case 1:
//        resolution_.setWidth(1280);
//        resolution_.setHeight(800);
//        break;
//    case 2:
//        resolution_.setWidth(1920);
//        resolution_.setHeight(1080);
//        break;
//    case 3:
//        resolution_.setWidth(4096);
//        resolution_.setHeight(2160);
//        break;
//    }
//    current_resolution_index_ = ui_->ui_resolution_combo_box_->currentIndex();
    emit ResolutionChanged();
}

