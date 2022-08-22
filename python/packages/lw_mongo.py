import pymongo
import gridfs


class NBMongoConf:
    def __init__(self):
        self.hosts = ["127.0.0.1:15672"]
        self.ssl = False
        self.username = ''
        self.password = ''

    def get_url(self):
        #mongodb://[username:password@]host1[:port1][,...hostN[:portN]]][/[database][?options]]
        return 'mongodb://%s:%s@%s/?ssl=%s' % (self.username, self.password, ','.join(self.hosts), 'true' if self.ssl else 'false')
            

if __name__ == "__main__":
    conf = NBMongoConf()
    conf.hosts = ['10.99.99.92']
    conf.username = 'admin'
    conf.password = 'admin'

    client = pymongo.MongoClient(conf.get_url())
    db = client['Feature_Intent']
    collection = db['ODDeviceData']

    gridfs.GridFS(db, collection='DE')

    print(collection.find_one({'_id' : 'eeec4c31-2998-4ccc-9a40-bdef17668e1f'}))

