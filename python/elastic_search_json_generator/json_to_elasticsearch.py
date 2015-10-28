import os
#import file

f=open('5.json')


while True:
    t_line= f.readline()
    if(""== t_line):
        break
    #print t_line
    cmd='D:\Tools\curl-7.40.0\curl.exe -H "Content-type: application/json"   -X POST http://127.0.0.1:9200/enzo/wowza/ --data ' + "\"" + t_line + "\""
    os.system(cmd)

f.close()