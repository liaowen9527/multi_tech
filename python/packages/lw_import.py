import sys
import importlib
import os
import shutil
#import netbrain.common.nbfilelock as nbfilelock


g_lock_filepath = sys.executable

class NBPathFinder(object):
    def find_module(cls, name, path, target=None):
        print("Importing", name, path, target)
        for cls_temp in sys.meta_path:
            if cls_temp == cls:
                continue
            
            loader = None
            try:
                o = cls_temp()
                loader = o.find_module(name, path)
            except :
                loader = cls_temp.find_module(name, path)

            if not loader:
                continue

            nbloader = NBPathLoader()
            nbloader.loader = loader
            return nbloader

        return None

class NBPathLoader:
    def __init__(self):
        self.loader = None

    def load_module(self, name):
        if not self.loader:
            return None
        
        with nbfilelock.SharedLock(g_lock_filepath):
            return self.loader.load_module(name)

    
def set_lockfile(lock_filepath, w=False):
    g_lock_filepath = lock_filepath

def install():
    sys.meta_path.insert(0, NBPathFinder())

def get_module_name(relative_path):
    if not isinstance(relative_path, str):
        return None
    
    nlen = len(relative_path) - len('.py')
    if nlen < 0:
        return None

    ext = relative_path[nlen : ]
    if ext != ".py":
        return None

    filename = relative_path[0 : nlen]
    m = filename.replace('\\', '.')
    m = m.replace('/', '.')
    return m

def replace_many(srcfolder, destfolder, module='netbrain'):
    rootfolder = None
    modules = []

    with nbfilelock.Lock(g_lock_filepath):
        for root, dirs, files in os.walk(srcfolder):
            if rootfolder is None:
                rootfolder = root

            for name in dirs:
                src_temp = os.path.join(root, name)
                rpath = src_temp[len(rootfolder) : ]    #relative path
                dst_temp = os.path.join(destfolder, rpath)
                if not os.path.exists(dst_temp):
                    os.mkdir(dst_temp)

            for name in files:
                src_temp = os.path.join(root, name)
                rpath = src_temp[len(rootfolder) : ]    #relative path
                dst_temp = os.path.join(destfolder, rpath)
                shutil.copy(src_temp, dst_temp)

                m = get_module_name(rpath)
                if m is None:
                    continue

                if len(module) == 0:
                    modules.append(m[1 : ])
                else:
                    modules.append(module + m[1 : ])

    print(module.join('\r\n'))

def replace(o):
    srcfolder = o['src']
    destfolder = o['dest']
    module = ''
    if 'module' in o:
        module = o['module']

    replace_many(srcfolder, destfolder, module)


if __name__ == "__main__":
    print(__file__)

    arr = sys.meta_path
    install()
    import datetime
    #from aaa import test 
    import test_reload
    importlib.reload(test_reload)

    print(test_reload.name())