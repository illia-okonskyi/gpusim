#pragma once

#include "../IGenerator/GeneratorBorderConditions.h"

#include <QFrame>

namespace Core
{
    namespace Widgets
    {
        class GPUSIM_FE_CORE_EXPORT IGeneratorBoundaryConditionsEditorWidget: public QFrame
        {
            Q_OBJECT;

        public:
            IGeneratorBoundaryConditionsEditorWidget(QWidget *pParent = nullptr);
            virtual ~IGeneratorBoundaryConditionsEditorWidget();
       
        public slots:
            virtual Core::CGeneratorBoundaryConditions getConditions() const = 0;
            virtual void setConditions(const Core::CGeneratorBoundaryConditions &conditions) = 0;
        };
    }
}