

class UsefullLine:
    
    def __init__(self, category, line):
        self.category = category
        self.line = line

    @staticmethod
    def filter(usefull_lines, category):
        retlines = []
        for line in usefull_lines:
            if line.category == category:
                retlines.append(line)

        return retlines

    @staticmethod
    def save_to_file(filepath, usefull_lines):
        fileObject = open(filepath, "w")
        for line_info in usefull_lines:
            fileObject.write(line_info.line)

        fileObject.close()


class LogCollector:
    
    #finder_dict sample: {"find_name": ["name="], "find_age_name": ["age=", "name="]}
    def __init__(self, finder_dict):
        self.finder_dict = finder_dict

    def collect(self, filepath):
        usefull_lines = []

        print(filepath)

        fileObject = open(filepath, "r")
        while True:
            try:
                line = fileObject.readline()
                if line:
                    usefull_line = self.parser_line(line)
                    if usefull_line:
                        usefull_lines.append(usefull_line)
                else:
                    break
            except IOError:
                print("IOError")
                pass
            except UnicodeDecodeError:
                print("UnicodeDecodeError")
                pass

        fileObject.close()

        return usefull_lines

    def parser_line(self, line):
        for item in self.finder_dict.items():
            match = True
            for sample_value in item[1]:
                if line.find(sample_value) == -1:
                    match = False
                    break
            
            if match:
                return UsefullLine(item[0], line)

        return None

