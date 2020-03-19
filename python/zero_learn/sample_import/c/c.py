
def output():
    arr = [__file__]
    arr.append('c:')
    ret = '\r\n'.join(arr)
    print(ret)
    return ret