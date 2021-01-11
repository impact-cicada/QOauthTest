#include "twitter.h"

#include <QString>
#include <QByteArray>
#include <QList>
#include <QPair>
#include <QDateTime>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QCryptographicHash>
#include <QDesktopServices>

#include <QtDebug>

// 認証用 URL
const QByteArray Twitter::URL_REQUESTTOKEN("https://api.twitter.com/oauth/request_token");
const QByteArray Twitter::URL_AUTHORIZE("https://api.twitter.com/oauth/authorize");
const QByteArray Twitter::URL_ACCESSTOKEN("https://api.twitter.com/oauth/access_token");

// REST API
// Timeline リソース
const QByteArray Twitter::URL_STATUSES_PUBLIC_TIMELINE("https://api.twitter.com/1/statuses/public_timeline.xml");
const QByteArray Twitter::URL_STATUSES_HOME_TIMELINE("https://api.twitter.com/1/statuses/home_timeline.xml");
const QByteArray Twitter::URL_STATUSES_USER_TIMELINE("https://api.twitter.com/1/statuses/user_timeline.xml");
const QByteArray Twitter::URL_STATUSES_MENTIONS("https://api.twitter.com/1/statuses/mentions.xml");
const QByteArray Twitter::URL_STATUSES_RETWEETED_BY_ME("https://api.twitter.com/1/statuses/retweeted_by_me.xml");
const QByteArray Twitter::URL_STATUSES_RETWEETED_TO_ME("https://api.twitter.com/1/statuses/retweeted_to_me.xml");
const QByteArray Twitter::URL_STATUSES_RETWEETS_OF_ME("https://api.twitter.com/1/statuses/retweets_of_me.xml");
const QByteArray Twitter::URL_STATUSES_RETWEETED_TO_USER("https://api.twitter.com/1/statuses/retweeted_to_user.xml");
const QByteArray Twitter::URL_STATUSES_RETWEETED_BY_USER("https://api.twitter.com/1/statuses/retweeted_by_user.xml");

// Tweet リソース
const QByteArray Twitter::URL_STATUSES_SHOW_ID("https://api.twitter.com/1/statuses/show/:id.xml");
const QByteArray Twitter::URL_STATUSES_UPDATE("https://api.twitter.com/1/statuses/update.xml");
const QByteArray Twitter::URL_STATUSES_DESTROY_ID("https://api.twitter.com/1/statuses/destroy/:id.xml");
const QByteArray Twitter::URL_STATUSES_RETWEET_ID("https://api.twitter.com/1/statuses/retweet/:id.xml");
const QByteArray Twitter::URL_STATUSES_RETWEETS_ID("https://api.twitter.com/1/statuses/retweets/:id.xml");
const QByteArray Twitter::URL_STATUSES_ID_RETWEETED_BY("https://api.twitter.com/1/statuses/:id/retweeted_by.xml");
const QByteArray Twitter::URL_STATUSES_ID_RETWEETED_BY_IDS("https://api.twitter.com/1/statuses/:id/retweeted_by/ids.xml");

// Favorite リソース
const QByteArray Twitter::URL_FAVORITES("https://api.twitter.com/1/favorites.xml");
const QByteArray Twitter::URL_FAVORITES_CREATE_ID("https://api.twitter.com/1/favorites/create/:id.xml");
const QByteArray Twitter::URL_FAVORITES_DESTROY_ID("https://api.twitter.com/1/favorites/destroy/:id.xml");

// Friendsships リソース
const QByteArray Twitter::URL_FRIENDSHIIPS_CREATE("https://api.twitter.com/1/friendships/create.xml");
const QByteArray Twitter::URL_FRIENDSHIIPS_DESTROY("https://api.twitter.com/1/friendships/destroy.xml");
const QByteArray Twitter::URL_FRIENDSHIIPS_EXISTS("https://api.twitter.com/1/friendships/exists.xml");
const QByteArray Twitter::URL_FRIENDSHIIPS_SHOW("https://api.twitter.com/1/friendships/show.xml");
const QByteArray Twitter::URL_FRIENDSHIIPS_INCOMING("https://api.twitter.com/1/friendships/incoming.xml");
const QByteArray Twitter::URL_FRIENDSHIIPS_OUTGOING("https://api.twitter.com/1/friendships/outgoing.xml");
const QByteArray Twitter::URL_FRIENDSHIIPS_LOOKUP("https://api.twitter.com/1/friendships/lookup.xml");

