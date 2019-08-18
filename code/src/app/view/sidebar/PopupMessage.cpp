#include <QtWidgets/QGridLayout>
#include "PopupMessage.h"

PopupMessage::PopupMessage(QWidget *parent) : QFrame(parent) {
  setUpGui();
  loadImages();
}

void PopupMessage::setUpGui() {
  auto *layout = new QGridLayout(this);

  button_close_ = new QPushButton();
  button_close_->setText(tr("x"));
  button_close_->setAccessibleName(tr("collapse"));
  button_close_->setFlat(true);
  button_close_->setFocusPolicy(Qt::FocusPolicy::NoFocus);

  label_image_ = new QLabel(this);

  label_title_ = new QLabel(this);
  label_title_->setContentsMargins(0, 0, 0, 5);
  label_title_->setAccessibleName(tr("popup.title"));

  label_message_ = new QLabel(this);
  label_message_->setWordWrap(true);

  layout->addWidget(label_image_, 0, 0);
  layout->addWidget(label_title_, 0, 1, Qt::AlignLeft);
  layout->addWidget(label_message_, 1, 0, 1, 3, Qt::AlignTop);
  //layout->addWidget(button_close_, 0, 2, Qt::AlignRight);
  layout->setColumnStretch(1, 1);
  layout->setRowStretch(1, 1);

  setLayout(layout);
  setMinimumHeight(100);
  setAccessibleName(tr("popup.background"));
}

void PopupMessage::setMessage(const QString& title, const QString& message, MessageTyp type) {
  if (type == MessageTyp::OK) {
    label_image_->setPixmap(images[1]);
    label_image_->setVisible(true);
  } else if (type == MessageTyp::Error) {
    label_image_->setPixmap(images[0]);
    label_image_->setVisible(true);
  } else
    label_image_->setVisible(false);

  label_title_->setText(title);
  label_message_->setText(message);
}

void PopupMessage::loadImages() {
  QIcon risk_icon(":risk.png");
  images[0] = risk_icon.pixmap(QSize(16, 16));
  QIcon info_icon(":info.png");
  images[1] = info_icon.pixmap(QSize(16, 16));
}

void PopupMessage::mouseReleaseEvent(QMouseEvent *) {
  setVisible(false);
}
