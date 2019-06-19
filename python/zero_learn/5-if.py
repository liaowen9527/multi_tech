#本章节主要讲解条件语句
#条件判断及for循环在代码中是非常重要的
#如果说代码是写逻辑，那么逻辑就不可避免的会出现各种假设情况，这些假设情况我们又应该去做什么。


#条件语句
#条件语句中，我们主要通过if-elif-else来实现。这3个关键字可以构成任意的假设情况
#if：表示如果
#elif：表示否则如果
#else：表示否则

#我们来举个例子：
#老师买了3中水果(苹果，香蕉，梨)，班里有4个小组(A,B,C,D)；老师决定A组吃苹果，D组吃香蕉，其他组(C,D)吃梨
#我们可以定义一个函数，来实现这个条件判断；其中输入参数为小组编号
#我们可以学着对一些函数进行注释，描述函数名及其参数的含义
#函数名: 获取水果
#参数：groupName(字符串类型)--小组名
#返回值: (string)根据小组划分得到的水果
def getFruit(groupName):
    if groupName == "A":
        return "苹果"
    elif groupName == "D":
        return "香蕉"
    else:
        return "梨"
    
    return "if-elif-else基本包含了所有的条件，所以这个返回值不可能出现."

#由于班级只有4个小组(A.B.C.D), 所以，如果不是A,B,C,D的小组名，我们也可以检测出来
#函数名: 是否是有效的小组编号
#参数：groupName(字符串类型)--小组名
#返回值: (bool) 如果返回为True，则是正确的小组编号
def isValidGroup(groupName):
    if groupName == "A":
        return True
    elif groupName == "B":
        return True
    elif groupName == "C":
        return True
    elif groupName == "D":
        return True
    else:
        return False
    
    return False

#我们发现，上面的一个简单的编号居然写了那么多行代码，而且很多都是重复的return true
#我们在高中学过真假、与或非等运算；而If后面接的实际就是True或者是False
#所以我们可以通过下面的代码实现它
def isValidGroup2(groupName):
    #下面的代码表示的是：如果groupName为A，或者为B，或者为C，或者为D，则返回True
    if groupName == "A" or groupName == "B" or groupName == "C" or groupName == "D":
        return True
    
    #这里，其实也可以在if下面接个else并返回True
    #我们没有用else，表达的效果和else也一样，而且会更简单些
    #如果这里不理解，可以先用else实现，写多了代码，自然就明白了。
    return False


#我们来复习一遍对话，并利用上面的getFruit函数来实现根据小组名判断
def ask_for_fruit():
    print("老师：我们班只有4个小组，分别是A，B，C，D；小朋友们，来说出你们的编号来拿水果。")
    while True:
        groupName = input("小朋友: ")

        #not表示非运算；下面的代码表示，如果不是一个有效的小组编号
        if not isValidGroup2(groupName):
            print("老师：小朋友，你的编号说错了呢，请重新选择编号.")
        else:
            print("老师：小朋友，给你", getFruit(groupName))

ask_for_fruit()