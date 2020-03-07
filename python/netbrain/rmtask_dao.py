from packages.db_table import *
from rmagent_task import *

class RMTaskDao(NbDBTable):
    def __init__(self):
        super(RMTaskDao, self).__init__()
        self.name = 'RMTask'
        self.set_pk_field(RMTask.get_pk_field())

        t = RMTask()
        self.init_fields_by_dict(t.to_dict())

    def cost_tops(self, parent='', top_=5):
        conn = self.get_conn()
        c = conn.cursor()

        sql = ''
        if len(parent) == 0:
            sql = 'SELECT * from %s where taskid = root_taskid order by duration desc LIMIT %d' % (self.name, top_)
        else:
            sql = 'SELECT * from %s where parent_taskid = "%s" and parent_taskid != taskid order by duration desc LIMIT %d' % (self.name, parent, top_)

        #print("cost top %d, parent:%s, sql:%s" % (top_, parent, sql))
        cursor = c.execute(sql)

        ret = []
        for row_record in cursor:
            t = RMTask()
            t.self_taskid = row_record[0]
            t.worker_pid = row_record[1]
            t.task_type = row_record[2]
            t.parent_taskid = row_record[3]
            t.root_taskid = row_record[4]
            t.begin_utc_time = row_record[5]
            t.end_utc_time = row_record[6]

            ret.append(t)

        return ret

    def cost_gte(self, parent, time_, top_=5):
        conn = self.get_conn()
        c = conn.cursor()

        sql = 'SELECT * from %s where parent_taskid = "%s" and parent_taskid != taskid and duration > %d order by duration desc LIMIT %d' \
            % (self.name, parent, time_, top_)

        #print("cost gte %d top %d, parent:%s, sql:%s" % (time_, top_, parent, sql))
        cursor = c.execute(sql)

        ret = []
        for row_record in cursor:
            t = RMTask()
            t.self_taskid = row_record[0]
            t.worker_pid = row_record[1]
            t.task_type = row_record[2]
            t.parent_taskid = row_record[3]
            t.root_taskid = row_record[4]
            t.begin_utc_time = row_record[5]
            t.end_utc_time = row_record[6]

            ret.append(t)

        return ret


    def has_subtask(self, taskid):
        conn = self.get_conn()
        c = conn.cursor()

        sql = 'SELECT taskid from %s where parent_taskid = "%s" and taskid != parent_taskid LIMIT 1' % (self.name, taskid)

        cursor = c.execute(sql)

        for row_record in cursor:
            return True

        return False


        