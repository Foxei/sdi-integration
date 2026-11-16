#include "Mainwindow.h"

#include <QScrollArea>
#include <QSplitter>

void MainWindow::restoreWindowStates() {
  QSettings settings;
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("window_state").toByteArray());
}

void MainWindow::storeWindowState() {
  QSettings settings;
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
  setMinimumSize(QSize(760, 520));
  resize(1100, 720);
  restoreWindowStates();

  // Create instances
  auto *splitter = new QSplitter(this);
  auto *widget_right = new QWidget(splitter);
  auto *layout_right = new QVBoxLayout(widget_right);

  side_bar_ = new SideBar(widget_right);
  chart_view_ = new ChartView(splitter);
  control_bar_ = new ControlBar(widget_right);
  status_bar_ = new StatusBar(this);
  // end

  // Right side: scrollable sidebar with the calculate button below
  auto *scroll_area = new QScrollArea(widget_right);
  scroll_area->setWidget(side_bar_);
  scroll_area->setWidgetResizable(true);
  scroll_area->setFrameShape(QFrame::NoFrame);
  scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  layout_right->addWidget(scroll_area, 1);
  layout_right->addWidget(control_bar_);
  layout_right->setContentsMargins(0, 0, 0, 0);
  layout_right->setSpacing(0);
  widget_right->setMinimumWidth(side_bar_->minimumWidth());
  // end

  // Chart on the left takes the remaining space
  splitter->addWidget(chart_view_);
  splitter->addWidget(widget_right);
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 0);
  splitter->setSizes({width() - 340, 340});
  splitter->setChildrenCollapsible(false);
  // end

  // set basic main window stuff
  setStatusBar(status_bar_);
  setCentralWidget(splitter);
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
