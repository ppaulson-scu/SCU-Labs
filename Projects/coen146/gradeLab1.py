from __future__ import print_function

import os
import subprocess
import sys
import urllib2

def main():
    checkFiles()
    attemptToCompile()
    checkImage()
    checkInvalidInput()
    
def checkFiles():
    FILES = ['lab1.h', 'lab1.c', 'makefile']
    student = sys.argv[1]
    os.chdir(student)
    for f in FILES:
        if f not in os.listdir('./'):
            print('AUTOGRADER: Could not find {}'.format(f))
            if f == 'makefile':
                sys.exit(1)

def attemptToCompile():
    rval = subprocess.call(['make'])
    if rval != 0:
        print('AUTOGRADER: Failed to compile')
    if 'lab1' not in os.listdir('./'):
        print('AUTOGRADER: Could not find lab1 file')

def checkImage():
    with open('currgi.jpg', 'w') as f:
        f.write(urllib2.urlopen('http://students.engr.scu.edu/~aelahi/ta/146/corgi.jpg').read())
    rval = subprocess.call(['./lab1', '--encrypt', 'currgi.jpg', 'encryptedcurrgi.jpg', '16'])
    if rval != 0:
        print('AUTOGRADER: Encryption returned unsuccessful')

    rval = subprocess.call(['./lab1', '--decrypt', 'encryptedcurrgi.jpg', 'decryptedcurrgi.jpg', '16'])
    if rval != 0:
        print('AUTOGRADER: Decryption returned unsuccessful')

    rval = subprocess.call(['diff', 'currgi.jpg', 'decryptedcurrgi.jpg'])
    if rval != 0:
        print('AUTOGRADER: Diff returned unsuccessful')

def checkInvalidInput():
    try:
        output = subprocess.check_output(["./lab1", "--encrypt"])
    except subprocess.CalledProcessError as err:
        output = err.output
    finally:
        print(output)
        if 'Segmentation fault (core dumped)' in output or len(output) < 5:
            print('AUTOGRADER: Core dump or invalid output')

if __name__ == '__main__':
    main()
