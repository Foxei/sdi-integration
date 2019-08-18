#ifndef INTEGRATION_GUI_POPUP_MESSAGE_H
#define INTEGRATION_GUI_POPUP_MESSAGE_H

#include <QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include "view/status/StatusBar.h"

class PopupMessage : public QFrame {
 Q_OBJECT
 private:
  QLabel *label_image_ = nullptr;
  QLabel *label_title_ = nullptr;
  QLabel *label_message_ = nullptr;
  QPushButton *button_close_ = nullptr;
  QPixmap images[2];

  void setUpGui();

  void loadImages();

  void mouseReleaseEvent(QMouseEvent *event) override;

 public:
  explicit PopupMessage(QWidget *parent = nullptr);

  void setMessage(const QString& title, const QString& message, MessageTyp type = MessageTyp::NONE);

};

#endif //INTEGRATION_GUI_POPUP_MESSAGE_H
