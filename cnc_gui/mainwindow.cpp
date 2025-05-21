#include "mainwindow.h"
#include "colorcoder.h"
#include "./ui_mainwindow.h"
#include "../parser/parser.h"
#include "../generator/generator.h"
#include "../brands/machine_definitions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto machines = NCParser::MachineDefinitions::getMachines();
    QStringList qmachines;
    for (const auto &m : machines){
        qmachines.append(QString::fromUtf8(m.data(), m.size()));
    }
    colorcoder_l = new ISOHighlighter(ui->left->document());
    colorcoder_r = new ISOHighlighter(ui->right->document());
    ui->leftCombo->insertItems(0, qmachines);
    ui->rightCombo->insertItems(0, qmachines);
    connect(ui->left, &QPlainTextEdit::textChanged, this, &MainWindow::textEntered);
    connect(ui->leftCombo, &QComboBox::currentIndexChanged, this, &MainWindow::leftComboChanged);
    connect(ui->rightCombo, &QComboBox::currentIndexChanged, this, &MainWindow::rightComboChanged);
    leftComboChanged(0);
    ui->rightCombo->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::textEntered()
{
    if (!p || !g)
        return;
    p->parse(ui->left->toPlainText().toStdString());
    auto parse_tree = p->result();
    std::string result = g->generate(parse_tree);
    ui->right->setPlainText(QString::fromStdString(result));
}

void MainWindow::leftComboChanged(int index)
{
    p = std::make_unique<NCParser::parser>(NCParser::MachineDefinitions::getManufacturer(index),
                             NCParser::MachineDefinitions::getMachine(index));
    ui->left->setParameters(p->getParam());
    colorcoder_l->setMultiletter(p->getMultiletter());
}

void MainWindow::rightComboChanged(int index)
{
    g = std::make_unique<NCParser::generator>(NCParser::MachineDefinitions::getManufacturer(index),
                                                NCParser::MachineDefinitions::getMachine(index));
    ui->right->setParameters(g->getParam());
    colorcoder_r->setMultiletter(g->getMultiletter());
}
