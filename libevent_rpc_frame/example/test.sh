#!/usr/bin/sh
curl -d '{"_head":{"_callerServerId":"100009","_invokeId":"4ccfb1feafafaefeafaefaef","_timestamps":"123456789","_version":"1"}, "_param":{"msg":"hello rpc"}}' http://127.0.0.1:9995/.rpc.test
