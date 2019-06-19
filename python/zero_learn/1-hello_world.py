#这是一行注释。如果代码开头是#， 那么这一行就是注释；python运行器不会执行注释的代码
#注释行在vscode中一般是绿色的显示
#下面的语句执行后，会在控制台中输出hello world
print("hello world")

#在控制台中输出this is my first program
print("this is my first program")

#print是打印字符串到控制台的函数，字符串中可以包含中英文、数字及特殊字符
print("我也可以是中文，数字9527，也可以是特殊字符@@@@@")


#代码最基础的就是数据类型及函数定义； 为了好好学习，我们先认识什么是函数定义
#函数定义在代码的体现上很简单，就是用def开头， 以冒口结尾换行,如下实例
#函数名有规范，建议用a-z(abcdef...)， A-Z(ABCD...)的字母开头来命名
def func():
    print("我是一个函数")
    print("我是谁")
    print("我是XXXX")

#函数定义之后，不会执行，如果需要执行一个定义的函数，则需要执行它，如下
#调用func()后，就会执行函数func的代码了
func()


#定义一个有函数变量的函数； 上面的func是一个没有变量的函数
def func_param1(name):
    print("传入名字:", name)

#调用这个函数
func_param1("妖怪")