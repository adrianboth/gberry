**************
UI Design Tips
**************

Designing UI that can adapt to different screen sizes is challenging.

Also because of internationalization translated texts can have
different length.

Tips:
 - Avoid fixed pixel and text point sizes
 
 - Try to find something to anchor
   * Buttons should have certain physical size that they can be pressed
     easily.
   * Text should have certain size 
   
 - Design for small display (typically smart phone), then think how 
   more space could utilized, where to expand.
   
 - Use bottom up or top down strategy
   * In bottom up, lowest levels define minimum dimensions and upper
     levels adapt to it.
   * Top down, higher level components calculate maximum available
     space and lower UI components adapt to it.
     
     
 TODO: screen size classes