#ifndef INTEGRATION_GUI_MAIN_WINDOW_H
#define INTEGRATION_GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QtWidgets/QHBoxLayout>

#include "view/control/Controlbar.h"
#include "controller/Controller.h"
#include "view/status/StatusBar.h"
#include "view/sidebar/SideBar.h"
#include "view/chart/ChartView.h"

/**
 * Contains the Mainwindow
 */
class MainWindow : public QMainWindow {
 Q_OBJECT
 private:
  /**
   * Right side of window with all input fields and final data display
   */
  SideBar *side_bar_ = nullptr;

  /**
   * Toolbar with all function buttons
   */
  ControlBar *control_bar_ = nullptr;

  /**
   * StatusBar at the bottom of window will contains all information of process
   */
  StatusBar *status_bar_ = nullptr;

  /**
   * Main view with all plots
   */
  ChartView *chart_view_ = nullptr;

  /**
   * @brief Will restore the window state from the last season.
   */
  void restoreWindowStates();

  /**
   * @brief Will store the window state from the current season.
   */
  void storeWindowState();

 protected:

  /**
   * @brief Will store the current window state to a file so it can be loaded
   * at the next start.
   * @param event Event with details about the circumstances of the close.
   */
  void closeEvent(QCloseEvent *event) override;

 private:
  //< Chart on left side will plott all functions

  /**
   * Will connect all components to the finals windows
   */
  void setUpGui();

 public:
  /**
   * Will setup gui and use super constructor
   * @param parent Parent QWidget
   */
  explicit MainWindow(QWidget *parent = nullptr);

  /**
   * Getter allows access to widget but main window will position it on the screen
   * @return Sidebar instance
   */
  SideBar *getSidebar() const;

  /**
   * Getter allows access to widget but main window will position it on the screen
   * @return Control Bar instance
   */
  ControlBar *getControlBar() const;

  /**
   * Getter allows access to widget but main window will position it on the screen
   * @return Statusbar instance
   */
  StatusBar *getStatusbar() const;

  /**
   * Getter allows access to widget but main window will position it on the screen
   * @return ChartView instance
   */
  ChartView *getChartView() const;
};

#endif // INTEGRATION_GUI_MAIN_WINDOW_H
