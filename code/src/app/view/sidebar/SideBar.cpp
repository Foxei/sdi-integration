#include "SideBar.h"

#include "model/Data.h"
#include "PopupMessage.h"

#include <QDebug>
#include <QtWidgets/QStyledItemDelegate>

SideBar::SideBar(QWidget *parent) : QFrame(parent) {
  setUpGUI();
  fillData();
}

void SideBar::setUpGUI() {

  setAccessibleName("background");
  // Set up function block
  widget_function = new CollapsibleElement(this);
  QVBoxLayout *layout_function = &widget_function->getLayout();
  widget_function->setTitle(tr("Function"));
  widget_function->setDescription(tr("Choose the function you want to integrate"
                                     " and the integration borders."));

  combo_function_ = new QComboBox(widget_function);
  combo_function_->setToolTip(tr("Choose function to integrate."));
  combo_function_->setItemDelegate(new QStyledItemDelegate());
  layout_function->addWidget(combo_function_);

  auto *label_parameter_ = new QLabel(widget_function);
  label_parameter_->setText(tr("Parameter"));
  edit_parameter_ = new QLineEdit(widget_function);
  edit_parameter_->setPlaceholderText(tr("p"));
  edit_parameter_->setText(tr(std::to_string(Data::DEFAULT_PARAMETER).c_str()));
  edit_parameter_->setToolTip(tr("This parameter will effect the function evaluation."));
  layout_function->addWidget(label_parameter_);
  layout_function->addWidget(edit_parameter_);

  auto *label_border_ = new QLabel(widget_function);
  label_border_->setText(tr("Borders"));
  edit_border_a_ = new QLineEdit(widget_function);
  edit_border_a_->setValidator(new QDoubleValidator());
  edit_border_a_->setPlaceholderText(tr("a"));
  edit_border_a_->setText(tr(std::to_string(Data::DEFAULT_LOWER_BOUND).c_str()));
  edit_border_a_->setToolTip(tr("This will set the lower bound of your integral."));
  layout_function->addWidget(label_border_);
  layout_function->addWidget(edit_border_a_);

  edit_border_b_ = new QLineEdit(widget_function);
  edit_border_b_->setValidator(new QDoubleValidator(widget_function));
  edit_border_b_->setPlaceholderText(tr("b"));
  edit_border_b_->setText(tr(std::to_string(Data::DEFAULT_UPPER_BOUND).c_str()));
  edit_border_b_->setToolTip(tr("This will set the upper bound of your integral."));
  layout_function->addWidget(edit_border_b_);

  label_error_message_function_ = new QLabel(widget_function);
  label_error_message_function_->setAccessibleName("error");
  label_error_message_function_->setWordWrap(true);
  label_error_message_function_->setContentsMargins(0, 5, 0, 0);
  label_error_message_function_->setToolTip(tr("See details in statusbar bellow."));
  //label_error_message_function_->setText( tr("Test error message.") );
  label_error_message_function_->setVisible(false);
  layout_function->addWidget(label_error_message_function_);
  // Function block finished

  // Set up integration block
  auto *widget_integration = new CollapsibleElement(this);
  QVBoxLayout *layout_integration = &widget_integration->getLayout();
  widget_integration->setTitle(tr("Integration"));
  widget_integration->setDescription(tr("Choose a integration method and"
                                        " number of supporting points."));

  combo_integration_ = new QComboBox(widget_integration);
  combo_integration_->setToolTip(tr("Choose method for integration."));
  combo_integration_->setItemDelegate(new QStyledItemDelegate());

  layout_integration->addWidget(combo_integration_);

  auto label_points_ = new QLabel(widget_integration);
  label_points_->setText(tr("Number points"));
  spin_points_ = new QSpinBox(widget_integration);
  spin_points_->setRange(2, 100000);
  spin_points_->setValue(Data::DEFAULT_NUMBER_POINTS);
  spin_points_->setToolTip(tr("Number of supporting points."));
  layout_integration->addWidget(label_points_);
  layout_integration->addWidget(spin_points_);
  // Integration block finished

  // Set up Values block
  auto *widget_values = new CollapsibleElement(this);
  QVBoxLayout *layout_values = &widget_values->getLayout();
  widget_values->setTitle(tr("Values"));
  widget_values->setDescription(tr("Display all calculated value like derivative and integrals."));

  auto widget_values_container = new QWidget(widget_values);
  auto *layout_values_container = new QGridLayout(widget_values_container);
  widget_values_container->setLayout(layout_values_container);
  widget_values_container->setContentsMargins(0, 0, 0, 0);
  layout_values_container->setContentsMargins(0, 0, 0, 0);
  layout_values->setMargin(0);

  auto label_integral = new QLabel(widget_values_container);
  label_integral->setText(tr("Integral"));
  layout_values_container->addWidget(label_integral, 0, 0);
  label_value_integral_ = new QLabel(widget_values_container);
  label_value_integral_->setText(tr("NA"));
  layout_values_container->addWidget(label_value_integral_, 0, 1);

  auto *label_derivative = new QLabel(widget_values_container);
  label_derivative->setText(tr("Derivative"));
  layout_values_container->addWidget(label_derivative, 1, 0);
  label_value_derivative_ = new QLabel(widget_values_container);
  label_value_derivative_->setText(tr("NA"));
  layout_values_container->addWidget(label_value_derivative_);

  layout_values->addWidget(widget_values_container);
  // Values block finished

  // Add Widgets and set layout
  auto *widget_layout = new QGridLayout(this);
  widget_layout->addWidget(widget_function, 0, 0);
  widget_layout->addWidget(widget_integration, 1, 0);
  widget_layout->addWidget(widget_values, 2, 0);

  widget_layout->setRowStretch(7, 1);
  widget_layout->setContentsMargins(5, 5, 5, 5);

  setLayout(widget_layout);
  setMaximumWidth(400);
  setMinimumWidth(250);
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
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
  //  label_error_message_function_->setText( message );
  //label_error_message_function_->setVisible( true );
  // widget_function->setDescriptionVisible(false);
  QString title;
  if (type == MessageTyp::Error) {
    title = tr("Error");
  } else if (type == MessageTyp::OK) {
    title = tr("Information");
  }
  if (currentMessage == nullptr)
    currentMessage = new PopupMessage(this);

  currentMessage->setMessage(title, message, type);
  currentMessage->show();
  currentMessage->setMinimumWidth(size().width() - 5);
  currentMessage->move(0, 5);
}

