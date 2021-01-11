#ifndef TWITTEROBJECTS_H
#define TWITTEROBJECTS_H
#include <QObject>
#include <QDateTime>

class QByteArray;
class Entities;

class Status
{
public:
    Status();

private:
    QByteArray contributors; // 広告アカウント
    QByteArray coordinates; // ツイートが行われた場所（緯度経度）
    QDateTime created_at; // メッセージを送信した日時
//    Entities entities;
    bool favorited; // 自分がお気に入りしたかどうか


};

class DirectMessage
{
public:
    DirectMessage();

private:
    QDateTime created_at; // メッセージを送信した日時
    QByteArray id; // メッセージのID(文字列型)


};

#endif // TWITTEROBJECTS_H
