import sqlite3
from packages.nb_datetime import *


class NbDBFeled:
    def __init__(self):
        self.is_pk = False
        self.not_null = False
        self.name = ''
        self.type = ''
       

class NbDBTable:
    def __init__(self, name = ''):
        self.dbpath = ''
        self.name = name
        self.fields = []
        self.conn = None
        
    def create_table(self):
        conn = sqlite3.connect(self.dbpath)
        c = conn.cursor()

        fields = []
        for f in self.fields:
            ex = ''
            if f.is_pk:
                ex = ' PRIMARY KEY NOT NULL'
            elif f.not_null:
                ex = ' NOT NULL'

            fields.append(str('%s %s%s' % (f.name, f.type, ex)))

        sql = str('CREATE TABLE IF NOT EXISTS %s (%s);' % (self.name, ','.join(fields)))

        c.execute(sql)

        self.conn = conn

    def set_pk_field(self, field):
        f = self.get_field(field)
        if f is not None:
                f.is_pk = True
                return

        f = NbDBFeled()
        f.name = field
        f.is_pk = True
        self.fields.append(f)

    def get_field(self, field):
        for f in self.fields:
            if f.name == field:
                return f

        return None

    def init_fields_by_dict(self, dict_):
        for k, v in dict_.items():
            if self.get_field(k) is not None:
                continue

            field = NbDBFeled()
            field.name = k
            if isinstance(v, int):
                field.type = 'INTEGER'
            elif isinstance(v, float):
                field.type = 'NUMERIC'
            else:
                field.type = 'TEXT'

            self.fields.append(field)

    def get_conn(self):
        if self.conn is None:
            self.create_table()

        return self.conn

    def insert(self, dict_, commit=True):
        conn = self.get_conn()
        c = conn.cursor()

        self.insert_c(dict_, c)

        conn.commit()

    def insert_many(self, arr):
        conn = self.get_conn()
        c = conn.cursor()

        for dict_ in arr:
            self.insert_c(dict_, c)

        conn.commit()


    def insert_c(self, dict_, cursor):
        arr_f = []
        arr_v = []
        for k, v in dict_.items():
            arr_f.append(k)
            if isinstance(v, datetime.datetime):
                arr_v.append(nb_datetime.to_ms_string(v))
            else:
                arr_v.append(v)
            continue

            if isinstance(v, int) or isinstance(v, float):
                arr_v.append(str(v))
            else:
                temp = str(v).replace('"', '""')
                arr_v.append(str('"%s"' % str(v)))

        arr_o = ['?' for i in range(len(arr_f))]
        sql = str('INSERT INTO %s(%s) VALUES(%s)' % (self.name, ','.join(arr_f), ','.join(arr_o)) )
        
        cursor.execute(sql, arr_v)

    def update(self, obj):
        pass

    def update_many(self, arr):
        pass

    def upset(self, obj):
        pass

    def upset_many(self, arr):
        pass

    def delete(self):
        pass

    def delete_many(self):
        pass



if __name__ == "__main__":
    table = NbDBTable()
    table.name = 'aaa'
    table.dbpath = r"E:\\git_code\\ng-support\\ng\\Tool\\NBSupport\\package\\data\\aa.db"
    table.set_pk_field('_id')

    dict_ = {'_id':'aaa', 'name':'liaowen', 'num': 3}

    table.init_fields_by_dict(dict_)
    table.insert(dict_)