void SideBar::showValues(double integral, double derivative) {
  label_value_integral_->setText(tr(std::to_string(integral).c_str()));
  label_value_derivative_->setText(tr(std::to_string(derivative).c_str()));
}

void SideBar::hideValues() {
  label_value_derivative_->setText(tr("NA"));
  label_value_integral_->setText(tr("NA"));
}

void SideBar::sendValuesToController() {
  double a = edit_border_a_->text().toDouble();
  double b = edit_border_b_->text().toDouble();
  std::string p = edit_parameter_->text().toStdString();
  unsigned int m = spin_points_->text().toInt();
  size_t index_function = combo_function_->currentIndex();
  size_t index_integral = combo_integration_->currentIndex();

  bool all_set = true;
  if (edit_border_a_->text().isEmpty() || edit_border_b_->text().isEmpty()
      || edit_parameter_->text().isEmpty()) {
    all_set = false;
  }
  emit currentValues(a, b, p, m, index_function, index_integral, all_set);
}

void SideBar::blockInput(bool value) {
  setDisabled(value);
}

void SideBar::resizeEvent(QResizeEvent *) {
  if (currentMessage != nullptr) {
    int width = size().width();
    currentMessage->setMinimumWidth(width - 5);
    currentMessage->setMaximumWidth(width - 5);
  }

}
