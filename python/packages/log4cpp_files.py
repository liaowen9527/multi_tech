import os
import datetime

#a.log a.log.1
def get_logindex(filename, real_filename):
    begin = real_filename.find(filename)
    if  begin != 0:
        return -1

    indexBegin = begin + len(filename) + 1
    if indexBegin > len(real_filename):
        return 0

    #print(real_filename[indexBegin : len(real_filename)])
    return int(real_filename[indexBegin : len(real_filename)])

def sort_logfile(filename, filename_list):
    return sorted(filename_list, key=lambda real_filename: get_logindex(filename, real_filename), reverse=True)

def get_sorted_logfiles(dir_folder, filename):
    filename_list = []
    for root, subdir, files in os.walk(dir_folder):
        #print(root)  # 当前目录路径
        #print(subdir)  # 当前路径下所有子目录
        #print(files)  # 当前路径下所有非目录子文件
        for file in files:
            if file.find(filename) != -1:
                filename_list.append(file)
        
    return sort_logfile(filename, filename_list)

def get_subdirs(dir_folder, folder_name):
    subdir_list = []
    for root, subdirs, files in os.walk(dir_folder):
        for sub_dir in subdirs:
            if sub_dir.find(folder_name) != -1:
                subdir_list.append(sub_dir)
        
    return subdir_list

def calucate_cost(timestamp_string_begin, timestamp_string_end):
    datetime_diff = string_to_datetime(timestamp_string_end).timestamp() - string_to_datetime(timestamp_string_begin).timestamp()
    datetime_diff = round(datetime_diff * 1000)
    return datetime_diff

def ms_cost_to_timestring(ms):
    m, s = divmod(ms / 1000, 60)
    h, m = divmod(m, 60)
    return ("%02d:%02d:%02d %03d" % (h, m, s, ms%1000))

def get_timestamp_string(line):
    return line[0 : len("2018-03-12 05:55:26 943")]

def string_to_datetime(timestamp_string):
    length = len(timestamp_string)
    time_string = timestamp_string[0 : length - 4]
    msec = int(timestamp_string[length - 3 : length])
    datetime_temp = datetime.datetime.strptime(time_string, "%Y-%m-%d %H:%M:%S")
    datetime_temp = datetime_temp + datetime.timedelta(microseconds = msec * 1000)
    return datetime_temp

def string_to_ms(timestamp_string):
    datetime_temp = string_to_datetime(timestamp_string)
    return round(datetime_temp.timestamp() * 1000)

def get_thread_id(line):
    line_temp = line
    count = 2
    while count > 0:
        index = line_temp.find("[")
        line_temp = line_temp[index + 1 : len(line_temp)]
        index = line_temp.find("]")
        line_temp = line_temp[index + 1 : len(line_temp)]
        count -= 1

    index_begin = line_temp.find(":")
    index_end = line_temp.find("]")
    thread_id_string = line_temp[index_begin + 1 : index_end]
    
    return int(thread_id_string)


        