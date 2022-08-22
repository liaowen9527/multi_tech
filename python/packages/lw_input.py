

g_is_win = False

try:
    import msvcrt
    g_is_win = True
except:
    import sys
    import select
    import tty
    import termios


class NBInput():
    def __init__(self):
        if not g_is_win:
            self.old_settings = termios.tcgetattr(sys.stdin)
            tty.setcbreak(sys.stdin.fileno())

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        pass

    def __del__(self):
        print("=============")
        if not g_is_win:
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)

    def getch(self):
        if g_is_win:
            return self._getch_win()
        
        return self._getch_linux()

    def _getch_linux(self):
        is_data = select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])
        if is_data:
            ch = sys.stdin.read(1)
            return str(ch)
        
        return None

    def _getch_win(self):
        ch = ord(msvcrt.getch())
        ch = chr(ch)
        return str(ch)


def wait_input(endstr):
    ret = ''

    with NBInput() as nbinput:
        while True:
            ch = nbinput.getch()
            #print(ch)
            if isinstance(ch, str):
                ret = ret + ch

            if endstr in ret:
                break

    return ret



if __name__ == "__main__":
    a = wait_input(' ')
    print(a)
