import os
import shutil
import csv
import log4cpp_files
import log_collector
import sqlite3


class RMAgentLogParser:

    def __init__(self):
        self.discover_multi_ips = "discover_multi_ips"
        self.discover = "discover"
        
        self.discover_finder_dict = {
            self.discover_multi_ips : ["Starting sub task (LiveAccessWorker.dll DiscoverMultiIps()) in WorkerShell"],
            self.discover : ["Starting root task (LiveAccessWorker.dll Discover()) in WorkerShell"]
        }

    def collect_discover_log(self, backend_log_folder, rm_agent_filename):
        collector = log_collector.LogCollector(self.discover_finder_dict)
        rm_agent_logs = log4cpp_files.get_sorted_logfiles(backend_log_folder, rm_agent_filename)    

        usefull_lines = []
        for log_file in rm_agent_logs:
            usefull_lines_temp = collector.collect(backend_log_folder + "\\" + log_file)    
            usefull_lines.extend(usefull_lines_temp)

        return usefull_lines


class RMAgentTaskFlow:

    def __init__(self):
        self.pid = 0
        self.task_id = ""
        self.parent = ""
        self.root = ""

    def parser(self, line):
        self.pid = RMAgentTaskFlow.get_workshell_pid(line)

        begin = line.find("(selfTaskId")
        end = line.find(")", begin)
        line_temp = line[begin + 1 : end]
        key_values = line_temp.split()
        
        for item in key_values:
            task_info = item.split("=")
            id_key = task_info[0]
            id_value = task_info[1]
            if id_key == "selfTaskId":
                self.task_id = id_value
            elif id_key == "rootTaskId":
                self.root = id_value
            elif id_key == "parentTaskId":
                self.parent = id_value

        if self.parent == "":
            self.parent = self.root
    

    @staticmethod
    def get_workshell_pid(line):
        begin = line.find("in WorkerShell") + len("in WorkerShell")
        end = line.find("(selfTaskId")
        line_temp = line[begin : end]
        line_temp = line_temp.strip()

        return int(line_temp)

    @staticmethod
    def save_to_db(db_filepath, usefull_lines):
        conn = sqlite3.connect(db_filepath)
        c = conn.cursor()
        c.execute('''CREATE TABLE IF NOT EXISTS discover_worker
            (PID                    INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(150),
            TASK_ID                 CHAR(50),
            PARENT_TASK             CHAR(50),
            ROOT_TASK               CHAR(150),
            LOG                     TEXT);''')

        for usefull_line in usefull_lines:
            flow = RMAgentTaskFlow()
            flow.parser(usefull_line.line)
            log_info = log4cpp_files.LogInfo()
            log_info.parser_worker_log(usefull_line.line)
            log = usefull_line.line
            log = log.replace("\"", "\"\"")
            
            sql_string = "INSERT INTO discover_worker(PID, \
                TIMESTAMP_NUM, \
                TIMESTAMP_STRING, \
                CATEGORY, \
                TASK_ID, \
                PARENT_TASK, \
                ROOT_TASK, \
                LOG) VALUES(%d, %d, \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\")"%(
                    flow.pid,
                    log4cpp_files.string_to_ms(log_info.time_string),
                    log_info.time_string,
                    usefull_line.category,
                    flow.task_id,
                    flow.parent,
                    flow.root,
                    log
                )
            
            c.execute(sql_string)

        conn.commit()
        conn.close()

        return

class DirectLiveTask:

    def __init__(self, _id):
        self._id = _id
        self.type = 0
        self.send_from_worker = None
        self.recv_on_worker = []
        self.fsc_recved = None
        self.fsc_dispatch_tofs = None
        self.fsc_rsp_toworker = []

    @staticmethod
    def parser_send_detail_log(detail_log):
        part = "send direct task request, DLA type: "
        index_1 = detail_log.find(part) + len(part)
        
        log_temp = detail_log[index_1 : len(detail_log)]
        part = ", "
        index_1 = log_temp.find(part)
        task_type = int(log_temp[0 : index_1])

        log_temp = log_temp[index_1 + len(part) : len(log_temp)]
        log_temp = log_temp.strip()
        log_temp = log_temp.strip("\r\n")
        task_id = log_temp

        return [task_id, task_type]

    @staticmethod
    def parser_recv_detail_log(detail_log):
        part = ", "
        index_1 = detail_log.find(part) + len(part)
        log_temp = detail_log[index_1 : len(detail_log)]

        part = ", "
        if log_temp.find(part) != -1:
            index_1 = log_temp.find(part)
            log_temp = log_temp[0 : index_1]
        else:
            log_temp.strip("\n")

        log_temp = log_temp.strip()
        task_id = log_temp

        return task_id
        


