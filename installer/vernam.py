from operator import xor
import socket
import os


def encryptData(keys,text):
    encrypted = bytearray()
    j = 0
    x =''
    test =""

    length = len(text)
 
    # perform XOR operation of key
    # with every character in string
    for i in range(length):
     
        text = (text[:i] + chr(ord(text[i]) ^ ord(keys[j])) + text[i + 1:])
        
    return text

    