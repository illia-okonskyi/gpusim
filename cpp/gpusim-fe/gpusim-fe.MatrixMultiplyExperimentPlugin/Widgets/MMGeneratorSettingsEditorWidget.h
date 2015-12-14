#pragma once

#include "../../gpusim-fe.Core/Widgets/IGeneratorSettingsEditorWidget.h"
#include "ui_MMGeneratorSettingsEditorWidget.h"

namespace MMEP
{
    class CMMGeneratorSettingsEditorWidget: public Core::Widgets::IGeneratorSettingsEditorWidget
    {
        Q_OBJECT;
    public:
        CMMGeneratorSettingsEditorWidget(QWidget *pParent = nullptr);
        virtual ~CMMGeneratorSettingsEditorWidget();

    public slots:
        virtual Core::CGeneratorSettings getSettings() const;
        virtual void setSettings(const Core::CGeneratorSettings &settings);

    private:
        Ui::CMMGeneratorSettingsEditorWidget ui;
    };
}