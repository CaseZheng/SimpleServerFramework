#include "rabbitmq.h"

#include "log.h"

#define AMQP_RPC_REPLY_JUDEG(r, msg)  \
    if(!amqpRpcReplyJudeg(r)) \
    { \
        ERROR(string(msg)); \
        return false; \
    }

CRabbitMQ::CRabbitMQ(const string &strHostName, int iPort, const string &strUserName, const string &strPassword, const string &strVHost) 
    : m_socket(NULL), 
        m_conn(NULL), 
        m_strHostName(strHostName), 
        m_iPort(iPort), 
        m_strUserName(strUserName), 
        m_strPassword(strPassword), 
        m_strVHost(strVHost),
        m_iChannelIdMax(0),
        m_iFrameMax(AMQP_DEFAULT_FRAME_SIZE)
{
}

CRabbitMQ::~CRabbitMQ()
{
    if(!destroyConnect())
    {
        ERROR("destroyConnect error");
    }
    else 
    {
        INFO("destroy rabbitMQ connect success");
    }
}


bool CRabbitMQ::init()
{
    m_conn = amqp_new_connection(); //声明一个新的amqp connection
    if(NULL == m_conn)
    {
        ERROR("amqp_new_connection error");
        return false;
    }
    m_socket = amqp_tcp_socket_new(m_conn); //创建socket
    if(NULL == m_socket) 
    {
        ERROR("amqp_tcp_socket_new error");
        return false;
    }

    int status = amqp_socket_open(m_socket, m_strHostName.c_str(), m_iPort);    //打开socket
    if(0 != status)
    {
        ERROR("amqp_socket_open error");
        return false;
    }
    if(!connect())
    {
        ERROR("connect error");
        return false;
    }
    INFO("rabbitMQ connect success");

    return true;
}

bool CRabbitMQ::connect()
{
    if(NULL == m_conn)
    {
        return false;
    }

    AMQP_RPC_REPLY_JUDEG(amqp_login(m_conn, m_strVHost.c_str(), m_iChannelIdMax, m_iFrameMax, 0,
                AMQP_SASL_METHOD_PLAIN, m_strUserName.c_str(), m_strPassword.c_str()), "amqp_login error");
    return true;
}

bool CRabbitMQ::openChannel(const SChannelInfo &info)
{
    if(NULL == m_conn)
    {
        return false;
    }
    if(m_mChannelInfo.find(info.m_iChannelId) != m_mChannelInfo.end() 
            && m_mChannelInfo[info.m_iChannelId].m_bValid)
    {
        return false;
    }
    amqp_channel_open(m_conn, info.m_iChannelId);
    AMQP_RPC_REPLY_JUDEG(amqp_get_rpc_reply(m_conn), "amqp_channel_open error");

    if(info.m_bExchangeDeclare)
    {
        amqp_exchange_declare(m_conn, info.m_iChannelId, amqp_cstring_bytes(info.m_strExchangeName.c_str()), 
            amqp_cstring_bytes(info.m_strExchangeType.c_str()), info.m_iExchangePassive, 
            info.m_iExchangeDurable, info.m_iExchangeAutoDelete, 0, amqp_empty_table);
        AMQP_RPC_REPLY_JUDEG(amqp_get_rpc_reply(m_conn), "amqp_exchange_declare error");
    }

    if(info.m_bQueueDeclare)
    {
        amqp_queue_declare(m_conn, info.m_iChannelId, amqp_cstring_bytes(info.m_strQueueName.c_str()), 
            info.m_iQueuePassive, info.m_iQueueDurable,info. m_iQueueExclusive,
            info.m_iQueueAutoDelete, amqp_empty_table);
        AMQP_RPC_REPLY_JUDEG(amqp_get_rpc_reply(m_conn), "amqp_queue_declare error");
    }

    if(info.m_bQueueExchangeBind)
    {
        amqp_queue_bind(m_conn, info.m_iChannelId, amqp_cstring_bytes(info.m_strQueueName.c_str()), 
            amqp_cstring_bytes(info.m_strExchangeName.c_str()), 
            amqp_cstring_bytes(info.m_strExchangeType.c_str()), amqp_empty_table);
        AMQP_RPC_REPLY_JUDEG(amqp_get_rpc_reply(m_conn), "amqp_queue_bind error");
    }

    m_mChannelInfo[info.m_iChannelId] = info;
    m_mChannelInfo[info.m_iChannelId].m_bValid = true;

    return true;
}

bool CRabbitMQ::destroyChannel(int iChannel)
{
    if(m_mChannelInfo.find(iChannel) == m_mChannelInfo.end())
    {
        return true;
    }
    if(!m_mChannelInfo[iChannel].m_bValid)
    {
        return true;
    }
    if(NULL == m_conn)
    {
        return false;
    }

    AMQP_RPC_REPLY_JUDEG(amqp_channel_close(m_conn, iChannel, AMQP_REPLY_SUCCESS), 
            ("amqp_channel_close error channelId:" + to_string(iChannel)));

    m_mChannelInfo[iChannel].m_bValid = false;

    return true;
}

