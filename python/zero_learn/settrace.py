# Python program using 
# traces to kill threads 

import sys 
import trace 
import traceback
import threading 
import time 
class thread_with_trace(threading.Thread): 
    def __init__(self, *args, **keywords): 
        threading.Thread.__init__(self, *args, **keywords) 
        self.killed = False

    def start(self): 
        self.__run_backup = self.run 
        self.run = self.__run	 
        threading.Thread.start(self) 

    def __run(self): 
        sys.settrace(self.globaltrace) 
        self.__run_backup() 
        self.run = self.__run_backup 

    def globaltrace(self, frame, event, arg): 
        tid = threading.current_thread().ident
        print('globaltrace, thread ', tid) 
        print(frame, event, arg) 

        if event == 'call': 
            return self.localtrace 
        else: 
            return None

    def localtrace(self, frame, event, arg): 
        if self.killed: 
            if event == 'line': 
                tid = threading.current_thread().ident
                print('localtrace, thread ', tid) 
                print(frame, event, arg) 
                raise SystemExit() 
        return self.localtrace 

    def kill(self): 
        self.killed = True

def func(): 
    tid = threading.current_thread().ident
    print('thread running ', tid) 
    try:
        while True: 
            print('thread running 1 ', tid) 
            time.sleep(1)
            print('thread running 2 ', tid) 
    except:
        print(traceback.format_exc())

    if sys.gettrace() is None:
        print('no trace')

    print('thread end ', tid) 
    

if __name__ == "__main__11":
    tid = threading.current_thread().ident
    print('main thread ', tid)

    t1 = thread_with_trace(target = func) 
    t1.start() 
    time.sleep(4) 
    t1.kill() 
    t1.join() 
    if not t1.isAlive(): 
        print('thread killed') 

        

def test_child_thread():
    print('start, ooo')
    try:
        while True:
            time.sleep(0.1)
            #print('sleep, ooo')
    except:
        print('exception, ooo')
        
    print('finish, end, ooo')

def test_main_thread():
    print('start, main')
    t = threading.Thread(target=test_child_thread)
    t.setDaemon(True)
    t.start()
        
    time.sleep(3)
    _async_raise(t.ident, Exception)
    time.sleep(2)
    print('finish, main')

import inspect
import ctypes

def _async_raise(tid, exctype):
    '''Raises an exception in the threads with id tid'''
    if not inspect.isclass(exctype):
        raise TypeError("Only types can be raised (not instances)")
    res = ctypes.pythonapi.PyThreadState_SetAsyncExc(ctypes.c_long(tid),
                                                     ctypes.py_object(exctype))
    if res == 0:
        raise ValueError("invalid thread id")
    elif res != 1:
        # "if it returns a number greater than one, you're in trouble,
        # and you should call it again with exc=NULL to revert the effect"
        ctypes.pythonapi.PyThreadState_SetAsyncExc(ctypes.c_long(tid), None)
        raise SystemError("PyThreadState_SetAsyncExc failed")
 
if __name__ == "__main__":
    t = threading.Thread(target=test_main_thread)
    #t.setDaemon(True)
    t.start()
    t.join()
    t = None
    
    time.sleep(3)
    print('end, aaa')

