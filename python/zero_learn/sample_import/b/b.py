
from c import c
from SingleInstance import SInstance

def output():
    arr = [__file__]
    arr.append('b:')
    arr.append(c.output())
    ret = '\r\n'.join(arr)
    #print(ret)

    SInstance.Instance().add()
    return ret



