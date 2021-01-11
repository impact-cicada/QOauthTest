#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "pindialog.h"

PINDialog::PINDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    QFont font = QFont("MS P Gothic", 16);
    font.setBold(true);
    line = new QLineEdit;
    line->setFont(font);
    line->setMaxLength(15);
    button = new QPushButton(QString::fromLocal8Bit("認証"));
    button->setMinimumHeight(30);
    button->setDefault(true);
    connect(button, SIGNAL(clicked()),
            this, SLOT(buttonClicked()));

    layout->addWidget(line);
    layout->addWidget(button);

    setLayout(layout);
    setModal(true);
    setWindowTitle(QString::fromLocal8Bit("PINコードを入力"));
}

void PINDialog::buttonClicked()
{
    emit receivedndPINcode(line->text().toUtf8());
    close();
}
