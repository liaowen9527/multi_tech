#运算符是程序中最常见的知识之一
#可以百度到什么是程序: 程序＝算法+数据结构 的理解
#最简单的算法即为我们熟知的基本运算: 加(+)减(-)乘(*)除(/)
#接下来我们开始学习最基础的算法；操作运算符

#一般，写程序时不会使用中文去命名，如果在国内的一些公司，也会使用中文；这个可以依据自己的习惯及目标去书写
#你可以使用如下的命名: def shili_jia:  表示:式例_加
#sample_add代表的也是: 式例_加  的意思，只是用的英文书写的函数名
#一般临时变量可以用a, b, c表示
#什么是临时变量：退出函数后就会自动释放的变量叫函数变量。 这个概念如果不动，以后再了解
#加法
def sample_add():
    a = 3
    b = 2
    c = a + b
    print("a+b=", c)

#减法
def sample_subtract():
    a = 3
    b = 2
    c = a - b
    print("a-b=", c)


#乘法
def sample_multiply():
    a = 3
    b = 2
    c = a * b
    print("a*b=", c)

#除法
def sample_dividey():
    a = 3
    b = 2
    c = a / b
    print("a/b=", c)

sample_add()
sample_subtract()
sample_multiply()
sample_dividey()

#学习完加减乘除后，我们来学习怎么实现一个最简单的计算器
#计算器
def cal_add():
    a = input("a=")
    b = input("b=")
    try:
        a = int(a)
        b = int(b)
        print("a+b=", a + b)
    except:
        print("您输入的a和b类型不匹配，无法进行加法运算.")
    
def cal_subtract():
    a = input("a=")
    b = input("b=")
    try:
        a = int(a)
        b = int(b)
        print("a-b=", a - b)
    except:
        print("您输入的a和b类型不匹配，无法进行减法运算.")

def cal_multiply():
    a = input("a=")
    b = input("b=")
    try:
        a = int(a)
        b = int(b)
        print("a*b=", a * b)
    except:
        print("您输入的a和b类型不匹配，无法进行乘法运算.")

def cal_dividey():
    a = input("a=")
    b = input("b=")
    try:
        a = int(a)
        b = int(b)
        print("a/b=", a / b)
    except:
        print("您输入的a和b类型不匹配，无法进行除法运算.")

def calculator():
    #无限循环，只有选择退出时才会退出
    while True:
        print("********注意，请输入有效数字**********************")
        option = input("请选择您要做的运算: 1-加法， 2-减法，3-乘法，4-除法, 5-退出计算器\n")
        if option == "1":
            cal_add()
        elif option == "2":
            cal_subtract()
        elif option == "3":
            cal_multiply()
        elif option == "4":
            cal_dividey()
        elif option == "5":
            #退出循环
            break
        

calculator()     