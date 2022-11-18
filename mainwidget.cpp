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
    QObject::connect(exitAction, &QAction::triggered, this, &QWidget::close);
    m_menuBar->addMenu(fileMenu);
}

void MainWidget::createParseArcModule()
{
    m_parseArcDatModule = new QGroupBox("Parse ARC");
    m_parseArcDatVBoxLayout = new QVBoxLayout;
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    m_parseArcDatLayout = new QFormLayout;

    m_selectedInputDirLineEdit = new QLineEdit("");
    m_selectedInputDirLineEdit->setReadOnly(true);
    m_selectInputDirPushButton = new QPushButton("Input directory");
    QObject::connect(m_selectInputDirPushButton, &QPushButton::clicked, this,
                     [=]() { this->handleSelectDirClicked(m_selectedInputDirLineEdit); });
    m_parseArcDatLayout->addRow(m_selectedInputDirLineEdit, m_selectInputDirPushButton);

    m_selectedOutputDirLineEdit = new QLineEdit("");
    m_selectedOutputDirLineEdit->setReadOnly(true);
    m_selectOutputDirPushButton = new QPushButton("Output directory");
    QObject::connect(m_selectOutputDirPushButton, &QPushButton::clicked, this,
                     [=]() { this->handleSelectDirClicked(m_selectedOutputDirLineEdit); });
    m_parseArcDatLayout->addRow(m_selectedOutputDirLineEdit, m_selectOutputDirPushButton);

    m_parseArcDatExecutePushButton = new QPushButton("Parse");

    hboxLayout->addItem(m_parseArcDatLayout);
    hboxLayout->addWidget(m_parseArcDatExecutePushButton);
    hboxLayout->addSpacing(m_parseArcDatExecutePushButton->logicalDpiX());

    m_parseArcDatVBoxLayout->addItem(hboxLayout);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    buttonsLayout->addWidget(new QLabel("Channels"));
    QPushButton *buttonI = new QPushButton("I");
    buttonsLayout->addWidget(buttonI);

    QPushButton *buttonII = new QPushButton("II");
    buttonsLayout->addWidget(buttonII);

    QPushButton *buttonIII = new QPushButton("III");
    buttonsLayout->addWidget(buttonIII);

    QPushButton *buttonaVr = new QPushButton("aVR");
    buttonsLayout->addWidget(buttonaVr);

    QPushButton *buttonAVL = new QPushButton("AVL");
    buttonsLayout->addWidget(buttonAVL);

    QPushButton *buttonaVF = new QPushButton("aVF");
    buttonsLayout->addWidget(buttonaVF);

    QPushButton *buttonVa = new QPushButton("Va");
    buttonsLayout->addWidget(buttonVa);

    QPushButton *buttonV2 = new QPushButton("V2");
    buttonsLayout->addWidget(buttonV2);

    QPushButton *buttonV3 = new QPushButton("V3");
    buttonsLayout->addWidget(buttonV3);

    QPushButton *buttonV4 = new QPushButton("V4");
    buttonsLayout->addWidget(buttonV4);

    QPushButton *buttonV5 = new QPushButton("V5");
    buttonsLayout->addWidget(buttonV5);

    QPushButton *buttonV6 = new QPushButton("V6");
    buttonsLayout->addWidget(buttonV6);

    QPushButton *buttonAll = new QPushButton("All");
    buttonsLayout->addSpacing(10);
    buttonsLayout->addWidget(buttonAll);

    m_parseArcDatVBoxLayout->addItem(buttonsLayout);

    m_parseArcDatModule->setLayout(m_parseArcDatVBoxLayout);
}

void MainWidget::createAnalyzeCompareCsvModule()
{
    m_analyzeCompareCsvModule = new QGroupBox("Analyze and Compare CSV");
    m_analyzeCompareCsvVBoxLayout = new QVBoxLayout;
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    m_analyzeCompareCsvLayout = new QFormLayout;

    m_selectedInputDirCsvLineEdit = new QLineEdit("");
    m_selectedInputDirCsvLineEdit->setReadOnly(true);
    m_selectInputDirCsvPushButton = new QPushButton("Input directory");
    QObject::connect(m_selectInputDirCsvPushButton, &QPushButton::clicked, this,
                     [=]() { this->handleSelectDirClicked(m_selectedInputDirCsvLineEdit); });
    m_analyzeCompareCsvLayout->addRow(m_selectedInputDirCsvLineEdit, m_selectInputDirCsvPushButton);

    m_selectedOutputDirCsvLineEdit = new QLineEdit("");
    m_selectedOutputDirCsvLineEdit->setReadOnly(true);
    m_selectOutputDirCsvPushButton = new QPushButton("Output directory");
    QObject::connect(m_selectOutputDirCsvPushButton, &QPushButton::clicked, this,
                     [=]() { this->handleSelectDirClicked(m_selectedOutputDirCsvLineEdit); });
    m_analyzeCompareCsvLayout->addRow(m_selectedOutputDirCsvLineEdit, m_selectOutputDirCsvPushButton);

    m_analyzeCompareCsvExecutePushButton = new QPushButton("Parse");


    QVBoxLayout *vbox1 = new QVBoxLayout;
    QPushButton *btn1 = new QPushButton("E");
    QPushButton *btn2 = new QPushButton("G");
    vbox1->addWidget(btn1);
    vbox1->addWidget(btn2);

    hboxLayout->addItem(m_analyzeCompareCsvLayout);
    hboxLayout->addWidget(m_analyzeCompareCsvExecutePushButton);
    hboxLayout->addItem(vbox1);

    m_analyzeCompareCsvVBoxLayout->addItem(hboxLayout);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    buttonsLayout->addWidget(new QLabel("Channels"));
    QPushButton *buttonI = new QPushButton("I");
    buttonsLayout->addWidget(buttonI);

    QPushButton *buttonII = new QPushButton("II");
    buttonsLayout->addWidget(buttonII);

    QPushButton *buttonIII = new QPushButton("III");
    buttonsLayout->addWidget(buttonIII);

    QPushButton *buttonaVr = new QPushButton("aVR");
    buttonsLayout->addWidget(buttonaVr);

    QPushButton *buttonAVL = new QPushButton("AVL");
    buttonsLayout->addWidget(buttonAVL);

    QPushButton *buttonaVF = new QPushButton("aVF");
    buttonsLayout->addWidget(buttonaVF);

    QPushButton *buttonVa = new QPushButton("Va");
    buttonsLayout->addWidget(buttonVa);

    QPushButton *buttonV2 = new QPushButton("V2");
    buttonsLayout->addWidget(buttonV2);

    QPushButton *buttonV3 = new QPushButton("V3");
    buttonsLayout->addWidget(buttonV3);

    QPushButton *buttonV4 = new QPushButton("V4");
    buttonsLayout->addWidget(buttonV4);

    QPushButton *buttonV5 = new QPushButton("V5");
    buttonsLayout->addWidget(buttonV5);

    QPushButton *buttonV6 = new QPushButton("V6");
    buttonsLayout->addWidget(buttonV6);

    QPushButton *buttonAll = new QPushButton("All");
    buttonsLayout->addSpacing(10);
    buttonsLayout->addWidget(buttonAll);

    m_analyzeCompareCsvVBoxLayout->addItem(buttonsLayout);

    m_analyzeCompareCsvModule->setLayout(m_analyzeCompareCsvVBoxLayout);
}

