#!/usr/bin/python3

import settings
import connection

import os
import json
import argparse
import sys

from gberryusers import login
from gberryapplications import list_free_apps, list_apps, create_from_package, update_from_package, upgrade_from_package
    


parser = argparse.ArgumentParser(description='Manage GBerry applications')
parser.add_argument('commands', metavar='CMD', nargs='+',
                   help='an integer for the accumulator')

parser.add_argument('--sum', dest='accumulate', action='store_const',
                   const=sum, default=max,
                   help='sum the integers (default: find the max)')

args = parser.parse_args()
print(args)
#print(args.accumulate(args.integers))
commands = args.commands

# TODO: how to list commands in command line help
cmd = commands[0]

if cmd == 'list_free_apps':
    apps = list_free_apps()
    print(apps)

elif cmd == 'list_apps':
    session = login()
    apps = list_apps(session)
    print(apps)
    
elif cmd == 'login':
    session = login()
    print("DEBUG: user_token=" + session.userToken + ", private_token=" + session.privateToken)
    
elif cmd == 'create_from_pkg':
    session = login()
    filePath = commands[1]
    print("Uploading package %s" % filePath)
    f = open(filePath, 'rb')
    data = f.read()
    f.close()
    
    create_from_package(session, os.path.basename(filePath), data)
    
elif cmd == 'update_from_pkg':
    session = login()
    filePath = commands[1]
    print("Uploading package %s" % filePath)
    f = open(filePath, 'rb')
    data = f.read()
    f.close()
    
    update_from_package(session, os.path.basename(filePath), data)
    
elif cmd == 'upgrade_from_pkg':
    session = login()
    filePath = commands[1]
    print("Uploading package %s" % filePath)
    f = open(filePath, 'rb')
    data = f.read()
    f.close()
    
    upgrade_from_package(session, os.path.basename(filePath), data)
    
else:
    print("ERROR: Unknown command '" + cmd + "'")
    sys.exit(1)
        
sys.exit(0)
