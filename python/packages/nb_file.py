import csv

def save_csv(path, rows):
    csvfile = open(path, 'w', newline='')
    writer = csv.writer(csvfile)   
    writer.writerows(rows)

    csvfile.close()

def save_txt(path, lines):
    fo = open(path, "w")
    fo.writelines(lines)

    fo.close()