void MainWidget::createTrimCsvModule()
{
    m_trimCsvModule = new QGroupBox("Trim CSV");
    QVBoxLayout *vbox1 = new QVBoxLayout;
    m_trimCsvSlider = new RangeSlider(Qt::Horizontal);
//    m_trimCsvSlider->setTickPosition(QSlider::TicksBelow);
    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    boxLayout->addWidget(m_trimCsvSlider);
    vbox1->addItem(boxLayout);


    QHBoxLayout *hbox1 = new QHBoxLayout;
    QLineEdit *te1 = new QLineEdit;
    QLineEdit *te2 = new QLineEdit;
    hbox1->addSpacing(100);
    hbox1->addWidget(te1);
    hbox1->addSpacing(100);
    hbox1->addWidget(te2);
    hbox1->addSpacing(100);

    QHBoxLayout *hbox2 = new QHBoxLayout;
    QLabel *label1 = new QLabel("Start");
    QLabel *label2 = new QLabel("Stop");
    hbox2->addSpacing(100);
    hbox2->addWidget(label1);
    hbox2->addSpacing(100);
    hbox2->addWidget(label2);
    hbox2->addSpacing(100);

    vbox1->addItem(hbox1);
    vbox1->addItem(hbox2);
    m_trimCsvModule->setLayout(vbox1);
}

void MainWidget::createExportImageModule()
{
    m_exportImageModule = new QGroupBox("Export image");
    QFormLayout *formLayout = new QFormLayout;

    QMenu* menu = new QMenu;
    QAction* svgAction = menu->addAction(tr("svg"));
    menu->addAction(tr("jpg"));
    m_svgOrJpgToolButton = new QToolButton;
    m_svgOrJpgToolButton->setDefaultAction(svgAction);
    m_svgOrJpgToolButton->setMenu(menu);
    QObject::connect(m_svgOrJpgToolButton, &QToolButton::clicked, this,
                     [=]() { m_svgOrJpgToolButton->showMenu(); });
    QObject::connect(m_svgOrJpgToolButton, &QToolButton::triggered, this,
                     [=]( QAction *newValue ) { m_svgOrJpgToolButton->setDefaultAction(newValue );} );
    formLayout->addRow(m_svgOrJpgToolButton, new QLabel("Select Image format"));

    m_exportImageOutputFilename = new QLineEdit("");
    m_exportImageOutputFilename->setReadOnly(true);
    m_exportImagePushButton = new QPushButton("Export Image");
    formLayout->addRow(m_exportImageOutputFilename, m_exportImagePushButton);
    m_exportImageModule->setLayout(formLayout);
}

void MainWidget::createExportPdfModule()
{
    m_exportPdfModule = new QGroupBox("Export PDF");
    QFormLayout *formLayout = new QFormLayout;
    m_exportPdfOutputFilename = new QLineEdit("");
    m_exportPdfOutputFilename->setReadOnly(true);
    m_exportPdfPushButton = new QPushButton("Export PDF");
    formLayout->addRow(m_exportPdfOutputFilename, m_exportPdfPushButton);
    m_exportPdfModule->setLayout(formLayout);
}

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    m_mainLayout = new QGridLayout;
    m_modulesLayout = new QVBoxLayout();

    createChartRelatedStuff();
    createMenuBar();
    createParseArcModule();
    createAnalyzeCompareCsvModule();
    createTrimCsvModule();
    createExportImageModule();
    createExportPdfModule();

    m_modulesLayout->addWidget(m_parseArcDatModule);
    m_modulesLayout->addWidget(m_analyzeCompareCsvModule);
    m_modulesLayout->addWidget(m_trimCsvModule);
    m_modulesLayout->addWidget(m_exportImageModule);
    m_modulesLayout->addWidget(m_exportPdfModule);
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

void MainWidget::handleSelectDirClicked(QLineEdit *fieldToUpdate)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select dir"),"/Users/kkc/private-repos/holter/example-files");
    if (dir.length() != 0) {
        fieldToUpdate->clear();
        fieldToUpdate->setText(dir);
    }

}
