#include "view/Mainwindow.h"
#include "model/Definitions.h"
#include "view/ColorDefinitions.h"

#include <IntergrationLibrary/function_manager.h>
#include <IntergrationLibrary/integration_manager.h>

#include <QtWidgets/QApplication>
#include <QtCore/QFile>
#include <QtWidgets/QStyleFactory>
#include <QtGui/QPalette>

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
 * Setting style to dark theme and cross platform fonts
 * @param application Application were the style will be changed
 */
void setStyle(QApplication &application) {
  // Color theme
  qInfo("Do app style processing");
  QPalette darkPalette;
  darkPalette.setColor(QPalette::Window, Color::BACKGROUND);
  darkPalette.setColor(QPalette::WindowText, Color::BLACK);
  darkPalette.setColor(QPalette::Base, Color::BASE);
  darkPalette.setColor(QPalette::AlternateBase, Color::BACKGROUND);
  darkPalette.setColor(QPalette::ToolTipBase, Color::BASE);
  darkPalette.setColor(QPalette::ToolTipText, Color::BLACK);
  darkPalette.setColor(QPalette::Text, Color::BLACK);
  darkPalette.setColor(QPalette::Button, Color::BACKGROUND);
  darkPalette.setColor(QPalette::ButtonText, Color::BLACK);
  darkPalette.setColor(QPalette::BrightText, Color::WHITE);
  darkPalette.setColor(QPalette::Link, Color::CYAN);

  darkPalette.setColor(QPalette::Highlight, Color::BLUE);
  darkPalette.setColor(QPalette::HighlightedText, Color::BLACK);

  // Stylesheet
  qInfo("Loading Stylesheet: %s", "stylesheet.qss");
  QFile file_stylesheet(":stylesheet.qss");
  file_stylesheet.open(QFile::ReadOnly);
  QString stylesheet = QLatin1String(file_stylesheet.readAll());

  // Setup Fusion style
  qInfo("Loading Style: %s", "Fusion");
  QApplication::setPalette(darkPalette);
  QApplication::setStyle(QStyleFactory::create("Fusion"));
  application.setStyleSheet(stylesheet);

  qInfo("Setting font: %s", "WorkSans-Light.ttf");
  QFileInfo file_font(":WorkSans-Light.ttf");
  QFontDatabase::addApplicationFont(file_font.absoluteFilePath());

  QApplication::setWindowIcon(QIcon(":document_orientation_landscape.png"));
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
  setStyle(a);

  // Create windows content
  MainWindow window;

  // Generate connections in interface
  Controller controller(window.getSidebar(), window.getControlBar(), window.getStatusbar(), window.getChartView());

  // Start application
  window.show();
  int return_value = QApplication::exec();

  // Deallocate memory in lib
  freeManager();

  return return_value;
}
