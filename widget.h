#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QSettings>

#include "twitter.h"

class QByteArray;
class QPushButton;
class QTextEdit;
class QLineEdit;
class QString;
class PINDialog;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    static const QByteArray consumer_key;
    static const QByteArray consumer_secret;
    static const QByteArray access_token;
    static const QByteArray access_tokenSecret;

private slots:
    void appendText(QByteArray &reply);
    void excuteOAuth();
    void showHomeTL();
    void sendTweet();
    void onReceivedPINcode(const QByteArray &pinCode);

private:
    Twitter *twitter;
    QSettings oauthSettings;
    QByteArray token;
    QByteArray secret;

    QPushButton *excuteOAuthButton;
    QPushButton *showHomeTLButton;
    QPushButton *sendTweetButton;
    QTextEdit *timeLineArea;
    QLineEdit *tweetLine;

    PINDialog *d;
};

#endif // WIDGET_H
