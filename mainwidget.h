// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include <QSlider>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QGraphicsWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsGridLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QMenuBar>

#include "rangeslider.h"

QT_USE_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

public slots:
    void addSeries();
    void removeSeries();
    void connectMarkers();
    void disconnectMarkers();


private slots:
    void handleMarkerClicked();
    void handleSelectDirClicked(QLineEdit *fieldToUpdate);


private:
    void createMenuBar();
    void createChartRelatedStuff();
    void createParseArcModule();
    void createAnalyzeCompareCsvModule();
    void createTrimCsvModule();
    void createExportImageModule();
    void createExportPdfModule();

    // CHART
    QChart *m_chart;
    QList<QLineSeries *> m_series;

    // General
    QMenuBar *m_menuBar;
    QVBoxLayout *m_modulesLayout;
    QChartView *m_chartView;
    QGridLayout *m_mainLayout;

    // Arc Parse module
    QPushButton *m_selectInputDirPushButton;
    QLineEdit *m_selectedInputDirLineEdit;
    QPushButton *m_selectOutputDirPushButton;
    QLineEdit *m_selectedOutputDirLineEdit;
    QFormLayout *m_parseArcDatLayout;
    QVBoxLayout *m_parseArcDatVBoxLayout;
    QPushButton *m_parseArcDatExecutePushButton;
    QGroupBox *m_parseArcDatModule;

    // Analyzer Compare CSV module
    QPushButton *m_selectInputDirCsvPushButton;
    QLineEdit *m_selectedInputDirCsvLineEdit;
    QPushButton *m_selectOutputDirCsvPushButton;
    QLineEdit *m_selectedOutputDirCsvLineEdit;
    QFormLayout *m_analyzeCompareCsvLayout;
    QVBoxLayout *m_analyzeCompareCsvVBoxLayout;
    QPushButton *m_analyzeCompareCsvExecutePushButton;
    QGroupBox *m_analyzeCompareCsvModule;

    // Trim CSV module
    QGroupBox *m_trimCsvModule;
    RangeSlider *m_trimCsvSlider;

    // Export image
    QGroupBox *m_exportImageModule;
    QToolButton *m_svgOrJpgToolButton;
    QLineEdit *m_exportImageOutputFilename;
    QPushButton *m_exportImagePushButton;

    // Export PDF
    QGroupBox *m_exportPdfModule;
    QLineEdit *m_exportPdfOutputFilename;
    QPushButton *m_exportPdfPushButton;

    // Chart with a slider
    QGroupBox *m_chartModule;
    QSlider *m_chartSlider;


};

#endif // MAINWIDGET_H
