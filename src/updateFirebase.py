from firebase import firebase
import json
import time
import sys

def firebaseGet(name):
    result = firebase.get(name, None)
    return result

def firebaseWrite(name, data):
    firebase.post(name, entry)

def firebaseDelete(name, key):
    firebase.delete(name,key)

def firebaseUpdate(name, data):
    result = firebaseGet(name)
    if result == None:
        firebaseWrite(name, data)
    else:
        for key in result:
            deleteKey = key
        firebaseDelete(name, deleteKey)
        firebaseWrite(name, data)

#create the dictionary

name = sys.argv[1]
data = sys.argv[2]
splitComma = data.split(",")
entry = {}
for line in splitComma:
    splitEquals = line.split("=")
    entry[splitEquals[0]] = splitEquals[1]
firebase = firebase.FirebaseApplication('https://beacon-scanner-c41dc.firebaseio.com/')
if entry['parked'] == 'true':
    firebaseUpdate(name, entry)
else:
    result = firebaseGet(name)
    if result != None:
        for key in result:
            deleteKey = key
        firebaseDelete(name, deleteKey)
