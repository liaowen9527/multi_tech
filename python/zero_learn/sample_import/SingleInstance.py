
class SInstance:
    _instance = None

    def __init__(self):
        self.arr = []

    @staticmethod
    def Instance():
        if SInstance._instance is None:
            SInstance._instance = SInstance()
        
        return SInstance._instance

    def add(self):
        self.arr.append(1)
        print("len:", len(self.arr))