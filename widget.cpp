#include <QByteArray>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QDebug>

#include "widget.h"
#include "pindialog.h"

const QByteArray Widget::consumer_key = "consumer_key";
const QByteArray Widget::consumer_secret = "consumer_secret";
const QByteArray Widget::access_token = "access_token";
const QByteArray Widget::access_tokenSecret = "access_tokenSecret";

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    twitter = new Twitter;
    twitter->setConsumerKey(Widget::consumer_key);
    twitter->setConsumerKeySecret(Widget::consumer_secret);
    twitter->setAccessToken(access_token, access_tokenSecret);
//    connect(twitter, SIGNAL(replyReceived(QByteArray&)),
//            this, SLOT(appendText(QByteArray&)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    excuteOAuthButton = new QPushButton(QString::fromLocal8Bit("認証"));
    connect(excuteOAuthButton, SIGNAL(clicked()),
            this, SLOT(excuteOAuth()));

    showHomeTLButton = new QPushButton(QString::fromLocal8Bit("ホームTL"));
    connect(showHomeTLButton, SIGNAL(clicked()),
            this, SLOT(showHomeTL()));

    sendTweetButton = new QPushButton(QString::fromLocal8Bit("投稿"));
    connect(sendTweetButton, SIGNAL(clicked()),
            this, SLOT(sendTweet()));

    tweetLine = new QLineEdit;

    timeLineArea = new QTextEdit;
    timeLineArea->setReadOnly(true);

    buttonLayout->addWidget(excuteOAuthButton);
    buttonLayout->addWidget(showHomeTLButton);
    buttonLayout->addWidget(sendTweetButton);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(tweetLine);
    mainLayout->addWidget(timeLineArea);
    setLayout(mainLayout);
}

Widget::~Widget()
{
}

void Widget::appendText(QByteArray &reply) //SLOT
{
    timeLineArea->append(QString(reply));
//    timeLineArea->setHtml(QString(reply));
}

void Widget::excuteOAuth() //Button SLOT
{
    twitter->getRequestToken();
    d = new PINDialog;
    connect(d, SIGNAL(receivedndPINcode(QByteArray)),
            this, SLOT(onReceivedPINcode(QByteArray)));
    d->show();
}

void Widget::showHomeTL() //Button SLOT
{
    appendText(twitter->getHomeTimeLine());
}

void Widget::sendTweet()
{
    QByteArray tweet = tweetLine->text().toUtf8();
    qDebug() << tweet;
    twitter->updateStatus(tweet);
    tweetLine->clear();
}

void Widget::onReceivedPINcode(const QByteArray &pinCode) // PINコードを受け取った時のSLOT
{
    qDebug() << QString::fromLocal8Bit("PIN コード：") << pinCode;
    twitter->getAccessToken(pinCode); // PINコードを渡してアクセストークン取得
}
