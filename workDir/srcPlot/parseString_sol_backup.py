



import re

filename = "/home/kassuskley/Desktop/pythonParse"
lst = []

with open(filename, 'r') as f:
    for line in f.readlines():
        l1 = re.split('\(', line)
        if (len(l1) != 2 or len(l1[0]) == 0):
            continue
        l2 = re.split('\) *', l1[1])
        if ((len(l2) != 2) or (len(l2[0]) == 0) or (len(l2[1]) == 0)):
            continue
        l3 = re.split('=', l2[0])
        if ((len(l3) != 2) or (l3[0] != "id") or (len(l3[1]) == 0)):
#            print ((len(l3) != 2) , (len(l3[0]) != "id") , (len(l3[1]) == 0))
            continue
        name = l1[0]
        try:
            idx  = int(l3[1])
            value = float(l2[1])
        except:
            continue
        lst += [(name, idx, value)]

print lst
