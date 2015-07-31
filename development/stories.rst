GBerry User Stories
===================


= x.x. Initialization mobile application

== x.x.x New user taking mobile application into user

[No application installed previously]

Player downloads GBerry mobile applications from his mobile device application
store. 

When mobile application is started it will request to either 
 * Sign in with existing credentials
 * Create new user
 * Retrieve forgotten account
 * Use Guest account
 
[User doesn't have existing account]

Player creates a new user. 

.... TODO


== x.x.x User uses existing player account

TODO

== x.x.x User retrieves forgotten account

== x.x.x User uses guest account



= x.x. Console initialization

== x.x.x Blank device is configured

- self registration

TODO


= x.x. Downloading a game 

- mobile app
- web store (generic browser)
- console browser



-- Implementation
   
   


= x.x. Launching a game


= x.x. Login Dialog

- ENTERING: Pressing top bar user name

- fields to edit

- ACTION: Login
  * 'Save'
  * Try to do login on background
    * Animation (first just static icon)
    * If ok -> give indication (icon), feedback dialog
    * If fail -> give feedback
    * Store received tokens
       -> LoginModel
            * doLogin
            * user name
            * tokens
            * clear
    

- ACTION: Close
  * 'Cancel'
    * ?? Forget changes that are not saved

- ACTION: Selecting user name
  * ?? Save current
  
- ACTION: Editing user name


 