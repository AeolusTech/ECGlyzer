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

#include "parsearc.h"
#include "rapidcsv.h"

QT_USE_NAMESPACE

namespace {

std::array<std::string, MainWidget::noOfChannels> channelNames = {"I @", "II", "III", "aVR", "AVL", "aVF", "Va", "V2", "V3", "V4", "V5", "V6", "All"};
}


void MainWidget::createChartRelatedStuff()
{
    m_chart = new QChart();
    m_chartView = new QChartView(m_chart, this);

    // Set the title and show legend
    m_chart->setTitle("ECG chart (click on legend to hide/show signals)");
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    m_chartView->setRenderHint(QPainter::Antialiasing);

    m_mainLayout->addWidget(m_chartView, 0, 1, 3, 1);
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
    m_selectedInputDirLineEdit->setMinimumWidth(300);
    m_selectedInputDirLineEdit->setReadOnly(true);
    m_selectInputDirPushButton = new QPushButton("Input file");
    QObject::connect(m_selectInputDirPushButton, &QPushButton::clicked, this,
                     [=]() { this->handleSelectFileClicked(m_selectedInputDirLineEdit, tr("ARC File (*.arc)")); });
    m_parseArcDatLayout->addRow(m_selectedInputDirLineEdit, m_selectInputDirPushButton);

    m_selectedOutputDirLineEdit = new QLineEdit("");
    m_selectedOutputDirLineEdit->setMinimumWidth(m_selectedInputDirLineEdit->minimumWidth());
    m_selectedOutputDirLineEdit->setReadOnly(true);
    m_selectOutputDirPushButton = new QPushButton("Output directory");
    QObject::connect(m_selectOutputDirPushButton, &QPushButton::clicked, this,
                     [=]() { this->handleSelectDirClicked(m_selectedOutputDirLineEdit); });
    m_parseArcDatLayout->addRow(m_selectedOutputDirLineEdit, m_selectOutputDirPushButton);

    m_parseArcDatExecutePushButton = new QPushButton("Parse");
    QObject::connect(m_parseArcDatExecutePushButton, &QPushButton::clicked, this, &MainWidget::executeArcParsing);

    hboxLayout->addItem(m_parseArcDatLayout);
    hboxLayout->addWidget(m_parseArcDatExecutePushButton);
    hboxLayout->addSpacing(m_parseArcDatExecutePushButton->logicalDpiX());

    m_parseArcDatVBoxLayout->addItem(hboxLayout);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    buttonsLayout->addWidget(new QLabel("Channels"));

    for (const auto& channelName: channelNames) {
        QPushButton *button = new QPushButton(channelName.c_str());
        button->setCheckable(true);
        if (channelName == "All") {
            buttonsLayout->addSpacing(10);
        }
        buttonsLayout->addWidget(button);

        m_pushButtonsArcModule[channelName] = button;
    }

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
    m_selectInputDirCsvPushButton = new QPushButton("Input csv");
    QObject::connect(m_selectInputDirCsvPushButton, &QPushButton::clicked, this,
                     [=]() { this->handleSelectFileClicked(m_selectedInputDirCsvLineEdit, tr("CSV File (*.csv)")); });
    m_analyzeCompareCsvLayout->addRow(m_selectedInputDirCsvLineEdit, m_selectInputDirCsvPushButton);

    m_selectedOutputDirCsvLineEdit = new QLineEdit("");
    m_selectedOutputDirCsvLineEdit->setReadOnly(true);
    m_selectOutputDirCsvPushButton = new QPushButton("Output directory");
    QObject::connect(m_selectOutputDirCsvPushButton, &QPushButton::clicked, this,
                     [=]() { this->handleSelectDirClicked(m_selectedOutputDirCsvLineEdit); });
    m_analyzeCompareCsvLayout->addRow(m_selectedOutputDirCsvLineEdit, m_selectOutputDirCsvPushButton);

    m_analyzeCompareCsvExecutePushButton = new QPushButton("Parse");
    QObject::connect(m_analyzeCompareCsvExecutePushButton, &QPushButton::clicked, this, &MainWidget::addCsvToChar);

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
    for (const auto& channelName: channelNames) {
        QPushButton *button = new QPushButton(channelName.c_str());
        button->setCheckable(true);
        if (channelName == "All") {
            buttonsLayout->addSpacing(10);
        }
        buttonsLayout->addWidget(button);
        m_pushButtonsCsvModule[channelName] = button;
    }

    m_analyzeCompareCsvVBoxLayout->addItem(buttonsLayout);

    m_analyzeCompareCsvModule->setLayout(m_analyzeCompareCsvVBoxLayout);
}

