#include "Mainwindow.h"

#include <QScrollArea>

void MainWindow::restoreWindowStates() {
  QSettings settings
      (QSettings::IniFormat, QSettings::UserScope, "Papercards", "Window");
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("window_state").toByteArray());
}

void MainWindow::storeWindowState() {
  QSettings settings
      (QSettings::IniFormat, QSettings::UserScope, "Papercards", "Window");
  settings.setValue("geometry", saveGeometry());
  settings.setValue("window_state", saveState());
}

void MainWindow::closeEvent(QCloseEvent *event) {
  storeWindowState();
  QMainWindow::closeEvent(event);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
  setUpGui();
}

void MainWindow::setUpGui() {
  restoreWindowStates();
  setMinimumSize(QSize(800, 650));
  // create layout containers
  QWidget *widget_central = new QWidget(this);
  QHBoxLayout *layout_central = new QHBoxLayout(widget_central);
  auto *widget_left = new QFrame(this);
  QHBoxLayout *layout_left = new QHBoxLayout(widget_left);
  // end

  // Create instances
  side_bar_ = new SideBar(widget_central);
  chart_view_ = new ChartView(widget_central);
  control_bar_ = new ControlBar(widget_left);
  status_bar_ = new StatusBar(this);
  // end

  // Add to layout and modify layout parameter
  layout_left->addWidget(chart_view_);
  layout_left->addWidget(control_bar_);

  layout_left->setMargin(0);
  layout_left->setSpacing(0);
  layout_left->setAlignment(Qt::AlignTop);
  layout_left->setContentsMargins(0, 0, 0, 0);
  widget_left->setLayout(layout_left);
  widget_left->setAccessibleName("background");
  layout_central->addWidget(widget_left);

  QScrollArea *dummy = new QScrollArea(widget_central);
  dummy->setWidget(side_bar_);
  dummy->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
  dummy->setMinimumSize(side_bar_->minimumSize());
  layout_central->addWidget(dummy);

  // end
  layout_central->setMargin(0);
  layout_central->setStretch(0, 1);
  widget_central->setAccessibleName("background");
  // Will be removed if correct chartview is created

  // end

  // set basic main window stuff
  setStatusBar(status_bar_);
  setCentralWidget(widget_central);
  setWindowIcon(QIcon("res/line_chart.png"));
  // end
}

SideBar *MainWindow::getSidebar() const {
  return side_bar_;
}

ControlBar *MainWindow::getControlBar() const {
  return control_bar_;
}

StatusBar *MainWindow::getStatusbar() const {
  return status_bar_;
}

ChartView *MainWindow::getChartView() const {
  return chart_view_;
}
