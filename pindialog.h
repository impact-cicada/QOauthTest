#ifndef PINDIALOG_H
#define PINDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class PINDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PINDialog(QWidget *parent = 0);

signals:
    void receivedndPINcode(const QByteArray &pinCode);

private slots:
    void buttonClicked();

private:
    QLineEdit *line;
    QPushButton *button;
};

#endif // PINDIALOG_H
