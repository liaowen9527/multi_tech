import os
import sqlite3
import log4cpp_files
import csv

def delete_duplicate_data(db_filepath, table_name, field):
    conn = sqlite3.connect(db_filepath)
    conn_cursor = conn.cursor()

    sql_string = '''select TASK_ID, count(TASK_ID) as sum_count from %s
        group by TASK_ID having  count(TASK_ID) > 1;'''%(table_name)

    print(sql_string)
    cursor = conn_cursor.execute(sql_string)

    arr_delete = []
    for row in cursor:
        sql_string = '''delete from %s where rowid in 
            (select rowid from %s where TASK_ID == \"%s\" limit 0, %d);
            '''%(table_name, table_name, row[0], int(row[1]) - 1)
        arr_delete.append(sql_string)
    
    print("delete table:%s, count:%d"%(table_name, len(arr_delete)))
    done_count = 0
    for del_string in arr_delete:
        #print(sql_string)
        conn_cursor.execute(del_string)
        done_count += 1
        if done_count % 100 == 0:
            conn.commit()
            print("left count:%d"%(len(arr_delete) - done_count))
            conn_cursor = conn.cursor()
            cursor = conn_cursor.execute(sql_string)

    conn.commit()
    conn.close()

def collect_valid_worker(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS discover_worker_valid
            (PID                    INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(150),
            TASK_ID                 CHAR(50),
            PARENT_TASK             CHAR(50),
            ROOT_TASK               CHAR(150),
            LOG                     TEXT);''')

    sql_string = "delete from discover_worker_valid;"
    print(sql_string)
    cursor = c.execute(sql_string)

    sql_string = '''insert into discover_worker_valid select * from discover_worker where category == "discover";'''
    print(sql_string)
    c.execute(sql_string)
    
    sql_string = '''insert into discover_worker_valid select * from
        (select c.* from
		(select a.* from discover_worker a inner join discover_worker_valid b on a.PARENT_TASK == b.TASK_ID) c 
		left join discover_worker_valid d on  c.TASK_ID == d.TASK_ID where d.TASK_ID is null);'''

    count = 1
    while True:
        print(sql_string)
        c.execute(sql_string)

        sql_qry_count = "select count(PID) from discover_worker_valid"
        cursor = c.execute(sql_qry_count)
        need_insert_more = True
        for row in cursor:
            print("cur_row=%d, pre_count=%d"%(row[0], count))
            if row[0] == count:
                need_insert_more = False
                break
            
            count = row[0]

        if not need_insert_more:
            break

    conn.commit()
    conn.close()

def collect_valid_direct_live(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS direct_live_task_valid
            (TASK_ID                CHAR(50) NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(150),
            TYPE                    CHAR(150),
            LOG                     TEXT);''')

    sql_string = "delete from direct_live_task_valid;"
    print(sql_string)
    c.execute(sql_string)

    sql_string = '''insert into direct_live_task_valid select * from 
        (select a.* from direct_live_task a inner join discover_worker_valid b on a.PID == b.PID);'''

    print(sql_string)        
    c.execute(sql_string)

    conn.commit()
    conn.close()

def collect_last_recv_data(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS direct_live_last_recv_data
            (TASK_ID                CHAR(50) NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(150),
            TYPE                    CHAR(150),
            LOG                     TEXT);''')
    c.execute("delete from direct_live_last_recv_data;")    

    sql_string = '''insert into direct_live_last_recv_data 
        select a.* from direct_live_task_valid a 
        where CATEGORY != "send_direct_req" group by TASK_ID, PID, THREAD_ID 
        having a.TIMESTAMP_NUM = max(TIMESTAMP_NUM);'''

    print(sql_string)
    c.execute(sql_string)

    conn.commit()
    conn.close()


def collect_valid_fsc_log(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS fsc_direct_live_task_valid
            (TASK_ID                CHAR(50)    NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(150),
            MSG_TYPE                CHAR(150),
            LOG                     TEXT);''')

    sql_string = "delete from fsc_direct_live_task_valid;"
    print(sql_string)
    c.execute(sql_string)

    sql_string = '''insert into fsc_direct_live_task_valid select * from 
        (select a.* from fsc_direct_live_task a inner join 
        (select distinct TASK_ID from direct_live_task_valid) b on a.TASK_ID == b.TASK_ID);'''

    print(sql_string)
    c.execute(sql_string)

    conn.commit()
    conn.close()

