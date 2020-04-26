
# -*- coding: UTF-8 -*- 
import glob
import os.path
import re

def replaceGradle(path):
    if (os.path.exists(path)):
        f = open(path, "r")
        text = f.read()
        
        text = re.sub(r"buildToolsVersion \"\d+.\d.\d\"", "buildToolsVersion \"29.0.2\"", text)
        text = re.sub("compileSdkVersion \d+", "compileSdkVersion 29", text)
        text = re.sub("targetSdkVersion \d+", "targetSdkVersion 29", text)
        
        f.close()
        
        print(text)
        f = open(path, "w")
        f.write(text)
        f.close()
    #end if
    
#end of def replaceGradle(path)



def traversalDir_FirstDir(path):
    if (os.path.exists(path)):
    #获取该目录下的所有文件或文件夹目录路径
        files = glob.glob(path + '\\*' )
#        print(files)
        for file in files:
            #判断该路径下是否是文件夹
            if (os.path.isdir(file)):
                #分成路径和文件的二元元组
                h = os.path.split(file)
 #               print(file)
                replaceGradle(file + "\\build.gradle")
 #               print(h[1])
 #               innerfiles = glob.glob(file + '\\*' )
 #               for innerfile in innerfiles:
 #                   print(innerfile)
 #                   if (innerfile.endswith(".iml")):
 #                       os.remove(innerfile)



traversalDir_FirstDir(".")