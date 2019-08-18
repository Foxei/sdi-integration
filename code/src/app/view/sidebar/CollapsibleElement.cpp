#include "CollapsibleElement.h"

CollapsibleElement::CollapsibleElement(QWidget *parent) : QWidget(parent) {
  setUpGui();
  setUpConnection();
}

void CollapsibleElement::setUpGui() {
  // Create instances
  btn_toggle_ = new QPushButton(this);
  lbl_title_ = new QLabel(this);
  widget_body_ = new QWidget(this);
  lbl_description_ = new QLabel(this);
  layout_body_ = new QVBoxLayout(this);
  // end

  // Setup title bar
  btn_toggle_->setText(tr("-"));
  btn_toggle_->setAccessibleName(tr("collapse"));
  btn_toggle_->setFlat(true);
  btn_toggle_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  btn_toggle_->setAccessibleName("button.flat");

  lbl_title_->setAccessibleName(tr("headline"));

  lbl_description_->setContentsMargins(0, 0, 0, 10);
  lbl_description_->setAccessibleName(tr("description"));
  lbl_description_->setWordWrap(true);

  layout_body_->setContentsMargins(0, 0, 0, 0);

  auto *widget_title = new QWidget(this);
  auto *layout_title = new QGridLayout(widget_title);
  layout_title->setContentsMargins(0, 0, 0, 0);
  layout_title->addWidget(lbl_title_, 0, 0, Qt::AlignLeft);
  layout_title->addWidget(btn_toggle_, 0, 1, Qt::AlignRight);
  layout_title->addWidget(lbl_description_, 1, 0, 1, 2);
  widget_title->setLayout(layout_title);
  // end

  // Setup custom content
  widget_body_->setLayout(layout_body_);
  widget_body_->setContentsMargins(0, 0, 40, 0);
  // end

  // Combine both contents
  auto *layout = new QVBoxLayout(this);
  layout->addWidget(widget_title);
  layout->addWidget(widget_body_);

  setLayout(layout);
  // end

}

void CollapsibleElement::setUpConnection() {
  // Connect button to toggleStatus() function
  connect(btn_toggle_,
          SIGNAL (clicked()),
          this,
          SLOT (toggleStatus())
  );
  // Connecting finished
}

void CollapsibleElement::setTitle(const QString& title) {
  lbl_title_->setText(title);
}

void CollapsibleElement::setDescription(const QString& description) {
  lbl_description_->setText(description);
}

QVBoxLayout &CollapsibleElement::getLayout() {
  return *layout_body_;
}

void CollapsibleElement::collapse() {
  btn_toggle_->setText(tr("+"));
  widget_body_->setVisible(false);
  collapsed = true;
}

void CollapsibleElement::expand() {
  btn_toggle_->setText(tr("-"));
  widget_body_->setVisible(true);
  collapsed = false;
}

void CollapsibleElement::toggleStatus() {
  if (collapsed) {
    expand();
  } else {
    collapse();
  }
}