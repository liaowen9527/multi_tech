import datetime
import time



class nb_datetime:
    @staticmethod
    def get_current_time(utc = True):
        if utc:
            return datetime.datetime.utcnow()
    
        return datetime.datetime.now()

    @staticmethod
    def utc_to_local(param):
        epoch = nb_datetime.mktime(param)
        offset = datetime.datetime.fromtimestamp (epoch) - datetime.datetime.utcfromtimestamp (epoch)
        return param + offset

    @staticmethod
    def local_to_utc(param):
        return datetime.datetime.utcfromtimestamp(param.timestamp())

    #unit second
    @staticmethod
    def mktime(param):
        return time.mktime(param.timetuple())

    @staticmethod
    #"2020-02-07 13:13:41"
    def to_string(param):
        return param.strftime('%Y-%m-%d %H:%M:%S')

    @staticmethod
    def from_string(param):
        return datetime.datetime.strptime(param, "%Y-%m-%d %H:%M:%S") 

    #ISODate("2020-02-07T13:13:41Z")
    #ISODate("2020-02-07T13:13:41.158Z")
    @staticmethod
    def to_mongo_string(param):
        str_date = param.strftime('%Y-%m-%dT%H:%M:%S')
        msec = int(param.microsecond / 1000)
        if msec:
            return 'ISODate("%s.%dZ")' % (str_date, msec)

        return 'ISODate("%sZ")' % (str_date)

    @staticmethod
    def from_mongo_string(param):
        len_pre = len('ISODate("')
        len_date = len('2020-02-07T13:13:41')
        len_end = len('Z)"')
        len_ms0 = len_pre + len_date + len_end

        len_str = len(param)
        if len_str == len_ms0:
            return datetime.datetime.strptime(param, 'ISODate("%Y-%m-%dT%H:%M:%SZ")') 
        
        str_date = param[len_pre : len_pre + len_date]
        date1 = datetime.datetime.strptime(str_date, '%Y-%m-%dT%H:%M:%S')
        
        #.158Z")
        msec_str = param[len_pre + len_date + 1 : len_str - len_end]
        return date1 + datetime.timedelta(milliseconds=int(msec_str))


if __name__ == "__main__":
    print("test datetime")
    t = datetime.datetime.utcfromtimestamp(1581364480)
    print(nb_datetime.to_mongo_string(t))

    utc_time = nb_datetime.get_current_time(True)
    local_time = nb_datetime.utc_to_local(utc_time)
    new_utc_time = nb_datetime.local_to_utc(local_time)
    new_local_time = nb_datetime.utc_to_local(new_utc_time)

    print("local:%s, utc:%s" % (nb_datetime.to_mongo_string(local_time), nb_datetime.to_mongo_string(utc_time)))
    assert(str(utc_time) == str(new_utc_time))
    assert(str(local_time) == str(new_local_time))

    iso_date_str = nb_datetime.to_mongo_string(local_time)
    iso_data = nb_datetime.from_mongo_string(iso_date_str)

    print("iso_date:%s" % iso_date_str)
    assert(iso_date_str == nb_datetime.to_mongo_string(iso_data))

