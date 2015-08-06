import requests

import settings

gConnectionTraceEnabled = True

# TODO: I wonder if requests has builtin tracing possiblity ??

def build_url(path):
    return settings.URL_PREFIX + '/' + path
    
    
def get(url, **kwargs):
    s = requests.Session()
    req = requests.Request('GET',  url, **kwargs)
    prepped = s.prepare_request(req)
    
    print("----- Starting HTTP(S) GET connection")
    print("URL: %s" % prepped.url)
    print("Headers: %s" % prepped.headers)
    
    print("-- Making request")
    r = s.send(prepped)
    print("Response: %s" % r)
    print("Body: %s" % r.text)
    print("----- Connection finished")
    return r
    

def post(url, **kwargs):
    s = requests.Session()
    req = requests.Request('POST',  url, **kwargs)
    prepped = s.prepare_request(req)
    
    print("----- Starting HTTP(S) POST connection")
    print("URL: %s" % prepped.url)
    print("Headers: %s" % prepped.headers)
    print("Body: %s" % prepped.body)
    
    print("-- Making request")
    r = s.send(prepped)
    print("Response: %s" % r)
    print("Body: %s" % r.text)
    print("----- Connection finished")
    return r


def put(url, **kwargs):
    s = requests.Session()
    req = requests.Request('PUT',  url, **kwargs)
    prepped = s.prepare_request(req)
    
    print("----- Starting HTTP(S) PUT connection")
    print("URL: %s" % prepped.url)
    print("Headers: %s" % prepped.headers)
    print("Body: %s" % prepped.body)
    
    print("-- Making request")
    r = s.send(prepped)
    print("Response: %s" % r)
    print("Body: %s" % r.text)
    print("----- Connection finished")
    return r


def put_binary(url, **kwargs):
    s = requests.Session()
    req = requests.Request('PUT',  url, **kwargs)
    prepped = s.prepare_request(req)
    
    print("----- Starting HTTP(S) PUT connection")
    print("URL: %s" % prepped.url)
    print("Headers: %s" % prepped.headers)
    print("Body length: %s" % len(prepped.body))
    
    print("-- Making request")
    r = s.send(prepped)
    print("Response: %s" % r)
    print("Body: %s" % r.text)
    print("----- Connection finished")
    return r
  
  
def delete(url, **kwargs):
    s = requests.Session()
    req = requests.Request('DELETE',  url, **kwargs)
    prepped = s.prepare_request(req)
    
    print("----- Starting HTTP(S) DELETE connection")
    print("URL: %s" % prepped.url)
    print("Headers: %s" % prepped.headers)
    print("Body: %s" % prepped.body)
    
    print("-- Making request")
    r = s.send(prepped)
    print("Response: %s" % r)
    print("Body: %s" % r.text)
    print("----- Connection finished")
    return r

