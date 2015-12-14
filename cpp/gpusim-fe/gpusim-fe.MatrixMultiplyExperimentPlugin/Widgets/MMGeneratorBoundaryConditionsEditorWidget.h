#pragma once

#include "../../gpusim-fe.Core/Widgets/IGeneratorBoundaryConditionsEditorWidget.h"
#include "ui_MMGeneratorBoundaryConditionsEditorWidget.h"

namespace MMEP
{
    class CMMGeneratorBoundaryConditionsEditorWidget: public Core::Widgets::IGeneratorBoundaryConditionsEditorWidget
    {
        Q_OBJECT;
    public:
        CMMGeneratorBoundaryConditionsEditorWidget(QWidget *pParent = nullptr);
        virtual ~CMMGeneratorBoundaryConditionsEditorWidget();

    public slots:
        virtual Core::CGeneratorBoundaryConditions getConditions() const;
        virtual void setConditions(const Core::CGeneratorBoundaryConditions &conditions);

    private:
        Ui::CMMGeneratorBoundaryConditionsEditorWidget ui;
    };
}