#pragma once

#include "../../gpusim-fe.Core/Widgets/IGeneratorBoundaryConditionsEditorWidget.h"
#include "ui_NBGeneratorBoundaryConditionsEditorWidget.h"

namespace NBEP
{
    class CNBGeneratorBoundaryConditionsEditorWidget: public Core::Widgets::IGeneratorBoundaryConditionsEditorWidget
    {
        Q_OBJECT;
    public:
        CNBGeneratorBoundaryConditionsEditorWidget(QWidget *pParent = nullptr);
        virtual ~CNBGeneratorBoundaryConditionsEditorWidget();

    public slots:
        virtual Core::CGeneratorBoundaryConditions getConditions() const;
        virtual void setConditions(const Core::CGeneratorBoundaryConditions &conditions);

    private:
        Ui::CNBGeneratorBoundaryConditionsEditorWidget ui;
    };
}