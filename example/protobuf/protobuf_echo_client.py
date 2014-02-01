#!/usr/bin/python
# -*- coding: utf-8 -*-

import socket
import addressbook_pb2
import sys
import struct

# This function fills in a Person message based on user input.
def PromptForAddress(person):
    person.id = int(raw_input("Enter person ID number: "))
    person.name = raw_input("Enter name: ")

    email = raw_input("Enter email address (blank for none): ")
    if email != "":
        person.email = email

    while True:
        number = raw_input("Enter a phone number (or leave blank to finish): ")
        if number == "":
            break

        phone_number = person.phone.add()
        phone_number.number = number

        type = raw_input("Is this a mobile, home, or work phone? ")
        if type == "mobile":
            phone_number.type = addressbook_pb2.Person.MOBILE
        elif type == "home":
            phone_number.type = addressbook_pb2.Person.HOME
        elif type == "work":
            phone_number.type = addressbook_pb2.Person.WORK
        else:
            print "Unknown phone type; leaving as default value."

address_book = addressbook_pb2.AddressBook()
PromptForAddress(address_book.person.add())
print address_book

host = '127.0.0.1'  
port = 10086
bufsiz = 1024
ADDR = (host,port)

tcpCliSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpCliSock.connect(ADDR)

pack = address_book.SerializeToString()
print ("address book len {0}".format(len(pack)))
packlen = len(pack)
packlen += 4
print packlen

msg = struct.pack("!I", packlen)
tcpCliSock.send(msg)

raw_input("send protobuf msg>")
tcpCliSock.send(pack)

data = tcpCliSock.recv(bufsiz)
address_book1 = addressbook_pb2.AddressBook()
address_book1.ParseFromString(data)
print address_book1

