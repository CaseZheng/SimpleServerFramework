#include "example_logic.h"
#include "rabbitmq.h"
#include "configure.h"

bool CExampleLogic::RealDeal(
            const Json::Value &jParams,
            string &strRet, 
            string &strRetCode, 
            string &strRetInfo,
            Json::Value &jData)
{
    SET_RET_INFO("0", "0", "成功");
    CRabbitMQ mq(CConfigure::GetRabbitMqHost(), atoi(CConfigure::GetRabbitMqPort().c_str()), 
            CConfigure::GetRabbitMqUserName(), CConfigure::GetRabbitMqPasswd());
    mq.init();
    SChannelInfo channelInfo;
    channelInfo.setQueueInfo("test_queue");
    channelInfo.setExchangeInfo("test_exchange", EXCHANGE_TYPE_FANOUT);
    channelInfo.setExchangeQueueBind(true);
    mq.openChannel(channelInfo);
    mq.exchangePublish("test_exchange", "", "test_msg");

    string strMsg;
    SQueueSubInfo queueSubInfo;
    //mq.queueSubscribe("test_queue", strMsg);
    //DEBUG("Msg:" + strMsg);
    
    queueSubInfo.setNoAck(false);
    mq.queueSubscribe("test_queue", &CExampleLogic::DealMQMsg, NULL, &queueSubInfo);

    jData["info"] = "hello example";
    return true;
}

bool CExampleLogic::DealMQMsg(const string &strMsg)
{
    DEBUG("Msg:" + strMsg);
    return false;
}
