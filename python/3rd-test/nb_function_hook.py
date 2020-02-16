import sys

# demo functions, making calls and returning things

def foo(bar, baz):
    return bar(baz)

def spam(name):
    print(f"Hello, {name}")
    return [42 * i for i in range(17)]

# trace functions, one only call events, another combining calls and returns

def call_tracer(frame, event, arg):
    # called for every new scope, event = 'call', arg = None
    # frame is a frame object, not a function!
    print(f"Entering: {frame.f_code.co_name}")
    return None

def call_and_return_tracer(frame, event, arg):
    if event == 'call':
        print(f"Entering: {frame.f_code.co_name}")
        # for this new frame, only trace exceptions and returns
        frame.f_trace_lines = False
        return call_and_return_tracer
    elif event == 'c_call':
        print(f"Entering: {arg.__name__}")
    elif event == 'return':
        print(f"Returning: {arg!r}")
    elif event == 'c_return':
        print(f"Returning from: {arg.__name__}")


def inner_test():
    foo(spam, "world")
    print()

    sys.settrace(call_and_return_tracer)
    foo(spam, "universe")
    print()
    sys.settrace(None)

    sys.setprofile(call_and_return_tracer)
    foo(spam, "profile")

def test():
    while True:
        inner_test()

        time.sleep(1)

def test2():
    import threading
    import time

    t1 = threading.Thread(target=test)
    t1.start()

    hook = False
    while True:
        time.sleep(30)
        hook = not hook
        if hook:
            print("hook function")
            sys.settrace(call_tracer)
            
        else:
            print("remove hook")
            sys.settrace(None)


if __name__ == '__main__':
    inner_test()

    #test2()

    
    