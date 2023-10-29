#include "QCustomPlot.h"
namespace ps{
void showPlot(QCustomPlot* plot,
              QVector<double> &xValues,
              QVector<double> &yValues,
              const QString &spectr_type)
{
    plot->clearGraphs();
    plot->addGraph();
    auto it = std::minmax_element(yValues.begin(),yValues.end());
    double min = *it.first;
    double max = *it.second;
    auto xPair = std::minmax_element(xValues.begin(),xValues.end());
    auto doubleXmin = *xPair.first;
    auto doubleXmax = *xPair.second;
    plot->graph(0)->setData(xValues,yValues);
    plot->xAxis->setRange(doubleXmin,doubleXmax);
    //plot->xAxis->setLabel("длина волны");
    //plot->yAxis->setLabel("кся");
    if(spectr_type=="кся"){
        if(max>1){
            max=1;
            min=0;
        }
    }
    plot->yAxis->setRange(min,max);
    plot->replot();
}
}
