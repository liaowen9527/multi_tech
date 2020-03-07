import os
import csv
from packages import log4cpp_files
from packages import log_collector



class LogParser:
    def __init__(self):
        self.finder_dict = {}

    def parse(self, usefull_line):
        return None


class LogAnalysis:
    def __init__(self):
        self.log_dir = ""
        self.logfilename = ""
        self.output_dir = ""
        self.log_parser = None
        

    def analysis(self):
        if not os.path.exists(self.output_dir):
            os.mkdir(self.output_dir)

        logs = self.collect_log(self.log_dir, self.logfilename)

        log_collector.UsefullLine.save_to_file(self.output_dir + "\\" + self.logfilename, logs)

        result = []
        for usefull_line in logs:
            obj = self.log_parser.parse(usefull_line)
            if obj is not None:
                result.append(obj)

        return result

    def collect_log(self, log_dir, filename):
        collector = log_collector.LogCollector(self.log_parser.finder_dict)
        logs = log4cpp_files.get_sorted_logfiles(log_dir, filename)    

        usefull_lines = []
        for log_file in logs:
            usefull_lines_temp = collector.collect(log_dir + "\\" + log_file)    
            usefull_lines.extend(usefull_lines_temp)

        return usefull_lines
