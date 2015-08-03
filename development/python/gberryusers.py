import settings
import connection
import errors

import json


class UserInvocationError(errors.InvocationError):
    pass
                          

class UserSession:
    def __init__(self, userEmail, userToken, privateToken):
        self.userEmail = userEmail
        self.userToken = userToken
        self.privateToken = privateToken
        
        
# curl -v -H "Content-Type: application/json" -X POST --data "{\"email\": \"$1\", \"password\": \"$2\"}" http://${GBERRYHOST}/gberryrest/v1/user/login
def login():
    url = connection.build_url('user/login')
    
    payload = {'email': settings.USER_EMAIL,
               'password': settings.USER_PASSWORD}
    
    r = connection.post(url,
                       headers={'Content-Type': 'application/json',
                                'Accept': 'application/json'},
                        data=json.dumps(payload))
    
    if r.status_code != 200:
        raise UserInvocationError("User login failed: %s" % r.content)
    
    # TODO: parse resources
    responseJson = r.json()
    
    return UserSession(settings.USER_EMAIL, responseJson['user_token'], responseJson['private_token'])