// Friends and Followers リソース
const QByteArray Twitter::URL_FRIENDS_IDS("https://api.twitter.com/1/friends/ids.xml");
const QByteArray Twitter::URL_FOLLOWERS_IDS("https://api.twitter.com/1/followers/ids.xml");

// Notofications リソース
const QByteArray Twitter::URL_NOTIFICATIONS_FOLLOW("https://api.twitter.com/1/notifications/follow.xml");
const QByteArray Twitter::URL_NOTIFICATIONS_LEAVE("https://api.twitter.com/1/notifications/leave.xml");

// Brock リソース
const QByteArray Twitter::URL_BLOCKS_CREATE("https://api.twitter.com/1/blokcs/create.xml");
const QByteArray Twitter::URL_BLOCKS_DESTROY("https://api.twitter.com/1/blokcs/destroy.xml");
const QByteArray Twitter::URL_BLOCKS_EXISTS("https://api.twitter.com/1/blokcs/exists.xml");
const QByteArray Twitter::URL_BLOCKS_BLOCKING("https://api.twitter.com/1/blokcs/blocking.xml");
const QByteArray Twitter::URL_BLOCKS_BLOCKING_IDS("https://api.twitter.com/1/blokcs/blocking/ids.xml");

// Spam Reporting リソース
const QByteArray Twitter::URL_REPORT_SPAM("https://api.twitter.com/1/report_spam.xml");

// User リソース
const QByteArray Twitter::URL_USERS_SHOW("https://api.twitter.com/1/users/show.xml");
const QByteArray Twitter::URL_USERS_LOOKUP("https://api.twitter.com/1/users/lookup.xml");
const QByteArray Twitter::URL_USERS_SEARCH("https://api.twitter.com/1/users/search.xml");
const QByteArray Twitter::URL_USERS_SUGGESTIONS("https://api.twitter.com/1/users/suggestions.xml");
const QByteArray Twitter::URL_USERS_SUGGESTIONS_SLUG("https://api.twitter.com/1/users/suggestions/:slug.xml");
const QByteArray Twitter::URL_USERS_SUGGESTIONS_SLUG_MEMBERS("https://api.twitter.com/1/users/suggestions/:slug/members.xml");
const QByteArray Twitter::URL_USERS_PROFILE_IMAGE_SCREEN_NAME("https://api.twitter.com/1/users/profile_image/:screen_name.xml");

// Account リソース
const QByteArray Twitter::URL_ACCOUNT_VERIFY_CREDENTIALS("https://api.twitter.com/1/account/verify_credentials.xml");
const QByteArray Twitter::URL_ACCOUNT_RATE_LIMIT_STATUS("https://api.twitter.com/1/account/rate_limit_status.xml");
const QByteArray Twitter::URL_ACCOUNT_UPDATE_PROFILE_COLORS("https://api.twitter.com/1/account/update_profile_colors.xml");
const QByteArray Twitter::URL_ACCOUNT_UPDATE_PROFILE_IMAGE("https://api.twitter.com/1/accounts/update_profile_image.xml");
const QByteArray Twitter::URL_ACCOUNT_UPDATE_PROFILE_BACKGROUND_IMAGE("https://api.twitter.com/1/accounts/update_profile_background_image.xml");
const QByteArray Twitter::URL_ACCOUNT_UPDATE_PROFILE("https://api.twitter.com/1/accounts/update_profile.xml");
const QByteArray Twitter::URL_ACCOUNT_TOTALS("https://api.twitter.com/1/accounts/totals.xml");
const QByteArray Twitter::URL_ACCOUNT_SETTINGS("https://api.twitter.com/1/accounts/settings.xml");

// Direct Message リソース
const QByteArray Twitter::URL_DIRECT_MESSAGES("https://api.twitter.com/1/direct_messages.xml");
const QByteArray Twitter::URL_DIRECT_MESSAGES_SENT("https://api.twitter.com/1/direct_messages/sent.xml");
const QByteArray Twitter::URL_DIRECT_MESSAGES_NEW("https://api.twitter.com/1/direct_messages/new.xml");
const QByteArray Twitter::URL_DIRECT_MESSAGES_DESTROY_ID("https://api.twitter.com/1/direct_messages/destroy/:id.xml");
const QByteArray Twitter::URL_DIRECT_MESSAGES_SHOW_ID("https://api.twitter.com/1/direct_messages/show/:id.xml");

