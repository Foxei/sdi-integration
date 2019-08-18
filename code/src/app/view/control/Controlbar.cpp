#include "Controlbar.h"

ControlBar::ControlBar(QWidget *parent) : QWidget(parent) {
  setUpGui();
  setConnections();
}

void ControlBar::setUpGui() {
  // Set up toolbar
  auto *toolbar = new QToolBar(this);
  auto *icon_refresh = new QIcon(":refresh.png");
  action_refresh_ = new QAction(*icon_refresh, tr("refresh_plot"), this);
  action_refresh_->setToolTip(tr("Update plot with new values."));
  action_refresh_->setShortcut(Qt::CTRL + Qt::Key_R);
  toolbar->addAction(action_refresh_);
  // Toolbar finished

  // Set up layout
  auto *layout = new QVBoxLayout(this);
  layout->addWidget(toolbar);
  layout->setContentsMargins(0, 10, 0, 0);
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);
  // Layout finished

}

void ControlBar::setConnections() {
  connect(action_refresh_,
          SIGNAL (triggered()),
          this,
          SLOT (handleRefreshClick())
  );
}

void ControlBar::handleRefreshClick() {
  emit refreshClicked();
}

void ControlBar::blockInput(bool value) {
  setDisabled(value);
}