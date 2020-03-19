import sys
import os
sys.path.append(os.path.abspath(os.path.dirname(__file__)))

#print('\r\n'.join(sys.path))

from importlib import reload
from b import b
from c import c
from SingleInstance import SInstance

inst = SInstance.Instance()

def output():
    reload(b)
    SInstance.Instance().add()
    inst.add()

    b.output()


if __name__ == "__main__":
    output()

