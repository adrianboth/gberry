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
	* 'result' = ok|failure|status|
	* 'application_id'
	* 'player_id'
	* ['error_code']
	* ['error_string']
	* ['status' = started|progress]
	* ['progress_percentage' = <int>]
	
 * TODO: DownloadApplicationReply
 
 * TODO: QueryDownloadableApplications + reply
 
 * TODO: QueryLocalApplications + reply
 
 
<General result info>
 * 'result' = ok|failure|status
    * If error
      * result_details:
	* error_string = "<english description>"
	* TODO: localized text
	* errors = 
	  * code
	  * name
	* metas
	  * key
	  * value
	  
	* reasons
	  * [code]
	  * description
	  
	* subresults
	  * error_string
	  * errors
	  ...
	

 * QueryHeadServerStatus
    -> HeadServerStatus
         -> status = connected|disconnected
            old_status = connected|disconnected
	

    
    
command=