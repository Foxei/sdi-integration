#ifndef INTEGRATION_GUI_COLOR_DEFINITIONS_H
#define INTEGRATION_GUI_COLOR_DEFINITIONS_H

#include <QtGui/QColor>

/**
 * Dark theme colors, used where the platform has no native dark mode
 */
namespace Color {

const QColor ACCENT = QColor(76, 141, 246);
const QColor DANGER = QColor(229, 83, 75);
const QColor SURFACE = QColor(30, 30, 32);      // input fields, chart
const QColor BACKGROUND = QColor(40, 40, 43);   // windows, sidebar
const QColor BUTTON = QColor(55, 55, 59);
const QColor BASE = QColor(72, 72, 76);         // borders
const QColor TEXT = QColor(230, 230, 230);
const QColor TEXT_SECONDARY = QColor(150, 150, 155);

}

#endif //INTEGRATION_GUI_COLOR_DEFINITIONS_H
