#ifndef INTEGRATION_GUI_STATUS_BAR_H
#define INTEGRATION_GUI_STATUS_BAR_H

#include <QtWidgets/QStatusBar>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtGui/QIcon>

/**
 * Different typ of messages just effect the icon on status bar
 */
enum class MessageTyp {
  Error,
  OK,
  NONE
};

/**
 * Status bar can display messages, idl animations and icons
 */
class StatusBar : public QStatusBar {
 Q_OBJECT
 private:
  QIcon icons_[2]; //< Icons which can be displayed on status bar
  QProgressBar *progress_indicator = nullptr; //< Busy indicator while calculating
  QLabel *label_img = nullptr; //< Label to display icons
  QLabel *label_message = nullptr; //< Label to display the message, respects the padding

  /**
   * Setting up layou for statusbar
   */
  void setUpGui();

  /**
   * Loading images for statusbar
   */
  void loadImages();

 public:
  /**
   * Will setup gui, load images and use super constructor
   * @param parent Parent QWidget
   */
  explicit StatusBar(QWidget *parent = nullptr);

 public slots:
  /**
   * Clearing message from statusbar
   */
  void clearDisplayedMessage();

  /**
   * Show given message and icon
   */
  void displayMessage(const QString& message, MessageTyp typ);

  /**
   * Make idl indicator visible or not
   * @param value true visible, false not
   */
  void showIndicator(bool value);

};

#endif //INTEGRATION_GUI_STATUSBAR_H
