#ifndef INTEGRATION_GUI_CHART_VIEW_H
#define INTEGRATION_GUI_CHART_VIEW_H

#include <QtCharts>

#include <IntergrationLibrary/function_manager.h>

class ChartView : public QChartView {
 Q_OBJECT
 private:

  QChart *chart_ = nullptr;
  QSplineSeries *function_series_ = nullptr;
  QAreaSeries *area_series_ = nullptr;
  QValueAxis *axis_x_ = nullptr;
  QValueAxis *axis_y_ = nullptr;
  QWidget *overlay_busy_ = nullptr; //< Dims the chart while a calculation is running

  void setUpGui();
  void setUpBusyOverlay();
  void setStyle();
 public:
  explicit ChartView(QWidget *parent = nullptr);
  ~ChartView() override;

 protected:
  void resizeEvent(QResizeEvent *event) override;

 public slots:
  /**
   * Show or hide the busy overlay on top of the chart.
   * @param busy true while a calculation is running
   */
  void showBusy(bool busy);

  void clearPlot();
  void plot(const QVector<double>& values);

 signals:
  void finishedUpdatingPlot();
};

#endif //INTEGRATION_GUI_CHART_VIEW_H
