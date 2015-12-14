#include "MMGeneratorBoundaryConditionsEditorWidget.h"
#include "../MMGenerator.h"

using namespace MMEP;

CMMGeneratorBoundaryConditionsEditorWidget::CMMGeneratorBoundaryConditionsEditorWidget(QWidget *pParent /*= nullptr*/)
    : Core::Widgets::IGeneratorBoundaryConditionsEditorWidget(pParent)
{
    ui.setupUi(this);
}

CMMGeneratorBoundaryConditionsEditorWidget::~CMMGeneratorBoundaryConditionsEditorWidget()
{

}

Core::CGeneratorBoundaryConditions CMMGeneratorBoundaryConditionsEditorWidget::getConditions() const
{
    quint32 minMatrixSize       = ui.sbMinMatrixSize->value();
    quint32 maxMatrixSize       = ui.sbMaxMatrixSize->value();
    quint32 matrixSizeIncrement = ui.sbMatrixSizeIncrement->value();
    quint32 blockSize           = ui.sbBlockSize->value();

    Core::CGeneratorBoundaryConditions conditions;
    conditions[CMMGenerator::c_minMatrixSize_conditionName      ] = QVariant::fromValue(minMatrixSize      );
    conditions[CMMGenerator::c_maxMatrixSize_conditionName      ] = QVariant::fromValue(maxMatrixSize      );
    conditions[CMMGenerator::c_matrixSizeIncrement_conditionName] = QVariant::fromValue(matrixSizeIncrement);
    conditions[CMMGenerator::c_blockSize_conditionName          ] = QVariant::fromValue(blockSize          );
    return conditions;
}

void CMMGeneratorBoundaryConditionsEditorWidget::setConditions(const Core::CGeneratorBoundaryConditions &conditions)
{
    quint32 minMatrixSize       = conditions[CMMGenerator::c_minMatrixSize_conditionName      ].toUInt();
    quint32 maxMatrixSize       = conditions[CMMGenerator::c_maxMatrixSize_conditionName      ].toUInt();
    quint32 matrixSizeIncrement = conditions[CMMGenerator::c_matrixSizeIncrement_conditionName].toUInt();
    quint32 blockSize           = conditions[CMMGenerator::c_blockSize_conditionName          ].toUInt();

    ui.sbMinMatrixSize->setValue(minMatrixSize);
    ui.sbMaxMatrixSize->setValue(maxMatrixSize);
    ui.sbMatrixSizeIncrement->setValue(matrixSizeIncrement);
    ui.sbBlockSize->setValue(blockSize);
}