class WorkerShellLogParser:

    def __init__(self):
        self.send_direct_req = "send_direct_req"
        self.recv_snmp_rsp = "recv_snmp_rsp"
        self.recv_traceroute_rsp = "traceroute"
        self.recv_ping_rsp = "recv_pint_rsp"
        self.recv_Proxyping_rsp = "Proxyping"
        self.recv_SnmpBulk_rsp = "SnmpBulk"
        self.recv_SnmpWalk_rsp = "SnmpWalk"
        self.recv_SnmpTable_rsp = "SnmpTable"
        self.recv_sli_rsp = "cli"
        
        self.discover_finder_dict = {
            self.send_direct_req : ["send direct task request,"],
            self.recv_ping_rsp : ["ProcessOneMsg:299] ping taskid: "],
            self.recv_traceroute_rsp : ["ProcessOneMsg", "traceroute taskid:"],
            self.recv_snmp_rsp : ["ProcessOneMsg", "snmp taskid:"],
            self.recv_Proxyping_rsp : ["ProcessOneMsg", "Proxyping taskid:"],
            self.recv_SnmpBulk_rsp : ["ProcessOneMsg", "SnmpBulk taskid:"],
            self.recv_SnmpWalk_rsp : ["ProcessOneMsg", "SnmpWalk taskid:"],
            self.recv_SnmpTable_rsp : ["ProcessOneMsg", "SnmpTable taskid:"],
            self.recv_sli_rsp : ["ProcessOneMsg", "cli taskid:"]
        }

        self.direct_live_task = {}

    def is_recv_msg(self, category):
        return category == self.recv_ping_rsp or \
            category == self.recv_traceroute_rsp or \
            category == self.recv_snmp_rsp or \
            category == self.recv_Proxyping_rsp or \
            category == self.recv_SnmpBulk_rsp or \
            category == self.recv_SnmpWalk_rsp or \
            category == self.recv_SnmpTable_rsp or \
            category == self.recv_sli_rsp 

    def collect_direct_log(self, worker_root_dir):
        collector = log_collector.LogCollector(self.discover_finder_dict)
        worker_dir_list = log4cpp_files.get_subdirs(worker_root_dir, "WorkerShell_")

        usefull_lines = []
        for worker_foler in worker_dir_list:
            worker_logdir = worker_root_dir + "\\" + worker_foler + "\\log"
            workershell_logs = log4cpp_files.get_sorted_logfiles(worker_logdir, "NBLog.log")   
            for log_file in workershell_logs:
                usefull_lines_temp = collector.collect(worker_logdir + "\\" + log_file)    
                usefull_lines.extend(usefull_lines_temp)

        return usefull_lines

    def save_to_db(self, db_filepath, usefull_lines):
        conn = sqlite3.connect(db_filepath)
        c = conn.cursor()
        c.execute('''CREATE TABLE IF NOT EXISTS direct_live_task
            (TASK_ID                CHAR(50) NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(150),
            TYPE                    CHAR(150),
            LOG                     TEXT);''')

        for usefull_line in usefull_lines:
            loginfo = log4cpp_files.LogInfo()
            loginfo.parser_worker_log(usefull_line.line)
            log_temp = loginfo.detail
            task_id = ""
            task_type = usefull_line.category
            #send_log
            if usefull_line.category ==  self.send_direct_req:
                arr_info = DirectLiveTask.parser_send_detail_log(log_temp)
                task_id = arr_info[0]
                task_type = arr_info[1]
            elif self.is_recv_msg(usefull_line.category):
                task_id = DirectLiveTask.parser_recv_detail_log(log_temp)

            log = usefull_line.line
            log = log.replace("\"", "\"\"")
            sql_string = "INSERT INTO direct_live_task(TASK_ID, \
                PID, \
                THREAD_ID, \
                TIMESTAMP_NUM, \
                TIMESTAMP_STRING, \
                CATEGORY, \
                TYPE, \
                LOG) VALUES(\"%s\", %d, %d, %d, \"%s\", \"%s\", \"%s\", \"%s\")"%(
                    task_id,
                    int(loginfo.pid),
                    int(loginfo.thread_id),
                    log4cpp_files.string_to_ms(loginfo.time_string),
                    loginfo.time_string,
                    usefull_line.category,
                    task_type,                    
                    log
                )

            #print(sql_string)
            c.execute(sql_string)

        conn.commit()
        conn.close()

        return


