#include "view/Mainwindow.h"
#include "model/Definitions.h"
#include "view/ColorDefinitions.h"
#include "view/AppStyle.h"

#include <IntergrationLibrary/function_manager.h>
#include <IntergrationLibrary/integration_manager.h>

#include <QtWidgets/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QPalette>
#include <QtGui/QStyleHints>

/*! \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 * Numerical integration using the lower-, upper Darboux integral and trapezoidal rule.\n
 * Three different functions already implemented, user chooses integration boundaries,
 * number of sub intervals and different parameters for each function. \n
 * Output is the integral result as well as its derivative, using dco. \n
 *
 * \author Simon Schaefer
 * \author Markus Klaus Mizael Moser
 * \author Soeren Mehnert
 * \author Sharon Michael Rouf-Chowdhury
 * \date RWTH Aachen - Summersemster 2017
 */


/**
 * Dark application palette
 */
QPalette darkPalette() {
  QPalette palette;
  palette.setColor(QPalette::Window, Color::BACKGROUND);
  palette.setColor(QPalette::WindowText, Color::TEXT);
  palette.setColor(QPalette::Base, Color::SURFACE);
  palette.setColor(QPalette::AlternateBase, Color::BACKGROUND);
  palette.setColor(QPalette::ToolTipBase, Color::BASE);
  palette.setColor(QPalette::ToolTipText, Color::TEXT);
  palette.setColor(QPalette::Text, Color::TEXT);
  palette.setColor(QPalette::PlaceholderText, Color::TEXT_SECONDARY);
  palette.setColor(QPalette::Button, Color::BUTTON);
  palette.setColor(QPalette::ButtonText, Color::TEXT);
  palette.setColor(QPalette::BrightText, Color::TEXT);
  palette.setColor(QPalette::Light, Color::BASE.lighter(130));
  palette.setColor(QPalette::Mid, Color::BASE);
  palette.setColor(QPalette::Dark, Color::SURFACE.darker(130));
  palette.setColor(QPalette::Link, Color::ACCENT);
  palette.setColor(QPalette::Highlight, Color::ACCENT);
  palette.setColor(QPalette::HighlightedText, Qt::white);
  palette.setColor(QPalette::Accent, Color::ACCENT);
  palette.setColor(QPalette::Disabled, QPalette::Text, Color::TEXT_SECONDARY);
  palette.setColor(QPalette::Disabled, QPalette::WindowText, Color::TEXT_SECONDARY);
  palette.setColor(QPalette::Disabled, QPalette::ButtonText, Color::TEXT_SECONDARY);
  return palette;
}

/**
 * Fusion style with a dark palette and the system font on all platforms.
 * Unlike the native styles, Fusion draws all inputs at the same, freely
 * chosen size. The dark color scheme keeps native title bars dark.
 */
void setStyle() {
  QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Dark);
  QApplication::setStyle(new AppStyle());
  QApplication::setPalette(darkPalette());
  QApplication::setWindowIcon(QIcon(":icons/app.svg"));
}

/**
 * Initialise function and integration manager (library)
 */
void initManager() {
  FunctionManager *function_manager =
      &FunctionManager::getInstance();
  IntegrationManager *integration_manager =
      &IntegrationManager::getInstance();
  function_manager->init();
  integration_manager->init();
}

/**
 * Free all allocated memory from function and integration manager (library)
 */
void freeManager() {
  FunctionManager *function_manager =
      &FunctionManager::getInstance();
  IntegrationManager *integration_manager =
      &IntegrationManager::getInstance();
  function_manager->free();
  integration_manager->free();
}

int main(int argc, char *argv[]) {
  // Init functions and integrations
  initManager();

  // Create application and set style
  QApplication a(argc, argv);
  QApplication::setApplicationName("IntegrationGui");
  QApplication::setApplicationDisplayName(QObject::tr("Integration GUI"));
  QApplication::setOrganizationName("RWTH SDI");
  QApplication::setApplicationVersion(PROJECT_VERSION);
  setStyle();

  // Create windows content
  MainWindow window;

  // Generate connections in interface
  Controller controller(window.getSidebar(), window.getControlBar(), window.getStatusbar(), window.getChartView());

  // Start application with a plot of the default values
  window.show();
  QMetaObject::invokeMethod(window.getSidebar(), &SideBar::sendValuesToController, Qt::QueuedConnection);
  int return_value = QApplication::exec();

  // Deallocate memory in lib
  freeManager();

  return return_value;
}
