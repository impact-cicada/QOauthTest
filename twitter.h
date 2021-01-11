#ifndef OAUTH_H
#define OAUTH_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QTimer>
#include <QEventLoop>
#include <QDateTime>
#include <QColor>

class QByteArray;
class QUrl;
class QNetworkAccessManager;
class QNetworkReply;

class Status;
class Entities;
class User;
class DirectMessage;
class UserList;
class SavedSearch;
class Trend;
class Location;
class Place;
class Twitter;
class Paging;

class Paging {
    friend class Twitter;
public:
    Paging() {
        since_id = 0;
        max_id = 0;
        count = 0;
        page = 1;
        cursor = -1;
    }

    int since_id;
    int max_id;
    int count;
    int page;
    int cursor;

private:
    typedef QPair<QByteArray, QByteArray> Parameter;
    typedef QList< Parameter > ParameterList;

    ParameterList toParameterList(){
        ParameterList pl;
        if(since_id > 0) {
            Parameter p("since_id", QByteArray::number(since_id));
            pl.append(p);
        }
        if(max_id > 0) {
            Parameter p("max_id", QByteArray::number(max_id));
            pl.append(p);
        }
        if(count > 0) {
            Parameter p("count", QByteArray::number(count));
            pl.append(p);
        }
        if(page > 1) {
            Parameter p("page", QByteArray::number(page));
            pl.append(p);
        }
        if(cursor > -1) {
            Parameter p("cursor", QByteArray::number(cursor));
            pl.append(p);
        }
        return pl;
    }
};

class Twitter : public QObject
{
    Q_OBJECT

public:
    Twitter(QObject *parent = 0);
    void setConsumerKey(const QByteArray &key){ consumer_key = key; }
    void setConsumerKeySecret(const QByteArray &keySecret) { consumer_keySecret = keySecret; }
    void setAccessToken(const QByteArray access_token, const QByteArray access_tokenSecret);
    QPair<QByteArray, QByteArray> accessToken();
    bool isAuthorized(){ return authorized; }
    bool isVerified(){ return verified; }

    // 認証
    void getRequestToken();
    void getAccessToken(const QByteArray &pinCode);

    // REST API
    // Timeline リソース
    QByteArray getPublicTimeline(bool trim_user = false, bool include_entities = false);
    QByteArray getHomeTimeLine(bool trim_user = false, bool include_entities = false, Paging paging = Paging());
    bool updateStatus(const QByteArray &status);

signals:
    void replyReceived(QByteArray &reply);

private slots:
    void replyFinished(QNetworkReply* reply);

private:
    typedef QPair<QByteArray, QByteArray> Parameter;
    typedef QList< Parameter > ParameterList;

    enum HttpMethod {
        GET,
        POST,
        DELETE
    };

    QByteArray generateNonce();
    QByteArray generateTimestamp();
    QByteArray generateSignature(HttpMethod method, QUrl &url, ParameterList &addParams = ParameterList());
    void parseTokens(const QByteArray &tokens);
    QByteArray sendRequest(HttpMethod method, QUrl &url, ParameterList &addParams = ParameterList(), ParameterList &bodyParams = ParameterList());
    void openAuthorizePage();
    void sortParams();
    void setAuthorized(bool isAuthorized){ authorized = isAuthorized; }
    void setVerified(bool isVerified){ verified = isVerified; }

    QNetworkAccessManager *manager; // ネットワークコネクション
    int timeout; // タイムアウト時間

    // Params Names
    QByteArray consumer_keyName;
    QByteArray nonceName;
    QByteArray signatureMethodName;
    QByteArray timestampName;
    QByteArray tokenName;
    QByteArray versionName;
    QByteArray signatureName;

    // Params for request and signature
    QByteArray consumer_key;
    QByteArray consumer_keySecret;
    QByteArray token;
    QByteArray tokenSecret;
    QByteArray signature;
    QByteArray signatureMethod;
    QByteArray timestamp;
    QByteArray nonce;
    QByteArray version;

    ParameterList params;

    // Verified Data
    QByteArray user_id;
    QByteArray screen_name;

    bool authorized; // トークン取得フラグ(アクセストークンでもリクエストトークンでも良い)
    bool verified; // PINコード送信フラグ(アクセストークン取得フラグ)

    // 認証用 URL
    static const QByteArray URL_REQUESTTOKEN;
    static const QByteArray URL_AUTHORIZE;
    static const QByteArray URL_ACCESSTOKEN;

    // Twitter URL
    static const QByteArray URL_TWITTER_1;

    // REST API
    // Timeline リソース
    static const QByteArray URL_STATUSES_PUBLIC_TIMELINE;
    static const QByteArray URL_STATUSES_HOME_TIMELINE;
    static const QByteArray URL_STATUSES_USER_TIMELINE;
    static const QByteArray URL_STATUSES_MENTIONS;
    static const QByteArray URL_STATUSES_RETWEETED_BY_ME;
    static const QByteArray URL_STATUSES_RETWEETED_TO_ME;
    static const QByteArray URL_STATUSES_RETWEETS_OF_ME;
    static const QByteArray URL_STATUSES_RETWEETED_TO_USER;
    static const QByteArray URL_STATUSES_RETWEETED_BY_USER;