// List リソース
const QByteArray Twitter::URL_LISTS_CREATE("https://api.twitter.com/1/lists/create.xml");
const QByteArray Twitter::URL_LISTS_UPDATE("https://api.twitter.com/1/lists/update.xml");
const QByteArray Twitter::URL_LISTS("https://api.twitter.com/1/lists.xml");
const QByteArray Twitter::URL_LISTS_SHOW("https://api.twitter.com/1/lists/show.xml");
const QByteArray Twitter::URL_LISTS_DESTROY("https://api.twitter.com/1/lists/destroy.xml");
const QByteArray Twitter::URL_LISTS_STATUSES("https://api.twitter.com/1/lists/statuses.xml");
const QByteArray Twitter::URL_LISTS_MEMBERSHIPS("https://api.twitter.com/1/lists/memberships.xml");
const QByteArray Twitter::URL_LISTS_SUBSCRIPTIONS("https://api.twitter.com/1/lists/subscriptions.xml");
const QByteArray Twitter::URL_LISTS_ALL("https://api.twitter.com/1/lists/all.xml");

// List Members リソース
const QByteArray Twitter::URL_LISTS_MEMBERS("https://api.twitter.com/1/lists/members.xml");
const QByteArray Twitter::URL_LISTS_MEMBERS_CREATE("https://api.twitter.com/1/lists/members/create.xml");
const QByteArray Twitter::URL_LISTS_MEMBERS_CREATE_ALL("https://api.twitter.com/1/lists/members/create_all.xml");
const QByteArray Twitter::URL_LISTS_MEMBERS_DESTROY("https://api.twitter.com/1/lists/members/destroy.xml");
const QByteArray Twitter::URL_LISTS_MEMBERS_SHOW("https://api.twitter.com/1/lists/members/show.xml");

// List Subscriber リソース
const QByteArray Twitter::URL_LISTS_SUBSCRIBERS("https://api.twitter.com/1/lists/subscribers.xml");
const QByteArray Twitter::URL_LISTS_SUBSCRIBERS_CREATE("https://api.twitter.com/1/lists/subscribers/create.xml");
const QByteArray Twitter::URL_LISTS_SUBSCRIBERS_DESTROY("https://api.twitter.com/1/lists/subscribers/destroy.xml");
const QByteArray Twitter::URL_LISTS_SUBSCRIBERS_SHOW("https://api.twitter.com/1/lists/subscribers/show.xml");

// Saved Search リソース
const QByteArray Twitter::URL_SAVED_SEARCHES("https://api.twitter.com/1/saved_searches.xml");
const QByteArray Twitter::URL_SAVED_SEARCHES_SHOW_ID("https://api.twitter.com/1/saved_searches/show/:id.xml");
const QByteArray Twitter::URL_SAVED_SEARCHES_CREATE("https://api.twitter.com/1/saved_searches/create.xml");
const QByteArray Twitter::URL_SAVED_SEARCHES_DESDTROY_ID("https://api.twitter.com/1/saved_searches/destroy/:id.xml");

// Trends リソース
const QByteArray Twitter::URL_TRENDS("https://api.twitter.com/1/trends.xml");
const QByteArray Twitter::URL_TRENDS_CURRENT("https://api.twitter.com/1/trends/current.xml");
const QByteArray Twitter::URL_TRENDS_DAILY("https://api.twitter.com/1/trends/daily.xml");
const QByteArray Twitter::URL_TRENDS_WEEKLY("https://api.twitter.com/1/trends/weekly.xml");

// Local Trends リソース
const QByteArray Twitter::URL_TRENDS_AVAILABLE("https://api.twitter.com/1/trends/available.xml");
const QByteArray Twitter::URL_TRENDS_WOEID("https://api.twitter.com/1/trends/:woeid.xml");

// Geo リソース
const QByteArray Twitter::URL_GEO_SEARCH("https://api.twitter.com/1/geo/search.xml");
const QByteArray Twitter::URL_GEO_SIMILAR_PLACES("https://api.twitter.com/1/geo/similar_places.xml");
const QByteArray Twitter::URL_GEO_REVERSE_GEOCODE("https://api.twitter.com/1/geo/reverse_geocode.xml");
const QByteArray Twitter::URL_GEO_ID_PLACE_ID("https://api.twitter.com/1/geo/id/:place_id.xml");
const QByteArray Twitter::URL_GEO_PLACE("https://api.twitter.com/1/geo/place.xml");