def collect_last_upload_toworker(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS last_upload_fsc_to_worker
            (TASK_ID                CHAR(50)    NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(150),
            MSG_TYPE                CHAR(150),
            LOG                     TEXT);''')

    c.execute("delete from last_upload_fsc_to_worker;")

    sql_string = '''insert into last_upload_fsc_to_worker 
        select a.* from fsc_direct_live_task a 
        where MSG_TYPE != "4" and MSG_TYPE != "9" and MSG_TYPE != "12" and CATEGORY == "recv_data" group by TASK_ID 
        having a.TIMESTAMP_NUM = max(TIMESTAMP_NUM);'''

    print(sql_string)
    c.execute(sql_string)

    conn.commit()
    conn.close()

def collect_valid_fs_log(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS fs_direct_live_task_valid
            (TASK_ID                CHAR(50)    NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(50),
            MSG_TYPE                CHAR(150),
            LOG                     TEXT);''')

    sql_string = "delete from fs_direct_live_task_valid;"
    print(sql_string)
    c.execute(sql_string)

    sql_string = '''insert into fs_direct_live_task_valid select * from 
        (select a.* from fs_direct_live_task a inner join 
        (select distinct TASK_ID from direct_live_task_valid) b on a.TASK_ID == b.TASK_ID);'''

    print(sql_string)
    c.execute(sql_string)

    conn.commit()
    conn.close()

def collect_last_upload_tofsc(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS last_upload_fs_to_fsc
            (TASK_ID                CHAR(50)    NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(150),
            MSG_TYPE                CHAR(150),
            LOG                     TEXT);''')

    c.execute("delete from last_upload_fs_to_fsc;")

    sql_string = '''insert into last_upload_fs_to_fsc 
        select a.* from fs_direct_live_task a 
        where CATEGORY == "upload_data" and MSG_TYPE != "4" and MSG_TYPE != "9" and MSG_TYPE != "12" group by TASK_ID 
        having a.TIMESTAMP_NUM = max(TIMESTAMP_NUM);'''

    print(sql_string)
    c.execute(sql_string) 

    conn.commit()
    conn.close()

def collect_live_begin(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS fs_live_begin
            (TASK_ID                CHAR(50)    NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(50),
            MSG_TYPE                CHAR(150),
            LOG                     TEXT);''')

    c.execute("delete from fs_live_begin;")

    sql_string = '''insert into fs_live_begin 	
        select a.* from fs_direct_live_task a 
        where CATEGORY = "onbegin" group by TASK_ID 
        having a.TIMESTAMP_NUM = min(TIMESTAMP_NUM);'''

    print(sql_string)
    c.execute(sql_string)

    conn.commit()
    conn.close()

