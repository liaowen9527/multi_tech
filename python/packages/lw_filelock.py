try:
    import fcntl
except :
    fcntl = None
    import NBPythonUtil as pythonlib
    
    

class FileMutex(object):
    def __init__(self, filepath):
        if fcntl is None:
            self.f = pythonlib.NBPyFilelock(filepath)
        else:
            self.f = open(filepath)

    def __del__(self):
        self.unlock()

    def lock(self, shared=False):
        if not self.f:
            return

        if fcntl is None:
            self.f.lock() 
        else:
            fcntl.flock(self.f, fcntl.LOCK_EX)
                

    def lock_sharable(self):
        if fcntl is None:
            self.f.lock_sharable()
        else:
            fcntl.flock(self.f, fcntl.LOCK_SH)

    def unlock(self):
        if not self.f:
            return

        if fcntl:
            fcntl.flock(self.f, fcntl.LOCK_UN)
            
        self.f = None


class Lock(object):
    def __init__(self, filepath):
        self.mutex = FileMutex(filepath)

    def __enter__(self):
        self.mutex.lock()

    def __exit__(self, exc_type, exc_value, traceback):
        self.mutex.unlock()

class SharedLock(object):
    def __init__(self, filepath):
        self.mutex = FileMutex(filepath)

    def __enter__(self):
        self.mutex.lock_sharable()

    def __exit__(self, exc_type, exc_value, traceback):
        self.mutex.unlock()


if __name__ == "__main__":
    filepath = R'E:\git_code\ng\CrossPlatform\package\data\lock\devicesetting.db.lock'

    import time
    import threading
    def test_r(filepath):
        with SharedLock(filepath):
            print('test r')
            time.sleep(3)

    def test_w(filepath):
        with Lock(filepath):
            print('test w')
            time.sleep(4)
            pass
        
    t1 = threading.Thread(target=test_r, args=[filepath,])
    t2 = threading.Thread(target=test_w, args=[filepath,])
    t1.run()
    t2.run()

    with SharedLock(filepath):
        print('test 1')

        t3 = threading.Thread(target=test_r, args=[filepath,])
        t3.run()
        time.sleep(3)
    
    
    t1.join()
    t2.join()