class FscLogParser:

    def __init__(self):
        self.recv_direct_req = "recv_direct_req"
        self.recv_data = "recv_data"
        self.free_session = "free_session"
        self.send_to_worker = "send_to_worker"
        
        self.finder_dict = {
            self.recv_direct_req : ["handle req direct live access, body size"],
            self.recv_data : ["revc direct live data task id: "],
            self.free_session : ["OnDirectReq", "fsc handle DLA done ok for task id: "],
            self.send_to_worker : ["ResponseThread", "sent to worker taskid: "]
        }

    def collect_log(self, fsc_root_dir):
        collector = log_collector.LogCollector(self.finder_dict)

        usefull_lines = []
        direct_logs = log4cpp_files.get_sorted_logfiles(fsc_root_dir, "log")   
        for log_file in direct_logs:
            usefull_lines_temp = collector.collect(fsc_root_dir + "\\" + log_file)    
            usefull_lines.extend(usefull_lines_temp)

        return usefull_lines

    def save_to_db(self, db_filepath, usefull_lines):
        conn = sqlite3.connect(db_filepath)
        c = conn.cursor()
        c.execute('''CREATE TABLE IF NOT EXISTS fsc_recv_direct_live_task
            (PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(50),
            LOG                     TEXT);''')

        c.execute('''CREATE TABLE IF NOT EXISTS fsc_direct_live_task
            (TASK_ID                CHAR(50)    NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(150),
            MSG_TYPE                CHAR(150),
            LOG                     TEXT);''')

        for usefull_line in usefull_lines:
            loginfo = log4cpp_files.LogInfo()
            loginfo.parser_fsc_log(usefull_line.line)
            log_temp = loginfo.detail
            log = usefull_line.line
            log = log.replace("\"", "\"\"")

            task_id = ""
            task_type = usefull_line.category
            sql_string = ""
            #print(usefull_line.line)
            #send_log
            if usefull_line.category ==  self.recv_direct_req:
                sql_string = "INSERT INTO fsc_recv_direct_live_task( \
                    PID, \
                    THREAD_ID, \
                    TIMESTAMP_NUM, \
                    TIMESTAMP_STRING, \
                    CATEGORY, \
                    LOG) VALUES(%d, %d, %d, \"%s\", \"%s\", \"%s\")"%(
                        int(loginfo.pid),
                        int(loginfo.thread_id),
                        log4cpp_files.string_to_ms(loginfo.time_string),
                        loginfo.time_string,
                        usefull_line.category,
                        log
                    )
            else:
                arr_task_info = self.parser_log(usefull_line.category, log_temp)
                task_id = arr_task_info[0]
                task_type = arr_task_info[1]

                sql_string = "INSERT INTO fsc_direct_live_task(TASK_ID, \
                    PID, \
                    THREAD_ID, \
                    TIMESTAMP_NUM, \
                    TIMESTAMP_STRING, \
                    CATEGORY, \
                    MSG_TYPE, \
                    LOG) VALUES(\"%s\", %d, %d, %d, \"%s\", \"%s\", \"%s\", \"%s\")"%(
                        task_id,
                        int(loginfo.pid),
                        int(loginfo.thread_id),
                        log4cpp_files.string_to_ms(loginfo.time_string),
                        loginfo.time_string,
                        usefull_line.category,
                        task_type,
                        log
                    )

            #print(sql_string)
            c.execute(sql_string)

        conn.commit()
        conn.close()

        return

    def parser_log(self, category, log):
        task_id = ""
        msg_type = ""
        if category == self.recv_data:
            return FscLogParser.parser_recv_data(log)
        elif category == self.free_session:
            return FscLogParser.parser_free_session(log)
        elif category == self.send_to_worker:
            return FscLogParser.parser_send_to_worker(log)
        
        return [task_id, msg_type]

    @staticmethod
    def parser_recv_data(detail_log):
        part = "revc direct live data task id: "
        index_1 = detail_log.find(part) + len(part)
        
        log_temp = detail_log[index_1 : len(detail_log)]
        part = "; "
        index_1 = log_temp.find(part)
        task_id = log_temp[0 : index_1]

        part = "\"msgType\":"
        index_1 = log_temp.find(part)
        log_temp = log_temp[index_1 + len(part) : len(log_temp)]

        part = ","
        index_1 = log_temp.find(part)
        msg_type = int(log_temp[0 : index_1])

        return [task_id, msg_type] 

    @staticmethod
    def parser_send_to_worker(detail_log):
        part = "sent to worker taskid: "
        index_1 = detail_log.find(part) + len(part)
        
        log_temp = detail_log[index_1 : len(detail_log)]
        part = ", "
        index_1 = log_temp.find(part)
        task_id = log_temp[0 : index_1]

        part = "\"msgType\":"
        index_1 = log_temp.find(part)
        log_temp = log_temp[index_1 + len(part) : len(log_temp)]

        part = ","
        index_1 = log_temp.find(part)
        msg_type = int(log_temp[0 : index_1])

        return [task_id, msg_type] 

    @staticmethod
    def parser_free_session(detail_log):
        part = "fsc handle DLA done ok for task id: "
        index_1 = detail_log.find(part) + len(part)
        
        log_temp = detail_log[index_1 : len(detail_log)]
        part = ", "
        index_1 = log_temp.find(part)
        task_id = log_temp[0 : index_1]

        return [task_id, ""]


