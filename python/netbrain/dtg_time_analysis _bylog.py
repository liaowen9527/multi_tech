from pymongo import MongoClient
import csv
import datetime
import time
import log4cpp_files
import dtglog_keyword


def ms_to_timestring(ms):
    m, s = divmod(ms / 1000, 60)
    h, m = divmod(m, 60)
    return ("%02d:%02d:%02d %03d" % (h, m, s, ms%1000))

class DtgInfo:

    def __init__(self, dtgid):
        self.id = dtgid
        self.sub_dtg_ids = []

        self.recv_from_network = 0
        self.ack_recv_dtg = 0
        self.succeed_load_dtg = 0
        self.ack_dispatch_fromfs = 0  


class DtgSummaryInfo:

    def __init__(self):
        self.recv_dtg_counts = 0
        self.dtg_string_to_json_cost = 0
        self.save_dtg_todb_cost = 0
        self.parser_dtgjson_cost = 0
        self.insert_queue_cost = 0
        self.dispatch_cost = 0
        self.trydispatch_cost = 0
        self.check_resdata_cost = 0
        self.splitdtg_cost = 0
        self.save_subdtg_cost = 0

        self.first_recv_dtg_time = 0
        self.last_recv_dtg_time = 0
        self.first_ack_dtg_time = 0
        self.last_ack_dtg_time = 0
        self.ack_dtg_cost = 0

    def export_csv(self, strFilePath):
        csvfile = open(strFilePath, 'w', newline='')
        writer = csv.writer(csvfile)

        rows = []
        rows.append([
            "recv_dtg_counts", 
            "dtg_string_to_json_cost",
            "save_dtg_todb_cost", 
            "ack_dtg_cost", 
            "parser_dtgjson_cost", 
            "insert_dtg_queue", 
            "dispatch_cost", 
            "trydispatch_cost", 
            "check_resdata_cost", 
            "splitdtg_cost", 
            "save_subdtg_cost"
        ])

        rows.append([
            self.recv_dtg_counts, 
            ms_to_timestring(self.dtg_string_to_json_cost), 
            ms_to_timestring(self.save_dtg_todb_cost), 
            ms_to_timestring(self.ack_dtg_cost),
            ms_to_timestring(self.parser_dtgjson_cost), 
            ms_to_timestring(self.insert_queue_cost), 
            ms_to_timestring(self.dispatch_cost), 
            ms_to_timestring(self.trydispatch_cost), 
            ms_to_timestring(self.check_resdata_cost), 
            ms_to_timestring(self.splitdtg_cost), 
            ms_to_timestring(self.save_subdtg_cost)
        ])
                
        writer.writerows(rows)

        csvfile.close()



class UsefulLine:

    def __init__(self, line, key, datetime, thread_id):
        self.line = line
        self.key = key
        self.datetime = datetime
        self.thread_id = thread_id

    @staticmethod
    def datetime_diff(begin_timestamp, end_timestamp):
        datetime_diff = end_timestamp - begin_timestamp
        datetime_diff = round(datetime_diff * 1000)
        return datetime_diff

    @staticmethod
    def calculate_datetime_diff(begin, end):
        return UsefulLine.datetime_diff(begin.datetime.timestamp(), end.datetime.timestamp())

    @staticmethod
    def calculate_cost(begin_list, end_list):
        cost = 0
        for index, line_info_end in enumerate(end_list):
            line_info_begin = begin_list[index]
            cost += UsefulLine.calculate_datetime_diff(line_info_begin, line_info_end)

        return cost

    @staticmethod
    def calculate_cost_for_multithread(begin_list, end_list):
        first_begin = begin_list[0]
        last_end = end_list[len(end_list) - 1]
        return UsefulLine.calculate_datetime_diff(first_begin, last_end)


class PairBeginEnd:

    def __init__(self, begin, end):
        self.begin = begin
        self.end = end

    @staticmethod
    def calculate_pair(begin_list, end_list):
        end_list = end_list[:]
        pair_list = []
        for line_info_begin in begin_list:
            for index, line_info_end in enumerate(end_list):
                if line_info_begin.thread_id == line_info_end.thread_id:
                    duration = PairBeginEnd(line_info_begin, line_info_end)
                    pair_list.append(duration)
                    end_list.pop(index)
                    break

        return pair_list

    @staticmethod
    def calculate_cost(pair_list):
        cost = 0
        for pair_begin_end in pair_list:
            line_info_begin = pair_begin_end.begin
            line_info_end = pair_begin_end.end
            cost += UsefulLine.calculate_datetime_diff(line_info_begin, line_info_end)

        return cost