    // Tweet リソース
    static const QByteArray URL_STATUSES_SHOW_ID;
    static const QByteArray URL_STATUSES_UPDATE;
    static const QByteArray URL_STATUSES_DESTROY_ID;
    static const QByteArray URL_STATUSES_RETWEET_ID;
    static const QByteArray URL_STATUSES_RETWEETS_ID;
    static const QByteArray URL_STATUSES_ID_RETWEETED_BY;
    static const QByteArray URL_STATUSES_ID_RETWEETED_BY_IDS;

    // Favorite リソース
    static const QByteArray URL_FAVORITES;
    static const QByteArray URL_FAVORITES_CREATE_ID;
    static const QByteArray URL_FAVORITES_DESTROY_ID;

    // Friendships リソース
    static const QByteArray URL_FRIENDSHIIPS_CREATE;
    static const QByteArray URL_FRIENDSHIIPS_DESTROY;
    static const QByteArray URL_FRIENDSHIIPS_EXISTS;
    static const QByteArray URL_FRIENDSHIIPS_SHOW;
    static const QByteArray URL_FRIENDSHIIPS_INCOMING;
    static const QByteArray URL_FRIENDSHIIPS_OUTGOING;
    static const QByteArray URL_FRIENDSHIIPS_LOOKUP;

    // Friends and Followers リソース
    static const QByteArray URL_FRIENDS_IDS;
    static const QByteArray URL_FOLLOWERS_IDS;

    // Notifications リソース
    static const QByteArray URL_NOTIFICATIONS_FOLLOW;
    static const QByteArray URL_NOTIFICATIONS_LEAVE;

    // Block リソース
    static const QByteArray URL_BLOCKS_CREATE;
    static const QByteArray URL_BLOCKS_DESTROY;
    static const QByteArray URL_BLOCKS_EXISTS;
    static const QByteArray URL_BLOCKS_BLOCKING;
    static const QByteArray URL_BLOCKS_BLOCKING_IDS;

    // Spam Reporting リソース
    static const QByteArray URL_REPORT_SPAM;

    // User リソース
    static const QByteArray URL_USERS_SHOW;
    static const QByteArray URL_USERS_LOOKUP;
    static const QByteArray URL_USERS_SEARCH;
    static const QByteArray URL_USERS_SUGGESTIONS;
    static const QByteArray URL_USERS_SUGGESTIONS_SLUG;
    static const QByteArray URL_USERS_SUGGESTIONS_SLUG_MEMBERS;
    static const QByteArray URL_USERS_PROFILE_IMAGE_SCREEN_NAME;

    // Account リソース
    static const QByteArray URL_ACCOUNT_VERIFY_CREDENTIALS;
    static const QByteArray URL_ACCOUNT_RATE_LIMIT_STATUS;
    static const QByteArray URL_ACCOUNT_UPDATE_PROFILE_COLORS;
    static const QByteArray URL_ACCOUNT_UPDATE_PROFILE_IMAGE;
    static const QByteArray URL_ACCOUNT_UPDATE_PROFILE_BACKGROUND_IMAGE;
    static const QByteArray URL_ACCOUNT_UPDATE_PROFILE;
    static const QByteArray URL_ACCOUNT_TOTALS;
    static const QByteArray URL_ACCOUNT_SETTINGS;

    // Direct Message リソース
    static const QByteArray URL_DIRECT_MESSAGES;
    static const QByteArray URL_DIRECT_MESSAGES_SENT;
    static const QByteArray URL_DIRECT_MESSAGES_NEW;
    static const QByteArray URL_DIRECT_MESSAGES_DESTROY_ID;
    static const QByteArray URL_DIRECT_MESSAGES_SHOW_ID;

    // List リソース
    static const QByteArray URL_LISTS_CREATE;
    static const QByteArray URL_LISTS_UPDATE;
    static const QByteArray URL_LISTS;
    static const QByteArray URL_LISTS_SHOW;
    static const QByteArray URL_LISTS_DESTROY;
    static const QByteArray URL_LISTS_STATUSES;
    static const QByteArray URL_LISTS_MEMBERSHIPS;
    static const QByteArray URL_LISTS_SUBSCRIPTIONS;
    static const QByteArray URL_LISTS_ALL;

    // List Members リソース
    static const QByteArray URL_LISTS_MEMBERS;
    static const QByteArray URL_LISTS_MEMBERS_CREATE;
    static const QByteArray URL_LISTS_MEMBERS_CREATE_ALL;
    static const QByteArray URL_LISTS_MEMBERS_DESTROY;
    static const QByteArray URL_LISTS_MEMBERS_SHOW;

