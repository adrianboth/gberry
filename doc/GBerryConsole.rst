GBerry Console
==============

.. COMMENT: Image paths need to be relative to original *_link.rst file
            under 'gberry-development/docs/source/'. Adequote solution
            is list all images here and refer them in the doc

.. |app_launch_statemachine| image:: ../../../gberry-console/doc/exports/application_launch_statemachine.png

and content continues here

Application Launching
---------------------

|app_launch_statemachine|

'mainui' is responsible to showing list of available applications and launching
them after user selection.

'comms' handles actual application launch and life time management.

Sequence:
 * 'mainui' queries a list local application from 'comms' via communication 
    channel.
 * 'mainui' shows list of application for users
 * When user selects an application to be launched 'mainui' sends a launch 
   request for 'comms'
   

Application Configuration
-------------------------

All applications are located in '<gberry_root>/apps/' where each application has
its own folder.

 * Folder is commonly named <application_id>-<application_version>. This is 
   name format 'comms' uses for downloaded applications. Otherwise folder name
   is not required to follow format, e.g. during development time.
   
 * There can be multiple versions of one application as long as they have a 
   different version number and folder name is different.
   
Application id and version form an unique id for application. Application has
also more verbose name that is shown in UI instead of id.

Each application needs to have '*appcfg.json' file on application own folder.
This file contains configuration information:

 * id
 * version
 * name
 * description: shown in UI
 * developer_comment: internal comment (optional)
 * catalog_image: relative path (from appcfg.json) to image for shown in
   application catalog.
 * application_exe: defines executable to run [defaults to 'id'] [optional]
 * system_app: system apps are not shown in catalog [default=false] [optional]
 
'mainui' and 'waitapp' are applications as any other one. Difference is that
they are marked to be 'system_app' and thus not shown in catalogs.
 
 
 
 
TODO: runtime environment


TODO: communication channels