class FsLogParser:

    def __init__(self):
        self.recv_direct_req = "recv_direct_req"
        self.create_proxy_task = "create_proxy_task"
        self.onbegin = "onbegin"
        self.upload_data = "upload_data"
        self.onend = "onend"
        self.onallone = "onallone"
        self.remove_cache = "remove_cache"
        self.regist_keeplive = "regist_keeplive"
        self.schedule_task = "schedule_task"
        self.unregist_keeplive = "unregist_keeplive"
        self.task_executor = "task_executor"
        self.upload_task_save = "upload_task_save"
        
        self.finder_dict = {
            self.recv_direct_req : ["AddDirectLiveAccessTask", "Add DLA task, "],
            self.create_proxy_task : ["CreateITaskBase", "DLA msg type: "],
            self.onbegin : ["OnBegin", "Begin process "],
            self.upload_data : ["DoDirectLive", "Upload DLA task from msg"],
            self.onend : ["OnEnd", "End process"],
            self.onallone : ["OnAllDone", "process all"],
            self.remove_cache : ["RemoveTaskInstanceInCache", "task id: "],
            self.regist_keeplive : ["RegisterKeepLive", "register task to keep live."],
            self.schedule_task : ["AddTask", "add task result:"],
            self.unregist_keeplive : ["UnRegisterKeepLive", "unregister task to keep live:"],
            self.task_executor : ["FsUnclassifiedTaskExecutor", "Task manager run task"],
            self.upload_task_save : ["Save", "direct taskid: "]
        }

    def collect_log(self, fs_root_dir):
        collector = log_collector.LogCollector(self.finder_dict)

        usefull_lines = []
        direct_logs = log4cpp_files.get_sorted_logfiles(fs_root_dir, "log")   
        for log_file in direct_logs:
            usefull_lines_temp = collector.collect(fs_root_dir + "\\" + log_file)    
            usefull_lines.extend(usefull_lines_temp)

        return usefull_lines

    def save_to_db(self, db_filepath, usefull_lines):
        conn = sqlite3.connect(db_filepath)
        c = conn.cursor()
        c.execute('''CREATE TABLE IF NOT EXISTS fs_direct_live_task
            (TASK_ID                CHAR(50)    NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(50),
            MSG_TYPE                CHAR(150),
            LOG                     TEXT);''')

        for usefull_line in usefull_lines:
            loginfo = log4cpp_files.LogInfo()
            loginfo.parser_fs_log(usefull_line.line)
            arr_task_info = self.parser_log(usefull_line.category, loginfo.detail)
            task_id = arr_task_info[0]
            task_type = arr_task_info[1]
            log = usefull_line.line
            log = log.replace("\"", "\"\"")
            #print(usefull_line.line)
            sql_string = "INSERT INTO fs_direct_live_task( \
                    TASK_ID, \
                    PID, \
                    THREAD_ID, \
                    TIMESTAMP_NUM, \
                    TIMESTAMP_STRING, \
                    CATEGORY, \
                    MSG_TYPE, \
                    LOG) VALUES(\"%s\", %d, %d, %d, \"%s\", \"%s\", \"%s\", \"%s\")"%(
                        task_id,
                        int(loginfo.pid),
                        int(loginfo.thread_id),
                        log4cpp_files.string_to_ms(loginfo.time_string),
                        loginfo.time_string,
                        usefull_line.category,
                        task_type,
                        log
                    )

            #print(sql_string)
            c.execute(sql_string)

        conn.commit()
        conn.close()

        return

    def parser_log(self, category, detail_log):
        task_id = ""
        msg_type = ""

        pre = ""
        later = ""
        strip_list = ["\n", " "]
        if category == self.recv_direct_req:
            pre = "Add DLA task, "
            later = "."
        elif category == self.create_proxy_task:
            pre = "guid: "
            later = " ["
        elif category == self.onbegin:
            pre = "task id: "
            later = ", "
        elif category == self.upload_data:
            pre = "group id: "
            later = ", "
            msg_type = FsLogParser.sub_str(detail_log, "msg type:", ",", strip_list)
        elif category == self.onend:
            pre = "task id: "
            later = ", "
        elif category == self.onallone:
            pre = "task id: "
            later = ", "
        elif category == self.remove_cache:
            pre = "task id: "
            later = ", "
        elif category == self.regist_keeplive:
            pre = "register task to keep live. "
            later = " ["
        elif category == self.schedule_task:
            pre = "TaskID: "
            later = " ["
        elif category == self.unregist_keeplive:
            pre = "unregister task to keep live: "
            later = ". "
        elif category == self.task_executor:
            pre = "task id: "
            later = ", "
        elif category == self.upload_task_save:
            pre = "direct taskid: "
            later = "; "
            msg_type = FsLogParser.sub_str(detail_log, "\"msgType\":", ",", strip_list)
        else:
            return [task_id, msg_type]

        task_id = FsLogParser.sub_str(detail_log, pre, later, strip_list)
        return [task_id, msg_type]

    @staticmethod
    def sub_str(detail_log, pre, later, strip_list):
        index_1 = detail_log.find(pre) + len(pre)
        
        log_temp = detail_log[index_1 : len(detail_log)]
        
        index_1 = log_temp.find(later)
        ret_value = log_temp[0 : index_1]

        for temp in strip_list:
            ret_value.strip(temp)

        return ret_value

