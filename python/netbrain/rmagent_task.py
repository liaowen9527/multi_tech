import sys 
import os
sys.path.append(os.path.abspath(os.path.dirname(__file__) + '/..'))

print('\r\n'.join(sys.path))

import csv
from packages import log4cpp_files
from packages import log_collector
from packages.log_analysis import * 
from packages.db_table import *
from packages.nb_datetime import *
from packages import nb_filesystem


class RMTask:
    def __init__(self):
        self.worker_pid = 0
        self.self_taskid = ''
        self.parent_taskid = ''
        self.root_taskid = ''
        self.task_type = ''
        #2020-01-29 16:25:48,995
        self.begin_utc_time = ''
        self.end_utc_time = ''

    @staticmethod
    def get_pk_field():
        return "taskid"

    def to_dict(self):
        d = self.cost()
        return {
            'taskid': self.self_taskid,
            'worker_pid': self.worker_pid,
            "task_type": self.task_type,
            'parent_taskid': self.parent_taskid,
            'root_taskid': self.root_taskid,
            'begin_utc_time': self.begin_utc_time,
            'end_utc_time': self.end_utc_time,
            'duration' : d,
            'duration2' : nb_datetime.duration(d)
        }

    def cost(self):
        tlen = len('2020-01-29 16:25:48,995')
        to = len('2020-01-29 16:25:48')
        dt1 = None
        dt2 = None

        if len(self.begin_utc_time) == tlen:
            dt1 = nb_datetime.from_string(self.begin_utc_time[0: to], "%Y-%m-%d %H:%M:%S") 

        if len(self.end_utc_time) == tlen:
            dt2 = nb_datetime.from_string(self.end_utc_time[0: to], "%Y-%m-%d %H:%M:%S") 

        if dt1 is not None and dt2 is not None:
            return nb_datetime.cost(dt2, dt1)

        return 0

    def cost_s(self):
        return nb_datetime.duration(self.cost())


class RMAgentLogParser(LogParser):

    def __init__(self):
        self.start_task = "start_task"
        self.end_task = "end_task"
        
        self.finder_dict = {
            self.start_task : ["Starting"],
            self.end_task : ["ended", "(selfTaskId"]
        }

        self.tasks = {}

    def get_workshell_pid(self, line):
        key = "in WorkerShell"
        p1 = line.find(key)
        if p1 == -1:
            key = "in EVShellCSharp"
            p1 = line.find(key)

        if p1 == -1:
            return -1

        begin = p1 + len(key)
        end = line.find("(selfTaskId")
        line_temp = line[begin : end]
        line_temp = line_temp.strip()

        return int(line_temp)

    def get_task_type(self, line):
        key = "task ("
        p1 = line.find(key)
        if p1 == -1:
            return ""

        begin = p1 + len(key)
        end = line.find("())")
        line_temp = line[begin : end]
        line_temp = line_temp.strip()

        return line_temp
        

    def parse(self, usefull_line):
        task = RMTask()
        
        line = usefull_line.line
        utc_time = log4cpp_files.get_timestamp_string(line)

        begin = line.find("(selfTaskId")
        end = line.find(")", begin)
        line_temp = line[begin + 1 : end]
        key_values = line_temp.split()
        
        for item in key_values:
            task_info = item.split("=")
            id_key = task_info[0]
            id_value = task_info[1]
            if id_key == "selfTaskId":
                task.self_taskid = id_value
            elif id_key == "rootTaskId":
                task.root_taskid = id_value
            elif id_key == "parentTaskId":
                task.parent_taskid = id_value

        task.worker_pid = self.get_workshell_pid(line)

        if task.parent_taskid == "":
            task.parent_taskid = task.root_taskid

        if task.self_taskid not in self.tasks:
            self.tasks[task.self_taskid] = task

        #set time
        old_task = self.tasks[task.self_taskid]

        category = usefull_line.category
        if category == self.start_task:
            old_task.worker_pid = task.worker_pid
            old_task.begin_utc_time = utc_time
            old_task.task_type = self.get_task_type(line)
        elif category == self.end_task:
            old_task.end_utc_time = utc_time

def find_rmagent(dir_):
    files = nb_filesystem.get_all_files(dir_, filter_='RMAgent*', is_path=False)
    for f in files:
        if f.endswith('.log'):
            print('rmagent log name:', f)
            return f

    return ""


