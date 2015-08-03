
import settings
import connection

import json
import argparse
import sys

from gberryusers import login
from gberryapplications import list_free_apps, list_apps
    


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

for cmd in commands:
    
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
        
    else:
        print("ERROR: Unknown command '" + cmd + "'")
        sys.exit(1)
        
sys.exit(0)