def main():
    is_collect_worker = False
    is_collect_fsc = False
    is_collect_fs = True
    is_delete_last_db = True

    backend_logdir = r"D:\case_file\fsc\discover_log"
    worker_root_dir = r"D:\case_file\fsc\discover_log\2018-03-22 - Copy"
    fsc_root_dir = r"D:\case_file\fsc\discover_log\fsc - Copy"
    fs_root_dir = r"D:\case_file\fsc\2000Device\7.1\10.10.6.21\log\frontsvr"
    #fs_root_dir = r"E:\aaaa"
    rmagent_logfilename = "RMAgent_liaowen.log"
    worker_log_filename = "worker_direct_log.log"
    analysis_folder = r"dierct_live_analysis_log"

    db_filename = "discover_log.db"
    db_filepath = analysis_folder + "\\" + db_filename
    
    if os.path.exists(analysis_folder):
        if is_delete_last_db:
            #os.removedirs(analysis_folder) #删除空文件夹
            shutil.rmtree(analysis_folder)
            os.mkdir(analysis_folder)
    else:
        os.mkdir(analysis_folder)
    
    if is_collect_worker:
        rmagent_log_parser = RMAgentLogParser()
        #worker startup logs
        worker_startup_logs = rmagent_log_parser.collect_discover_log(backend_logdir, rmagent_logfilename)
        #root_tasklines = log_collector.UsefullLine.filter(worker_startup_logs, rmagent_log_parser.discover)

        log_collector.UsefullLine.save_to_file(analysis_folder + "\\" + "worker_startup.log", worker_startup_logs)
        RMAgentTaskFlow.save_to_db(db_filepath, worker_startup_logs)
        
        #worker shell logs
        workershell_log_parser = WorkerShellLogParser()
        worker_logs = workershell_log_parser.collect_direct_log(worker_root_dir)
        workershell_log_parser.save_to_db(db_filepath, worker_logs)
    
    #fsc logs
    if is_collect_fsc:
        fsc_log_parser = FscLogParser()
        fsc_logs = fsc_log_parser.collect_log(fsc_root_dir)
        fsc_log_parser.save_to_db(db_filepath, fsc_logs)
    
    #fs logs
    if is_collect_fs:
        fs_log_parser = FsLogParser()
        fs_logs = fs_log_parser.collect_log(fs_root_dir)
        fs_log_parser.save_to_db(db_filepath, fs_logs)
    
    print("succeed.")

main()

    
