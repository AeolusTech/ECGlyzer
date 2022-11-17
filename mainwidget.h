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
    void handleOpenFileClicked();

private slots:
    void createMenuBar();

private:

    QChart *m_chart;
    QList<QLineSeries *> m_series;

    QPushButton *m_selectInputDir;
    QTextEdit *m_selectedFileNamesTextEdit;
    QFormLayout *m_modulesLayout;
    QMenuBar *m_menuBar;

    QGroupBox *m_parseArcDatModule;
    QGroupBox *m_analyzeCompareCsvModule;
    QGroupBox *m_trimCsvModule;
    QGroupBox *m_exportImageModule;
    QGroupBox *m_exportPdfModule;
    QGroupBox *m_chartModule;

    QChartView *m_chartView;
    QGridLayout *m_mainLayout;

    void createChartRelatedStuff();
};

#endif // MAINWIDGET_H
