import socket
import sys
import os

HOST_NAME='A23101023.zte.intra'
PORT=50007
XP_DRIVER='x'
EXPLORER='D:\Tools\Total_Commander_v8.5\TOTALCMD.EXE'


prefix_str=os.path.expanduser('~')
full_path=os.path.abspath('.')

start_idx=len(prefix_str)
new_path='{0}:\\{1}'.format(XP_DRIVER, full_path[start_idx:])

xp_cmd='{0} {1}'.format(EXPLORER, new_path.replace('/','\\'))
print(xp_cmd)

HOST=socket.gethostbyname(HOST_NAME)
PORT=50007

s= socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))
s.sendall(xp_cmd.encode('utf-8'))
s.close()