bool CRabbitMQ::destroyConnect()
{
    for(auto &it : m_mChannelInfo)
    {
        destroyChannel(it.first);
    }

    AMQP_RPC_REPLY_JUDEG(amqp_connection_close(m_conn, AMQP_REPLY_SUCCESS),
            "amqp_connection_close error");

    if(AMQP_STATUS_OK != amqp_destroy_connection(m_conn))
    {
        ERROR("amqp_destroy_connection error");
        return false;
    }
    return true;
}

bool CRabbitMQ::amqpRpcReplyJudeg(const amqp_rpc_reply_t &r)
{
    if(r.reply_type == AMQP_RESPONSE_NORMAL)
    {
        return true;
    }
    else if(r.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION)
    {
        ERROR("AMQP_RESPONSE_LIBRARY_EXCEPTION");
        return false;
    }
    else if(r.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION)
    {
        ERROR("AMQP_RESPONSE_SERVER_EXCEPTION");
        return false;
    }
    else 
    {
        ERROR("Undefined reply_type:" + to_string(r.reply_type));
        return false;
    }

}

bool CRabbitMQ::queuePublish(const string &strQueueName, const string &strMsg, int iChannel)
{
    return exchangePublish("", strQueueName, strMsg, iChannel);
}

bool CRabbitMQ::exchangePublish(const string &strExchangeName, const string &strRoutingKey, 
        const string &strMsg, int iChannelId)
{
    DEBUG("publish ExchangeName:" + strExchangeName + " RoutingKey/QueueName:" 
            + strRoutingKey + " Msg:" + strMsg);
    int iMandatory = 0;
    int iImmediate = 0;
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = AMQP_DELIVERY_PERSISTENT;
    
    int ret = amqp_basic_publish(m_conn, iChannelId, amqp_cstring_bytes(strExchangeName.c_str()), 
        amqp_cstring_bytes(strRoutingKey.c_str()), iMandatory, iImmediate, &props, 
        amqp_cstring_bytes(strMsg.c_str()));
    if(AMQP_STATUS_OK != ret)
    {
        ERROR("exchangePublish error, ret:" + to_string(ret));
        return false;
    }
    return true;
}

bool CRabbitMQ::queueSubscribe(const string &strQueueName, string &strMsg, 
            struct timeval *pTimeVal, SQueueSubInfo *pQueueSubInfo)
{
    return queueSubscribe(strQueueName, strMsg, NULL, pTimeVal, pQueueSubInfo);
}

bool CRabbitMQ::queueSubscribe(const string &strQueueName, bool(*dealFun)(const string &),
            struct timeval *pTimeVal, SQueueSubInfo *pQueueSubInfo)
{
    string strMsg;
    return queueSubscribe(strQueueName, strMsg, dealFun, pTimeVal, pQueueSubInfo);
}

bool CRabbitMQ::queueSubscribe(const string &strQueueName, string &strMsg, bool(*dealFun)(const string &),
            struct timeval *pTimeVal, SQueueSubInfo *pQueueSubInfo)
{
    static SQueueSubInfo defaultQueueSubInfo;
    if(NULL == pQueueSubInfo)
    {
        pQueueSubInfo = &defaultQueueSubInfo;
    }

    bool   bNoLocal       = pQueueSubInfo->m_bNoLocal;
    bool   bNoAck         = pQueueSubInfo->m_bNoAck;
    bool   bExclusive     = pQueueSubInfo->m_bExclusive;
    int    iChannelId     = pQueueSubInfo->m_iChannelId;
    string strConsumerTag = pQueueSubInfo->m_strConsumerTag;

    int iPrefetchSize = 0;
    int iPrefetchCount = 1;
    amqp_basic_qos(m_conn, iChannelId, iPrefetchSize, iPrefetchCount, 0);

    amqp_basic_consume(m_conn, iChannelId, amqp_cstring_bytes(strQueueName.c_str()), 
        amqp_cstring_bytes(strConsumerTag.c_str()), bNoLocal, bNoAck, bExclusive, amqp_empty_table);

    amqp_envelope_t envelope;
    memset(&envelope,0,sizeof(envelope));
    amqp_maybe_release_buffers(m_conn);

    AMQP_RPC_REPLY_JUDEG(amqp_consume_message(m_conn, &envelope, pTimeVal, 0), "amqp_consume_message error");
    vector<char> vecChar(envelope.message.body.len, '\0');
    memcpy(&vecChar[0], envelope.message.body.bytes, envelope.message.body.len);
    strMsg.assign(vecChar.begin(), vecChar.end());
    bool ret = true;
    if(NULL != dealFun)
    {
        ret = dealFun(strMsg);
    }
    if(!bNoAck)
    {
        if(0 == amqp_basic_ack(m_conn, iChannelId, envelope.delivery_tag, ret ? 0 : -1))
        {
            INFO("ack success");
        }
        else 
        {
            INFO("ack failure");
        }
    }
    amqp_destroy_envelope(&envelope);

    return ret;
}
