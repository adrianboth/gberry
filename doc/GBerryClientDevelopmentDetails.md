GBerry Client Development Details
=================================
(tero.vuorela@iki.fi)

This document contains in journal like format some development decisions.


4.4.2015

It tooks so some time to find out working setup for sharing qml and js code
via `gberry-lib`. You can't include into qrc file any files outside project.

In past I solved problem by SVN externals, but now using git that doesn't
work. However using symbolic links looked like a good solution. Now 
`gberry-lib` files can be include to qrc. It is relative link and should
work if whole GBerry is checkout as instructed.

Few things to note:
 * You need to add all files, including `qmldir` into qrc. There is helper
   add directory but it just add files individually. So if new components
   appear qrc file needs to updated.

 * Keep prefix in qrc same as real directory path. Otherwise you get into
   to trouble with QtCreator as it won't find qml components. Note that
   GBerry files itself refer to component and js so you can't just change
   only prefix.
 
You use shared code like this
 
    import "ui/gberry-lib/qml" as GBerry
    ...
      GBerry.GButton {}
      
Or this way

    import "ui/gberry-lib/qml"
    ...
      GButton {}
      
Or even use both, but for *.js use only one way of referring. I would prefer
the first way as it clearly states from where component is coming from.

I would have like to use versioning (like `import GBerry 1.0`) but couldn't
get that working with qrc file. However this doesn't matter as shared code
is always included to final application.

NOTE: When adding new files into qrc, you should not add them into git!

NOTE: You need to define alias for added files into qrc or otherwise paths
      won't match (as it is just appended).
      
      
By defing QML_IMPORT_TRACE=1 environment variable you can troubleshoot import
problems.

Refs:
http://doc.qt.io/qt-5/resources.html
