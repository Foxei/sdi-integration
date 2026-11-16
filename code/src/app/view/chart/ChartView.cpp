#include "ChartView.h"

#include "model/Data.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>

ChartView::ChartView(QWidget *parent) : QChartView(parent) {
  setUpGui();
}

void ChartView::setUpGui() {
  chart_ = new QChart();
  chart_->legend()->hide();
  chart_->layout()->setContentsMargins(0, 0, 0, 0);
  chart_->setMargins(QMargins(16, 16, 16, 16));

  axis_x_ = new QValueAxis();
  axis_y_ = new QValueAxis();

  // Dark theme, darker plot area with light grid lines for contrast
  chart_->setTheme(QChart::ChartThemeDark);
  chart_->setBackgroundVisible(false);
  chart_->setPlotAreaBackgroundBrush(palette().base());
  chart_->setPlotAreaBackgroundVisible(true);
  setBackgroundBrush(palette().window());
  setFrameShape(QFrame::NoFrame);
  setRenderHint(QPainter::Antialiasing);
  setChart(chart_);

  chart_->addAxis(axis_x_, Qt::AlignBottom);
  chart_->addAxis(axis_y_, Qt::AlignLeft);

  setUpBusyOverlay();
  QColor color_grid = palette().color(QPalette::Text);
  color_grid.setAlpha(40);
  QColor color_axis = palette().color(QPalette::Text);
  color_axis.setAlpha(110);
  for (QValueAxis *axis : {axis_x_, axis_y_}) {
    axis->setLabelsFont(font());
    axis->setLabelsColor(palette().color(QPalette::PlaceholderText));
    axis->setGridLineColor(color_grid);
    axis->setLinePenColor(color_axis);
    axis->setMinorGridLineVisible(false);
  }
}

void ChartView::setUpBusyOverlay() {
  overlay_busy_ = new QWidget(this);
  overlay_busy_->setAttribute(Qt::WA_StyledBackground);
  QColor dim = palette().color(QPalette::Window);
  dim.setAlpha(190);
  overlay_busy_->setStyleSheet(QString("background: %1;").arg(dim.name(QColor::HexArgb)));

  auto *label = new QLabel(tr("Calculating…"), overlay_busy_);
  QFont font_label = label->font();
  font_label.setPointSizeF(font_label.pointSizeF() * 1.2);
  font_label.setWeight(QFont::DemiBold);
  label->setFont(font_label);
  label->setStyleSheet("background: transparent;");

  auto *progress = new QProgressBar(overlay_busy_);
  progress->setRange(0, 0);
  progress->setTextVisible(false);
  progress->setFixedSize(180, 6);

  auto *layout = new QVBoxLayout(overlay_busy_);
  layout->addStretch(1);
  layout->addWidget(label, 0, Qt::AlignHCenter);
  layout->addSpacing(8);
  layout->addWidget(progress, 0, Qt::AlignHCenter);
  layout->addStretch(1);

  overlay_busy_->setVisible(false);
}

void ChartView::showBusy(bool busy) {
  overlay_busy_->setGeometry(rect());
  overlay_busy_->raise();
  overlay_busy_->setVisible(busy);
}

void ChartView::resizeEvent(QResizeEvent *event) {
  QChartView::resizeEvent(event);
  if (overlay_busy_ != nullptr) {
    overlay_busy_->setGeometry(rect());
  }
}

// The view owns the chart and the chart owns its series and axes,
// so Qt deletes all of them.
ChartView::~ChartView() = default;

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
  QColor accent = palette().color(QPalette::Accent);

  QColor area_line = accent;
  area_line.setAlpha(160);
  QPen pen_area(area_line);
  pen_area.setWidthF(1);
  area_series_->setPen(pen_area);
  QColor area_fill = accent;
  area_fill.setAlpha(70);
  area_series_->setBrush(area_fill);

  QPen pen_function(accent.lighter(130));
  pen_function.setWidthF(2.5);
  function_series_->setPen(pen_function);
}
