#include "Controlbar.h"

#include <QtWidgets/QVBoxLayout>

#include "view/SizeDefinitions.h"

ControlBar::ControlBar(QWidget *parent) : QWidget(parent) {
  setUpGui();
  setConnections();
}

void ControlBar::setUpGui() {
  // Set up button (Ctrl+R, Cmd+R on macOS)
  button_calculate_ = new QPushButton(QIcon(":icons/calculate.svg"), tr("Calculate"), this);
  button_calculate_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
  button_calculate_->setToolTip(tr("Calculate the integral and update the plot (%1)")
                                    .arg(button_calculate_->shortcut().toString(QKeySequence::NativeText)));
  button_calculate_->setDefault(true);
  button_calculate_->setMinimumHeight(Size::BUTTON_HEIGHT);
  // Primary action: flat accent color
  QColor accent = palette().color(QPalette::Accent);
  button_calculate_->setStyleSheet(QString(
      "QPushButton { background: %1; color: white; border: none; border-radius: 6px; font-weight: 600; }"
      "QPushButton:hover { background: %2; }"
      "QPushButton:pressed { background: %3; }"
      "QPushButton:disabled { background: %4; color: %5; }")
      .arg(accent.name(), accent.lighter(110).name(), accent.darker(115).name(),
           palette().color(QPalette::Button).name(),
           palette().color(QPalette::Disabled, QPalette::ButtonText).name()));
  // Button finished

  // Set up layout
  auto *layout = new QVBoxLayout(this);
  layout->addWidget(button_calculate_);
  layout->setContentsMargins(12, 8, 12, 12);
  // Layout finished
}

void ControlBar::setConnections() {
  connect(button_calculate_, &QPushButton::clicked, this, &ControlBar::handleRefreshClick);
}

void ControlBar::handleRefreshClick() {
  emit refreshClicked();
}

void ControlBar::blockInput(bool value) {
  setDisabled(value);
}
