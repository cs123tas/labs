#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <assert.h>
#include "databinding.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    QGLFormat qglFormat;
    qglFormat.setVersion(4,0);
    qglFormat.setProfile(QGLFormat::CoreProfile);
    m_ui->setupUi(this);

    // canvas for drawing triangle
    QGridLayout *gridLayout = new QGridLayout(m_ui->triangleCanvas);
    m_triangleCanvas = new GLWidget(qglFormat, this);
    m_triangleCanvas->setMinimumSize(50, 50);
    gridLayout->addWidget(m_triangleCanvas, 0, 1);

    // canvas for fragment shaders
    QGridLayout *gridLayoutFragShaders = new QGridLayout(m_ui->fragCanvas);
    m_fragCanvas = new GLFragWidget(qglFormat, this);
    m_fragCanvas->setMinimumSize(50, 50);
    gridLayoutFragShaders->addWidget(m_fragCanvas, 0, 1);

    m_ui->tabWidget->setCurrentWidget(m_ui->triangle);
    m_ui->tabWidget->setCurrentWidget(m_ui->fragShaders);


    settings.loadSettingsOrDefaults();
    dataBind();

    // Restore the UI settings
    QSettings qtSettings("CS123", "Lab05");
    restoreGeometry(qtSettings.value("geometry").toByteArray());
    restoreState(qtSettings.value("windowState").toByteArray());
}

MainWindow::~MainWindow()
{
    foreach (DataBinding *b, m_bindings) {
        delete b;
    }
    delete m_ui;
    delete m_triangleCanvas;
    delete m_fragCanvas;
}

void MainWindow::dataBind() {
#define BIND(b) { DataBinding *_b = (b); m_bindings.push_back(_b); assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); }

    BIND(BoolBinding::bindCheckbox(m_ui->linesEnabled, settings.linesEnabled));

    BIND(ChoiceBinding::bindRadioButtons(NUM_SHADER_PROGRAMS, settings.shaderProgram,
                                    m_ui->solidShader,
                                    m_ui->gradientShader,
                                    m_ui->textureShader));


#undef BIND
}

void MainWindow::settingsChanged() {
    m_triangleCanvas->update();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Save the settings before we quit
    settings.saveSettings();
    QSettings qtSettings("CS123", "Lab05");
    qtSettings.setValue("geometry", saveGeometry());
    qtSettings.setValue("windowState", saveState());

    QMainWindow::closeEvent(event);
}
