#pragma once

#include "../../gpusim-fe.Core/Widgets/IGeneratorSettingsEditorWidget.h"
#include "ui_NBGeneratorSettingsEditorWidget.h"

namespace NBEP
{
    class CNBGeneratorSettingsEditorWidget: public Core::Widgets::IGeneratorSettingsEditorWidget
    {
        Q_OBJECT;
    public:
        CNBGeneratorSettingsEditorWidget(QWidget *pParent = nullptr);
        virtual ~CNBGeneratorSettingsEditorWidget();

    public slots:
        virtual Core::CGeneratorSettings getSettings() const;
        virtual void setSettings(const Core::CGeneratorSettings &settings);

    private:
        Ui::CNBGeneratorSettingsEditorWidget ui;
    };
}