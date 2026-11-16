#include <QtWidgets/QGridLayout>
#include <QtGui/QPainter>
#include "PopupMessage.h"

#include "view/ColorDefinitions.h"

PopupMessage::PopupMessage(QWidget *parent) : QFrame(parent) {
  setUpGui();
  loadImages();
}

void PopupMessage::setUpGui() {
  auto *layout = new QGridLayout(this);

  label_image_ = new QLabel(this);

  label_title_ = new QLabel(this);
  QFont font_title = label_title_->font();
  font_title.setWeight(QFont::DemiBold);
  label_title_->setFont(font_title);

  label_message_ = new QLabel(this);
  label_message_->setWordWrap(true);

  layout->addWidget(label_image_, 0, 0);
  layout->addWidget(label_title_, 0, 1);
  layout->addWidget(label_message_, 1, 1);
  layout->setColumnStretch(1, 1);
  layout->setContentsMargins(12, 10, 12, 10);

  setToolTip(tr("Click to dismiss"));
  setCursor(Qt::PointingHandCursor);
}

void PopupMessage::setMessage(const QString& title, const QString& message, MessageTyp type) {
  if (type == MessageTyp::OK) {
    label_image_->setPixmap(icons_[1].pixmap(18));
    label_image_->setVisible(true);
    color_ = Color::ACCENT;
  } else if (type == MessageTyp::Error) {
    label_image_->setPixmap(icons_[0].pixmap(18));
    label_image_->setVisible(true);
    color_ = Color::DANGER;
  } else {
    label_image_->setVisible(false);
    color_ = palette().color(QPalette::PlaceholderText);
  }
  update();

  label_title_->setText(title);
  label_message_->setText(message);
}

void PopupMessage::loadImages() {
  icons_[0] = QIcon(":icons/error.svg");
  icons_[1] = QIcon(":icons/info.svg");
}

void PopupMessage::mouseReleaseEvent(QMouseEvent *) {
  setVisible(false);
}

void PopupMessage::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QColor fill = color_;
  fill.setAlpha(40);
  QColor border = color_;
  border.setAlpha(200);

  painter.setPen(QPen(border, 1));
  painter.setBrush(fill);
  painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 6, 6);
}