// Legal リソース
const QByteArray Twitter::URL_LEGAL_TOS("https://api.twitter.com/1/legal/tos.xml");
const QByteArray Twitter::URL_LEGAL_PRIVACY("https://api.twitter.com/1/legal/privacy.xml");

// Help リソース
const QByteArray Twitter::URL_HELP_TEST("https://api.twitter.com/1/help/test.xml");
const QByteArray Twitter::URL_HELP_LANGUAGES("https://api.twitter.com/1/help/languages.xml");
const QByteArray Twitter::URL_HELP_CONFIGURATION("https://api.twitter.com/1/help/configuration.xml");

// 検索 API
const QByteArray Twitter::URL_SEARCH_API("http://search.twitter.com/search.xml");

// Streaming API
const QByteArray Twitter::URL_STREAM_SAMPLE("http://stream.twitter.com/1/statuses/sample.xml");
const QByteArray Twitter::URL_STREAM_FILTER("http://stream.twitter.com/1/statuses/filter.xml");
const QByteArray Twitter::URL_STREAM_FIREHOSE("http://stream.twitter.com/1/statuses/firehose.xml"); // 要申請
const QByteArray Twitter::URL_STREAM_LINKS("http://stream.twitter.com/1/statuses/links.xml"); // 要申請
const QByteArray Twitter::URL_STREAM_RETWEET("http://stream.twitter.com/1/statuses/retweet.xml"); // 要申請
const QByteArray Twitter::URL_USERSTREAM("http://userstream.twitter.com/2/user.xml");
const QByteArray Twitter::URL_SITESTREAM("http://sitestream.twitter.com/2b/site.xml"); // ベータサービス期間中(2011年11月22日現在)


//Public function
Twitter::Twitter(QObject *parent)
    :QObject(parent)
{
    manager = new QNetworkAccessManager(this);
//    connect(manager, SIGNAL(finished(QNetworkReply*)),
//            this, SLOT(replyFinished(QNetworkReply*)));
    timeout = 10000;

    // Params Names
    consumer_keyName = "oauth_consumer_key";
    nonceName = "oauth_nonce";
    signatureMethodName = "oauth_signature_method";
    timestampName = "oauth_timestamp";
    tokenName = "oauth_token";
    versionName = "oauth_version";
    signatureName = "oauth_signature";

    // Params for request and signature
    token = "";
    tokenSecret = "";
    signatureMethod = "HMAC-SHA1";
    version = "1.0";

    // Verified Data
    user_id = "";
    screen_name = "";

    authorized = false;
    verified = false;
}

void Twitter::setAccessToken(const QByteArray access_token, const QByteArray access_tokenSecret)
{
    token = access_token;
    tokenSecret = access_tokenSecret;
    authorized = true;
    verified = true;
}

void Twitter::getRequestToken()
{
    qDebug() << "--------Start getting Request token--------";

    QUrl url(Twitter::URL_REQUESTTOKEN);
    signature = generateSignature(GET, url);

    QByteArray reply = sendRequest(GET, url);
    if(!verified)
        parseTokens(reply);
}

void Twitter::getAccessToken(const QByteArray &pinCode)
{
    qDebug() << "--------Start getting Access token--------";

    QUrl url(Twitter::URL_ACCESSTOKEN);
    url.addQueryItem("oauth_verifier", pinCode);
    ParameterList addParams;
    addParams.append(Parameter("oauth_verifier", pinCode)); // PINコード パラメータ
    signature = generateSignature(GET, url, addParams);

    QByteArray reply = sendRequest(GET, url);
    if(!verified)
        parseTokens(reply);
}

QPair<QByteArray, QByteArray> Twitter::accessToken()
{
    QPair<QByteArray, QByteArray> tokenset(token, tokenSecret);
    return tokenset;
}

bool Twitter::updateStatus(const QByteArray &status)
{
    QUrl url(Twitter::URL_STATUSES_UPDATE);
    ParameterList addParams;
    addParams.append(Parameter("status", status)); // status パラメータ

    signature = generateSignature(POST, url, addParams);
    QByteArray reply = sendRequest(POST, url, ParameterList(), addParams);
    qDebug() << reply;
    return true;
}

