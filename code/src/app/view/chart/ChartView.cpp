#include "ChartView.h"

#include "model/Data.h"

ChartView::ChartView(QWidget *parent) : QChartView(parent) {
  setUpGui();
}

void ChartView::setUpGui() {
  chart_ = new QChart();
  chart_->legend()->hide();
  chart_->layout()->setContentsMargins(10, 10, 0, 10);

  axis_x_ = new QValueAxis();
  axis_y_ = new QValueAxis();

  chart_->setTheme(QChart::ChartThemeDark);
  QBrush axisBrush(QColor(35, 35, 35));
  chart_->setBackgroundBrush(axisBrush);
  chart_->setBackgroundVisible(true);
  setRenderHint(QPainter::Antialiasing);
  setChart(chart_);

  chart_->addAxis(axis_x_, Qt::AlignBottom);
  chart_->addAxis(axis_y_, Qt::AlignLeft);
}

ChartView::~ChartView() {
  delete chart_;
  delete function_series_;
  delete area_series_;
  delete axis_x_;
  delete axis_y_;
}

void ChartView::clearPlot() {
  if (function_series_ != nullptr) {
    chart_->removeSeries(function_series_);
    delete function_series_;
    function_series_ = nullptr;
  }
  if (area_series_ != nullptr) {
    chart_->removeSeries(area_series_);
    delete area_series_;
    area_series_ = nullptr;
  }
  axis_x_->setRange(0, 1);
  axis_y_->setRange(0, 1);
}

void ChartView::plot(const QVector<double>& values) {
  Data *data = &Data::getInstance();
  double a = data->LOWER_BOUND;
  double b = data->UPPER_BOUND;
  std::string p = data->PARAMETER;
  size_t index_function = data->FUNCTION_INDEX;
  double m = Data::FUNCTION_FINENESS;
  double h = (b - a) / m;
  FunctionManager *function_manager = &FunctionManager::getInstance();

  if (function_series_ != nullptr) {
    chart_->removeSeries(function_series_);
    delete function_series_;
  }
  if (area_series_ != nullptr) {
    chart_->removeSeries(area_series_);
    delete area_series_;
  }

  function_series_ = new QSplineSeries(chart_);

  double min = 0;
  double max = 0;
  for (int i = 0; i <= m; i++) {
    double x = a + h * i;
    double f = function_manager->evaluateFunction(index_function, p, x);
    if (min > f)
      min = f;
    if (max < f)
      max = f;
    function_series_->append(QPointF(x, f));
  }

  auto *zero_line = new QLineSeries(chart_);
  *zero_line << QPointF(a, 0) << QPointF(b, 0);
  auto *bar_line = new QLineSeries(chart_);
  for (int i = 0; i < values.size(); i += 2) {
    *bar_line << QPointF(values.at(i), values.at(i + 1));
  }

  area_series_ = new QAreaSeries(chart_);
  if (values.empty())
    area_series_->setUpperSeries(function_series_);
  else
    area_series_->setUpperSeries(bar_line);
  area_series_->setLowerSeries(zero_line);

  axis_x_->setRange(a, b);
  double eps = fabs(fmax(fabs(max), fabs(min))) / 20;
  axis_y_->setRange(min - eps, max + eps);

  chart_->addSeries(function_series_);
  chart_->addSeries(area_series_);

  function_series_->attachAxis(axis_y_);
  area_series_->attachAxis(axis_y_);

  setStyle();
  emit finishedUpdatingPlot();
}

void ChartView::setStyle() {
  QPen pen_area(QColor(100, 100, 120));
  pen_area.setWidth(1);
  area_series_->setPen(pen_area);
  QBrush brush(QColor(120, 120, 140, 80));
  area_series_->setBrush(brush);

  QPen pen_function(0x2a82da);
  pen_function.setWidth(3);
  function_series_->setPen(pen_function);
}


