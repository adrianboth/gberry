import connection

import errors


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
                       params={'user_token': userSession.userToken},
                       headers={'Accept': 'application/json'})
    
    if r.status_code != 200:
        raise ApplicationInvocationError("Requesting applications failed: %s" % r.content)
    
    # TODO: parse resources
    listOfApplications = r.json()
    return listOfApplications
