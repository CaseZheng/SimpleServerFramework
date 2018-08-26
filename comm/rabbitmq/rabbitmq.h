#ifndef _RABBITMQ_H_
#define _RABBITMQ_H_

#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <string>
#include <vector>
#include <map>
#include "tools.h"

using namespace std;

static const int  CHANNEL_DEFAULT_ID           = 1;     // 渠道 默认为1
static const int  EXCHANGE_DEFAULT_PASSIVE     = 0;     // 交换器 默认不检测exchange存在
static const int  EXCHANGE_DEFAULT_DURABLE     = 1;     // 交换器 默认队列持久化
static const int  EXCHANGE_DEFAULT_AUTO_DELETE = 0;     // 交换器 默认连接断开不自动删除exchange
static const int  QUEUE_DEFAULT_PASSIVE        = 0;     // 队列 默认不检测queue存在
static const int  QUEUE_DEFAULT_DURABLE        = 1;     // 队列 默认队列持久化
static const int  QUEUE_DEFAULT_EXCLUSIVE      = 0;     // 队列 默认非私有队列
static const int  QUEUE_DEFAULT_AUTO_DELETE    = 0;     // 队列 默认连接断开不自动删除queue

//rabbitMQ交换器类型
static const string EXCHANGE_TYPE_FANOUT = "fanout";
static const string EXCHANGE_TYPE_DIRECT = "direct";
static const string EXCHANGE_TYPE_TOPIC  = "topic";

struct SChannelInfo
{
    SChannelInfo() : m_iChannelId(CHANNEL_DEFAULT_ID),
            m_bExchangeDeclare(false),
            m_strExchangeName(""), 
            m_strExchangeType(""), 
            m_iExchangePassive(EXCHANGE_DEFAULT_PASSIVE), 
            m_iExchangeDurable(EXCHANGE_DEFAULT_DURABLE),
            m_iExchangeAutoDelete(EXCHANGE_DEFAULT_AUTO_DELETE), 
            m_bQueueDeclare(false),
            m_strQueueName(""), 
            m_iQueuePassive(QUEUE_DEFAULT_PASSIVE), 
            m_iQueueDurable(QUEUE_DEFAULT_DURABLE),
            m_iQueueExclusive(QUEUE_DEFAULT_EXCLUSIVE), 
            m_iQueueAutoDelete(QUEUE_DEFAULT_AUTO_DELETE),
            m_bQueueExchangeBind(false),
            m_bValid(false)
    {
    }

    void setQueueInfo(const string &strQueueName,
            bool bQueueDeclare   = true,
            int iQueuePassive    = QUEUE_DEFAULT_PASSIVE,
            int iQueueDurable    = QUEUE_DEFAULT_DURABLE,
            int iQueueExclusive  = QUEUE_DEFAULT_EXCLUSIVE,
            int iQueueAutoDelete = QUEUE_DEFAULT_AUTO_DELETE)
    {
        m_strQueueName       = strQueueName;
        m_bQueueDeclare      = bQueueDeclare;
        m_iQueuePassive      = iQueuePassive;
        m_iQueueDurable      = iQueueDurable;
        m_iQueueExclusive    = iQueueExclusive;
        m_iQueueAutoDelete   = iQueueAutoDelete;
    }

    void setExchangeInfo(const string &strExchangeName, const string &strExchangeType,
            bool bExchangeDeclare   = true,
            int iExchangePassive    = EXCHANGE_DEFAULT_PASSIVE,
            int iExchangeDurable    = EXCHANGE_DEFAULT_DURABLE,
            int iExchangeAutoDelete = EXCHANGE_DEFAULT_AUTO_DELETE)
    {
        m_strExchangeName     = strExchangeName;
        m_strExchangeType     = strExchangeType;
        m_bExchangeDeclare    = bExchangeDeclare;
        m_iExchangePassive    = iExchangePassive;
        m_iExchangeDurable    = iExchangeDurable;
        m_iExchangeAutoDelete = iExchangeAutoDelete;
    }

    void setExchangeQueueBind(bool bBind) { m_bQueueExchangeBind = bBind; }

    int      m_iChannelId;
    bool     m_bExchangeDeclare;
    string   m_strExchangeName;
    string   m_strExchangeType;
    int      m_iExchangePassive;
    int      m_iExchangeDurable;
    int      m_iExchangeAutoDelete;
    bool     m_bQueueDeclare;
    string   m_strQueueName;
    int      m_iQueuePassive;
    int      m_iQueueDurable;
    int      m_iQueueExclusive;
    int      m_iQueueAutoDelete;
    bool     m_bQueueExchangeBind;
    bool     m_bValid;
};

struct SQueueSubInfo
{
    SQueueSubInfo() : m_iChannelId(CHANNEL_DEFAULT_ID), m_bNoLocal(false), m_bNoAck(true), 
        m_bExclusive(false), m_strConsumerTag(tools::GetProcessName())
    {
    }
    void setChannelId(int iChannelId) { m_iChannelId = iChannelId; }
    void setNoLocal(bool bNoLocal) { m_bNoLocal = bNoLocal; }
    void setNoAck(bool bNoAck) { m_bNoAck = bNoAck; }
    void setExclusive(bool bExclusive) { m_bExclusive = bExclusive; }
    void setConsumerTag(const string &strConsumerTag) { m_strConsumerTag = strConsumerTag; }

    int    m_iChannelId;
    bool   m_bNoLocal;
    bool   m_bNoAck;
    bool   m_bExclusive;
    string m_strConsumerTag;
};

class CRabbitMQ
{
public:
    CRabbitMQ(const string &strHostName, int iPort, const string &strUserName, const string &strPassword, const string &strVHost="/");
    virtual ~CRabbitMQ();


    bool init();
    void setVHost(const string &strVHost) { m_strVHost = strVHost; }
    bool destroyChannel(int iChannelId);
    bool openChannel(const SChannelInfo &info);
    bool queuePublish(const string &strQueueName, const string &strMsg, int iChannelId=CHANNEL_DEFAULT_ID);
    bool exchangePublish(const string &strExchangeName, const string &strRoutingKey, const string &strMsg, int iChannelId=CHANNEL_DEFAULT_ID);
    bool queueSubscribe(const string &strQueueName, string &strMsg, struct timeval *pTimeVal=NULL, 
            SQueueSubInfo *pQueueSubInfo=NULL);

    bool queueSubscribe(const string &strQueueName, bool(*dealFun)(const string &),
            struct timeval *pTimeVal=NULL, SQueueSubInfo *pQueueSubInfo=NULL);

private:
    bool connect();
    bool destroyConnect();
    bool amqpRpcReplyJudeg(const amqp_rpc_reply_t &r);

    bool subscribe(const string &strQueueName, string &strMsg, bool(*dealFun)(const string &),
            struct timeval *pTimeVal, SQueueSubInfo *pQueueSubInfo);


protected:
    amqp_socket_t * m_socket;
    amqp_connection_state_t m_conn;

private:
    string   m_strHostName;
    int      m_iPort;
    string   m_strUserName;
    string   m_strPassword;
    string   m_strVHost;
    int      m_iChannelIdMax;
    int      m_iFrameMax;

    map<int, SChannelInfo> m_mChannelInfo;
};

#endif
