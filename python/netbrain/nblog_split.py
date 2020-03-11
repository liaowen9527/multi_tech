
import sys
import os
sys.path.append(os.path.abspath(os.path.dirname(__file__) + '/..'))

from packages.log_split import *
from packages import log4cpp_files
from nblog_linetool import *




def _default(line):
    #2020-02-28 22:11:27 002 [INFO] [33724:40560]
    info = NbLogLine()
    try:
        info.from_fs_log(line)
    except Exception as e:
        return None

    if len(info.pid) ==0 or len(info.thread_id) == 0 or len(info.time_string) == 0:
        return None

    key = "%s_%s" % (info.pid, info.thread_id)

    return LogCategory(key, filename=key)


def split_files(log_folder, filename, default=_default):
    split = LogSplit()
    split.output_dir = output_dir
    split.func_find_keyword = default
    
    files = log4cpp_files.get_sorted_logfiles(log_folder, filename)
    for f in files:
        filepath = os.path.join(log_folder, f)
        split.split(filepath)


if __name__ == "__main__":
    folder = r'C:\Code\git_Code\ng\Backend\log\fsc'
    output_dir = os.path.join(folder, 'split')
    split_files(folder, output_dir, 'fsc.log')