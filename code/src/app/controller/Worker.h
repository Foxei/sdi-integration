#ifndef INTEGRATION_GUI_WORKER_H
#define INTEGRATION_GUI_WORKER_H

#include <QObject>
#include <QVector>
#include <QtCore/QThread>

#include <IntergrationLibrary/integration_manager.h>

/**
 * Worker do integration and derivative stuff in other thread
 */
class Worker : public QObject {
 Q_OBJECT
 private:
  double a_; //< Lower bound
  double b_; //< Upper bound
  std::string p_; //< Parameter
  unsigned int m_; //< Count of supporting points
  size_t index_function_; //< index of function in library
  size_t index_integration_; //< index of integral in library

 public:
  /**
   * Creates integral and derivative worker with all values needed see class fields
   */
  Worker();

 public slots:
  /**
   * Starts heavy background work
   */
  void process();

 signals:
  /**
   * Signal needed to stop thread correctly
   */
  void finished();

  /**
   * Signal used to send result to main thread
   */
  void sendResult(double integral, QVector<double> values);
};

#endif //INTEGRATION_GUI_WORKER_H
