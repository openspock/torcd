#!/usr/bin/python3

import sys, getopt, socket

def main(argv):
    host = ''
    port = ''
    cmd = ''
   
    try:
        opts, args = getopt.getopt(argv,"ht:p:c:",["host=","port=","cmd="])
    except getopt.GetoptError:
        print ('test.py -t <host> -p <port> -c <cmd>')
        sys.exit(2)
    
    for opt, arg in opts:
        if opt == '-h':
            print ('test.py -t <host> -p <port> -c <cmd>')
            sys.exit()
        elif opt in ("-t", "--host"):
            host = arg
        elif opt in ("-p", "--port"):
            port = arg
        elif opt in ("-c", "--cmd"):
            cmd = arg
    
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host,int(port)))
        f = s.makefile('w', None)
        f.write(cmd)
        f.flush()
        print('sent command name')
        cmd = 'invoke'
        f = s.makefile('w', None)
        f.write(cmd)
        f.flush()        
        print('invoking')
        data = s.recv(1024)
        s.close()

    print('Response: ', repr(data))

if __name__ == "__main__":
   main(sys.argv[1:])