#pragma once

#include "../gpusim-fe.Core/Plot/Plot.h"
#include "ui_PlotWidget.h"

class CPlotWidget: public QFrame
{
    Q_OBJECT;
public:
    CPlotWidget(const Core::CPlot &plot, QWidget *pParent = nullptr);

    static quint32 getPensCount();
    static QPen getPen(quint32 penNumber);

private:
    Ui::CPlotWidget ui;

private:
    static QVector<QPen> s_pens;
    static void initPens();
};