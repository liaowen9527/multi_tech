import pymongo


class NBMongoConf:
    def __init__(self):
        self.host = "127.0.0.1"
        self.port = 15672
        self.ssl = False

class NBMongoClient:
    def __init__(self):
        self.conf = None
        self.client = None


    def connect(self):
        try:
            self.client = pymongo.mongo_client()
        except Exception as e:
            


