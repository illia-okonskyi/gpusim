#include "PlotWidget.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>

#ifdef _DEBUG
#pragma comment(lib, "qwtd.lib")
#else
#pragma comment(lib, "qwt.lib")
#endif // _DEBUG

QVector<QPen> CPlotWidget::s_pens;

CPlotWidget::CPlotWidget(const Core::CPlot &plot, QWidget *pParent /*= nullptr*/)
    : QFrame(pParent)
{
    ui.setupUi(this);

    // 1) Configure plot widget
    //
    QwtPlotGrid *pGrid = new QwtPlotGrid();
    pGrid->setPen(QPen(Qt::lightGray));
    pGrid->attach(ui.plot);
    ui.plot->canvas()->setCursor(Qt::ArrowCursor);
    ui.plot->insertLegend(new QwtLegend(this), QwtPlot::BottomLegend);

    // 2) Set plot name and description
    //
    ui.lblPlotName->setText(plot.getName());
    ui.lblPlotDesc->setText(plot.getDesc());

    // 3) Set axes titles
    //
    ui.plot->setAxisTitle(QwtPlot::xBottom, plot.getXAxisName());
    ui.plot->setAxisTitle(QwtPlot::yLeft, plot.getYAxisName());

    // 4) Create curves
    //
    quint32 i = 0;
    foreach(const Core::CPlotCurve &curve, plot.getCurvesRef())
    {
        QwtPlotCurve *pCurve = new QwtPlotCurve(curve.getName());
        pCurve->setSamples(curve.getPoints());
        pCurve->setPen(getPen(i++));
        pCurve->attach(ui.plot);
    }

    ui.plot->replot();
}

//////////////////////////////////////////////////////////////////////////

quint32 CPlotWidget::getPensCount()
{
    initPens();
    return s_pens.size();
}

QPen CPlotWidget::getPen(quint32 penNumber)
{
    initPens();
    return s_pens[penNumber % s_pens.size()];
}

//////////////////////////////////////////////////////////////////////////

void CPlotWidget::initPens()
{
    if (s_pens.size())
        return;

    s_pens.push_back(QPen(Qt::blue, 2.0f));
    s_pens.push_back(QPen(Qt::red, 2.0f));
    s_pens.push_back(QPen(Qt::darkGreen, 2.0f));
}