    // List Subscriber リソース
    static const QByteArray URL_LISTS_SUBSCRIBERS;
    static const QByteArray URL_LISTS_SUBSCRIBERS_CREATE;
    static const QByteArray URL_LISTS_SUBSCRIBERS_DESTROY;
    static const QByteArray URL_LISTS_SUBSCRIBERS_SHOW;

    // Saved Dearches リソース
    static const QByteArray URL_SAVED_SEARCHES;
    static const QByteArray URL_SAVED_SEARCHES_SHOW_ID;
    static const QByteArray URL_SAVED_SEARCHES_CREATE;
    static const QByteArray URL_SAVED_SEARCHES_DESDTROY_ID;

    // Trends リソース
    static const QByteArray URL_TRENDS;
    static const QByteArray URL_TRENDS_CURRENT;
    static const QByteArray URL_TRENDS_DAILY;
    static const QByteArray URL_TRENDS_WEEKLY;

    // Local Trends リソース
    static const QByteArray URL_TRENDS_AVAILABLE;
    static const QByteArray URL_TRENDS_WOEID;

    // Geo リソース
    static const QByteArray URL_GEO_SEARCH;
    static const QByteArray URL_GEO_SIMILAR_PLACES;
    static const QByteArray URL_GEO_REVERSE_GEOCODE;
    static const QByteArray URL_GEO_ID_PLACE_ID;
    static const QByteArray URL_GEO_PLACE;

    // Legal リソース
    static const QByteArray URL_LEGAL_TOS;
    static const QByteArray URL_LEGAL_PRIVACY;

    // Help リソース
    static const QByteArray URL_HELP_TEST;
    static const QByteArray URL_HELP_LANGUAGES;
    static const QByteArray URL_HELP_CONFIGURATION;

    // 検索API
    static const QByteArray URL_SEARCH_API;

    //Streaming API
    static const QByteArray URL_STREAM_SAMPLE;
    static const QByteArray URL_STREAM_FILTER;
    static const QByteArray URL_STREAM_FIREHOSE; // 要申請
    static const QByteArray URL_STREAM_LINKS; // 要申請
    static const QByteArray URL_STREAM_RETWEET; // 要申請
    static const QByteArray URL_USERSTREAM;
    static const QByteArray URL_SITESTREAM; // ベータサービス期間中(2011年11月22日現在)
};

// status オブジェクト
//class Status {
//public:
//    Status();

//private:
//    QList<QByteArray> contributors;
//    QPair<QByteArray, QByteArray> coordinates;
//    QDateTime created_at;
//    Entities *entities;
//    bool favorited;
//    QPair<QByteArray, QByteArray> geo;
//    QByteArray id;
//    QByteArray in_reply_to_screen_name;
//    QByteArray in_reply_to_status_id;
//    QByteArray in_reply_to_user_id;
//    QByteArray place;
//    QByteArray retweet_count;
//    bool retweeted;
//    QByteArray source;
//    Status *retweeted_status;
//    QByteArray text;
//    bool trnacated;
//    User *user;
//};

//// entities オブジェクト
//class Entities {
//public:
//    Entities();

//private:

//};

//// user オブジェクト
//class User {
//public:
//    User();

//private:
//    bool contributors_enabled;
//    QDateTime created_at;
//    QByteArray description;
//    int favorites_count;
//    bool follow_request_sent;
//    int followers_count;
//    int friends_count;
//    bool geo_enabled;
//    QByteArray id;
//    bool is_translator;
//    QByteArray lang;
//    int listed_count;
//    QByteArray location;
//    QByteArray name;
//    QColor profile_background_color;
//    QUrl profile_background_image_url_https;
//    bool profile_background_tile;
//    QUrl profile_image_url_https;
//    QColor profile_link_color;
//    QColor profile_sidebar_border_color;
//    QColor profile_sidebar_fill_color;
//    QColor profile_text_color;
//    bool profile_use_background_image;
//    bool _protected;
//    QByteArray screen_name;
//    bool show_all_inline_media;
//    int statuses_count;
//    QByteArray time_zone;
//    QUrl url;
//    int utc_offset;
//    bool verified;
//};

//// directmessage オブジェクト
//class DirectMessage {
//public:
//    DirectMessage();

//private:
//    QDateTime created_at;
//    QByteArray id;
//    User *recipient;
//    QByteArray recipient_id;
//    QByteArray recipient_screen_name;
//    User *sender;
//    QByteArray sender_id;
//    QByteArray sender_screen_name;
//    QByteArray text;
//};

//// list オブジェクト
//class UserList {
//public:
//    UserList();

//private:
//    QByteArray description;
//    bool following;
//    QByteArray full_name;
//    QByteArray id;
//    int member_count;
//    QByteArray mode;
//    QByteArray name;
//    QByteArray slug;
//    int subscriber_count;
//    QByteArray uri;
//    User *user;

//};

//// savedsearch オブジェクト
//class SavedSearch {

//};

//// trend オブジェクト
//class Trend {

//};

//// location オブジェクト
//class Location {

//};

//// place オブジェクト
//class Place {

//};

#endif // OAUTH_H
