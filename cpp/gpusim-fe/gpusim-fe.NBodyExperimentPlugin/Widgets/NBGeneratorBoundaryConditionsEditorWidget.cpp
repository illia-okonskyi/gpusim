#include "NBGeneratorBoundaryConditionsEditorWidget.h"
#include "../NBGenerator.h"

using namespace NBEP;

CNBGeneratorBoundaryConditionsEditorWidget::CNBGeneratorBoundaryConditionsEditorWidget(QWidget *pParent /*= nullptr*/)
    : Core::Widgets::IGeneratorBoundaryConditionsEditorWidget(pParent)
{
    ui.setupUi(this);
}

CNBGeneratorBoundaryConditionsEditorWidget::~CNBGeneratorBoundaryConditionsEditorWidget()
{

}

Core::CGeneratorBoundaryConditions CNBGeneratorBoundaryConditionsEditorWidget::getConditions() const
{
    quint32 minN   = ui.sbMinN->value();
    quint32 maxN   = ui.sbMaxN->value();
    quint32 minTPB = ui.sbMinTPB->value();
    quint32 maxTPB = ui.sbMaxTPB->value();

    Core::CGeneratorBoundaryConditions conditions;
    conditions[CNBGenerator::c_minN_conditionName  ] = QVariant::fromValue(minN  );
    conditions[CNBGenerator::c_maxN_conditionName  ] = QVariant::fromValue(maxN  );
    conditions[CNBGenerator::c_minTPB_conditionName] = QVariant::fromValue(minTPB);
    conditions[CNBGenerator::c_maxTPB_conditionName] = QVariant::fromValue(maxTPB);
    return conditions;
}

void CNBGeneratorBoundaryConditionsEditorWidget::setConditions(const Core::CGeneratorBoundaryConditions &conditions)
{
    quint32 minN   = conditions[CNBGenerator::c_minN_conditionName  ].toUInt();
    quint32 maxN   = conditions[CNBGenerator::c_maxN_conditionName  ].toUInt();
    quint32 minTPB = conditions[CNBGenerator::c_minTPB_conditionName].toUInt();
    quint32 maxTPB = conditions[CNBGenerator::c_maxTPB_conditionName].toUInt();

    ui.sbMinN->setValue(minN);
    ui.sbMaxN->setValue(maxN);
    ui.sbMinTPB->setValue(minTPB);
    ui.sbMaxTPB->setValue(maxTPB);
}