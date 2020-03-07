import os
import csv
import log4cpp_files
import log_collector
import tree_builder


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
    def build_tree(task_flows):
        relationship_list = []
        for flow in task_flows:
            relationship_list.append(tree_builder.RelationShip(flow.task_id, flow.parent, flow))

        flow_tree = tree_builder.Tree()
        flow_tree.build_tree(relationship_list)

        return flow_tree

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
    def get_csv_head():
        return [
            "task_id", 
            "send_pid",
            "send_thread",
            "cost_num(ms)"
            "cost_time", 
            "send_timestamp", 
            "last_recv_timestamp",
            "recv_timestamp"
        ]

    @staticmethod
    def get_send_index():
        return 5

    @staticmethod
    def get_last_recv_index():
        return 6

    @staticmethod
    def get_cost_index():
        return 3

    def to_csv_row(self):
        arr = []
        arr.append(self._id)
        arr.append(self.send_from_worker.pid)
        arr.append(self.send_from_worker.thread_id)

        last_recv_timestamp = ""
        recv_on_worker = sorted(self.recv_on_worker, key=lambda log_info: log_info.time_string, reverse=False)
        if len(recv_on_worker) > 0:
            last_recv_timestamp = recv_on_worker[len(recv_on_worker) - 1].time_string

        if last_recv_timestamp == "":
            diff = 0
        else:
            diff = log4cpp_files.calucate_cost(self.send_from_worker.time_string, last_recv_timestamp)

        arr.append(diff)
        arr.append(log4cpp_files.ms_cost_to_timestring(diff))
        arr.append(self.send_from_worker.time_string)
        arr.append(last_recv_timestamp)

        for recv_info in recv_on_worker:
            arr.append(recv_info.time_string)

        return arr


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

    def collect_direct_log(self, worker_root_dir, pid):
        collector = log_collector.LogCollector(self.discover_finder_dict)
        worker_logdir = WorkerShellLogParser.get_logdir(worker_root_dir, pid)
        workershell_logs = log4cpp_files.get_sorted_logfiles(worker_logdir, "NBLog.log")    

        usefull_lines = []
        for log_file in workershell_logs:
            usefull_lines_temp = collector.collect(worker_logdir + "\\" + log_file)    
            usefull_lines.extend(usefull_lines_temp)

        return usefull_lines

    def analysis(self, direct_logs):
        dic_dla = {}
        for usefull_line in direct_logs:
            loginfo = log4cpp_files.LogInfo()
            loginfo.parser_worker_log(usefull_line.line)
            log_temp = loginfo.detail
            task_id = ""
            #send_log
            if usefull_line.category ==  self.send_direct_req:
                arr_info = DirectLiveTask.parser_send_detail_log(log_temp)
                task_id = arr_info[0]
                task_type = arr_info[1]
                if not dic_dla.get(task_id):
                    dic_dla[task_id] = DirectLiveTask(task_id)
                
                dic_dla[task_id].task_type = task_type
                dic_dla[task_id].send_from_worker = loginfo

            elif self.is_recv_msg(usefull_line.category):
                task_id = DirectLiveTask.parser_recv_detail_log(log_temp)
                if not dic_dla.get(task_id):
                    dic_dla[task_id] = DirectLiveTask(task_id)

                dic_dla[task_id].recv_on_worker.append(loginfo)
        
        return dic_dla

    @staticmethod
    def export_multi_csv(folder, dic_dla):
        dic_csv = {}
        dla_list = sorted(dic_dla.items(), key=lambda d:d[1].send_from_worker.time_string) 
        for item in dla_list: 
            dla = item[1]
            pid = dla.send_from_worker.pid
            thread_id = dla.send_from_worker.thread_id
            append_string = "%s_%s"%(pid, thread_id)
            if not dic_csv.get(append_string):
                dic_csv[append_string] = []

            dic_csv[append_string].append(dla.to_csv_row())

        for item in dic_csv.items():
            append_string = item[0]
            filename = "%s//dla_%s.csv"%(folder, append_string)

            total_cost = 0
            recv_data_cost = 0
            begin_send = ""
            last_recv = ""
            for num in range(0, len(item[1])):
                row = item[1][num]
                if num == 0:
                    begin_send = row[DirectLiveTask.get_send_index()]
                elif num == len(item[1]) - 1:
                    last_recv = row[DirectLiveTask.get_last_recv_index()]
                    if last_recv == "":
                        last_recv = row[DirectLiveTask.get_send_index()]
            
                recv_data_cost += row[DirectLiveTask.get_cost_index()]

            total_cost = log4cpp_files.calucate_cost(begin_send, last_recv)

            item[1].insert(0, DirectLiveTask.get_csv_head())
            item[1].insert(0, [total_cost, 
                log4cpp_files.ms_cost_to_timestring(recv_data_cost),
                "total_cost:",
                log4cpp_files.ms_cost_to_timestring(total_cost)])

            csvfile = open(filename, 'w', newline='')
            writer = csv.writer(csvfile)
            writer.writerows(item[1])
            csvfile.close()
                

    @staticmethod
    def export_csv(filename, dic_dla):
        csvfile = open(filename, 'w', newline='')
        writer = csv.writer(csvfile)

        rows = []
        rows.append(DirectLiveTask.get_csv_head())

        dla_list = sorted(dic_dla.items(), key=lambda d:d[1].send_from_worker.time_string) 
        for item in dla_list: 
            dla = item[1]
            rows.append(dla.to_csv_row())
                
        writer.writerows(rows)

        csvfile.close()


    @staticmethod
    def get_logdir(worker_root_dir, pid):
        return worker_root_dir + "\\" + WorkerShellLogParser.getfolder(worker_root_dir, pid) + "\\log"

    @staticmethod
    def getfolder(root_folder, pid):
        folder_key =  "WorkerShell_%d" % (pid)
        for root, subdirs, files in os.walk(root_folder):
            for sub_dir in subdirs:
                if sub_dir.find(folder_key) == 0:
                    return sub_dir

        return None



