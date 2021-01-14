#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "radar_indicator.h"

#include <QGraphicsScene>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->view_radar->setBackgroundBrush(Qt::black);
    QGraphicsScene* scene = new QGraphicsScene;
    RadarIndicator* radar = new RadarIndicator(ui->view_radar);
    scene->addItem(radar);
    ui->view_radar->setScene(scene);

}

MainWindow::~MainWindow() {
    delete ui;
}

