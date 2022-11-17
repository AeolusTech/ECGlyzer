// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "mainwidget.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtCore/QDebug>
#include <QtCharts/QLegend>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QLineSeries>
#include <QtCharts/QXYLegendMarker>
#include <QtCore/QtMath>

QT_USE_NAMESPACE

void MainWidget::createChartRelatedStuff()
{
    m_chart = new QChart();
    m_chartView = new QChartView(m_chart, this);
    m_mainLayout = new QGridLayout();

    // Set the title and show legend
    m_chart->setTitle("Legendmarker example (click on legend)");
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    m_chartView->setRenderHint(QPainter::Antialiasing);

    m_mainLayout->addWidget(m_chartView, 0, 1, 3, 1);


    // Add few series
    addSeries();
    addSeries();
    addSeries();
    addSeries();

    connectMarkers();
}

void MainWidget::createMenuBar()
{
    m_menuBar = new QMenuBar;
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    QAction *exitAction = fileMenu->addAction(tr("&Exit"));
//    QObject::connect(exitAction, &QAction::triggered, this, );
    m_menuBar->addMenu(fileMenu);
}

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    createChartRelatedStuff();
    createMenuBar();

    m_modulesLayout = new QFormLayout();
    m_selectInputDir = new QPushButton("Input directory");
    QObject::connect(m_selectInputDir, &QPushButton::clicked, this, &MainWidget::handleOpenFileClicked);

    m_selectedFileNamesTextEdit = new QTextEdit("");
    m_selectedFileNamesTextEdit->setReadOnly(true);
    m_modulesLayout->addRow(m_selectInputDir, m_selectedFileNamesTextEdit);
    m_mainLayout->addLayout(m_modulesLayout, 0, 0);


    setLayout(m_mainLayout);

}

void MainWidget::addSeries()
{
    QLineSeries *series = new QLineSeries();
    m_series.append(series);

    series->setName(QString("line " + QString::number(m_series.count())));

    // Make some sine wave for data
    QList<QPointF> data;
    int offset = m_chart->series().count();
    for (int i = 0; i < 360; i++) {
        qreal x = offset * 20 + i;
        data.append(QPointF(i, qSin(qDegreesToRadians(x))));
    }

    series->append(data);
    m_chart->addSeries(series);

    if (m_series.count() == 1)
        m_chart->createDefaultAxes();
}

void MainWidget::removeSeries()
{
    // Remove last series from chart
    if (m_series.count() > 0) {
        QLineSeries *series = m_series.last();
        m_chart->removeSeries(series);
        m_series.removeLast();
        delete series;
    }
}

void MainWidget::connectMarkers()
{
//![1]
    // Connect all markers to handler
    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWidget::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &MainWidget::handleMarkerClicked);
    }
//![1]
}

void MainWidget::disconnectMarkers()
{
//![2]
    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWidget::handleMarkerClicked);
    }
//![2]
}

void MainWidget::handleMarkerClicked()
{
//![3]
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
//![3]

//![4]
    switch (marker->type())
//![4]
    {
    case QLegendMarker::LegendMarkerTypeXY:
        {
//![5]
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);
//![5]

//![6]
        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible())
            alpha = 0.5;

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

//![6]
        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}

void MainWidget::handleOpenFileClicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/Users/kkc/private-repos/holter/example-files",tr("ARC Files (*.arc)"));
    if (fileNames.length() != 0) {
        m_selectedFileNamesTextEdit->clear();
        for (const auto& fileName: fileNames) {
            m_selectedFileNamesTextEdit->insertPlainText(fileName);
            m_selectedFileNamesTextEdit->insertPlainText("\n");
        }
    }

}
