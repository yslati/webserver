#!/usr/bin/env python
import time
from http import cookies
from os import environ

env = environ['HTTP_COOKIE']

cookie = cookies.SimpleCookie(env)


# cookie['lastvisit'] = str(time.time())
print (cookie)
print (f"Hello {cookie['name'].value}")