QByteArray Twitter::getHomeTimeLine(bool trim_user, bool include_entities, Paging paging)
{
    QUrl url(Twitter::URL_STATUSES_HOME_TIMELINE);
    ParameterList addParams;
    if(trim_user)
        addParams.append(Parameter("trim_user", "true"));
    if(include_entities)
        addParams.append(Parameter("include_entities", "true"));
    addParams.append(paging.toParameterList());

    signature = generateSignature(GET, url);

    QByteArray reply = sendRequest(GET, url);
    return reply;
}

// Private slot もういらない
void Twitter::replyFinished(QNetworkReply *reply)
{
    QByteArray data(reply->readAll());
    if(!verified)
        parseTokens(data);
    emit replyReceived(data); //XML解析でemitするシグナルを分別？
}

//Private function
QByteArray Twitter::generateNonce() // nonce(11桁の乱数)生成
{
    QByteArray chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    int max = chars.size();

    int len = 11; // max=16

    QByteArray tmp;
    for(int i = 0; i < len; ++i){
        tmp.append( chars[qrand() % max] );
    }

    return tmp;
}

QByteArray Twitter::generateTimestamp() // timestamp生成
{
    QDateTime current = QDateTime::currentDateTime();
    uint seconds = current.toTime_t();
    qsrand(seconds);

    return QString::number(seconds).toUtf8();
}

QByteArray Twitter::generateSignature(HttpMethod method, QUrl &url, ParameterList &addParams) //signature生成
{
    nonce = generateNonce(); // nonce 生成
    timestamp = generateTimestamp(); // timestamp 生成

    // params 初期化
    params.clear();
    params.append(Parameter(consumer_keyName, consumer_key));
    params.append(Parameter(nonceName, nonce));
    params.append(Parameter(signatureMethodName,signatureMethod));
    params.append(Parameter(timestampName, timestamp));
    params.append(Parameter(versionName, version));
    if(authorized) // トークン取得後
        params.append(Parameter(tokenName, token));

    params.append(addParams); // パラメータの追加

    sortParams(); // パラメータ(params)を昇順ソート

    //Signature Base String 生成
    QByteArray base;
    if(method == GET)
        base.append("GET&"); // GET method
    else if(method == POST)
        base.append("POST&"); // POST method
    else if(method == DELETE)
        base.append("DELETE&"); // DELETE method (多分使わない)

    // URL と パラメータ を繋ぐ
    base.append(QUrl::toPercentEncoding(url.toString(QUrl::RemoveQuery))+"&");

    QByteArray paramStrings;
    foreach (Parameter p, params) {
        paramStrings.append(p.first);
        paramStrings.append('=');
        paramStrings.append(QUrl::toPercentEncoding(p.second));
        if(p != params.last())
            paramStrings.append('&');
    }
    base.append(QUrl::toPercentEncoding(paramStrings));

    qDebug() << "Base String : " << base;

    //Secret 生成
    QByteArray secret = QUrl::toPercentEncoding(consumer_keySecret) + '&' + QUrl::toPercentEncoding(tokenSecret);
    qDebug() << "Secret : " << secret;

    //HMAC-SHA1 処理
    QByteArray keyBytes = QString(secret).toAscii();
    int keyLength = keyBytes.size();
    const int blockSize = 64;
    if(keyLength > blockSize) {
        QCryptographicHash hash(QCryptographicHash::Sha1);
        hash.addData(keyBytes);
        keyBytes = hash.result();
    }

    // http://tools.ietf.org/html/rfc2104 (1)
    QByteArray ipad;
    QByteArray opad;

    ipad.fill(0, blockSize);
    opad.fill(0, blockSize);

    ipad.replace(0, keyBytes.length(), keyBytes);
    opad.replace(0, keyBytes.length(), keyBytes);

    //(2)&(5)
    for(int i = 0; i < 64; i++) {
        ipad[i] = ipad[i] ^ 0x36;
        opad[i] = opad[i] ^ 0x5c;
    }

    //(3)
    QByteArray workArray;
    workArray.clear();
    workArray.append(ipad, 64);
    workArray.append(QString(base).toAscii());

    //(4)
    QByteArray sha1 = QCryptographicHash::hash(workArray, QCryptographicHash::Sha1);

    //(6)
    workArray.clear();
    workArray.append(opad, 64);
    workArray.append(sha1);

    sha1.clear();

    //(7)
    sha1 = QCryptographicHash::hash(workArray, QCryptographicHash::Sha1);
    return sha1.toBase64();
}

