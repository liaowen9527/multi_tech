import os
from packages import nb_string


def get_current_path():
    return __file__

def get_filename(path):
    return os.path.basename(path)

def get_dir(path):
    return os.path.abspath(os.path.dirname(path))

def get_parent_dir(path):
    if os.path.isdir(path):
        return get_dir(path)
    
    return get_dir(get_dir(path))

def get_all_files(dir_folder, filter_='*', loop_sub_folders=False, is_path=True):
    ret = []
    for root, subdirs, files in os.walk(dir_folder):
        #print(root)  # 当前目录路径
        #print(subdir)  # 当前路径下所有子目录
        #print(files)  # 当前路径下所有非目录子文件
        for subdir in subdirs:
            temp_dir = dir_folder + '\\' + subdir
            arr = get_all_files(temp_dir, filter_, loop_sub_folders, is_path)
            ret.extend(arr)   

        for file in files:
            if not nb_string.like(file, filter_):
                continue

            if is_path:
                ret.append(dir_folder + '\\' + file)
            else:
                ret.append(file)

    return ret

def get_subdirs(dir_folder, folder_name):
    subdir_list = []
    for root, subdirs, files in os.walk(dir_folder):
        for sub_dir in subdirs:
            if sub_dir.find(folder_name) != -1:
                subdir_list.append(sub_dir)
        
    return subdir_list



if __name__ == "__main__":
    path = get_current_path()
    folder = get_dir(path)
    print("path:%s, dir:%s" % (path, folder) )

    print('\r\n'.join(get_all_files(folder, filter_='*.py')))
    print("parent dir:%s", get_parent_dir(path))