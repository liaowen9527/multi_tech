import sqlite3
import pymongo
import sys
import pathlib
import os
import nbjson
import datetime

g_conn_str = 'mongodb://10.99.99.232:27017'
g_domain = 'Domain_Liaowen'

g_db_conn = pymongo.MongoClient(g_conn_str) 

def enum_members(cls):
    attrs = {}
    for attr in dir(cls):
        if attr.startswith('__'):
            continue
        
        attrs[attr] = getattr(cls, attr)

    return attrs

class OpenTopoTables:
    Device = "OpenTopoDeviceTable"
    Interface = "OpenTopoIPInterfaceTable"
    ArpTable = "OpenTopoArpTable"
    CdpTable = "OpenTopoCdpTable"
    MacTable = "OpenTopoMacTable"
    SegmentTable = "OpenTopoSegment"
    L3Segment = "OpenTopoL3Segment"
    L2Segment = "OpenTopoL2Segment"
    L3Topo = "OpenTopoL3TopoType"
    Media = "OpenTopoMedia"


class Mongo2Sqlite:
    def __init__(self, collection) -> None:
        # report folder
        folder = os.path.abspath(os.path.dirname(__file__))
        self.folder = '%s/datas/%s' % (folder, g_domain)
        pathlib.Path(self.folder).mkdir(parents=True, exist_ok=True)

        self.collection = collection
        self.db_path = '%s/%s.db' % (self.folder, collection)
        try:
            os.remove(self.db_path)
        except:
            pass
        
    def run(self):
        mongo_cur = g_db_conn[g_domain][self.collection].find({})

        conn = None
        full_d = {}
        docs = []
        for d in mongo_cur:
            docs.append(d)
            for k, v in d.items():
                if k is not None and k not in full_d:
                    full_d[k] = v

        if not full_d:
            return

        self._create_table(full_d)
        conn = sqlite3.connect(self.db_path)
        conn.execute("BEGIN TRANSACTION")

        for d in docs:
            keys = []  
            values = []
            for k, v in d.items():
                if v is None:
                    continue

                keys.append(k)
                v_s = ''
                if isinstance(v, datetime.datetime):
                    v_s = v.strftime('%Y-%m-%dT%H:%M:%S')
                elif isinstance(v, list):
                    v_s = nbjson.dumps(v)
                elif isinstance(v, dict):
                    v_s = nbjson.dumps(v)
                else:
                    v_s = str(v)
                    
                values.append("'%s'" % v_s)

            s = "INSERT INTO %s(%s) values(%s)" % (self.collection, ','.join(keys), ','.join(values))
            conn.execute(s)

        conn.execute("COMMIT")

    def _create_table(self, d):
        conn = sqlite3.connect(self.db_path)
        fields = []
        for k, v in d.items():
            if isinstance(v, int):
                fields.append('%s INT' % k)
            else:
                fields.append('%s TEXT' % k)

        c = conn.cursor()
        c.execute('''CREATE TABLE IF NOT EXISTS %s (%s);''' % (self.collection, ','.join(fields)))


if __name__ == '__main__':
    if 1:
        sys.argv.append(g_conn_str)
        sys.argv.append(g_domain)

    count = len(sys.argv)
    if count < 3:
        print('usage: <mongo_str> <osg_dir> <domain_name> <type>')
        exit()
        
    g_conn_str = sys.argv[1]
    g_domain = sys.argv[2]
    g_db_conn = pymongo.MongoClient(g_conn_str) 

    for k, v in enum_members(OpenTopoTables).items():
        p = Mongo2Sqlite(v)
        p.run()

