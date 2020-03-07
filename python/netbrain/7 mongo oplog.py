# -*- coding=utf-8 -*-
from pymongo import MongoClient
import csv
import time
import datetime
from urllib.parse import quote_plus


domainname = 'local'


passwd = "mongodb"
username = passwd
#username = "mongodb"

host = "10.10.4.231:27017"

uri = "mongodb://%s:%s@%s" % (
    quote_plus(username), quote_plus(passwd), host)


connetion = MongoClient(uri)
domaindb = connetion.get_database(domainname)
results = domaindb.oplog.rs.find({"ns":"Initial_Tenant.FSC_DtgRecvDataCounter"})

def op2str(item):
    if item["op"] == "i":
        return "insert"
    if item["op"] == "u":
        return "update"
    if item["op"] == "d":
        return "delete"

    return "unknown"

if False: #check MongoDB current time
    
    NgSystem = connetion.get_database("NGSystem")
    NgSystem.xxx.insert({"make":"123"})
    results1 = domaindb.oplog.rs.find({"ns":"NGSystem.xxx"})

    mongoTime = ""
    for item in results1:
        oData = item.get('o')
        ts = item.get("ts")
        #print((ts.as_datetime()), op2str(item),oData)
        mongoTime = ts.as_datetime()
    NgSystem.xxx.drop()

    print('local utc:' +str(datetime.datetime.utcnow() ), " mongo utc:", mongoTime)

if False:
    
    for item in results:
        oData = item.get('o')
        ts = item.get("ts")
        print((ts.as_datetime()), op2str(item),oData)
else:
    wfp = open(r"oplog.txt","w", encoding='utf-8')

    for item in results:
        oData = item.get('o')
        ts = item.get("ts")
    
        wfp.write(str(ts.as_datetime()))
        wfp.write(", ")
        wfp.write(op2str(item))
        wfp.write(", ")
        wfp.write(str(oData))
        wfp.write("\n")

    wfp.close()


print("done")
