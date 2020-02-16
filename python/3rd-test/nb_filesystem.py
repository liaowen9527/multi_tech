import os


def get_current_path():
    return __file__

def get_dir(path):
    return os.path.abspath(os.path.dirname(path))

def get_parent_dir(path):
    if os.path.isdir(path):
        return get_dir(path)
    
    return get_dir(get_dir(path))


path = get_current_path()
print("path:%s", path)
print("dir:%s", get_dir(path))
print("parent dir:%s", get_parent_dir(path))