#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_nCurrentTransactionIndex(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<Transaction*>();
    qRegisterMetaType<QVector<Transaction*>>();

    m_pController = new BudgetController();
    m_pController->moveToThread(&m_ControllerThread);

    m_pCal = ui->calendarWidget;
    m_pList = ui->TransactionListWidget;
    ui->BudgetSaveLineEdit->setText(DEFAULT_BUDGET_FILE);
    // gui to mainwindow connects
    connect(m_pCal,SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));
    connect(m_pList,SIGNAL(currentRowChanged(int)),this,SLOT(onSelectedTransIndexChanged(int)));
    connect(ui->SaveBudgetBrowseButton,SIGNAL(clicked()), this, SLOT(onSaveBrowsePressed()));
    connect(ui->SaveBudgetPushButton,SIGNAL(clicked()),this,SLOT(onSaveButtonPressed()));
    connect(ui->SaveTransactionButton,SIGNAL(clicked()),this,SLOT(onSaveTransactionPressed()));
    connect(ui->RemoveSelectedButton,SIGNAL(clicked()),this,SLOT(onRemoveSelectedPressed()));
    connect(ui->LoadBudgetPushButton,SIGNAL(clicked()),this,SLOT(onLoadButtonPressed()));
    connect(ui->LoadBudgetBrowseButton,SIGNAL(clicked()),this,SLOT(onLoadBrowsePressed()));

    // mainwindow -> controller connects
    connect(this,SIGNAL(RemoveSelected()),m_pController,SLOT(onRemoveSelected()));
    connect(this,SIGNAL(SelectedTransactionChanged(int)),m_pController,SLOT(onSelectedTransactionChanged(int)));
    connect(this,SIGNAL(RequestSelectedDate(QDate)),m_pController,SLOT(onRequestTransactionsAt(QDate)));
    connect(this,SIGNAL(SaveBudgetToJsonFile(QString)),m_pController,SLOT(onSaveBudgetToJsonFileRequested(QString)));
    connect(this,SIGNAL(RequestSaveTransaction(Transaction)),m_pController,SLOT(onSaveSelected(Transaction)));
    connect(this,SIGNAL(LoadBudgetFromJsonFile(QString)),m_pController,SLOT(onLoadBudgetFromJsonFileRequested(QString)));

    // controller -> mainwindow connects
    connect(m_pController,SIGNAL(LogToGui(QString)),this,SLOT(LogToGuiWindow(QString)));
    connect(m_pController,SIGNAL(updateGuiTransactions(QVector<Transaction*>)),this,SLOT(onUpdateGuiTransactions(QVector<Transaction*>)));
    connect(m_pController,SIGNAL(RequestUpdateGui()),this,SLOT(onControllerRequestUpdateGui()));


    // starts controller thread to get backend working
    m_ControllerThread.start();
}

MainWindow::~MainWindow()
{
    if(m_ControllerThread.isRunning()) {
        m_ControllerThread.terminate();
    }
    m_ControllerThread.deleteLater();
    delete ui;
}

void MainWindow::LogToGuiWindow(QString logMessage)
{
    QString mes = QDateTime::currentDateTime().time().toString("hh:mm:ss.zzz");
    mes += ":  " + logMessage;
    qDebug(mes.toStdString().c_str());

    ui->LogWindowTextEdit->append(mes);
}

void MainWindow::onSaveTransactionPressed()
{
    LogToGuiWindow("MainWindow::onSaveTransactionPressed()");
    Transaction t;
    t.setDate(m_pCal->selectedDate());
    t.setTitle(ui->TitleLineEdit->text());
    t.setDescription(ui->DescriptionsTextEdit->toPlainText());
    t.setValue(ui->ValueSpinBox->value());
//    t.setTransactionType()
    emit RequestSaveTransaction(t);
//    LogToGuiWindow("\tonSaveTransactionPressed()-> FAILED.");
}

void MainWindow::onRemoveSelectedPressed()
{
    LogToGuiWindow("onRemoveSelectedPressed()");
    emit RemoveSelected();
}


void MainWindow::onSelectedTransIndexChanged(int index)
{
    qDebug("MainWindow::onSelectedTransIndexChanged( %d )",index);
    LogToGuiWindow("MainWindow::onSelectedTransIndexChanged( " + QString::number(index) + " )");
    if(index > -1) {
        updateSelectedTransaction(index);
    }
    m_nCurrentTransactionIndex = index;
    emit SelectedTransactionChanged(m_nCurrentTransactionIndex);
}


