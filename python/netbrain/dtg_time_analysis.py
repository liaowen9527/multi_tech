from pymongo import MongoClient
import csv
import datetime
import time

class DtgInfo:

    def __init__(self, dtgid):
        self.id = dtgid
        self.renewtime = 0
        self.finishtime = datetime.datetime.fromtimestamp(0)
        self.dispatchlist = []

    def load_dispinfo_fromdb(self, db_tenant):
        dispdoc_list = db_tenant.FSC_DataTaskGroup_DispatchInfo.find({'datataskgroupid': self.id})
        for disp_doc in dispdoc_list:
            disp_info = DtgDispatchInfo(disp_doc.get('_id'), self.id)
            #batch=local timestamp + 1h
            disp_info.batch = disp_doc.get('batch')
            if disp_doc.get('status'):
                disp_info.dispatch_time = disp_doc.get('status').get('fs_dispatch').get('time')
            else:
                disp_info.dispatch_time = datetime.datetime.fromtimestamp(disp_info.batch + 5 * 60 - 9 * 60 * 60)
            self.dispatchlist.append(disp_info)

    def get_begin_time(self):
        if len(self.dispatchlist) == 0:
            print("without dispatch info. dtg_id:%s" % (self.id))
            return datetime.datetime.fromtimestamp(0)

        temp_list = self.dispatchlist
        sorted(temp_list, key=lambda info: info.batch)

        return datetime.datetime.fromtimestamp(temp_list[0].batch - 9 * 60 * 60)

    def get_finish_dispatch_time(self):
        begin_time = self.get_begin_time()
        timestamp = (int)(time.mktime(begin_time.timetuple()))
        first_dispatch_list = self.filterby_batch(timestamp + 9 * 60 * 60)
        if len(first_dispatch_list) == 0:
            print("without dispatch info. dtg_id:%s, batch:%s" % (self.id, begin_time.strftime("%Y-%m-%d %H:%M:%S")))
            return begin_time

        sorted(first_dispatch_list, key=lambda info: info.dispatch_time)

        return first_dispatch_list[-1].dispatch_time

    def get_finish_time(self):
        return self.finishtime

    def get_total_cost(self):
        return self.finishtime - self.get_begin_time()

    def get_dispatch_cost(self):
        return self.get_finish_dispatch_time() - self.get_begin_time()

    def get_retrive_data_cost(self):
        return self.finishtime - self.get_finish_dispatch_time()

    def filterby_batch(self, batch):
        retlist = []
        for dispatch_info in self.dispatchlist:
            if dispatch_info.batch == batch:
                retlist.append(dispatch_info)

        return retlist


class DtgDispatchInfo:

    def __init__(self, subdtg_id, dtgid):
        self.subDtgId = subdtg_id
        self.dtgId = dtgid
        self.batch = 0
        self.dispatch_time = datetime.datetime.fromtimestamp(0)


def load_data(uri, tenantname):
    connetion = MongoClient(uri)
    db_tenant = connetion.get_database(tenantname)
    dtg_doc_list = db_tenant.FSC_DataTaskGroup.find({'status': {'$exists': 1}})

    dtg_list = []
    for dtg_doc in dtg_doc_list:
        dtg_info = DtgInfo(dtg_doc.get('_id'))
        dtg_info.renewtime = dtg_doc.get('renewtime')
        if dtg_doc.get('status'):
            dtg_info.finishtime = dtg_doc.get('status').get('finish').get('time')

        dtg_info.load_dispinfo_fromdb(db_tenant)

        dtg_list.append(dtg_info)

    return  dtg_list


def export_csv(dtg_list, strFilePath):
    print("Start Save CSV!")
    csvfile = open(strFilePath, 'w', newline='')
    writer = csv.writer(csvfile)

    rows = []
    rows.append(["data_task_group_id", "total_cost", "save_to_db", "rspFromFs_dispatch",
                 "dispatch_cost", "retrive_data_cost"])

    for dtg_info in dtg_list:
            rows.append([dtg_info.id, dtg_info.get_total_cost(), dtg_info.get_begin_time(),
                         dtg_info.get_finish_dispatch_time(), dtg_info.get_dispatch_cost(), dtg_info.get_retrive_data_cost()])

    writer.writerows(rows)

    csvfile.close()
    print("Save CSV Success!")

def main():
    uri = "mongodb://10.10.0.30:27017/admin?authMechanism=SCRAM-SHA-1"
    tenantname = 'FSC_Tenant'
    csv_path = "fsc_dtg_time_analysis.csv"
    dtg_list = load_data(uri, tenantname)
    export_csv(dtg_list, csv_path);

main()