class DtgParser:

    dic_dtg_keywords = {
        dtglog_keyword.recv_from_network: ["recv data task group:"],
        dtglog_keyword.ack_recv_dtg: ["response creare data task group, fsCode:"],
        dtglog_keyword.consumer_dtgs_begin: ["begin consumer data task group, size:"],
        dtglog_keyword.consumer_dtgs_end: ["TaskManager::DoConsumerOnce", "end."],
        dtglog_keyword.dtg_string_to_json_begin: ["TaskManager::ParserDtgAndAsyncSave", "begin."],
        dtglog_keyword.dtg_string_to_json_end: ["TaskManager::ParserDtgAndAsyncSave", "end parser to json."],
        dtglog_keyword.save_to_db_begin: ["TaskManager::SaveDtgToDB", "begin."],
        dtglog_keyword.save_to_db_end: ["TaskManager::SaveDtgToDB", "end."],
        dtglog_keyword.parser_dtg_to_object_begin: ["TaskManager::AddOneToPreparedQueue", "begin."],
        dtglog_keyword.parser_dtg_to_object_end: ["TaskManager::InnerAdd", "begin add to queue, id:"],
        dtglog_keyword.insert_to_mgrqueue_end: ["TaskManager::InnerAdd", "end."],
        dtglog_keyword.dispatch_begin: ["begin dispatch dtg, dtgId:"],
        dtglog_keyword.dispatch_end: ["end dispatch dtg, succeed:"],
        dtglog_keyword.trydispatch_begin: ["begin try dispatch, device count:"],
        dtglog_keyword.trydispatch_end: ["end try dispatch"],
        dtglog_keyword.check_resdata_begin: ["begin check resdata ready."],
        dtglog_keyword.check_resdata_end: ["end check resdata ready"],
        dtglog_keyword.split_dtg_end: ["succeed split dtg."],
        dtglog_keyword.save_subdtg_end: ["end dispatch, dtgId:"],
        dtglog_keyword.ack_dispatch_fromfs: ["recv response sub data task group:"]
    }

    def __init__(self):
        self.useful_lines = []
        self.category_useful_lines = {}

        #for multithread
        self.pair_parser_dtg_to_obj = []
        self.pair_save_to_db = []

    def collect_useful_log(self, filepath):
        fileObject = open(filepath, "r")
        while True:
            line = fileObject.readline()
            if line:
                match_line = DtgParser.parser_line(line)
                if match_line:
                    self.add_match_line(match_line[0], line)
            else:
                break

        fileObject.close()

        self.calculate_step_duration()

    def add_match_line(self, key, line):
        timestamp_string = log4cpp_files.get_timestamp_string(line)
        datetime = log4cpp_files.string_to_datetime(timestamp_string)
        thread_id = log4cpp_files.get_thread_id(line)

        line_info = UsefulLine(line, key, datetime, thread_id)
        self.useful_lines.append(line_info)

        if not(self.category_useful_lines.get(key)):
            self.category_useful_lines[key] = []
        
        self.category_useful_lines[key].append(line_info)

    def calculate_step_duration(self):
        #parser_dtg_to_obj
        begin_list = self.category_useful_lines[dtglog_keyword.dtg_string_to_json_begin]
        end_list = self.category_useful_lines[dtglog_keyword.dtg_string_to_json_end]
        #end_list = end_list[:]
        self.pair_parser_dtg_to_obj = PairBeginEnd.calculate_pair(begin_list, end_list)

        #save_to_db
        begin_list = self.category_useful_lines[dtglog_keyword.save_to_db_begin]
        end_list = self.category_useful_lines[dtglog_keyword.save_to_db_end]
        self.pair_save_to_db = PairBeginEnd.calculate_pair(begin_list, end_list)

    def statistic(self):
        return

    def statistic_summaryinfo(self):
        summary_info = DtgSummaryInfo()
        #recv_from_network
        summary_info.recv_dtg_counts = len(self.category_useful_lines[dtglog_keyword.recv_from_network])
        first_recv_dtg = self.category_useful_lines[dtglog_keyword.recv_from_network][0]
        last_recv_dtg = self.category_useful_lines[dtglog_keyword.recv_from_network][summary_info.recv_dtg_counts - 1]
        summary_info.first_recv_dtg_time = first_recv_dtg.datetime.timestamp()
        summary_info.last_recv_dtg_time = last_recv_dtg.datetime.timestamp()

        #act dtg
        len_ack_dtg = len(self.category_useful_lines[dtglog_keyword.ack_recv_dtg])
        first_ack_dtg = self.category_useful_lines[dtglog_keyword.ack_recv_dtg][0]
        last_ack_dtg = self.category_useful_lines[dtglog_keyword.ack_recv_dtg][len_ack_dtg - 1]
        summary_info.first_ack_dtg_time = first_ack_dtg.datetime.timestamp()
        summary_info.last_ack_dtg_time = last_ack_dtg.datetime.timestamp()
        
        #ack_dtg_cost
        summary_info.ack_dtg_cost = UsefulLine.datetime_diff(summary_info.first_recv_dtg_time, summary_info.last_ack_dtg_time)

        #dtg string to json
        string_tojson_begin_list = self.category_useful_lines[dtglog_keyword.dtg_string_to_json_begin]
        string_tojson_end_list = self.category_useful_lines[dtglog_keyword.dtg_string_to_json_end]
        summary_info.dtg_string_to_json_cost = UsefulLine.calculate_cost_for_multithread(string_tojson_begin_list, string_tojson_end_list)

        #savedtg to db
        savedb_begin_list = self.category_useful_lines[dtglog_keyword.save_to_db_begin]
        savedb_end_list = self.category_useful_lines[dtglog_keyword.save_to_db_end]
        summary_info.save_dtg_todb_cost = UsefulLine.calculate_cost_for_multithread(savedb_begin_list, savedb_end_list)

        #parserdtg
        parserdtg_begin_list = self.category_useful_lines[dtglog_keyword.parser_dtg_to_object_begin]
        parserdtg_end_list = self.category_useful_lines[dtglog_keyword.parser_dtg_to_object_end]
        summary_info.parser_dtgjson_cost = UsefulLine.calculate_cost(parserdtg_begin_list, parserdtg_end_list)

        #insert to manage queue
        insert_queue_end_list = self.category_useful_lines[dtglog_keyword.insert_to_mgrqueue_end]
        summary_info.insert_queue_cost = UsefulLine.calculate_cost(parserdtg_end_list, insert_queue_end_list)

        #dispatch
        dispatch_begin_list = self.category_useful_lines[dtglog_keyword.dispatch_begin]
        dispatch_end_list = self.category_useful_lines[dtglog_keyword.dispatch_end]
        summary_info.dispatch_cost = UsefulLine.calculate_cost_for_multithread(dispatch_begin_list, dispatch_end_list)

        #try dispatch
        trydispatch_begin_list = self.category_useful_lines[dtglog_keyword.trydispatch_begin]
        trydispatch_end_list = self.category_useful_lines[dtglog_keyword.trydispatch_end]
        summary_info.trydispatch_cost = UsefulLine.calculate_cost(trydispatch_begin_list, trydispatch_end_list)

        #check resdata
        checkresdata_begin_list = self.category_useful_lines[dtglog_keyword.check_resdata_begin]
        checkresdata_end_list = self.category_useful_lines[dtglog_keyword.check_resdata_end]
        summary_info.check_resdata_cost = UsefulLine.calculate_cost(checkresdata_begin_list, checkresdata_end_list)

        #splitdtg
        splitdtg_end_list = self.category_useful_lines[dtglog_keyword.split_dtg_end]
        summary_info.splitdtg_cost = UsefulLine.calculate_cost(checkresdata_end_list, splitdtg_end_list)

        #save subdtg to db, will do other thing
        savesubdtg_end_list = self.category_useful_lines[dtglog_keyword.save_subdtg_end]
        summary_info.save_subdtg_cost = UsefulLine.calculate_cost(splitdtg_end_list, savesubdtg_end_list)

        return summary_info

    def save_to_file(self, filepath):
        fileObject = open(filepath, "w")
        for line_info in self.useful_lines:
            fileObject.write(line_info.line)

        fileObject.close()

    def savedbinfo_to_file(self, filepath):
        fileObject = open(filepath, "w")
        for pair_info in self.pair_save_to_db:
            fileObject.write(pair_info.begin.line)
            fileObject.write(pair_info.end.line)

        fileObject.close()


    @staticmethod
    def parser_line(line):
        for def_keywords in DtgParser.dic_dtg_keywords.items():
            match = True
            for keyword in def_keywords[1]:
                if line.find(keyword) == -1:
                    match = False
                    break
            
            if match:
                return def_keywords

        return None


def main():
    logfolder = r"D:\case_file\fsc\FSnorunparser\FSC7222log\log\2018-03-12\FScontroller_2472_04-34\log"
    filename = "NBLog.log"
    sorted_filefiles = log4cpp_files.get_sorted_logfiles(logfolder, filename)
    #print(sorted_filefiles)

    dtg_parser = DtgParser()
    for filename in sorted_filefiles:
        dtg_parser.collect_useful_log(logfolder + "\\" + filename)

    dtg_parser.save_to_file("userful.log")
    dtg_parser.savedbinfo_to_file("savedbinfo.log")

    summary_info = dtg_parser.statistic_summaryinfo()
    summary_info.export_csv("summaryinfo.csv")

    print("succeed")

main()