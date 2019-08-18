#ifndef INTEGRATION_GUI_CONTROL_BAR_H
#define INTEGRATION_GUI_CONTROL_BAR_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>

/**
 * Simple class for control toolbar
 */
class ControlBar : public QWidget {
 Q_OBJECT
 private:

  /**
   * Action to refresh values and gui
   */
  QAction *action_refresh_ = nullptr;

  /**
   * Set up gui.
   */
  void setUpGui();

  /**
   * Connects signals to dummy signals
   */
  void setConnections();

 public:

  /**
   * Setup widget gui.
   * @param parent parent widget for deallocation of pointers
   */
  explicit ControlBar(QWidget *parent = nullptr);

 private slots:
  /**
   * Dummy slot to forward signal of button
   */
  void handleRefreshClick();

 public slots:
  /**
   * Block or allows input in sidebar
   * @param value true blocked, false allowed
   */
  void blockInput(bool value);

 signals:
  /**
   * Dummy signal to forward signal from button
   */
  void refreshClicked();

};

#endif //INTEGRATION_GUI_CONTROL_BAR_H
