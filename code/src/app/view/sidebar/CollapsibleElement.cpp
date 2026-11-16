#include "CollapsibleElement.h"

#include <QtWidgets/QVBoxLayout>

#include "view/SizeDefinitions.h"

CollapsibleElement::CollapsibleElement(QWidget *parent) : QWidget(parent) {
  setUpGui();
  setUpConnection();
}

void CollapsibleElement::setUpGui() {
  // Create instances
  btn_title_ = new QToolButton(this);
  lbl_description_ = new QLabel(this);
  widget_body_ = new QWidget(this);
  layout_body_ = new QFormLayout(widget_body_);
  // end

  // Setup title bar
  QFont font_title = btn_title_->font();
  font_title.setPointSizeF(font_title.pointSizeF() * 1.2);
  font_title.setWeight(QFont::DemiBold);
  btn_title_->setFont(font_title);
  btn_title_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  btn_title_->setIcon(QIcon(":icons/chevron-down.svg"));
  btn_title_->setIconSize(QSize(14, 14));
  btn_title_->setCursor(Qt::PointingHandCursor);
  btn_title_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  // Plain header text, native styles would draw a button bezel
  btn_title_->setStyleSheet("QToolButton { border: none; background: transparent; padding: 0; }");

  lbl_description_->setWordWrap(true);
  lbl_description_->setForegroundRole(QPalette::PlaceholderText);
  // end

  // Setup custom content
  layout_body_->setContentsMargins(0, 8, 0, 0);
  layout_body_->setVerticalSpacing(Size::FORM_SPACING);
  layout_body_->setHorizontalSpacing(Size::FORM_SPACING);
  layout_body_->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  layout_body_->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
  layout_body_->setRowWrapPolicy(QFormLayout::DontWrapRows);
  // end

  // Combine both contents
  auto *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 16);
  layout->addWidget(btn_title_);
  layout->addWidget(lbl_description_);
  layout->addWidget(widget_body_);
  // end
}

void CollapsibleElement::setUpConnection() {
  connect(btn_title_, &QToolButton::clicked, this, [this] { setExpanded(!widget_body_->isVisibleTo(this)); });
}

void CollapsibleElement::setTitle(const QString& title) {
  btn_title_->setText(title);
}

void CollapsibleElement::setDescription(const QString& description) {
  lbl_description_->setText(description);
}

QFormLayout &CollapsibleElement::getLayout() {
  return *layout_body_;
}

void CollapsibleElement::setExpanded(bool expanded) {
  btn_title_->setIcon(QIcon(expanded ? ":icons/chevron-down.svg" : ":icons/chevron-right.svg"));
  widget_body_->setVisible(expanded);
}
