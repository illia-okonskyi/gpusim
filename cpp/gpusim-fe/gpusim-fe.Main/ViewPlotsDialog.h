#pragma once

#include "../gpusim-fe.Core/Plot/Plot.h"
#include "ui_ViewPlotsDialog.h"

class CViewPlotsDialog : public QDialog
{
    Q_OBJECT;
public:
    CViewPlotsDialog(const Core::CPlotsList &plots, QWidget *pParent = nullptr);

private:
    Ui::CViewPlotsDialog ui;
};