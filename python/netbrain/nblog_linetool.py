

class NbLogLine:

    def __init__(self):
        self.time_string = ""
        self.log_level = ""
        self.pid = 0
        self.thread_id = 0
        self._function = ""
        self.detail = ""

def from_worker_log(line):
    ret = NbLogLine()
    line_temp = line

    #timestamp
    index_1 = 0
    index_2 = len("2018-03-12 05:55:26 943")
    self.time_string = line_temp[index_1 : index_2]

    #[DEBUG]
    line_temp = line_temp[index_2 + 1 : len(line_temp)]
    index_1 = line_temp.find("[") + 1
    index_2 = line_temp.find("]")
    self.log_level = line_temp[index_1 : index_2]

    #[hostname]
    line_temp = line_temp[index_2 + 1 : len(line_temp)]
    index_1 = line_temp.find("[") + 1
    index_2 = line_temp.find("]")

    #[pid:thread_id]
    line_temp = line_temp[index_2 + 1 : len(line_temp)]
    index_1 = line_temp.find("[") + 1
    index_3 = line_temp.find(":")
    index_2 = line_temp.find("]")
    self.pid = line_temp[index_1 : index_3]
    self.thread_id = line_temp[index_3 + 1 : index_2]

    #[log_instance]
    line_temp = line_temp[index_2 + 1 : len(line_temp)]
    index_1 = line_temp.find("[") + 1
    index_2 = line_temp.find("]")

    #[function]
    line_temp = line_temp[index_2 + 1 : len(line_temp)]
    index_1 = line_temp.find("[") + 1
    index_2 = line_temp.find("]")
    self._function = line_temp[index_1 : index_2]

    #[log_detail]
    line_temp = line_temp[index_2 + 1 : len(line_temp)]
    self.detail = line_temp

def from_fs_log(self, line):
    line_temp = line

    #timestamp
    index_1 = 0
    index_2 = len("2018-03-27 22:50:43 285")
    self.time_string = line_temp[index_1 : index_2]
    #self.time_string = "2018-" + self.time_string

    #[DEBUG]
    line_temp = line_temp[index_2 + 1 : len(line_temp)]
    index_1 = line_temp.find("[") + 1
    index_2 = line_temp.find("]")
    self.log_level = line_temp[index_1 : index_2]

    #[pid:thread_id]
    line_temp = line_temp[index_2 + 1 : len(line_temp)]
    line_temp = line_temp[line_temp.find("[") : len(line_temp)]
    index_1 = line_temp.find("[") + 1
    index_3 = line_temp.find(":")
    index_2 = line_temp.find("]")
    self.pid = line_temp[index_1 : index_3]
    self.thread_id = line_temp[index_3 + 1 : index_2]

    #[function]
    line_temp = line_temp[index_2 + 1 : len(line_temp)]
    index_1 = line_temp.find("[") + 1
    index_2 = line_temp.find("]")
    self._function = line_temp[index_1 : index_2]

    #[log_detail]
    line_temp = line_temp[index_2 + 1 : len(line_temp)]
    self.detail = line_temp


