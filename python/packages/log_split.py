import os

class LogCategory:
    def __init__(self, key, filename):
        self.key = key
        self.filename = filename


class LogSplit:
    def __init__(self):
        self.output_dir = ''
        #return None or LogCategory
        self.func_find_keyword = None
        #file max size = 1M
        self.file_maxsize = 1 * 1024 * 1024
        self.categories = {}

    def split(self, filepath):
        if not os.path.exists(self.output_dir):
            os.mkdir(self.output_dir)

        fileObject = open(filepath, "r")
        while True:
            try:
                line = fileObject.readline()
                if not line:
                    break
                
                self.parse_line(line)
            except IOError:
                print("IOError")
                pass
            except UnicodeDecodeError:
                print("UnicodeDecodeError")
                pass

        fileObject.close()

        #save all files
        for key, f in self.categories.items():
            f.save()

    def parse_line(self, line):
        category = self.func_find_keyword(line)
        if category is None:
            return

        if category.key not in self.categories:
            self.categories[category.key] = SmallFile(self.output_dir, category.filename, self.file_maxsize)

        file = self.categories[category.key]
        file.add_line(line)


class SmallFile:
    def __init__(self, folder, filename, maxsize=1 * 1024 * 1024):
        self.folder = folder
        self.filename = filename
        self.index = 0
        self.lines = []
        self.size = 0
        self.file_maxsize = maxsize

    def add_line(self, line):
        self.lines.append(line)
        self.size += len(line)
        if self.size < self.file_maxsize:
            return

        self.save()
        

    def save(self):
        if len(self.lines) <= 0:
            return

        filepath = os.path.join(self.folder, self.filename)
        if self.index > 0:
            filepath += '.%d' % (self.index) 

        fo = open(filepath, "w")
        fo.writelines(self.lines)

        print("save small file:%s" % filepath)

        fo.close()

        self.lines = []
        self.size = 0
        self.index += 1
        



        
