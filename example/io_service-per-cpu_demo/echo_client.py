#!/usr/bin/python
# -*- coding: utf-8 -*-

import socket

host = '127.0.0.1'  
port = 10086
bufsiz = 1024
ADDR = (host,port)

tcpCliSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpCliSock.connect(ADDR)

while True:
    data = raw_input('> ')
    if not data:
        break
    tcpCliSock.send(data)
    data = tcpCliSock.recv(bufsiz)
    if not data:
        break
    print data

tcpCliSock.close()