void MainWindow::onSelectionChanged()
{
    QDate curDate = m_pCal->selectedDate();
    qDebug("onSelectionChanged() %s",curDate.toString().toStdString().c_str());
    LogToGuiWindow("onSelectionChanged() \tDate: " + curDate.toString());
    ui->DateEdit->setDate(curDate);
    fillInTransactionData(nullptr);
    emit RequestSelectedDate(curDate);

}

void MainWindow::onUpdateGuiTransactions(QVector<Transaction*> transactions)
{
    if(m_CurrentSelectionTransactions != transactions) {
        m_CurrentSelectionTransactions = transactions;
        LogToGuiWindow("New set of Transactions recieved.");
    }
    m_CurrentSelectionTransactions = transactions;
    // fills in items in list view on gui
    populateTransactionList(m_CurrentSelectionTransactions);
    // prev line should reset the current selected transaction to index zero...?
    if(m_CurrentSelectionTransactions.length()>0) {
        m_pList->setCurrentRow(0);
        fillInTransactionData(m_CurrentSelectionTransactions[0]);
    }
}

void MainWindow::updateSelectedTransaction(int nIndex)
{
    fillInTransactionData(m_CurrentSelectionTransactions[nIndex]);
}

void MainWindow::populateTransactionList(QVector<Transaction*> trans)
{
    QListWidgetItem *item;
    int count = trans.length();
    qDebug("populateTransactionList(count: [%d])",count);
    m_pList->blockSignals(true);
    // clear the list to repopulate
    m_pList->clear();
    // populate with all transactions in vector
    for(int i = 0; i< count; i++) {
        item = new QListWidgetItem(trans[i]->title(),m_pList);
        m_pList->insertItem(i,item);
    }
    m_pList->blockSignals(false);
    if(m_pList->count()>0) {
        m_nCurrentTransactionIndex = 0;
    } else {
        m_nCurrentTransactionIndex = -1;
    }
    m_pList->setCurrentRow(m_nCurrentTransactionIndex);
}

void MainWindow::onSaveBrowsePressed()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Budget to File"), "../", tr("Json Files (*.json)"));
    ui->BudgetSaveLineEdit->setText(fileName);
}

void MainWindow::onLoadBrowsePressed()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Budget from File"), "../", tr("Json Files (*.json)"));
    ui->BudgetLoadLineEdit->setText(fileName);
}

void MainWindow::onSaveButtonPressed()
{
    QString fileName = ui->BudgetSaveLineEdit->text();
    if(QFile::exists(fileName)){

    } else {
        QFile f(fileName);
        // to create? could probably be done deeper in the budget or controller class
        f.open(QFile::OpenModeFlag::WriteOnly);
        f.close();
    }
    emit SaveBudgetToJsonFile(fileName);
}

void MainWindow::onLoadButtonPressed()
{
    QString fileName = ui->BudgetLoadLineEdit->text();
    if(QFile::exists(fileName)) {
        emit LoadBudgetFromJsonFile(fileName);
    } else {
        LogToGuiWindow("Budget file doesn't exist.");
    }
}

void MainWindow::fillInTransactionData(Transaction* trans)
{
    qDebug("fillInTransactionData %d",trans!=nullptr);
    if(trans) {
        if ( !ui->TitleLineEdit->isEnabled()) { ui->TitleLineEdit->setEnabled(true); }
        if ( !ui->DescriptionsTextEdit->isEnabled()) { ui->DescriptionsTextEdit->setEnabled(true); }
        if ( !ui->ValueSpinBox->isEnabled()) { ui->ValueSpinBox->setEnabled(true); }
        ui->TitleLineEdit->setText(trans->title());
        ui->DescriptionsTextEdit->setText(trans->description());
        ui->ValueSpinBox->setValue(trans->value());
    } else {
        if ( ui->TitleLineEdit->isEnabled()) { ui->TitleLineEdit->setEnabled(false); }
        if ( ui->DescriptionsTextEdit->isEnabled()) { ui->DescriptionsTextEdit->setEnabled(false); }
        if ( ui->ValueSpinBox->isEnabled()) { ui->ValueSpinBox->setEnabled(false); }
        ui->TitleLineEdit->setText("N/A");
        ui->DescriptionsTextEdit->setText("N/A");
        ui->ValueSpinBox->setValue(0);
    }
}
