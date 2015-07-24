*********************************
Emulating Different Display Sizes
*********************************

Overview
========

Mobile devices have
 * different screen sizes in pixels
 * different screen sizes in physical dimensions
 * changing orientation
 
Application UI needs to adapt and make sure
 * UI is usable visually
 * Text can be read
 * Touchable elements are big enough for touch
 * On bigger screens bigger area is utilized
 
Developing with one code base means you need to test with lots of combinations:
 * Testing with real devices is time consuming
 * Testing with emulators is better but still time consuming
 
Solution is to have mechanism where different combinations can be well enough
tested on desktop. GBerry has 'DisplayProfile' that enables to have different
display variations on desktop.

What
====

On desktop application is shown in a window. The windows size can be changed
according to used DisplayProfile. 

In addition to using specific resolution or display size, used DisplayProfile 
can be scaled by scale factor downwards or upwards. 

There is autoscaling mode where the size of window is tried to maximize while 
keeping aspect ratio and the whole window visible.

GBerry provides two modes:

**PixelMatching**

Exactly same amount of pixels are used. 

**DimesionMatching**

UI elements are tried shown exactly with same physical dimension as on a real
display. E.g. element height is 1cm on both real device and on desktop.

Dimension matching utilizes dots-per-inch details of a target screen and 
desktop screen.


Usage
=====

TODO: now hard coded default but in future config file

Utilizing requires:

 * Avoiding hard coding pixel sizes or text point sizes
 * Bind sizes to properties
 
Dimension matching technique requires that component sizes have been defined
physical dimensions.
 
 

