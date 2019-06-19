#这个程序会学习各种数据类型
#数据类型包括：整形int, 浮点数fload, 布尔变量bool, 字符串string, 字典dictionary, 数组array, 类实例instance


#整形int
#整形就是指1、2、3等整数
#为了代码分块好看，我们把每种数据的例子，封装在对应的函数中
def int_sample():
    num = 1
    print("数字:", num)

    #输出数据类型, 可以使用type()来获取数据类型
    print("num的数据类型是:", type(num))

    #可以用isinstance来判断数据类型
    is_int = isinstance(num, int)
    print("num是整形? True or False:", is_int)

#浮点数fload
#浮点数指的就是小数
def float_sample():
    num = 1.2
    print("浮点数:", num)

    #输出数据类型, 可以使用type()来获取数据类型
    print("num的数据类型是:", type(num))

    #可以用isinstance来判断数据类型
    print("num是浮点数? True or False:", isinstance(num, float))

    print("32是浮点数? True or False:", isinstance(32, float))


#布尔变量bool
#布尔变量指的就是真还是假
def bool_sample():
    bool_value = True
    print("bool变量:", bool_value)

    #输出数据类型, 可以使用type()来获取数据类型
    print("bool_value的数据类型是:", type(bool_value))

    #可以用isinstance来判断数据类型
    print("bool_value是bool变量? True or False:", isinstance(bool_value, bool))


#字符串string
#字符串简单说就是文本
def string_sample():
    string_value = "我是字符串"
    print("字符串:", string_value)

    #输出数据类型, 可以使用type()来获取数据类型
    print("string_value的数据类型是:", type(string_value))

    #可以用isinstance来判断数据类型
    print("string_value是字符串? True or False:", isinstance(string_value, str))


#数组array, 也叫链表list
#数组用中括号包括起来，用逗号分隔每个元素；每个元素的类型要统一
#元素可以是任意类型，例如int，float，str，array，dictionary
def array_sample():
    array_value = ["元素1", "元素2", "元素3"]
    print("数组:", array_value)

    #输出数据类型, 可以使用type()来获取数据类型
    print("array_value的数据类型是:", type(array_value))

    #可以用isinstance来判断数据类型
    print("array_value是数组? True or False:", isinstance(array_value, list))


#字典dictionary
#字典是key-value的形势存在，用大括号包起来。可以方便的查找一个key值
def dict_sample():
    dict_value = {"姓名": "潘长江", "年龄": 48, "性别": "男"}
    print("字典:", dict_value)

    #输出数据类型, 可以使用type()来获取数据类型
    print("dict_value的数据类型是:", type(dict_value))

    #如果你想知道dict_value的姓名，可以使用下面的代码获取
    print("你的姓名：", dict_value["姓名"])

    #可以用isinstance来判断数据类型
    print("dict_value是字典? True or False:", isinstance(dict_value, dict))


int_sample()
float_sample()
bool_sample()
string_sample()
array_sample()
dict_sample()

