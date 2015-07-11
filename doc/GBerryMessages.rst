GBerry Messsages
================

There are communication channels
 * Console <-> mobile client (mobapp, i.e. a player)
 * Console <-> application(s)
 * Via above channels application <-> mobile client
 
Messages are Json based.
 
 
command=CloseChannel
 - Informs to recipient that channel is about to be closed
 
 
!!ping
!!pingreply

General
 * 'command'
 * 'result' for replies
 
 
?? would there be a way to formally define messages
  -> generating documentation
  -> !! I could even generate read/write classes
    * and validation
    
application -> comms
 * DownloadApplication
    * 'application_id'
    -> DownloadApplicationReply
	* 'result' = OK|Failure
	* 'application_id'
	* 'error_code'
	* 'error_string'
	

    
    
command=