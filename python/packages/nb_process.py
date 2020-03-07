import os
import datetime
import threading

_enable = True
_utc = False


def get_process_id():
    return os.getpid()

def get_thread_id():
    return threading.current_thread().ident

def nbprint(*args, **kwargs):
    if not _enable:
        return

    now = None
    if _utc:
        now = datetime.datetime.utcnow()
    else:
        now = datetime.datetime.now()

    str_date = now.strftime('%Y-%m-%d %H:%M:%S')
    msec = int(now.microsecond / 1000)
    pid = get_process_id()
    tid = get_thread_id()
    #func_name = sys._getframe().f_code.co_name
    log = str(*args, **kwargs)

    print("%s %s [%d:%d] %s" % (str_date, str(msec).zfill(3), pid, tid, log))



if __name__ == "__main__":
    nbprint("test:%d, name:%s" % (11, "liaowen"))    
