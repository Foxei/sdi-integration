#ifndef INTEGRATION_GUI_POPUP_MESSAGE_H
#define INTEGRATION_GUI_POPUP_MESSAGE_H

#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtGui/QIcon>

#include "view/status/StatusBar.h"

/**
 * Inline message banner with an icon, a title and a text. Hides itself when clicked.
 */
class PopupMessage : public QFrame {
 Q_OBJECT
 private:
  QLabel *label_image_ = nullptr;
  QLabel *label_title_ = nullptr;
  QLabel *label_message_ = nullptr;
  QIcon icons_[2];
  QColor color_ = Qt::transparent; //< Border and tint color of the current message type

  void setUpGui();

  void loadImages();

  void mouseReleaseEvent(QMouseEvent *event) override;

  /**
   * Draw a rounded, tinted box with a border in the message color.
   */
  void paintEvent(QPaintEvent *event) override;

 public:
  explicit PopupMessage(QWidget *parent = nullptr);

  void setMessage(const QString& title, const QString& message, MessageTyp type = MessageTyp::NONE);

};

#endif //INTEGRATION_GUI_POPUP_MESSAGE_H
