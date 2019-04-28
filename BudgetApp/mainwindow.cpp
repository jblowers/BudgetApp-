#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pController = new BudgetController();
    m_pCal = ui->calendarWidget;
    m_pList = ui->TransactionListWidget;
    connect(m_pCal,SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));
    connect(m_pList,SIGNAL(currentRowChanged(int)),this,SLOT(onSelectedTransIndexChanged(int)));
    m_pCal->setMinimumDate(m_pController->getStartDate());
    m_pCal->setMaximumDate(m_pController->getEndDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSelectedTransIndexChanged(int index)
{
    qDebug("onSelectedTransIndexChanged( %d )",index);
    if ( m_CurrentSelectionTransactions.length() == 0 ) {
        fillInTransactionData(nullptr);
    }
    if ( index <= m_CurrentSelectionTransactions.length() && index >= 0) {
        fillInTransactionData(m_CurrentSelectionTransactions[index]);
    }
}


void MainWindow::onSelectionChanged()
{
    qDebug("onSelectionChanged()");
    QDate curDate = m_pCal->selectedDate();
    ui->DateEdit->setDate(curDate);

    m_CurrentSelectionTransactions = m_pController->getTransaction(curDate);
    populateComboBox(m_CurrentSelectionTransactions);

}

void MainWindow::populateComboBox(QVector<Transaction*> trans)
{
    int count = trans.length();
    qDebug("populateComboBox(count: [%d])",count);

    // list view update
    m_pList->clear();
    QListWidgetItem *item;
    for(int i = 0; i< count; i++) {
        item = new QListWidgetItem(trans[i]->title(),m_pList);
        m_pList->insertItem(i,item);
    }
    if (count == 0) {
        item = new QListWidgetItem("[empty]",m_pList);
        m_pList->insertItem(0,item);

    }
    m_pList->setCurrentRow(0);


}

void MainWindow::fillInTransactionData(Transaction* trans)
{
    qDebug("fillInTransactionData %d",trans!=nullptr);
    if(trans) {
        ui->TitleLineEdit->setText(trans->title());
        ui->DescriptionsTextEdit->setText(trans->description());
        ui->ValueSpinBox->setValue(trans->value());
    } else {
        ui->TitleLineEdit->setText("N/A");
        ui->DescriptionsTextEdit->setText("N/A");
        ui->ValueSpinBox->setValue(0);
    }
}
