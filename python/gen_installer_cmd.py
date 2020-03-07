import sys
import os
from packages import nb_filesystem

if __name__ == "__main__":
    sys.argv.append('aaa')
    print(sys.argv)
    #gen_installer_cmd.py main_py
    if len(sys.argv) < 2:
        print("must need main_py param.")
        exit(0)

    main_py = sys.argv[1]
    folder = nb_filesystem.get_dir(nb_filesystem.get_current_path())
    print('folder:', folder)
    #pyinstaller -F entrance.py -p clip.py -p data.py --hidden-import clip --hidden-import data
    pyfiles = nb_filesystem.get_all_files(folder, filter_='*.py', is_path=True, loop_sub_folders=True)

    cmd = ''
    if len(pyfiles) > 0:
        cmd = 'pyinstaller -F %s -p %s' % (main_py, ' -p '.join(pyfiles))
    else:
        cmd = 'pyinstaller -F %s' % (main_py)

    print("command:", cmd)

    filename = 'py_exe.bat'
    fo = open(filename, "w")
    fo.write(cmd)
    fo.close()

    print("export bat file ", filename)