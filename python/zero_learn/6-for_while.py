#本章节讲得是循环语句
#每件事，我们不可能只做一次，重复做的事，我们可以通过for循环简单实现

#在前面的章节中，我们已经预先学些了一些简单的循环用法
#本章节中会讲解一些更高级的用法，使代码更简洁，好看

#跳出循环
#我们不希望一件事一直运行下去，在必要的时候应该退出循环
def break_loop():
    print("告诉你一个秘密哟: 输入exit可以退出循环")
    #while(True)表达的是一个死循环，死循环的意思就是无限循环
    while(True):
        a = input("随便输入吧，反正我是死循环，嘿嘿\n")
        if a == "exit":
            print("oh, no。你怎么发现exit可以退出的。")
            break

break_loop()

#跳出本次循环，重新开始循环
#这个解释可能不好理解，我们来说个场景
#比如，除了lucy休息， 其他人呢，先去做早操；做完早操呢，男同学提水，女同学打扫卫生
#我们来用代码表示
def do_something():
    print("告诉你一个秘密哟: 输入exit可以退出循环")
    print("老师：除了lucy休息， 其他人呢，先去做早操；做完早操呢，男同学提水，女同学打扫卫生")
    while(True):
        name = input("你叫什么名字:")
        if name == "lucy":
            continue
        elif name == "exit":
            break
        
        print("做早操")
        sex = input("性别boy or girl:")
        if sex == "boy":
            print("去提水")
        elif sex == "girl":
            print("去打扫卫生")
        else:
            print("卧槽，这是什么性别? 我只知道boy和girl")

do_something()


#for循环
#for也可以实现循环，一般用于范围内循环
#例如输出1-10
def print_1_10():
    print("通过for循环，打印1-10")
    for i in range(1, 11):
        print(i)

#我们也可以通过while来实现输出1-10，但是code会显得不优美
def while_print_1_10():
    print("通过while循环，打印1-10")
    i = 1
    #当i == 11 的时候，就会自动退出循环了，这里不是死循环
    #while括号后为False时就会自动退出循环
    while(i < 11):
        print(i)
        i = i + 1

def loop_array():
    print("遍历数组中的水果")
    fruits = ['banana', 'apple',  'mango']
    for fruit in fruits:        # 第二个实例
        print('当前水果 :', fruit)

print_1_10()
while_print_1_10()
loop_array()


#双重循环
#双重循环也是经常用到的
def loop_twice():
    for i in range(1, 10):
        for j in range(1, 10):
            print("i * j =", i, "*", j, "=", i * j )

loop_twice()

