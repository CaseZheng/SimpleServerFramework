# 请求报文
```
{
    "_head":{
        "_interface":"test",
        "_timestamps":"12345678",
        "_invokeId":"123456",
        "_version":"1.0.1",
        "_callerServerId":"9999"
    },
    "_params":{
    }
}

{
    "_body":{
        "_data":{
            "info":"hello example"
        },
        "_ret":"0",
        "_retCode":"0",
        "_retInfo":"成功"
    },
    "_head":{
        "_callerServerId":"9999",
        "_interface":"test",
        "_invokeId":"123456",
        "_timestamps":"12345678",
        "_version":"1.0.1"
    }
}
```

# 请求示例
```
[CaseZheng@VM_187_252_centos example]$ curl -d '{"_head":{"_interface":"test","_timestamps":"12345678","_invokeId":"123456","_version":"1.0.1","_callerServerId":"9999"},"_params":{}}' http://127.0.0.1:9999/example/uri

{"_body":{"_data":{"info":"hello example"},"_ret":"0","_retCode":"0","_retInfo":"成功"},"_head":{"_callerServerId":"9999","_interface":"test","_invokeId":"123456","_timestamps":"12345678","_version":"1.0.1"}}

```
