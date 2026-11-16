#include "StatusBar.h"

#include "view/SizeDefinitions.h"

StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent) {
  setUpGui();
  loadImages();
}

void StatusBar::setUpGui() {
  setContentsMargins(Size::STATUS_PADDING, Size::STATUS_PADDING / 2, Size::STATUS_PADDING, Size::STATUS_PADDING / 2);
  setMinimumHeight(Size::STATUS_HEIGHT);
  setSizeGripEnabled(false);

  progress_indicator = new QProgressBar(this);
  progress_indicator->setRange(0, 0);
  progress_indicator->setTextVisible(false);
  progress_indicator->setMaximumWidth(100);
  progress_indicator->setVisible(false);

  label_img = new QLabel(this);

  // Long messages may be cut off instead of widening the window
  label_message = new QLabel(this);
  label_message->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
  addWidget(label_message, 1);

  addPermanentWidget(progress_indicator);
  addPermanentWidget(label_img);
}

void StatusBar::displayMessage(const QString& message, MessageTyp typ) {
  label_message->setText(message);
  label_message->setToolTip(message);
  QPixmap img;
  switch(typ){
    case MessageTyp ::Error:img = icons_[0].pixmap(16);break;
    case MessageTyp ::OK:img = icons_[1].pixmap(16);break;
    case MessageTyp ::NONE:img = QPixmap();break;
  }
  label_img->setPixmap(img);
}

void StatusBar::clearDisplayedMessage() {
  label_img->setPixmap(QPixmap());
  label_message->clear();
  label_message->setToolTip(QString());
}

void StatusBar::showIndicator(bool value) {
  progress_indicator->setVisible(value);
}

void StatusBar::loadImages() {
  icons_[0] = QIcon(":icons/error.svg");
  icons_[1] = QIcon(":icons/ok.svg");
}
