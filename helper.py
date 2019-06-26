import random
import json
import string

def to_binary(n):
    return "{0:b}".format(n)

def to_decimal(b):
    return int(b, 2)

def random_binary(length):
    b = ''
    for _ in range(length):
        b = b + str(random.randint(0, 1))
    return b

def pretty_dict(d):
    print(json.dumps(d, sort_keys=True, indent=4))

def random_name():
    return ''.join(random.choices(string.ascii_letters + string.digits, k=16))