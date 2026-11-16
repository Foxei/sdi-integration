#include "SideBar.h"

#include "model/Data.h"
#include "PopupMessage.h"
#include "view/SizeDefinitions.h"

#include <QDebug>
#include <QtWidgets/QVBoxLayout>

SideBar::SideBar(QWidget *parent) : QFrame(parent) {
  setUpGUI();
  fillData();
}

void SideBar::setUpGUI() {
  // Message banner, hidden until a message is shown
  currentMessage = new PopupMessage(this);
  currentMessage->setVisible(false);

  // Set up function block
  widget_function = new CollapsibleElement(this);
  QFormLayout *layout_function = &widget_function->getLayout();
  widget_function->setTitle(tr("Function"));
  widget_function->setDescription(tr("Choose the function you want to integrate"
                                     " and the integration bounds."));

  combo_function_ = new QComboBox(widget_function);
  combo_function_->setToolTip(tr("Choose function to integrate."));
  combo_function_->setMinimumHeight(Size::INPUT_HEIGHT);
  layout_function->addRow(tr("f(x)"), combo_function_);

  edit_parameter_ = new NumberInput(4, widget_function);
  edit_parameter_->setRange(-1000, 1000);
  edit_parameter_->setValue(Data::DEFAULT_PARAMETER);
  edit_parameter_->setToolTip(tr("This parameter will effect the function evaluation."));
  layout_function->addRow(tr("Parameter p"), edit_parameter_);

  edit_border_a_ = new NumberInput(4, widget_function);
  edit_border_a_->setRange(-1e6, 1e6);
  edit_border_a_->setValue(Data::DEFAULT_LOWER_BOUND);
  edit_border_a_->setToolTip(tr("This will set the lower bound of your integral."));
  layout_function->addRow(tr("Lower bound a"), edit_border_a_);

  edit_border_b_ = new NumberInput(4, widget_function);
  edit_border_b_->setRange(-1e6, 1e6);
  edit_border_b_->setValue(Data::DEFAULT_UPPER_BOUND);
  edit_border_b_->setToolTip(tr("This will set the upper bound of your integral."));
  layout_function->addRow(tr("Upper bound b"), edit_border_b_);
  // Function block finished

  // Set up integration block
  auto *widget_integration = new CollapsibleElement(this);
  QFormLayout *layout_integration = &widget_integration->getLayout();
  widget_integration->setTitle(tr("Integration"));
  widget_integration->setDescription(tr("Choose an integration method and the"
                                        " number of supporting points."));

  combo_integration_ = new QComboBox(widget_integration);
  combo_integration_->setToolTip(tr("Choose method for integration."));
  combo_integration_->setMinimumHeight(Size::INPUT_HEIGHT);
  layout_integration->addRow(tr("Method"), combo_integration_);

  spin_points_ = new NumberInput(0, widget_integration);
  spin_points_->setRange(2, 100000);
  spin_points_->setValue(Data::DEFAULT_NUMBER_POINTS);
  spin_points_->setToolTip(tr("Number of supporting points."));
  layout_integration->addRow(tr("Points"), spin_points_);
  // Integration block finished

  // Set up Values block
  auto *widget_values = new CollapsibleElement(this);
  QFormLayout *layout_values = &widget_values->getLayout();
  widget_values->setTitle(tr("Values"));
  widget_values->setDescription(tr("Results of the last calculation."));

  label_value_integral_ = new QLabel(tr("NA"), widget_values);
  label_value_integral_->setTextInteractionFlags(Qt::TextSelectableByMouse);
  layout_values->addRow(tr("Integral"), label_value_integral_);

  label_value_derivative_ = new QLabel(tr("NA"), widget_values);
  label_value_derivative_->setTextInteractionFlags(Qt::TextSelectableByMouse);
  layout_values->addRow(tr("Derivative"), label_value_derivative_);
  // Values block finished

  // Align the fields of all sections by giving all labels the same width,
  // and center each label vertically on its field
  int label_width = 0;
  QList<QWidget *> labels;
  for (CollapsibleElement *section : {widget_function, widget_integration, widget_values}) {
    QFormLayout &form = section->getLayout();
    for (int row = 0; row < form.rowCount(); row++) {
      QLayoutItem *label = form.itemAt(row, QFormLayout::LabelRole);
      QLayoutItem *field = form.itemAt(row, QFormLayout::FieldRole);
      if (label && field) {
        labels << label->widget();
        label_width = qMax(label_width, label->widget()->sizeHint().width());
        label->widget()->setMinimumHeight(field->widget()->minimumSizeHint().height());
      }
    }
  }
  for (QWidget *label : labels) {
    label->setMinimumWidth(label_width);
  }

  // Calculate when pressing enter in a number input
  for (NumberInput *input : {edit_parameter_, edit_border_a_, edit_border_b_, spin_points_}) {
    connect(input, &NumberInput::returnPressed, this, &SideBar::sendValuesToController);
  }

  // Add Widgets and set layout
  auto *widget_layout = new QVBoxLayout(this);
  widget_layout->addWidget(currentMessage);
  widget_layout->addWidget(widget_function);
  widget_layout->addWidget(widget_integration);
  widget_layout->addWidget(widget_values);
  widget_layout->addStretch(1);
  widget_layout->setContentsMargins(12, 12, 12, 0);

  setMinimumWidth(280);
  // finished
}

void SideBar::fillData() {
  FunctionManager *function_manager =
      &FunctionManager::getInstance();
  for (size_t i = 0; i < function_manager->getNumberRegisteredFunctions(); i++) {
    combo_function_->addItem(tr(function_manager->getFunctionName(i).c_str()));
  }
  IntegrationManager *integration_manager =
      &IntegrationManager::getInstance();
  for (size_t i = 0; i < integration_manager->getNumberRegistertedIntegration(); i++) {
    combo_integration_->addItem(tr(integration_manager->getIntegrationName(i).c_str()));
  }
}

void SideBar::hideErrorMessage() {
  if (currentMessage != nullptr) {
    currentMessage->setVisible(false);
  }
}

void SideBar::showMessage(const QString& message, MessageTyp type) {
  QString title;
  if (type == MessageTyp::Error) {
    title = tr("Error");
  } else if (type == MessageTyp::OK) {
    title = tr("Information");
  }
  currentMessage->setMessage(title, message, type);
  currentMessage->show();
}

void SideBar::showValues(double integral, double derivative) {
  label_value_integral_->setText(QString::number(integral, 'g', 10));
  label_value_derivative_->setText(QString::number(derivative, 'g', 10));
}

void SideBar::hideValues() {
  label_value_derivative_->setText(tr("NA"));
  label_value_integral_->setText(tr("NA"));
}

void SideBar::sendValuesToController() {
  double a = edit_border_a_->value();
  double b = edit_border_b_->value();
  std::string p = QString::number(edit_parameter_->value(), 'g', 15).toStdString();
  auto m = static_cast<unsigned int>(spin_points_->value());
  size_t index_function = combo_function_->currentIndex();
  size_t index_integral = combo_integration_->currentIndex();

  // Number inputs always hold a value
  emit currentValues(a, b, p, m, index_function, index_integral, true);
}

void SideBar::blockInput(bool value) {
  setDisabled(value);
}
