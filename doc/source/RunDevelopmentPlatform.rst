Run Development Platform
========================

This page contains instructions how to run the GBerry platform during 
development time.


Running Console
---------------

Once full build has been finished the console can be started by running
`builds/deploy-<target>/bin/start.sh`.

The script launches `comms` which launches `waitapp` and `mainui`.


Running Minimal Console
-----------------------

"Minimal" refers to setup where `comms` is launched but it doesn't launch 
`mainui` at all. You are free to start you app manually. This setup servers
well when you are developing a game.

To start minimal `comms` provide command line arguments 
`--disable-wait-ui --disable-main-ui`

If you are running `comms` from different location that `deploy-<target>`
directory, e.g. Debug version via QtCreator, provide argument to specify
'--comms-config %{sourceDir}/app/comms.ini' where %{sourceDir} is QtCreator
provided variable.

Note to launch apps comms requires `deploy-<target>/` setup.


Running Applications
--------------------

To launch an application from QtCreator, just run application executable. 
QtCreator build has already all needed configurations in place to find all
required libraries.

In `deploy-<target>/` setup a single application can be started as standalone
by using script `bin/start_app.sh <app_folder_name> <app_executable>`. Note
that application is not reading *appcfg.json because it is read by `comms`.

## Defining Application Identity

Normally `comms` provides an application code for a launched application via 
command line parameter, and the application passes the code when creation
TCP/IP connection. This way `comms` knows that connected application was the
one that was just launched. This especially necessary for `mainui` and 
`waitapp` applications.

If during development time `comms` is launched in minimal mode by disabling
normal `waitapp` and `mainui` launch, then an application code can be
provided on command line, for example: `--application-code=mainui` 

Normally code is generated but if `comms` is in minimal mode then it 
recognizes codes `mainui` and `waitapp`.


Running Unit Tests
------------------

You can run all unit tests using `development/run_unittests.sh`.

There are integration tests `development/run_integrationtests.sh` but they 
require specific external setup (webstore).

You can run tests also via QtCreator. No special setup is required just
execute test binaries. Components (gberry-lib, console-lib, comms) have a
test executable that contains all tests.

You can filter what are run by providing command line argument 
`--gtest_filter=<filter>`. For example to run all QtLibrariesManager tests
`--gtest_filter=QtLibrariesManagerTests.*`


