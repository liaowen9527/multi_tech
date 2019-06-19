#本章节讲解的是文件操作
#首先，我们得知道什么是文件，文件有哪些操作
#文件，存储在磁盘上，txt、word、excel都是文件，还包含一些未知扩展名的文件
#文件操作，主要包含：打开、关闭、保存、写文件、读文件、新建文件、删除文件、重命名文件
#文件系统：这里，我们得了解一个词，叫系统。系统一般是指对某一种类型的事务，整体规划，成为一个体系，就成为了系统。
#所以文件系统指的就是对文件操作的体系，用程序员的说话也就是类库。包含对文件操作的各种类和方法

#如果需要对文件进行操作，我们需要引入os的库，os库可以简单的理解windows的资源管理器
#包含文件目录和文件的查找、遍历；文件的删除、重命名等
import os

#新建文件
def new_file(filename):
    #如果我们需要对文件进行内容操作，则必须得打开关闭文件。
    #如果文件被打开了，为了保证文件资源被释放，我们必须在结束后调用close方法关闭文件
    #什么叫资源释放，我们可以用通俗的理解为：某台电视只能被一个人观看，且需要登记；
    #如果我们想看电视，必须得先打开电视并登记，如果我们不想看了之后，需要关闭电视，并注销登记。
    #如果我们离开时不关闭电视和注销登记，其他人将不能申请对电视的使用，导致这个电视一直处于使用状态，但实际上，却没人使用，导致这个电视闲置了

    #打开一个文件, 以写模式打开；如果这个文件不存在，会默认创建一个文件
    #文件打开模式有多中，其中有写模式、读模式、读写模式、等等
    #写模式(mode='w')：只能写
    #读模式(mode='r')：只能读
    #读写模式(mode='r+')
    #关于更多的模式，后续再讲，这里先了解基本的文件操作
    fo = open(filename, mode='w')
    fo.close()
    pass


#删除文件
def remove_file(filename):
    #如果要删除文件，需要使用os库，os库可以简单的理解windows的资源管理器
    os.remove(filename)


#将字符串写入txt文件
def write_txt(filename):
    #我们在每个字符串后面写入一个换行符；换行符对应的字符串就是'\n'；这个可以从ASCIIC码表中的转义字符列得到，对应的十进制是10
    #什么是ASCIIC码表，它是计算机最初设计的时候，定义的二进制与字符的对应关系。计算机中实际存储的是二进制。
    #ASCIIC码表，我们不需要记他们，但是我们得知道他们。对于换行符是经常用到的，所以需要记住
    lines = ["line 1\n", "line 2\n", "line 3\n", 'aaaa\n']
    #打开一个文件，以写模式打开
    #mode='w'时，如果文件存在，它会删掉文件中之前的所有内容，再它从第一行开始写
    fo = open(filename, mode='w')
    for line in lines:
        fo.write(line)

    #上面的是一行一行的写入，实际上我们也可以使用writelines一次性写入
    fo.writelines(lines)

    #文件只要打开了，我们在用完之后一定要记得关闭
    fo.close()

#读取txt文件内容，并打印
def read_txt(filename):
    fo = open(filename, mode='r')
    
    #读取所有的行数
    lines = fo.readlines()
    #遍历所有的行，并打印
    for line in lines:
        #print时，结尾不加换行符，因为line中就包含了换行符\n；这样就避免换行2次
        #print(line)    #这样，结尾会多加一个换行符\n，就成了换两行了
        print(line, end = '')
    
    fo.close()

#自定义格式的文件
def custom_file(src_file, dest_file):
    #打开一个文件，以写模式打开
    #mode='w'或mode='r'时，默认操作的是文本，文本只能写入字符串
    #mode='wb'或mode='rb'时，写入的则是二进制，能写图片等信息
    src_fo = open(src_file, mode='rb')
    dest_fo = open(dest_file, mode='wb')

    while True:
        data_temp = src_fo.read(2048)
        if data_temp:
            dest_fo.write(data_temp)
        else:
            break

    #文件只要打开了，我们在用完之后一定要记得关闭
    src_fo.close()
    dest_fo.close()

filename = "aaa.txt"
new_file(filename)
remove_file(filename)
write_txt(filename)
read_txt(filename)

#我们执行下面的代码前，先拷贝一个a.jpg文件到aaa.txt所在的目录下
#执行完custom_file代码之后，你可以使用图片查看器查看a.aaaa文件，会显示和a.jpg一样的图片
custom_file("a.jpg", "a.aaaa")


