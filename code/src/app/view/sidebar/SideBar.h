#ifndef INTEGRATION_GUI_SIDE_BAR_H
#define INTEGRATION_GUI_SIDE_BAR_H

#include <QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QGridLayout>

#include <IntergrationLibrary/function_manager.h>
#include <IntergrationLibrary/integration_manager.h>

#include "view/sidebar/CollapsibleElement.h"
#include "view/sidebar/PopupMessage.h"

/**
 * Sidebar of application to set up parameter and choose functions
 */
class SideBar : public QFrame {
 Q_OBJECT
 private:
  PopupMessage *currentMessage = nullptr;

  CollapsibleElement *widget_function = nullptr;

  /**
   * Combobox to choose function
   */
  QComboBox *combo_function_ = nullptr;

  /**
   * Textfield to set parameter p, only doubles are valid
   */
  QLineEdit *edit_parameter_ = nullptr;

  /**
   * Textfield to set upper bound of integral, only doubles are valid
   */
  QLineEdit *edit_border_a_ = nullptr;

  /**
   * Textfield to set lower bound of integral, only doubles are valid
   */
  QLineEdit *edit_border_b_ = nullptr;

  /**
   * Shows error messages in function area
   */
  QLabel *label_error_message_function_ = nullptr;

  /**
   * Combobox to choose integration method
   */
  QComboBox *combo_integration_ = nullptr;

  /**
   * Spinner to choose parameter n, only integers are valid
   */
  QSpinBox *spin_points_ = nullptr;

  /**
   * Label shows final integral value
   */
  QLabel *label_value_integral_ = nullptr;

  /**
   * Label shows final derivative
   */
  QLabel *label_value_derivative_ = nullptr;

  /**
   * Setting up gui.
   */
  void setUpGUI();

  /**
   * Filling combo boxes with data
   */
  void fillData();

  /**
   * Update the size of the message box needed.
   */
  void resizeEvent(QResizeEvent *) override;

 public:

  /**
   * Setup widget gui.
   * @param parent parent widget for deallocation of pointers
   */
  explicit SideBar(QWidget *parent = nullptr);

 public slots:
  /**
   * Shows error message in label
   * @param message Error message text
   */
  void showMessage(const QString &message, MessageTyp type = MessageTyp::Error);

  /**
   * Hide error message label from window
   */
  void hideErrorMessage();

  /**
   * Update values in "Value" area
   * @param integral new integral value
   * @param derivative new derivative value
   */
  void showValues(double integral, double derivative);

  /**
   * Set value text to "NA" to avoid confusion
   */
  void hideValues();

  /**
   * This will bring the sidebar to emit all values to controller
   */
  void sendValuesToController();

  /**
   * Block or allows input in sidebar
   * @param value true blocked, false allowed
   */
  void blockInput(bool value);

 signals:
  /**
   * Will emit all input values
   * @param a lower bound
   * @param b upper bound
   * @param p parameter
   * @param m count of supporting points
   * @param index_function index of function in function manager
   * @param index_integral index of integration in integration manager
   */
  void currentValues(double a,
                     double b,
                     std::string p,
                     unsigned int m,
                     size_t index_function,
                     size_t index_integral,
                     bool all_set);
};

#endif //INTEGRATION_GUI_SIDE_BAR_H
