// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsGridLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QMenuBar>

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

    void handleMarkerClicked();
    void handleSelectDirClicked(QTextEdit *fieldToUpdate);

private slots:
    void createMenuBar();
    void createChartRelatedStuff();

    void createParseArcModule();

private:


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
    QTextEdit *m_selectedInputDirTextEdit;
    QPushButton *m_selectOutputDirPushButton;
    QTextEdit *m_selectedOutputDirTextEdit;
    QGroupBox *m_parseArcDatModule;
    QFormLayout *m_parseArcDatLayout;
    QVBoxLayout *m_parseArcDatVBoxLayout;

    QGroupBox *m_analyzeCompareCsvModule;
    QGroupBox *m_trimCsvModule;
    QGroupBox *m_exportImageModule;
    QGroupBox *m_exportPdfModule;
    QGroupBox *m_chartModule;


};

#endif // MAINWIDGET_H