def collect_live_end(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS fs_live_end
            (TASK_ID                CHAR(50)    NOT NULL,
            PID                     INT    NOT NULL,
            THREAD_ID               INT    NOT NULL,
            TIMESTAMP_NUM           INT    NOT NULL,
            TIMESTAMP_STRING        CHAR(50),
            CATEGORY                CHAR(50),
            MSG_TYPE                CHAR(150),
            LOG                     TEXT);''')

    c.execute("delete from fs_live_end;")

    sql_string = '''insert into fs_live_end 	
        select a.* from fs_direct_live_task a 
        where CATEGORY = "onend" group by TASK_ID 
        having a.TIMESTAMP_NUM = max(TIMESTAMP_NUM);'''

    print(sql_string)
    c.execute(sql_string)

    conn.commit()
    conn.close()    

def collect_live_duration(db_filepath):
    collect_live_begin(db_filepath)
    collect_live_end(db_filepath)

    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS fs_live_duration
            (TASK_ID                CHAR(50)    NOT NULL,
            PID                     INT,
            THREAD_ID               INT,
            TM_BEGIN                CHAR(50),
            TM_BEGIN_NUM            INT,
            TM_END                  CHAR(50),
            TM_END_NUM              INT,
            COST                    INT,
			TM_BEGIN_LOG			TEXT,
			TM_END_LOG				TEXT);''')

    c.execute("delete from fs_live_duration;")

    sql_string = '''insert into fs_live_duration 	
        select a.TASK_ID, a.PID, a.THREAD_ID, a.TIMESTAMP_STRING, a.TIMESTAMP_NUM, 
        b.TIMESTAMP_STRING, b.TIMESTAMP_NUM, b.TIMESTAMP_NUM - a.TIMESTAMP_NUM,
        a.LOG, b.LOG from fs_live_begin a inner join fs_live_end b on a.TASK_ID == b.TASK_ID;'''

    print(sql_string)
    c.execute(sql_string)

    conn.commit()
    conn.close()

def collect_valid_data(db_filepath, is_collect_worker, is_collect_fsc, is_collect_fs):
    if is_collect_worker:
        collect_valid_worker(db_filepath)
        collect_valid_direct_live(db_filepath)
        collect_last_recv_data(db_filepath)

    if is_collect_fsc:
        #collect_valid_fsc_log(db_filepath)
        collect_last_upload_toworker(db_filepath)

    if is_collect_fs:
        #collect_valid_fs_log(db_filepath)
        collect_live_duration(db_filepath)
        collect_last_upload_tofsc(db_filepath)

