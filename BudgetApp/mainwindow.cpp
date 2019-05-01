#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_nCurrentTransactionIndex(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pController = new BudgetController();
    m_pCal = ui->calendarWidget;
    m_pList = ui->TransactionListWidget;
    connect(m_pCal,SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));
    connect(m_pList,SIGNAL(currentRowChanged(int)),this,SLOT(onSelectedTransIndexChanged(int)));
    connect(ui->SaveTransactionButton,SIGNAL(clicked()),this,SLOT(onSaveTransactionPressed()));
    connect(ui->RemoveSelectedButton,SIGNAL(clicked()),this,SLOT(onRemoveSelectedPressed()));

    connect(ui->SaveBudgetBrowseButton,SIGNAL(clicked()), this, SLOT(onSaveBrowsePressed()));
    connect(ui->SaveBudgetPushButton,SIGNAL(clicked()),this,SLOT(onSaveButtonPressed()));

    connect(m_pController,SIGNAL(LogToGui(QString)),this,SLOT(LogToGuiWindow(QString)));

    connect(this,SIGNAL(SaveBudgetToJsonFile(QString)),m_pController,SLOT(onSaveBudgetToJsonFileRequested(QString)));

    connect(m_pController,SIGNAL(updateTransactionGui()),this,SLOT(onSelectionChanged()));
//    m_pCal->setMinimumDate(m_pController->getStartDate());
//    m_pCal->setMaximumDate(m_pController->getEndDate());


//    fillInTransactionData(nullptr);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LogToGuiWindow(QString logMessage)
{
    qDebug(logMessage.toStdString().c_str());
    ui->LogWindowTextEdit->setText(ui->LogWindowTextEdit->toPlainText() + "\n" + logMessage);
}

void MainWindow::onSaveTransactionPressed()
{
    int nIndex = m_nCurrentTransactionIndex;
    QString title = ui->TitleLineEdit->text();
    QString desc = ui->DescriptionsTextEdit->toPlainText();
    double dVal = ui->ValueSpinBox->value();
    QDate date = ui->DateEdit->date();
    m_pController->removeTransaction(date,m_nCurrentTransactionIndex);

    Transaction trans;
    trans.setTitle(title);
    trans.setDescription(desc);
    trans.setValue(dVal);
    trans.setDate(date);
    m_pController->addTransaction(trans);
    updateGuiData();
    m_pList->setCurrentRow(nIndex);

}

void MainWindow::onRemoveSelectedPressed()
{
    int currRow = m_nCurrentTransactionIndex;//m_pList->currentRow();
    if( currRow > -1 ) {
        QDate date = m_pCal->selectedDate();
        m_pController->removeTransaction(date,currRow);
        updateGuiData();
    }

//    if ( currRow <= m_CurrentSelectionTransactions.length() && m_CurrentSelectionTransactions.length() > 0) {
//        Transaction* pT = m_CurrentSelectionTransactions[m_pList->currentRow()];
//        m_pController->removeTransaction(pT);
//        updateGuiData();
//    } else {
//        qDebug("Did not remove selected.");
//    }
}


void MainWindow::onSelectedTransIndexChanged(int index)
{
    qDebug("onSelectedTransIndexChanged( %d )",index);
    if(index > -1) {
        updateGuiData();
    }
    m_nCurrentTransactionIndex = index;
}


void MainWindow::onSelectionChanged()
{
    QDate curDate = m_pCal->selectedDate();
    qDebug("onSelectionChanged() %s",curDate.toString().toStdString().c_str());

    updateGuiData();

//    ui->DateEdit->setDate(curDate);
//    m_CurrentSelectionTransactions = m_pController->getTransactionVector(curDate);
//    int nPrevRow = m_pList->currentRow();
//    populateTransactionList(m_CurrentSelectionTransactions);
//    while(nPrevRow > m_CurrentSelectionTransactions.length()) {
//        nPrevRow--;
//        qDebug("Decrementing prev row. Now: %d",nPrevRow);
//    }
//    m_pList->setCurrentRow(nPrevRow);

}

void MainWindow::updateGuiData()
{
    qDebug("updateGuiData()");
    // list of transactions, current row
    // -> date edit, title, desc, amount
    QDate selectedDate = m_pCal->selectedDate();
    QVector<Transaction*> transactions = m_pController->getTransactionVector(selectedDate);
    int nSelectedRow = m_pList->currentRow();

    ui->DateEdit->setDate(selectedDate);
    populateTransactionList(transactions);
    int newRow = 0;
    if (nSelectedRow == -1) {
        newRow = nSelectedRow;
    } else if(nSelectedRow < transactions.length()) {
        newRow = 0;
    } else {
        newRow = transactions.length()-1;
    }
//    m_pList->setCurrentRow(newRow);
    if (nSelectedRow > -1 && nSelectedRow < transactions.length()) {
        fillInTransactionData(transactions[nSelectedRow]);
    } else
        fillInTransactionData(nullptr);

}

void MainWindow::populateTransactionList(QVector<Transaction*> trans)
{
    QListWidgetItem *item;
    int count = trans.length();
    qDebug("populateTransactionList(count: [%d])",count);
    // clear the list to repopulate
    m_pList->clear();
    // if count is zero, add in filler item (set flag for empty transactions?)
    if (count == 0) {
//        item = new QListWidgetItem("[empty]",m_pList);
//        m_pList->insertItem(0,item);
    } else {
        // populate with all transactions in vector
        for(int i = 0; i< count; i++) {
            item = new QListWidgetItem(trans[i]->title(),m_pList);
            m_pList->insertItem(i,item);
        }
    }
}

void MainWindow::onSaveBrowsePressed()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Budget to File"), "../", tr("Json Files (*.json)"));
    ui->BudgetSaveLineEdit->setText(fileName);
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
