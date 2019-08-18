#ifndef INTEGRATION_GUI_CONTROLLER_H
#define INTEGRATION_GUI_CONTROLLER_H

#include <QObject>
#include <QDebug>
#include <cstdlib>
#include <sstream>
#include <QVector>
#include <QThread>

#include "view/sidebar/SideBar.h"
#include "view/control/Controlbar.h"
#include "view/status/StatusBar.h"
#include "controller/Worker.h"
#include "view/chart/ChartView.h"
#include <IntergrationLibrary/function_manager.h>
#include <IntergrationLibrary/integration_manager.h>

/**
 * Controller is the heart of all interactions on the interface.
 * It will connect all input actions to background actions
 */
class Controller : public QObject {
 Q_OBJECT

 private:
  SideBar *side_bar_; //< Right side of window with all input fields and final data display
  ControlBar *control_bar_; //< Toolbar with all function buttons
  StatusBar *status_bar_; //< StatusBar at the bottom of window will contains all information of process
  ChartView *chart_view_; //< Chart on left side will plot all functions

  /**
   * Connect all parts of the interface together
   */
  void setUpConnections();

 public:
  /**
   * Controller just connect all parts of the interface
   * @param sidebar Have to initialised first
   * @param control_bar Have to initialised first
   * @param statusbar Have to initialised first
   */
  Controller(SideBar *sidebar, ControlBar *control_bar, StatusBar *statusbar, ChartView *chart_view);

 public slots:
  /**
   * Check values handle error and starts worker in other thread all values needed see class fields
   */
  void handleUpdatedValues(double a, double b, const std::string& p, unsigned int m, size_t index_function,
                           size_t index_integration, bool all_set);

  /**
   * Handle the result from the other thread and will unblock the interface
   * @param bar_heights
   */
  void handleFinishCalculation(double integral, QVector<double> bar_heights);

  /**
   * Will unblock interface so you can use it again.
   */
  void handlePlotFinished();

 signals:

  /**
   * Will block or unblock all interface parts if calculation is running
   * @param value true blocked, false unblocked
   */
  void fireBlockInput(bool value);

  /**
   * Will show or hide process indicators on interface
   * @param value true shown, false hidden
   */
  void fireShowProcessIndicator(bool value);

  /**
   * Will send a message to statusbar
   * @param message message text
   * @param typ set image of message on statusbar
   */
  void fireStatusbarMessage(QString message, MessageTyp typ);

  /**
   * Will clear statusbar to no image and no text
   */
  void fireStatusbarClearMessage();

  /**
   * Will set values in "Values" area to "NA"
   */
  void fireSidebarHideValues();

  /**
   * Will hide small error message in sidebar
   */
  void fireSidebarHideErrorMessage();

  /**
   * Will update values in "Values" area on sidebar
   * @param integral new integral value
   * @param derivative new derivative value
   */
  void fireSidebarUpdateValues(double integral, double derivative);

  /**
   * Will show an error message on sidebar. Just print one or two lines here for bigger messages use statusbar
   * @param message
   */
  void fireSidebarShowErrorMessage(QString message, MessageTyp type);

  /**
   * Will update the correct function in chart view
   * @param a Lower bound
   * @param b Upper bound
   * @param p Parameter
   * @param function_index Index of function in manager
   */
  void firePlotFunction(QVector<double> values);

  /**
   * Will clear plot if error occurred
   */
  void fireClearPlot();

};

#endif //INTEGRATION_GUI_CONTROLLER_H