def collect_direct_live_task_cost(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS direct_live_cost
            (TASK_ID                CHAR(50),
            PID_WORKER              INT    NOT NULL,
            THREAD_ID_WORKER        INT    NOT NULL,
            TM_SEND_WORKER          CHAR(50),
            TM_LAST_RECV_WORKER     CHAR(50),
            TM_RECV_FSC             CHAR(50),
            TM_RECV_FS              CHAR(50),
            TM_LAST_UPLOAD_FS       CHAR(50),
            TM_LAST_UPLOAD_FSC      CHAR(50),
            TM_FIRST_LIVE_BEGIN     CHAR(50),
            TM_LAST_LIVE_END        CHAR(50),
            COST_TOTAL              INT,
            COST_TOTAL_STRING       CHAR(50),
            COST_FSC_FS             INT,
            COST_FSC_FS_STRING      CHAR(50),
            COST_SEND               INT,
            COST_SEND_STRING        CHAR(50),
            COST_UPLOAD             INT,
            COST_UPLOAD_STRING      CHAR(50),
            COST_DISPATCH           INT,
            COST_DISPATCH_STRING    CHAR(50),
            COST_LIVE               INT,
            COST_LIVE_STRING        CHAR(50));''')

    c.execute("delete from direct_live_cost;")

    sql_string = '''insert into direct_live_cost 
        select a.TASK_ID, a.PID, a.THREAD_ID, a.TIMESTAMP_STRING as TM_SEND_WORKER, 
        direct_live_last_recv_data.TIMESTAMP_STRING as TM_LAST_RECV_WORKER, "TM_RECV_FSC",
        fs_direct_live_task.TIMESTAMP_STRING as TM_RECV_FS, last_upload_fs_to_fsc.TIMESTAMP_STRING as TM_LAST_UPLOAD_FS, 
        last_upload_fsc_to_worker.TIMESTAMP_STRING as TM_LAST_UPLOAD_FSC,
        fs_live_duration.TM_BEGIN as TM_FIRST_LIVE_BEGIN, fs_live_duration.TM_END as TM_LAST_LIVE_END,
        direct_live_last_recv_data.TIMESTAMP_NUM - a.TIMESTAMP_NUM as COST_TOTAL, "",
        0, "",
        fs_direct_live_task.TIMESTAMP_NUM - a.TIMESTAMP_NUM, "",
        direct_live_last_recv_data.TIMESTAMP_NUM - last_upload_fs_to_fsc.TIMESTAMP_NUM, "",
        fs_live_duration.TM_BEGIN_NUM - fs_direct_live_task.TIMESTAMP_NUM, "",
        fs_live_duration.COST, "" 
        from direct_live_task_valid a  
        inner join direct_live_last_recv_data 
        on a.TASK_ID == direct_live_last_recv_data.TASK_ID
        inner join fs_direct_live_task
        on a.TASK_ID == fs_direct_live_task.TASK_ID 
        inner join last_upload_fs_to_fsc
        on a.TASK_ID == last_upload_fs_to_fsc.TASK_ID 
        inner join last_upload_fsc_to_worker
        on a.TASK_ID == last_upload_fsc_to_worker.TASK_ID 
        inner join fs_live_duration
        on a.TASK_ID == fs_live_duration.TASK_ID 
        where a.CATEGORY == "send_direct_req" and fs_direct_live_task.CATEGORY == "recv_direct_req";'''

    print(sql_string)
    c.execute(sql_string)

    sql_string = '''update direct_live_cost set COST_FSC_FS = COST_TOTAL - COST_LIVE;'''
    print(sql_string)
    c.execute(sql_string)
    
    conn.commit()
    conn.close()

def delete_all_duplicate_data(db_filepath):
    delete_duplicate_data(db_filepath, "direct_live_last_recv_data", "TASK_ID")
    delete_duplicate_data(db_filepath, "last_upload_fsc_to_worker", "TASK_ID")
    delete_duplicate_data(db_filepath, "last_upload_fs_to_fsc", "TASK_ID") 

def collect_fs_direct_live_task_cost(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS fs_direct_live_cost
            (TASK_ID                CHAR(50),
            TM_RECV_FS              CHAR(50),
            TM_LAST_UPLOAD_FS       CHAR(50),
            TM_FIRST_LIVE_BEGIN     CHAR(50),
            TM_LAST_LIVE_END        CHAR(50),
            COST_DISPATCH           INT,
            COST_LIVE               INT);''')

    sql_string = "delete from fs_direct_live_cost;"
    print(sql_string)
    c.execute(sql_string)

    sql_string = '''insert into fs_direct_live_cost
        select a.TASK_ID, a.TIMESTAMP_STRING as TM_RECV_FS, 
		last_upload_fs_to_fsc.TIMESTAMP_STRING, 
        fs_live_duration.TM_BEGIN, fs_live_duration.TM_END, 
        fs_live_duration.TM_BEGIN_NUM - a.TIMESTAMP_NUM,
        fs_live_duration.COST
        from fs_direct_live_task a
        left join last_upload_fs_to_fsc
        on a.TASK_ID == last_upload_fs_to_fsc.TASK_ID 
        inner join fs_live_duration
        on a.TASK_ID == fs_live_duration.TASK_ID 
        where a.CATEGORY == "recv_direct_req";	'''

    print(sql_string)
    c.execute(sql_string)
    
    conn.commit()
    conn.close()

