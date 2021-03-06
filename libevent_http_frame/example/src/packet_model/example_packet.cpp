#include "example_packet.h"
#include <evhttp.h>
#include "log.h"
#include "rdkafkacpp.h"
#include <ctime>
#include "configure.h"
#include <boost/shared_ptr.hpp>

class KafakaEventCb : public RdKafka::EventCb 
{
public:
    void event_cb (RdKafka::Event &event)
    { 
        switch (event.type())
        {  
            case RdKafka::Event::EVENT_ERROR:
                ERROR("EVENT" << event.type() << RdKafka::err2str(event.err()) << " " << event.str());
            break;
            case RdKafka::Event::EVENT_STATS:
                ERROR("EVENT" << event.type() << RdKafka::err2str(event.err()) << " " << event.str());
            break;
            case RdKafka::Event::EVENT_LOG:
                ERROR("EVENT" << event.type() << RdKafka::err2str(event.err()) << " " << event.str());
            break;
            default:
                DEBUG("EVENT" << event.type() << RdKafka::err2str(event.err()) << " " << event.str());
            break;
        }  
    }

};  

class KafkaDeliveryReportCb : public RdKafka::DeliveryReportCb 
{
public:
    void dr_cb (RdKafka::Message &message)
    { 
        DEBUG("Message delivery for (" << message.len() << " bytes): " << message.errstr());
        if(message.key())
        {
            DEBUG("Key: " << *(message.key()));
        }
    }
};

bool CExamplePacket::DealPacket()
{
    boost::shared_ptr<RdKafka::Conf> gConf(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)); 
    boost::shared_ptr<RdKafka::Conf> tConf(RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC));


    std::string errstr;
    gConf->set("metadata.broker.list", CConfigure::GetKafkaHostPort(), errstr);
    KafakaEventCb kafakaEventCb;
    gConf->set("event_cb", &kafakaEventCb, errstr);

    KafkaDeliveryReportCb kafkaDeliveryReportCb;
    /* Set delivery report callback */ 
    gConf->set("dr_cb", &kafkaDeliveryReportCb, errstr);

    /* Create producer using accumulated global configuration. */
    boost::shared_ptr<RdKafka::Producer> producer(RdKafka::Producer::create(gConf.get(), errstr));
    if(!producer)
    {
        ERROR("Failed to create producer: " << errstr);
        return false;
    }
    DEBUG("Created producer: " << producer->name());

    string strTopic = "testTopic";
    /* Create topic handle. */
    boost::shared_ptr<RdKafka::Topic> topic(RdKafka::Topic::create(producer.get(), 
                                                    strTopic, tConf.get(), errstr));
    if(!topic)
    {
        ERROR("Failed to create topic: " << errstr);
        return false;
    }
	string strMsg = "hello Kafak" + to_string(time(NULL));
	RdKafka::ErrorCode resp = producer->produce(topic.get(), RdKafka::Topic::PARTITION_UA,
          RdKafka::Producer::RK_MSG_COPY,
          const_cast<char *>(strMsg.c_str()), strMsg.size(),
          NULL, NULL);
  	if(resp != RdKafka::ERR_NO_ERROR)
	{
		ERROR("Produce failed: " << RdKafka::err2str(resp));
		return false;
	}
	else
	{
		DEBUG("Produced message (" << strMsg.size() << " bytes)"); 
  		producer->poll(0);
	}

    sleep(3);

    m_strRespCode = "0";
    m_strOut      = "hello ExamplePacket";
    m_strReason   = "";
    return true;
}