QByteArray Twitter::sendRequest(HttpMethod method, QUrl &url, ParameterList &addParams, ParameterList &bodyParams) //リクエスト送信
{
    if(addParams.isEmpty())
        qDebug() << "Add Params is empty.";
    else {
        Parameter param;
        foreach(param, addParams) {
            qDebug() << "Add Param : " << param.first << " = " << param.second;
        }
    }
    // ヘッダに用いるパラメータリストを初期化
    params.clear();
    params.append(Parameter(consumer_keyName, consumer_key));
    params.append(Parameter(nonceName, nonce));
    params.append(Parameter(signatureMethodName,signatureMethod));
    params.append(Parameter(timestampName, timestamp));
    params.append(Parameter(versionName, version));
    if(authorized) // トークン取得後
        params.append(Parameter(tokenName, token));

    params.append(addParams); // オプションパラメータを追加

    sortParams(); // params をソート

    //ヘッダ生成
    QByteArray authHeader = "OAuth ";
    foreach (Parameter p, params) {
        authHeader.append(p.first);
        authHeader.append("=\"");
        authHeader.append(QUrl::toPercentEncoding(p.second));
        authHeader.append("\",");
    }
    authHeader.append(signatureName); // シグネチャは最後に追加する
    authHeader.append("=\"");
    authHeader.append(QUrl::toPercentEncoding(signature));

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", authHeader);
//    request.setRawHeader("Accept-Encoding", "gzip, deflate"); // レスポンスのgzip圧縮指定

    qDebug() << "Request URL : " << request.url();
    foreach(QByteArray headerName, request.rawHeaderList()) {
        qDebug() << "Request Header : " <<  headerName << request.rawHeader(headerName);
    }

    QTimer replyTimer; // タイマー
    QEventLoop waitReplyLoop; // イベントループ

    replyTimer.setSingleShot(true);
    connect(&replyTimer, SIGNAL(timeout()),
            &waitReplyLoop, SLOT(quit())); // タイムアウトしたらループを終了する
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            &waitReplyLoop, SLOT(quit())); // リプライが返って来たらループを終了する

    QNetworkReply *reply;
    if(method == GET) {
        reply = manager->get(request);
    } else if(method == POST) {
        QByteArray requestBody(""); // リクエストボディ
        foreach (Parameter p, bodyParams) {
            requestBody.append(p.first);
            requestBody.append("=");
            requestBody.append(QUrl::toPercentEncoding(p.second));
            if(p != bodyParams.last())
                requestBody.append(",");
        }
        qDebug() << "Request Body : " << requestBody;
        reply = manager->post(request, requestBody);
    } else if(method == DELETE){
    } else {
    }

    replyTimer.start(timeout); // タイマースタート(10s)
    waitReplyLoop.exec(); // イベントループスタート

    if(replyTimer.isActive()) {
        // リプライが返ってきた
        QByteArray data = reply->readAll();
//        data = qUncompress(data); // gzip解凍
        replyTimer.stop();
        return data;
    } else {
        // タイムアウト
        return QByteArray("Timeout");
    }
}

void Twitter::parseTokens(const QByteArray &tokens)
{
    QList<QByteArray> tokenSets = tokens.split('&');
    QByteArray tmp;

    if(tokenSets.count() > 1) {
        tmp = tokenSets.at(0);
        token = tmp.replace("oauth_token=", "");

        tmp = tokenSets.at(1);
        tokenSecret = tmp.replace("oauth_token_secret=", "");
        if(!authorized) {
            openAuthorizePage();
            authorized = true;
        } else {
            tmp = tokenSets.at(2);
            user_id = tmp.replace("user_id=", "");
            tmp = tokenSets.at(3);
            screen_name = tmp.replace("screen_name=", "");
            verified = true;
        }
        qDebug() << "token : " + token;
        qDebug() << "secret : " + tokenSecret;
    }
}

void Twitter::openAuthorizePage()
{
    QUrl openUrl(URL_AUTHORIZE);
    openUrl.addQueryItem("oauth_token", token);
    QDesktopServices::openUrl(openUrl);
}

void Twitter::sortParams()
{
    int size = params.size();

    if(size > 1) {
        for (int i = 0; i < size - 1; i++) {
            for(int j = i + 1; j < size; j++) {
                Parameter pi = params.at(i);
                Parameter pj = params.at(j);
                if(pi.first > pj.first)
                    params.swap(i, j);
            }
        }
    }
}
