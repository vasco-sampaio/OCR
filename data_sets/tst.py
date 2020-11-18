import os

def changename(s):
    os.rename(s,s[1:])


for f in os.listdir(os.getcwd()):
    print(f)
    if f[0] == '1' and f[1] != '.':
        changename(f)
