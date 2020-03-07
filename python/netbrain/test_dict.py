import log4cpp_files

class test_a:
    
    def __init__(self, a, b):
        self.a = a
        self.b = b

dict1 = {"1" : test_a(1, "1"), "3" : test_a(3, "3"), "2" : test_a(2, "2")}
dict1 = {}
#dict1["1"] = test_a(1, "1")
#dict1["3"] = test_a(3, "3")
#dict1["2"] = test_a(2, "2")

print(dict1)
#print(sorted(dict1.items(), key=lambda d: d[0]) )
print(sorted(dict1.items(), key=lambda d: d[1].b) )

aa = test_a(1, "1")

print(log4cpp_files.ms_cost_to_timestring(79401235))
print(log4cpp_files.ms_cost_to_timestring(353040517))
print(log4cpp_files.ms_cost_to_timestring(152571))

