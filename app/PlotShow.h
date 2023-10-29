#ifndef PLOTSHOW_H
#define PLOTSHOW_H
#include <QVector>
class QCustomPlot;
class QString;

namespace ps{
void showPlot(QCustomPlot *plot,
              QVector<double> &xValues,
              QVector<double> &yValues,
              const QString &spectr_type);
}
#endif // PLOTSHOW_H