def collect_fsc_fs_direct_live_task_cost(db_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS fsc_fs_direct_live_cost
            (TASK_ID                CHAR(50),
            TM_RECV_FSC             CHAR(50),
            TM_RECV_FS              CHAR(50),
            TM_LAST_UPLOAD_FS       CHAR(50),
            TM_LAST_UPLOAD_FSC      CHAR(50),
            TM_FIRST_LIVE_BEGIN     CHAR(50),
            TM_LAST_LIVE_END        CHAR(50),
            COST_SEND               INT,
            COST_UPLOAD             INT,
            COST_DISPATCH           INT,
            COST_LIVE               INT);''')

    sql_string = "delete from fsc_fs_direct_live_cost;"
    print(sql_string)
    c.execute(sql_string)

    sql_string = '''insert into fsc_fs_direct_live_cost
        select a.TASK_ID, "TM_RECV_FSC", a.TIMESTAMP_STRING as TM_RECV_FS, 
		last_upload_fs_to_fsc.TIMESTAMP_STRING, last_upload_fsc_to_worker.TIMESTAMP_STRING,
        fs_live_duration.TM_BEGIN, fs_live_duration.TM_END, 0, 0,
        fs_live_duration.TM_BEGIN_NUM - a.TIMESTAMP_NUM,
        fs_live_duration.COST
        from fs_direct_live_task a
        inner join last_upload_fs_to_fsc
        on a.TASK_ID == last_upload_fs_to_fsc.TASK_ID 
        inner join last_upload_fsc_to_worker
        on a.TASK_ID == last_upload_fsc_to_worker.TASK_ID 
        inner join fs_live_duration
        on a.TASK_ID == fs_live_duration.TASK_ID 
        where a.CATEGORY == "recv_direct_req";	'''

    print(sql_string)
    c.execute(sql_string)
    
    conn.commit()
    conn.close()

def export_summary_csv(db_filepath, csv_filepath):
    conn = sqlite3.connect(db_filepath)
    c = conn.cursor()

    sql_string = '''select PID_WORKER, THREAD_ID_WORKER, sum(COST_TOTAL), sum(COST_FSC_FS) as sum_cost_fsc_fs, 
        sum(COST_SEND), sum(COST_UPLOAD), sum(COST_DISPATCH), sum(COST_LIVE) from direct_live_cost 
        group by PID_WORKER, THREAD_ID_WORKER;'''
 
    print(sql_string)
    cursor = c.execute(sql_string)

    rows = []
    rows.append([
            "PID_WORKER", 
            "THREAD_ID_WORKER",
            "COST_TOTAL_STRING", 
            "COST_FSC_FS_STRING", 
            "COST_SEND_STRING", 
            "COST_UPLOAD_STRING", 
            "COST_DISPATCH_STRING", 
            "COST_LIVE_STRING",
            "COST_TOTAL", 
            "COST_FSC_FS", 
            "COST_SEND", 
            "COST_UPLOAD", 
            "COST_DISPATCH", 
            "COST_LIVE"
        ])

    for row_record in cursor:
        rows.append([
            row_record[0], 
            row_record[1], 
            log4cpp_files.ms_cost_to_timestring(int(row_record[2])), 
            log4cpp_files.ms_cost_to_timestring(int(row_record[3])), 
            log4cpp_files.ms_cost_to_timestring(int(row_record[4])), 
            log4cpp_files.ms_cost_to_timestring(int(row_record[5])), 
            log4cpp_files.ms_cost_to_timestring(int(row_record[6])), 
            log4cpp_files.ms_cost_to_timestring(int(row_record[7])), 
            row_record[2], 
            row_record[3],
            row_record[4], 
            row_record[5], 
            row_record[6], 
            row_record[7]
        ])

    conn.close()

    csvfile = open(csv_filepath, 'w', newline='')
    writer = csv.writer(csvfile)   
    writer.writerows(rows)

    csvfile.close()

def main():
    analysis_folder = "dierct_live_analysis_log"
    db_filename = "discover_log.db"
    db_filepath = analysis_folder + "\\" + db_filename
    csv_filepath = analysis_folder + "\\discover_summary.csv"
    if not os.path.exists(db_filepath):
        print("without the db_file:%s."%(db_filepath))
        return  
    
    is_collect_worker = False
    is_collect_fsc = False
    is_collect_fs = True
    collect_valid_data(db_filepath, is_collect_worker, is_collect_fsc, is_collect_fs)
    if is_collect_worker and is_collect_fsc and is_collect_fs:
        collect_direct_live_task_cost(db_filepath)
    elif is_collect_fsc and is_collect_fs:
        collect_fsc_fs_direct_live_task_cost(db_filepath)
    elif is_collect_fs:
        collect_fs_direct_live_task_cost(db_filepath)

    #export_summary_csv(db_filepath, csv_filepath)
    
    print("succeed.")

main()

    
