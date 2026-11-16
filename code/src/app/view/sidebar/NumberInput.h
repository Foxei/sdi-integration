#ifndef INTEGRATION_GUI_NUMBER_INPUT_H
#define INTEGRATION_GUI_NUMBER_INPUT_H

#include <QtWidgets/QDoubleSpinBox>
#include <QtGui/QKeyEvent>

#include "view/SizeDefinitions.h"

/**
 * Number input with steppers, sized for touch and without trailing zeros.
 */
class NumberInput : public QDoubleSpinBox {
 Q_OBJECT
 public:
  /**
   * Setup number input.
   * @param decimals number of decimals, 0 for integers
   * @param parent parent widget for deallocation of pointers
   */
  explicit NumberInput(int decimals, QWidget *parent = nullptr) : QDoubleSpinBox(parent) {
    QLocale number_locale = locale();
    number_locale.setNumberOptions(QLocale::OmitGroupSeparator);
    setLocale(number_locale);
    setDecimals(decimals);
    if (decimals > 0) {
      setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    }
    setAccelerated(true);
    setMinimumHeight(Size::INPUT_HEIGHT);
  }

  QString textFromValue(double value) const override {
    return locale().toString(value, 'g', 12);
  }

 signals:
  /**
   * Emitted when the user presses enter in the input.
   */
  void returnPressed();

 protected:
  void keyPressEvent(QKeyEvent *event) override {
    QDoubleSpinBox::keyPressEvent(event);
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
      emit returnPressed();
    }
  }
};

#endif //INTEGRATION_GUI_NUMBER_INPUT_H
