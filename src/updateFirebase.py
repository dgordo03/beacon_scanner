from firebase import firebase
import json
import time

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

firebase = firebase.FirebaseApplication('https://beacon-scanner-c41dc.firebaseio.com/')
entry = {'parked':'true','row':1,'column':1}
name = 'car1'
if entry['parked'] == 'true':
    firebaseUpdate(name, entry)
else:
    result = firebaseGet(name)
    if result == None:
        firebaseWrite(name, data)
    else:
        for key in result:
            deleteKey = key
        firebaseDelete(name, deleteKey)
