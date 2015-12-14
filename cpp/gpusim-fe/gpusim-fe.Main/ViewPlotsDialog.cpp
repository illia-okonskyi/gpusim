#include "ViewPlotsDialog.h"
#include "PlotWidget.h"

CViewPlotsDialog::CViewPlotsDialog(const Core::CPlotsList &plots, QWidget *pParent /*= nullptr*/)
    : QDialog(pParent)
{
    ui.setupUi(this);

    Q_ASSERT(!plots.isEmpty());
    if (plots.isEmpty())
        return;

    QStringList plotsNames;
    foreach(const Core::CPlot &plot, plots)
    {
        plotsNames << plot.getName();
        CPlotWidget *pPlotWidget = new CPlotWidget(plot, this);
        ui.swPlots->addWidget(pPlotWidget);
    }

    ui.cbPlotNames->addItems(plotsNames);
    ui.cbPlotNames->setCurrentIndex(0);
}