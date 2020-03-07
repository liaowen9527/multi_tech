import requests
import threading
import traceback
import datetime
import time
from packages import nb_process

class NBRequest:
    def __init__(self, endpoint, url, header, body):
        self.endpoint = endpoint
        self.url = url
        self.header = header
        self.body = body
        self.proxies = {}
        self.timeout = 600

        self.can_merge = True

        self.regist_time = None
        self.begin_time = None
        self.end_time = None

    def equal(self, right):
        return self.endpoint == right.endpoint \
            and self.url == right.url \
            and self.header == right.header \
            and self.body == right.body \
            and self.proxies == right.proxies 

class WaitStatus:
    def __init__(self):
        self.dup = None
        self.not_found = False
        self.wait_again = False

class NbMergeItem:
        def __init__(self):
            self.items = []
            self.resp = None

class NbMergeRequst:
    def __init__(self):
        self.category = {}

    def regist(self, req):
        if None == self.category.get(req):
            self.category[req] = NbMergeItem()

    def add_item(self, key, req):
        item = self.get_item(key)
        if None == item:
            return False
        
        item.items.append(req)
        return True

    def remove_item(self, key, req):
        item = self.get_item(key)
        if None == item:
            return False
        
        if key != req:
            item.items.remove(req)

        if len(item.items) == 0:
            self.category.pop(key)

        return True

    def get_item(self, key):
        return self.category.get(key)

    def get_resp(self, key):
        item = self.get_item(key)
        if None == item:
            return None

        return item.resp

    def set_resp(self, key, resp):
        item = self.get_item(key)
        if None == item:
            return False

        item.resp = resp
        return True

class NbResponse:
    def __init__(self):
        self.req = None
        self.req_real = None
        self.resp = None


class NbRequestMgr:
    def __init__(self):
        self.lock = threading.Lock()
        self.req_buffer = []
        self.req_running = {}
        self.req_merge = NbMergeRequst()

        self.utc_time = True
        self.max_request = 10
        self.req_index = {}

    def post(self, req):
        result = self.post_once(req)

        if req.can_merge and result.resp == None and result.req != result.req_real:
            nb_process.nbprint("post again req: %d" % (req.index))
            req.can_merge = False
            result = self.post_once(req)

        return result.resp
    
    def post_once(self, req):
        self.regist(req)
        #nb_process.nbprint("begin req: %d" % (req.index))

        result = self.do_post(req)

        #nb_process.nbprint("end req: %d" % (req.index))
        self.unregist(result.req, result.req_real, result.resp)
        return result

    def do_post(self, req):
        result = NbResponse()
        result.req = req
        result.req_real = req

        status = self.wait_access(req)
        if status.dup:
            result.req_real = status.dup
            result.resp = self.wait_response(status.dup)
            if result.resp is not None:
                nb_process.nbprint("get resp by merge request: %s, index: %d, merge_index:%d" 
                    % (req.endpoint, req.index, status.dup.index))
                
        else:
            result.resp = self.http_post(req)
            
        return result

    def http_post(self, req):
        nb_process.nbprint("execute endpoint: %s, index: %d" % (req.endpoint, req.index))
        resp = None
        try:
            req.begin_time = self.get_current_time()
            resp = requests.post(req.url,
                headers=req.header,
                data=req.body,
                proxies=req.proxies,
                verify=False,
                timeout=req.timeout)

        except Exception as e:
            print(e)
            print(traceback.format_exc())
            print(req.url)

        return resp

    def wait_response(self, req):
        while True:
            with self.lock:
                if None == self.req_running.get(req.endpoint):
                    return None

                arr = self.req_running.get(req.endpoint)
                if req not in arr:
                    return self.req_merge.get_resp(req)

            time.sleep(0.1)
            
        return None

    def cancel(self, req):
        pass

    def regist(self, req):
        with self.lock:
            #category request by endpoint
            if None == self.req_index.get(req.endpoint):
                self.req_index[req.endpoint] = 1
            req.index = self.req_index[req.endpoint]
            self.req_index[req.endpoint] = req.index + 1

            req.regist_time = self.get_current_time()
            self.req_buffer.append(req)

    def unregist(self, req, real_req, resp):
        req.end_time = self.get_current_time()
        with self.lock:
            if req in self.req_buffer:
                self.req_buffer.remove(req)
            else:
                #remove from running request
                arr = self.req_running.get(req.endpoint)
                if arr and req in arr:
                    arr.remove(req)
                    #record result, for merge items
                    self.req_merge.set_resp(req, resp)
                    nb_process.nbprint("unregist endpoint: %s, index: %d" % (req.endpoint, req.index))
            
            self.req_merge.remove_item(real_req, req)

    def wait_access_once(self, req):
        status = WaitStatus()
        with self.lock:
            if req not in self.req_buffer:
                status.not_found = True
                return status

            if None == self.req_running.get(req.endpoint):
                self.req_running[req.endpoint] = []
            
            arr = self.req_running.get(req.endpoint)

            #check if has same request is running
            same_req = None
            if req.can_merge:
                for temp in arr:
                    if req.equal(temp):
                        same_req = temp
                        break
            #
            if same_req:
                #the same_req is running now
                self.req_merge.add_item(same_req, req)
                status.dup = same_req
                #move from buffer queue, avoid block other request
                self.req_buffer.remove(req)
                return status
            else:
                if len(arr) < self.max_request:
                    #must at the front of queue
                    first = self.req_buffer[0]
                    if first == req:
                        #move to running list
                        self.req_buffer.remove(req)
                        arr.append(req)
                        #regist a merge category
                        self.req_merge.regist(req)

                        return status
        
        status.wait_again = True
        return status


    def wait_access(self, req):
        while True:
            status = self.wait_access_once(req)
            if not status.wait_again:
                return status

            time.sleep(0.1)


    def get_current_time(self):
        if self.utc_time:
            return datetime.datetime.utcnow().isoformat()

        return datetime.datetime.now().isoformat()


def test_post(mgr):
    i = int(int(threading.current_thread().ident) / 7)
    while True:
        i = i + 1
        endpoint = "127.0.0.1" if int(i % 2 == 1) else "10.99.98.53"
        req = NBRequest(endpoint=endpoint, url="http://localhost/NetBrainNG/#/"
        , header={"Content-Type": "application/json"}
        , body={})

        mgr.post(req)

        time.sleep(1)

if __name__ == "__main__":
    mgr = NbRequestMgr()

    for i in range(0, 32):
        t = threading.Thread(target=test_post, args=(mgr,))
        t.start()

    while True:
        time.sleep(3)

    

    

