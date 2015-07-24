
== Design Decisions

Why Django was selected?
 * Previous experince from Python -> reducing amount of learning although some years ago basic Django experience
 
Why 'comms' exists?
 * Adds localhost TCP communication app<->comms.
 * Wanted to create clear separation for platform and apps
   * Possibly to sandbox apps
     * Crash can be handled
   * Although Qt apps targeted keeps door open for other programming technologies
   * Clear separation allows better testing (TDD)
   

Why 'comms' is Qt based?
 * There are some similarities between apps/client
   -> Shared code
   -> Qt learning benefits
   
 * Binary is little bit protected from anyone to change
 * It's superfast!
 
 