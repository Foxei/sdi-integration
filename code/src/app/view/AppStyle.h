#ifndef INTEGRATION_GUI_APP_STYLE_H
#define INTEGRATION_GUI_APP_STYLE_H

#include <QtWidgets/QComboBox>
#include <QtWidgets/QProxyStyle>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QStyleOptionComplex>

#include "view/SizeDefinitions.h"

/**
 * Fusion where combo boxes and spin boxes look alike: same field background
 * and the same horizontal text padding
 */
class AppStyle : public QProxyStyle {
 public:
  AppStyle() : QProxyStyle(QStyleFactory::create("Fusion")) {}

  void polish(QWidget *widget) override {
    QProxyStyle::polish(widget);
    if (qobject_cast<QComboBox *>(widget)) {
      QPalette palette = widget->palette();
      palette.setColor(QPalette::Button, palette.color(QPalette::Base));
      widget->setPalette(palette);
    }
  }

  QRect subControlRect(ComplexControl control, const QStyleOptionComplex *option,
                       SubControl sub_control, const QWidget *widget) const override {
    QRect rect = QProxyStyle::subControlRect(control, option, sub_control, widget);
    if ((control == CC_ComboBox && sub_control == SC_ComboBoxEditField)
        || (control == CC_SpinBox && sub_control == SC_SpinBoxEditField)) {
      rect.setLeft(option->rect.left() + Size::INPUT_PADDING);
    }
    return rect;
  }
};

#endif //INTEGRATION_GUI_APP_STYLE_H