def main():
    backend_logdir = r"D:\case_file\fsc\discover_log"
    worker_root_dir = r"D:\case_file\fsc\discover_log\2018-03-22 - Copy"
    rmagent_logfilename = "RMAgent_liaowen.log"
    worker_log_filename = "worker_direct_log.log"
    analysis_folder = "dierct_live_analysis_log"
    if not os.path.exists(analysis_folder):
        os.mkdir(analysis_folder)

    rmagent_log_parser = RMAgentLogParser()
    #worker startup logs
    worker_startup_logs = rmagent_log_parser.collect_discover_log(backend_logdir, rmagent_logfilename)
    #root_tasklines = log_collector.UsefullLine.filter(worker_startup_logs, rmagent_log_parser.discover)

    log_collector.UsefullLine.save_to_file(analysis_folder + "\\" + "worker_startup.log", worker_startup_logs)

    task_flows = []
    for usefull_line in worker_startup_logs:
        flow = RMAgentTaskFlow()
        flow.parser(usefull_line.line)
        task_flows.append(flow)

    taskflow_tree = RMAgentTaskFlow.build_tree(task_flows)
    sorted_nodes = taskflow_tree.level_loop(taskflow_tree.root_list)

    workershell_log_parser = WorkerShellLogParser()
    direct_logs = []
    for node in sorted_nodes:
        pid = node._data.pid
        direct_logs_temp = workershell_log_parser.collect_direct_log(worker_root_dir, pid)
        direct_logs.extend(direct_logs_temp)

        direct_logs_temp = sorted(direct_logs_temp, key=lambda usefull_line: log4cpp_files.get_timestamp_string(usefull_line.line))
        log_collector.UsefullLine.save_to_file(analysis_folder + "\\"  + "worker_direct_log_%d.log"%(pid), direct_logs_temp)

        dic_dla = workershell_log_parser.analysis(direct_logs_temp)
        #workershell_log_parser.export_csv(analysis_folder + "\\"  + "dla_%d.csv"%(pid), dic_dla)
        workershell_log_parser.export_multi_csv(analysis_folder, dic_dla)

    #direct_logs = sorted(direct_logs, key=lambda usefull_line: log4cpp_files.get_timestamp_string(usefull_line.line))
    #log_collector.UsefullLine.save_to_file(worker_log_filename, direct_logs)

    #dic_dla = workershell_log_parser.analysis(direct_logs)
    #workershell_log_parser.export_csv(csv_filename, dic_dla)

    print("succeed.")

main()

    
