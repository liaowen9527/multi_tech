import sys 
import os
sys.path.append(os.path.abspath(os.path.dirname(__file__) + '/..'))
sys.path.append(r'C:\Users\wliao\AppData\Local\Programs\Python\Python37\Lib\site-packages')

print('\r\n'.join(sys.path))

import uuid
import json
from packages import log4cpp_files
from packages.nb_datetime import *
from packages.db_table import *

import mtools
from mtools.util.cmdlinetool import LogFileTool


class MongoConnLog:
    def __init__(self, conn, listener):
        self.conn = conn
        self.listener = listener
        self.details = []
        self.uuid = str(uuid.uuid1())

    def lifecycle(self):
        begin = self.details[0]
        end = self.details[len(self.details) - 1]
        cost = nb_datetime.cost_ms(end.datetime, begin.datetime)

        return {
            'uuid' : self.uuid,
            'conn' : self.conn,
            'begin' : nb_datetime.to_string(begin.datetime),
            'end' : nb_datetime.to_string(end.datetime),
            'listener' : self.listener,
            'cost': cost,
            'cost_t' : nb_datetime.duration_ms(cost)
        }

    @staticmethod
    def get_default_detail():
        return {
            'uuid': '12345',
            "_id": 10188,
            "datetime": "2019-11-07T21:26:10.704000+00:00",
            "operation": "command",
            "thread": "conn48155",
            "namespace": "USAA.$cmd",
            "nscanned": 0,
            "nreturned": 0,
            "duration": 169,
            "numYields": 0,
            "w": 1,
            "r": 1,
            "line_str": "2019-11-07T21:26:10.704Z I COMMAND [conn48155] command "
        }

    def get_details(self):
        fields = MongoConnLog.get_default_detail().keys()

        ret = []
        for logevent in self.details:
            temp = logevent.to_dict(fields)
            temp['uuid'] = self.uuid
            ret.append(temp)

        return ret


class MongoLogAnalysis:

    def __init__(self):
        self.log_folder = ''
        self.filename = ''
        self.output_dir = '' 
        self.conns_temp = {}
        self.done_conns = []

    def get_dao(self):
        dao = NbDBTable()
        dao.dbpath = os.path.join(self.output_dir, 'mongo.db')
        return dao
    
    def savedb(self):
        dao = self.get_dao()
        if os.path.exists(dao.dbpath):
            os.remove(dao.dbpath)

        files = log4cpp_files.get_sorted_logfiles( self.log_folder,  self.filename)
        for f in files:
            self.load_file(f)
            self.save_conns_todb(self.done_conns)
            self.done_conns = []

        #save opening connections
        self.save_conns_todb(self.conns_temp.values())

    def load_file(self, filename):
        print('load data from %s' % (filename))
        self.mlogtool = LogFileTool()
        filepath = os.path.join(self.log_folder, filename)
        sys.argv.append(filepath)

        LogFileTool.run(self.mlogtool)
        self._export()

        sys.argv.pop()

    def save_conns_todb(self, conns):
        if len(conns) == 0:
            return

        print('save conns into db, count:%d' % (len(self.done_conns)))
        #save db
        dao = self.get_dao()

        arr = []
        arr_details = []
        for conn in conns:
            arr_details.extend(conn.get_details())
            arr.append(conn.lifecycle())

        #save conn
        dao.name = 'MongoConn'
        dao.set_pk_field('uuid')
        dao.init_fields_by_dict(arr[0])

        dao.insert_many(arr)

        #save details
        dao = self.get_dao()
        dao.name = 'MongoConnDetail'
        #dao.set_pk_field('uuid')
        dao.init_fields_by_dict(MongoConnLog.get_default_detail())

        dao.insert_many(arr_details)

    def _export(self):        
        for line_no, logevent in enumerate(self.mlogtool.args['logfile']):
            if logevent.thread is None:
                continue

            obj = None
            conn_id = logevent.thread
            if logevent.thread == 'listener':
                #get connection id
                conn_id = self.get_conn_from_listener(logevent.line_str)
                if conn_id is None:
                    continue

                obj = MongoConnLog(conn_id, True)

            if logevent.thread not in self.conns_temp:
                obj = MongoConnLog(conn_id, False)
            
            if obj is None:
                obj = self.conns_temp[conn_id]
            else:
                self.conns_temp[conn_id] = obj

            logevent._id = line_no
            obj.details.append(logevent)

            if logevent.line_str.find('] end connection ') != -1:
                self.done_conns.append(obj)
                self.conns_temp.pop(conn_id)      

    def get_conn_from_listener(self, line_str):
        key = ' #'
        p1 = line_str.find(key)
        if p1 == -1:
            return None

        begin = p1 + len(key)
        end = line_str.find(" (")
        
        ret = line_str[begin : end]
        return 'conn' + ret.strip()


if __name__ == '__main__':
    sys.argv.append(r'E:\thoubleshooting\case\USAA\mongo_log_2\mongo.log')
    if len(sys.argv) < 2:
        print("must need folder and rmagent file name.")
        exit(0)

    filepath = sys.argv[1]
    sys.argv.pop()

    tool = MongoLogAnalysis()
    tool.log_folder = os.path.abspath(os.path.dirname(filepath))
    tool.filename = os.path.basename(filepath)
    tool.output_dir = os.path.join(tool.log_folder, 'output')
    tool.savedb()

    
