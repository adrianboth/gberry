# Copied from http://stackoverflow.com/questions/151677/tool-for-adding-license-headers-to-source-files
# Edited by tero.vuorela(at)iki.fi

# updates the copyright information for all .cs files
# usage: call recursive_traversal, with the following parameters
# parent directory, old copyright text content, new copyright text content

import os
import sys
import re

excludePaths = ['\\.git', 
                '/gtest',
                '/gmock',
                '/qhttpserver',
                '/qt5rpi']

def update_source(filename, oldcopyright, copyright):
    utfstr = chr(0xef)+chr(0xbb)+chr(0xbf)
    fdata = file(filename, 'r').read()
    isUTF = False
    
    if (fdata.startswith(utfstr)):
        isUTF = True
        fdata = fdata[3:]
        
    if (oldcopyright != None):
        if (fdata.startswith(oldcopyright)):
            fdata = fdata[len(oldcopyright):]
            
    #print("### DEBUG: '%s'" % copyright)
    #print("### DEBUG: '%s'" % fdata)
    
    # do count whitespaces
    strippedFData = fdata.strip()
    strippedCopyright = copyright.strip()
    
    if not strippedFData.startswith(strippedCopyright):
        print("    Updating %s" % filename)
        fdata = copyright + fdata
        if True: # test mode
            if (isUTF):
                file(filename, 'w').write(utfstr+fdata)
            else:
                file(filename, 'w').write(fdata)
    else:
        print("        File %s already up to date" % filename)

def recursive_traversal(dir,  oldcopyright, licenseTemplates):
    global excludePaths
    fns = os.listdir(dir)
    print("Processing directory %s ..." % dir)
    
    for fn in fns:
        fullfn = os.path.join(dir,fn)
        skipFile = False
        for rx in excludePaths:
            if re.search(rx, fullfn):
                print("Excluding %s" % fullfn)
                skipFile = True
                break
            
        if skipFile:
            continue
        
        if (fullfn in excludePaths):
            continue
        if (os.path.isdir(fullfn)):
            recursive_traversal(fullfn, oldcopyright, licenseTemplates)
        else:
            ext = os.path.splitext(fullfn)[1]
            if licenseTemplates.has_key(ext):
                licenseTemplate = licenseTemplates[ext]
                print("    Processing %s ..." % fullfn)
                update_source(fullfn, oldcopyright, licenseTemplate)
                
            else:
                print("    Skipping %s as there is no template associated" % fullfn)
   


#oldcright = file("oldcr.txt","r+").read()
#cright = file("copyrightText.txt","r+").read()

templateCpp = file("license_template_cpp.txt", 'r').read()
templateQml = file("license_template_qml.txt", 'r').read()
templatePro = file("license_template_pro.txt", 'r').read()
templateSh  = file("license_template_sh.txt", 'r').read()
licenseTemplates = {'.cpp': templateCpp,
                    '.h': templateCpp,
                    '.qml': templateQml,
                    '.js': templateQml,
                    '.pro': templatePro,
                    '.pri': templatePro,
                    '.sh': templateSh}


if len(sys.argv) == 1:
    print("ERROR: No path provided")
    sys.exit(1)
    
print("Checking path %s ..." % sys.argv[1])
recursive_traversal(sys.argv[1], None, licenseTemplates)
print("[DONE]")
exit()
