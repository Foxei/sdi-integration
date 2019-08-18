#include "Worker.h"

#include "model/Definitions.h"
#include "model/Data.h"

#include <QDebug>

Worker::Worker() {
  Data *data = &Data::getInstance();
  a_ = data->LOWER_BOUND;
  b_ = data->UPPER_BOUND;
  p_ = data->PARAMETER;
  m_ = data->NUMBER_POINTS;
  index_function_ = data->FUNCTION_INDEX;
  index_integration_ = data->INTEGRATION_INDEX;
}

void Worker::process() {
  // Integration result have to changed to qVector to send easy via threads
  IntegrationManager *integration_manager = &IntegrationManager::getInstance();

  std::string p = p_;
  ad_type integral = integration_manager->evaluateIntegral(index_integration_,
                                                           a_, b_, m_, p,
                                                           index_function_);

  std::vector<ad_type> result = integration_manager->getBarHeight
      (index_integration_);

  QVector<double> vector;
  double h = (b_ - a_) / (m_);
  vector << a_ << 0.0;
  for (size_t i = 0; i < result.size(); i++) {
    double x = a_ + h * i;
    //double f = result.at(i)._value();
    double f = result.at(i);
    vector << x << f << x + h << f;
    vector << x + h << 0;
  }
  //QThread::sleep(5);

  // Send values than close thread
  emit sendResult(integral, vector);
  emit finished();
}