void MainWidget::createTrimCsvModule()
{
    m_trimCsvModule = new QGroupBox("Trim CSV");
    QVBoxLayout *vbox1 = new QVBoxLayout;
    m_trimCsvSlider = new RangeSlider(Qt::Horizontal);
    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    boxLayout->addWidget(m_trimCsvSlider);
    vbox1->addItem(boxLayout);


    QHBoxLayout *hbox1 = new QHBoxLayout;
    QLineEdit *le1 = new QLineEdit;
    QLineEdit *le2 = new QLineEdit;
    hbox1->addSpacing(100);
    hbox1->addWidget(le1);
    hbox1->addSpacing(100);
    hbox1->addWidget(le2);
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
    m_errorDialog = new QErrorMessage;
    m_msgDialog = new QMessageBox;

    m_mainLayout = new QGridLayout;
    m_modulesLayout = new QVBoxLayout;

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


void MainWidget::addCustomSeries(const std::vector<float>& dataX, const std::vector<float>& dataY, const std::string& chartName)
{
    QLineSeries *series = new QLineSeries();
    m_series[chartName] = series;

    series->setName(chartName.c_str());

    QList<QPointF> data;
    for (unsigned i = 0; i < dataX.size(); i++) {
        data.append(QPointF(dataX[i], dataY[i]));
    }

    series->append(data);
    m_chart->addSeries(series);

    if (m_series.size() == 1)
        m_chart->createDefaultAxes();
}

void MainWidget::clearChart() {
    for (const auto &chartElem: m_series.keys()) {
        removeSeries(chartElem);
    }
}

void MainWidget::removeSeries(const std::string& chartName)
{
    if (m_series.size() > 0) {
        QLineSeries *series = m_series[chartName];
        m_chart->removeSeries(series);
        m_series.remove(chartName);
        delete series;
    }
}

void MainWidget::connectMarkers()
{
    // Connect all markers to handler
    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWidget::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &MainWidget::handleMarkerClicked);
    }
}

void MainWidget::disconnectMarkers()
{
    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWidget::handleMarkerClicked);
    }
}

void MainWidget::handleMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);

    switch (marker->type())
    {
    case QLegendMarker::LegendMarkerTypeXY:
    {
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);

        qreal alpha = 1.0; // Dim the marker, if series is not visible

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

void MainWidget::handleSelectFileClicked(QLineEdit *fieldToUpdate, const QString& fileFilter)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/Users/kkc/private-repos/holter/example-files", fileFilter);
    if (fileName.length() != 0) {
        fieldToUpdate->clear();
        fieldToUpdate->setText(fileName);
    }
}


void MainWidget::executeArcParsing()
{


    if (m_selectedInputDirLineEdit->text().length() == 0 ||
            m_selectedOutputDirLineEdit->text().length() == 0 ) {
        m_msgDialog->setText("Select file to parse and the output directory first");
        m_msgDialog->show();
        return;
    }

    std::string filename = m_selectedInputDirLineEdit->text().toStdString();
    std::string outputDir = m_selectedOutputDirLineEdit->text().toStdString();


    auto outputFilePath = QDir(QString(outputDir.c_str())).absoluteFilePath("channel_all.csv");

    int successfullyParsed = parsearc(filename, outputFilePath.toStdString());

    if (successfullyParsed != 0) {
        m_msgDialog->setText("Sth went wrong during parsing! Please contact the developer");
        m_msgDialog->show();
        return;
    }

    m_msgDialog->setText("Successfully parsed and created chanell_all.csv file");
    m_msgDialog->show();
}

void MainWidget::addCsvToChar()
{
    if (m_selectedInputDirCsvLineEdit->text().length() == 0) {
        m_msgDialog->setText("Select file to ingest first");
        m_msgDialog->show();
        return;
    }

    std::string filename = m_selectedInputDirCsvLineEdit->text().toStdString();


    try {
        rapidcsv::Document doc(filename, rapidcsv::LabelParams(0,-1), rapidcsv::SeparatorParams(';'));

        std::vector<std::string> checkedChannels;

        if (m_pushButtonsCsvModule["All"]->isChecked()) {
            checkedChannels = doc.GetColumnNames();
            std::remove_if(checkedChannels.begin(), checkedChannels.end(), [=](const std::string& name) {return name == "Time";});
            checkedChannels.pop_back();
        } else {
            for (const auto& [channelName, button]: m_pushButtonsCsvModule) {
                if (button->isChecked()) {
                    checkedChannels.push_back(channelName);
                }
            }
        }

        if (checkedChannels.empty()) {
            m_msgDialog->setText("Select which channels you want to visualize first");
            m_msgDialog->show();
            return;
        }
        clearChart();

        std::vector<float> dataX = doc.GetColumn<float>("Time");


        for (const auto& checkedChannel: checkedChannels) {
            addCustomSeries(dataX, doc.GetColumn<float>(checkedChannel), checkedChannel);
        }

        connectMarkers();

        m_msgDialog->setText("Successfully parsed and created chanell_all.csv file");
        m_msgDialog->show();

    } catch (const std::exception& e) {
        std::string errorMessage = "Error during CSV parsing!\n";
        errorMessage += std::string(e.what());
        m_msgDialog->setText(errorMessage.c_str());
        m_msgDialog->show();
    }

}

