#pragma once

#include "../IGenerator/GeneratorSettings.h"

#include <QFrame>

namespace Core
{
    namespace Widgets
    {
        class GPUSIM_FE_CORE_EXPORT IGeneratorSettingsEditorWidget: public QFrame
        {
            Q_OBJECT;

        public:
            IGeneratorSettingsEditorWidget(QWidget *pParent = nullptr);
            virtual ~IGeneratorSettingsEditorWidget();

        public slots:
            virtual Core::CGeneratorSettings getSettings() const = 0;
            virtual void setSettings(const Core::CGeneratorSettings &settings) = 0;
        };
    }
}