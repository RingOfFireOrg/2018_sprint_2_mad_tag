#!/usr/bin/env python3

import os
import sys
import termios
import tty
import time
import socket

HOST = '192.168.4.1'  # The server's hostname or IP address
PORT = 15000        # The port used by the server

def inkey():
    global inkey_buffer
    inkey_buffer=1
    fd=sys.stdin.fileno()
    remember_attributes=termios.tcgetattr(fd)
    tty.setraw(sys.stdin.fileno())
    character=sys.stdin.read(inkey_buffer)
    termios.tcsetattr(fd, termios.TCSADRAIN, remember_attributes)
    return character


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s_robot:
    s_robot.connect((HOST, PORT))
    s_robot.sendall(b'H')

    stop = 1;
    while (stop):
        stringIn = inkey()
        if (stringIn == 'q'):
            stop = 0;
#        print("%s" % stringIn, end=''),
        sys.stdout.write(stringIn)
        sys.stdout.flush()
        keystroke = bytes(stringIn, "UTF-8")
        s_robot.sendall(keystroke)
        
print("Done with the whole thing\n")