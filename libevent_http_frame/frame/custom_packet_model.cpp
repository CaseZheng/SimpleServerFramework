#include "custom_packet_model.h"
#include <evhttp.h>
#include "log.h"

void CPacket::HttpCb(evhttp_request *pEvHttpRequest, void *pArg)
{
    DEBUG("==================RPC START===================");
    CPacket *pPacket = static_cast<CPacket*>(pArg);
    if(NULL == pPacket)
    {
        ERROR("CPacket is NULL");
        return;
    }
    pPacket->Reset();
    pPacket->m_pEvHttpRequest = pEvHttpRequest;
    if(!pPacket->HttpCbStart())
    {
        ERROR("HttpCbStart failure");
        return;
    }
    pPacket->DealPacket();
    pPacket->HttpCbEnd();
}


bool CPacket::HttpCbStart()
{
    m_eHttpType = evhttp_request_get_command(m_pEvHttpRequest);
    const char* szUri = evhttp_request_uri(m_pEvHttpRequest);
    boost::shared_ptr<struct evhttp_uri> uriDecode;
    uriDecode.reset(evhttp_uri_parse(szUri), evhttp_uri_free);
    if (NULL == uriDecode)
    {
        evhttp_add_header(m_pEvHttpRequest->output_headers, "Content-Type", "text/html; charset=UTF-8");
        evhttp_add_header(m_pEvHttpRequest->output_headers, "Server", "");
        evhttp_add_header(m_pEvHttpRequest->output_headers, "Connection", "close");

        boost::shared_ptr<struct evbuffer> evbuff;
        evbuff.reset(evbuffer_new(), evbuffer_free);
        const string err = "非法的请求，url请求解析错误";
        evbuffer_add(evbuff.get(), (const void*)err.c_str(), err.length());
        evhttp_send_reply(m_pEvHttpRequest, HTTP_BADREQUEST, "OK", evbuff.get());
        ERROR(std::string("非法的请求，url请求解析错误:") + std::string(szUri));
        DEBUG("==================RPC END===================");
        return false;
    }

    const char* szUrlPath = evhttp_uri_get_path(uriDecode.get());
    const char* szUrlParams = evhttp_uri_get_query(uriDecode.get());
    szUrlPath = (NULL == szUrlPath) ? "" : szUrlPath;
    szUrlParams = (NULL == szUrlParams) ? "" : szUrlParams;

    string strAddUrlParams;
    struct evkeyvalq* evKV = evhttp_request_get_input_headers(m_pEvHttpRequest);
    if (NULL != evKV)
    {
            //const char* szIP = evhttp_find_header(evKV, "cli_real_ip");
            //strAddUrlParams += std::string("&http_cli_ip=") + ((NULL == szIP) ? "" : szIP);
            //const char* szAllIP = evhttp_find_header(evKV, "cli_all_ip");
            //strAddUrlParams += std::string("&http_all_ip=") + ((NULL == szAllIP) ? "" : szAllIP);
    }

    std::size_t iLen = EVBUFFER_LENGTH(m_pEvHttpRequest->input_buffer);

    m_strUrlPath = string(szUrlPath);
    m_strUrlParams = string(szUrlParams) + strAddUrlParams;
    std::vector<char> vecChar(iLen);
    memcpy(&vecChar[0], (void*)EVBUFFER_DATA(m_pEvHttpRequest->input_buffer), iLen);
    m_strIn.assign(vecChar.begin(), vecChar.end());

    //输入参数
    DEBUG("evhttp_cmd_type=" + to_string((unsigned long)m_eHttpType));
    DEBUG("urlpath=" + m_strUrlPath);
    DEBUG("urlparams=" + m_strUrlParams);
    DEBUG("indata=" + m_strIn);
    return true;
}

bool CPacket::HttpCbEnd()
{
    DEBUG("RespCode=" + m_strRespCode);
    DEBUG("Reason="   + m_strReason);
    DEBUG("outdata="  + m_strOut);

    evhttp_add_header(m_pEvHttpRequest->output_headers, "Content-Type", "text/html; charset=UTF-8");
    evhttp_add_header(m_pEvHttpRequest->output_headers, "Server", "");
    evhttp_add_header(m_pEvHttpRequest->output_headers, "Connection", "close");

    //http报文返回
    boost::shared_ptr<struct evbuffer> evbuff;
    evbuff.reset(evbuffer_new(), evbuffer_free);
    evbuffer_add(evbuff.get(), (const void*)m_strOut.c_str(), m_strOut.length());
    evhttp_send_reply(m_pEvHttpRequest, ::atoi(m_strRespCode.c_str()), m_strReason.c_str(), evbuff.get());

    DEBUG("==================RPC END===================");
    return true;
}
