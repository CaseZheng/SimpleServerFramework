#include <event2/event.h>
#include <evhttp.h>
#include <event2/buffer.h>

#include "http_server.h"
#include "log.h"
#include "configure.h"
#include "main_conf.h"

void CHttpServer::HttpLog(int severity, const char *msg)
{
    if(EVENT_LOG_DEBUG == severity || EVENT_LOG_MSG == severity)
    {
        DEBUG(msg);
    }
    else if(EVENT_LOG_WARN == severity)
    {
        WARNING(msg);
    }
    else
    {
        ERROR(msg);
    }
}

bool CHttpServer::Init()
{
    //event_enable_debug_logging(EVENT_DBG_ALL);
    event_enable_debug_logging(EVENT_DBG_NONE);
    event_set_log_callback(CHttpServer::HttpLog);

    m_pEventBase.reset(event_base_new(), event_base_free);
    if(NULL == m_pEventBase)
    {
        ERROR("event_base_new failure");
        return false;
    }

    m_pEvHttp.reset(evhttp_new(m_pEventBase.get()), evhttp_free);
    if(NULL == m_pEvHttp)
    {
        ERROR("evhttp_new failure");
        return false;
    }

    if(0 != evhttp_bind_socket(m_pEvHttp.get(), 
                CConfigure::GetIp().c_str(), 
                ::atoi(CConfigure::GetPort().c_str()) ))
    {
        ERROR("evhttp_bind_socket failure");
        return false;
    }

    m_pPacketModel.reset(new (std::nothrow)  CPacketModel);
    if(NULL == m_pPacketModel)
    {
        ERROR("new CPacketModel failure");
        return false;
    }

    evhttp_set_gencb(m_pEvHttp.get(), &HttpCb, m_pPacketModel.get());

    if(!CMainConf::AddInterface())
    {
        ERROR("CMainConf::AddInterface failure");
        return false;
    }

    return true;
}

bool CHttpServer::Run()
{
    if(0 != event_base_dispatch(m_pEventBase.get()))
    {
        ERROR("event_base_dispatch failure");
        return false;
    }
    return true;
}

bool CHttpServer::AddUri(const string &strUri, 
        void (*cb)(evhttp_request *, void*),
        void *cbarg)
{
    if(0 != evhttp_set_cb(m_pEvHttp.get(), strUri.c_str(), cb, cbarg))
    {
        ERROR("evhttp_set_cb failure");
        return false;
    }
    return true;
}

void CHttpServer::HttpCb(evhttp_request *pEvHttpRequest, void *pArg)
{
    DEBUG("==================RPC START===================");
    CPacketModel *pPacketModel = static_cast<CPacketModel*>(pArg);
    if(NULL == pPacketModel)
    {
        ERROR("pPacketModel is NULL");
        return;
    }

    enum evhttp_cmd_type eHttpType = evhttp_request_get_command(pEvHttpRequest);
    const char* szUri = evhttp_request_uri(pEvHttpRequest);
    struct evhttp_uri* uriDecode = evhttp_uri_parse(szUri);
    if (NULL == uriDecode)
    {
        evhttp_add_header(pEvHttpRequest->output_headers, "Content-Type", "text/html; charset=UTF-8");
        evhttp_add_header(pEvHttpRequest->output_headers, "Server", "");
        evhttp_add_header(pEvHttpRequest->output_headers, "Connection", "close");

        struct evbuffer* evbuff = evbuffer_new();
        const string err = "非法的请求，url请求解析错误";
        evbuffer_add(evbuff, (const void*)err.c_str(), err.length());
        evhttp_send_reply(pEvHttpRequest, HTTP_BADREQUEST, "OK", evbuff);
        evbuffer_free(evbuff);
        ERROR(std::string("非法的请求，url请求解析错误:") + std::string(szUri));
        DEBUG("==================RPC END===================");
        return;
    }

    const char* szUrlPath = evhttp_uri_get_path(uriDecode);
    const char* szUrlParams = evhttp_uri_get_query(uriDecode);
    szUrlPath = (NULL == szUrlPath) ? "" : szUrlPath;
    szUrlParams = (NULL == szUrlParams) ? "" : szUrlParams;

    string strAddUrlParams;
    struct evkeyvalq* evKV = evhttp_request_get_input_headers(pEvHttpRequest);
    if (NULL != evKV)
    {
            //const char* szIP = evhttp_find_header(evKV, "cli_real_ip");
            //strAddUrlParams += std::string("&http_cli_ip=") + ((NULL == szIP) ? "" : szIP);
            //const char* szAllIP = evhttp_find_header(evKV, "cli_all_ip");
            //strAddUrlParams += std::string("&http_all_ip=") + ((NULL == szAllIP) ? "" : szAllIP);
    }

    std::size_t iLen = EVBUFFER_LENGTH(pEvHttpRequest->input_buffer);

    std::string strIn;
    std::string strUrlPath(szUrlPath);
    std::string strUrlParams(szUrlParams);
    strUrlParams += strAddUrlParams;
    std::vector<char> vecChar(iLen);
    memcpy(&vecChar[0], (void*)EVBUFFER_DATA(pEvHttpRequest->input_buffer), iLen);
    strIn.assign(vecChar.begin(), vecChar.end());

    //输入参数
    pPacketModel->InitInPacket(eHttpType, strIn, strUrlPath, strUrlParams);
    DEBUG("evhttp_cmd_type=" + to_string((unsigned long)eHttpType));
    DEBUG("urlpath=" + strUrlPath);
    DEBUG("urlparams=" + strUrlParams);
    DEBUG("indata=" + strIn);

    //报文模型类实例 进行处理，主要业务逻辑都在这里展开
    pPacketModel->DealPacket();

    //输出参数
    std::string strRespCode = pPacketModel->GetRespCode();
    std::string strOut = pPacketModel->GetOut();
    std::string strReason = pPacketModel->GetReason();
    DEBUG("RespCode=" + strRespCode);
    DEBUG("Reason=" + strReason);
    DEBUG("outdata=" + strOut);

    evhttp_add_header(pEvHttpRequest->output_headers, "Content-Type", "text/html; charset=UTF-8");
    evhttp_add_header(pEvHttpRequest->output_headers, "Server", "");
    evhttp_add_header(pEvHttpRequest->output_headers, "Connection", "close");

    //http报文返回
    struct evbuffer* evbuff = evbuffer_new();
    evbuffer_add(evbuff, (const void*)strOut.c_str(), strOut.length());
    evhttp_send_reply(pEvHttpRequest, ::atoi(strRespCode.c_str()), strReason.c_str(), evbuff);

    evbuffer_free(evbuff);
    evhttp_uri_free(uriDecode);

    DEBUG("==================RPC END===================");
}
