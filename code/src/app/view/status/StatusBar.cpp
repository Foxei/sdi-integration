#include "StatusBar.h"

#include <QIcon>

StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent) {
  setUpGui();
  loadImages();
}

void StatusBar::setUpGui() {

  label_indicator = new QLabel(this);
  label_img = new QLabel(this);

  auto *movie = new QMovie(label_indicator);
  movie->setFileName(tr("res/loader.gif"));
  movie->start();
  label_indicator->setMargin(0);
  label_indicator->setMovie(movie);
  label_indicator->setVisible(false);

  addPermanentWidget(label_indicator);
  addPermanentWidget(label_img);

}

void StatusBar::displayMessage(const QString& message, MessageTyp typ) {
  showMessage(message);
  QPixmap img;
  switch(typ){
    case MessageTyp ::Error:img = images[0];break;
    case MessageTyp ::OK:img = images[1];break;
    case MessageTyp ::NONE:img = QPixmap();break;
  }
  label_img->setPixmap(img);
}

void StatusBar::clearDisplayedMessage() {
  label_img->setPixmap(QPixmap());
  clearMessage();
}

void StatusBar::showIndicator(bool value) {
  label_indicator->setVisible(value);
}

void StatusBar::loadImages() {
  QIcon risk_icon(":risk.png");
  images[0] = risk_icon.pixmap(QSize(16, 16));
  QIcon info_icon(":checkmark.png");
  images[1] = info_icon.pixmap(QSize(16, 16));
}
