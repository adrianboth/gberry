import connection
import errors

import json


class ApplicationInvocationError(errors.InvocationError):
    pass


# curl -v -X GET  http://${GBERRYHOST}/gberryrest/v1/application/list?type=free
def list_free_apps():

    url = connection.build_url('application/list')
    r = connection.get(url,
                       params={'type': 'free'},
                       headers={'Accept': 'application/json'})
    
    if r.status_code != 200:
        raise ApplicationInvocationError("Requesting free applications failed: %s" % r.content)
    
    # TODO: parse resources
    listOfApplications = r.json()
    return listOfApplications
    

# curl -v -X GET  http://${GBERRYHOST}/gberryrest/v1/application/list?type=free
def list_apps(userSession):
    url = connection.build_url('application/list')
    r = connection.get(url,
                       params={'user_token': userSession.Token},
                       headers={'Accept': 'application/json'})
    
    if r.status_code != 200:
        raise ApplicationInvocationError("Requesting applications failed: %s" % r.content)
    
    # TODO: parse resources
    listOfApplications = r.json()
    return listOfApplications


def create_from_package(userSession, fileName, data):
    url = connection.build_url('application/create/from_package/' + fileName)
    r = connection.put_binary(url,
                       params={'private_token': userSession.privateToken},  #        'validate_only': True},
                       
                       headers={'Accept': 'application/json',
                                'Content-Type': 'application/octet-stream'},
                       data=data)
    
    if r.status_code != 200:
        #print("DEBUG: " + str(r.content, 'utf-8'))
        responseJson = json.loads(str(r.content, 'utf-8'))
        if 'stack_trace' in responseJson:
            print("stack_trace: \n" + responseJson['stack_trace'])
        if 'error_string' in responseJson:
            print("ERROR: " + responseJson['error_string'])
            
        raise ApplicationInvocationError("Requesting applications failed: %s" % str(r.content, 'utf-8'))
    
    return r.json()


def update_from_package(userSession, fileName, data):
    url = connection.build_url('application/update/from_package/' + fileName)
    r = connection.put_binary(url,
                       params={'private_token': userSession.privateToken},  #        'validate_only': True},
                       
                       headers={'Accept': 'application/json',
                                'Content-Type': 'application/octet-stream'},
                       data=data)
    
    if r.status_code != 200:
        #print("DEBUG: " + str(r.content, 'utf-8'))
        responseJson = json.loads(str(r.content, 'utf-8'))
        if 'stack_trace' in responseJson:
            print("stack_trace: \n" + responseJson['stack_trace'])
        if 'error_string' in responseJson:
            print("ERROR: " + responseJson['error_string'])
            
        raise ApplicationInvocationError("Requesting applications failed: %s" % str(r.content, 'utf-8'))
    
    return r.json()

def upgrade_from_package(userSession, fileName, data):
    url = connection.build_url('application/upgrade_resource/from_package/' + fileName)
    r = connection.put_binary(url,
                       params={'private_token': userSession.privateToken},  #        'validate_only': True},
                       
                       headers={'Accept': 'application/json',
                                'Content-Type': 'application/octet-stream'},
                       data=data)
    
    if r.status_code != 200:
        #print("DEBUG: " + str(r.content, 'utf-8'))
        responseJson = json.loads(str(r.content, 'utf-8'))
        if 'stack_trace' in responseJson:
            print("stack_trace: \n" + responseJson['stack_trace'])
        if 'error_string' in responseJson:
            print("ERROR: " + responseJson['error_string'])
            
        raise ApplicationInvocationError("Requesting applications failed: %s" % str(r.content, 'utf-8'))
    
    return r.json()
