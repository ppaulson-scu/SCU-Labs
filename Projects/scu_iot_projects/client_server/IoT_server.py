import socket

s = socket.socket()
print "Socket created."

port = 6999

#s.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
s.bind(('', port))
print "socket bound to %s" %(port)

s.listen(5)
print "Listening..."


c,addr = s.accept()
print "Connected to ", addr
while True:
    string = raw_input("Input:")
    if string == "q":
        break
    print string
    c.send(string)
c.close()
