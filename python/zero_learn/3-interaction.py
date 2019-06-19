#本节讲解如何在控制台上进行交互
#可以通过input的函数，获取用户输入的内容
#下面演示输入的内容转换成各种类型的数据

#整形int
#整形就是指1、2、3等整数
#为了代码分块好看，我们把每种数据的例子，封装在对应的函数中
def int_sample():
    temp = input("请输入一个整数a:")
    
    print(type(temp))
    #如果输入的是字符串，且不是全为数字，则不是整数
    #str.isdigit()会判断字符串是否全为数字
    if isinstance(temp, str):
        if not temp.isdigit():
            print("抱歉，您输入的内容不是一个有效的数字.")
            return
    elif not isinstance(temp, int):
        #判断输入的是否是一个整数类型
        print("抱歉，您输入的内容不是一个有效的数字.")
        return

    a = int(temp)
    print("a+1=", a + 1)

#浮点数fload
#浮点数指的就是小数
def float_sample():
    str_temp = input("请输入一个浮点数f:")
    #先定义一个变量f，用于存储浮点数；这里涉及到变量的作用域问题；在后续的章节会讲解。先不纠结这个
    f = 0.0
    #使用try 来拦截程序异常。如果您输入的是一个非浮点数，强转为浮点数会抛出异常，导致程序异常终止。
    #try的用法不是本章节主要的讲解内容，先了解即可
    try:
        f = float(str_temp)
    except Exception:
        print("抱歉，您输入的内容不是一个有效的浮点数")
        return

    print("f*10=:", f * 10)


#布尔变量bool
#布尔变量指的就是真还是假
def bool_sample():
    str_temp = input("请输入一个布尔变量b:")
    #先定义一个变量b，用于存储布尔变量bool；同上涉及到变量的作用域问题；在后续的章节会讲解。
    b = False

    #if 是条件判断语句，后续章节会讲解。现在可以基本理解为：如果XXXX则XXXX，否则xxxx
    if str_temp == "True":
        b = True
    elif str_temp == "False":
        b = False
    else:
        print("请输入正确的布尔变量True或者False")
        return

    print("b=", b)




#接下来我们模拟一端对话
def what_you_name():
    #\n表示添加一个换行符， \是转义符号；在后续的章节中会讲解
    name = input("hello, what is your name?\n")
    print("hi ", name, ", my name is angelababy.")


int_sample()
float_sample()
bool_sample()
what_you_name()