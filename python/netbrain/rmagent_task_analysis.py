from rmagent_task import *
from rmtask_dao import *
from packages.nb_tree import *

class RMAgentTaskAnalysis:
    def __init__(self):
        self.log_dir=''
        self.output_dir=''
        self.dao = RMTaskDao()

    def get_dao(self):
        if self.dao.dbpath =='':
            self.dao.dbpath = self.output_dir + '\\RMTask.db'
        
        return self.dao

    def save_db(self):
        analysis = LogAnalysis()
        analysis.log_dir = self.log_dir
        analysis.logfilename = find_rmagent(analysis.log_dir)
        analysis.output_dir = self.output_dir

        analysis.log_parser = RMAgentLogParser()
        analysis.analysis()

        arr = []
        for id_, task in analysis.log_parser.tasks.items():
            arr.append(task.to_dict())

        #save db
        dao = self.get_dao()
        if os.path.exists(dao.dbpath):
            os.remove(dao.dbpath)

        dao.insert_many(arr)

    def performance(self, taskid=''):
        print("performance statistics:")
        tree = self.build_tree()

        for r in tree.root.children:
            print("\r\n\r\n")
            print("root_taskid", r._data.self_taskid)
            for node in NbTreePreOrder(r):
                #print(node._data.to_dict())
                t = node._data 
                print("%s %d %s %s" % (t.begin_utc_time, t.worker_pid, t.task_type, t.cost_s()))
                



    def build_tree(self):
        tree = NbTree()

        dao = self.get_dao()
        tasks = dao.cost_tops()
        while len(tasks) > 0:
            t = tasks.pop()
            min_t = t.cost()*0.3
            #temp_tasks = dao.cost_gte(parent=t.self_taskid, time_=min_t)
            temp_tasks = dao.cost_tops(parent=t.self_taskid)
            
            tasks.extend(temp_tasks)

            item = NbTreeNode(t.self_taskid, t)
            tree.insert_item(item, t.parent_taskid)

        return tree



    
if __name__ == "__main__":
    print(sys.argv)
    #sys.argv.append(r"E:\thoubleshooting\case\SCVMPWNETBN04 02192020\NetBrain_logs\Worker Server\log")
    if len(sys.argv) < 2:
        print("must need folder and rmagent file name.")
        exit(0)

    analysis = RMAgentTaskAnalysis()
    analysis.log_dir = sys.argv[1]
    analysis.output_dir = 'output'

    #analysis.save_db()
    analysis.performance()

    print("succeed.")