#include "Controller.h"

#include "model/Data.h"

Controller::Controller(SideBar *sidebar, ControlBar *control_bar, StatusBar *statusbar, ChartView *chart_view) {
  side_bar_ = sidebar;
  control_bar_ = control_bar;
  status_bar_ = statusbar;
  chart_view_ = chart_view;

  setUpConnections();

  qRegisterMetaType<QVector<double> >("QVector<double>");
  qRegisterMetaType<size_t>("size_t");
  qRegisterMetaType<double>("double");

  emit fireStatusbarMessage(tr("Ready."), MessageTyp::OK);
}

void Controller::setUpConnections() {
  connect(control_bar_, &ControlBar::refreshClicked, side_bar_, &SideBar::sendValuesToController);
  connect(side_bar_, &SideBar::currentValues, this, &Controller::handleUpdatedValues);
  connect(this, &Controller::fireSidebarShowErrorMessage, side_bar_, &SideBar::showMessage);
  connect(this, &Controller::fireSidebarHideErrorMessage, side_bar_, &SideBar::hideErrorMessage);
  connect(this, &Controller::fireSidebarUpdateValues, side_bar_, &SideBar::showValues);
  connect(this, &Controller::fireSidebarHideValues, side_bar_, &SideBar::hideValues);
  connect(this, &Controller::fireStatusbarMessage, status_bar_, &StatusBar::displayMessage);

  // Statusbar message handler
  connect(this, &Controller::fireStatusbarClearMessage, status_bar_, &StatusBar::clearDisplayedMessage);
  connect(this, &Controller::fireShowProcessIndicator, status_bar_, &StatusBar::showIndicator);

  // Block inputs
  connect(this, &Controller::fireBlockInput, control_bar_, &ControlBar::blockInput);
  connect(this, &Controller::fireBlockInput, side_bar_, &SideBar::blockInput);

  // Chart View
  connect(this, &Controller::firePlotFunction, chart_view_, &ChartView::plot);
  connect(this, &Controller::fireClearPlot, chart_view_, &ChartView::clearPlot);
  connect(chart_view_, &ChartView::finishedUpdatingPlot, this, &Controller::handlePlotFinished);
}

void Controller::handleUpdatedValues(double a,
                                     double b,
                                     const std::string &p,
                                     unsigned int m,
                                     size_t index_function,
                                     size_t index_integral,
                                     bool all_set) {

  emit fireSidebarHideErrorMessage();

  FunctionManager *function_manager = &FunctionManager::getInstance();

  // ErrorMessage handling
  if (m <= 1) {
    emit fireSidebarShowErrorMessage(tr("There must be at least a "
                                        "integration bar."), MessageTyp::Error);
    emit fireStatusbarMessage(tr("There must be at least a "
                                 "integration bar."),
                              MessageTyp::Error);
    emit fireSidebarHideValues();
    emit fireClearPlot();
    return;
  }
  if (a >= b) {
    emit fireSidebarShowErrorMessage(tr("The lower integration border must be "
                                        "smaller than the upper "
                                        "border."), MessageTyp::Error);
    emit fireStatusbarMessage(tr("The lower integration border must be "
                                 "smaller than the upper border."),
                              MessageTyp::Error);
    emit fireSidebarHideValues();
    emit fireClearPlot();
    return;
  }
  if (!function_manager->checkParameter(index_function, p)) {
    emit fireSidebarShowErrorMessage(tr("The parameter p is not in the "
                                        "domain of the chosen "
                                        "function."), MessageTyp::Error);
    emit fireStatusbarMessage(tr("The parameter p is not in the domain of "
                                 "the chosen function."), MessageTyp::Error);
    emit fireSidebarHideValues();
    emit fireClearPlot();
    return;
  }
  if (!all_set) {
    emit fireSidebarShowErrorMessage(tr("One ore more inputs are empty. "
                                        "Please make sure that all "
                                        "fields are filled."),
                                     MessageTyp::Error);
    emit fireStatusbarMessage(tr("One ore more inputs are empty. Please "
                                 "make sure that all fields are "
                                 "filled."),
                              MessageTyp::Error);
    emit fireSidebarHideValues();
    emit fireClearPlot();
    return;
  }

  emit fireStatusbarClearMessage();
  emit fireSidebarHideErrorMessage();
  // End

  Data *data = &Data::getInstance();
  data->LOWER_BOUND = a;
  data->UPPER_BOUND = b;
  data->PARAMETER = p;
  data->NUMBER_POINTS = m;
  data->FUNCTION_INDEX = index_function;
  data->INTEGRATION_INDEX = index_integral;

  //Integral stuff
  emit fireShowProcessIndicator(true);
  emit fireBlockInput(true);
  emit fireStatusbarMessage(tr("Calculation please wait..."), MessageTyp::NONE);

  auto *thread = new QThread;

  auto *worker = new Worker();
  connect(worker,
          SIGNAL (sendResult(double, QVector<double>)),
          this,
          SLOT (handleFinishCalculation(double, QVector<double>))
  );

  worker->moveToThread(thread);
  connect(thread, &QThread::started, worker, &Worker::process);
  connect(worker, &Worker::finished, thread, &QThread::quit);
  connect(worker, &Worker::finished, worker, &Worker::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  thread->start();
  //End
}

void Controller::handleFinishCalculation(double integral, QVector<double> bar_heights) {
  Data *data = &Data::getInstance();
  double a = data->LOWER_BOUND;
  double b = data->UPPER_BOUND;
  std::string p = data->PARAMETER;
  unsigned int m = data->NUMBER_POINTS;
  size_t index_function = data->FUNCTION_INDEX;
  size_t index_integration = data->INTEGRATION_INDEX;

  FunctionManager *function_manager = &FunctionManager::getInstance();
  IntegrationManager *integration_manager = &IntegrationManager::getInstance();

  std::stringstream message;
  message << "Integration successfully for a=" << a << ", b=" << b << ", p=" << p << ", m=" << m
          << ", function=" << function_manager->getFunctionName(index_function)
          << " and integration=" << integration_manager->getIntegrationName(index_integration) << ".";

  if (m >= 400) {
    emit fireSidebarShowErrorMessage(tr("More than 400 supporting points "
                                        "can't be plotted in single bars"
                                        " anymore."), MessageTyp::OK);
    bar_heights.clear();
  }

  emit firePlotFunction(bar_heights);
  emit fireSidebarUpdateValues(integral, 0.0);
  emit fireStatusbarMessage(tr(message.str().c_str()), MessageTyp::OK);

}

void Controller::handlePlotFinished() {
  emit fireShowProcessIndicator(false);
  emit fireBlockInput(false);
}
