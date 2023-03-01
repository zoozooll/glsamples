
# -*- coding: UTF-8 -*- 
# import glob
# import os.path
# import re
#
# def replaceGradle(path):
#     if (os.path.exists(path)):
#         f = open(path, "r")
#         text = f.read()
#
#         text = re.sub(r"buildToolsVersion \"\d+.\d.\d\"", "buildToolsVersion \"29.0.2\"", text)
#         text = re.sub("compileSdkVersion \d+", "compileSdkVersion 29", text)
#         text = re.sub("targetSdkVersion \d+", "targetSdkVersion 29", text)
#
#         f.close()
#
#         print(text)
#         f = open(path, "w")
#         f.write(text)
#         f.close()
#     #end if
#
# #end of def replaceGradle(path)
#
#
#
# def traversalDir_FirstDir(path):
#     if (os.path.exists(path)):
#     #获取该目录下的所有文件或文件夹目录路径
# #         print(path + os.sep + '*')
#         files = glob.glob(path + os.sep + '*')
# #         print(files)
#         for file in files:
# #             print(file)
#             #判断该路径下是否是文件夹
#             if (os.path.isdir(file)):
#                 #分成路径和文件的二元元组
#                 h = os.path.split(file)
# #                 replaceGradle(file + "\\build.gradle")
# #                 print(h[0])
#  #               innerfiles = glob.glob(file + '\\*' )
#  #               for innerfile in innerfiles:
#  #                   print(innerfile)
#  #                   if (innerfile.endswith(".iml")):
#  #                       os.remove(innerfile)
#             else:
# #                 if (file.startswith("._")):
#
#                 print(file)
#
#
#
# traversalDir_FirstDir(".")

import os
import shutil

def remove_files_starting_with_dot_underscore(directory):
    for root, dirs, files in os.walk(directory):
#        for file in files:
#            if file.startswith('._'):
#                os.remove(os.path.join(root, file))
        for dir in dirs:
            if dir == 'resources':
                path = os.path.join(root, dir)
                if os.path.isdir(path):
                    shutil.rmtree(path)
                else:
                    os.remove(path)

# Example usage
remove_files_starting_with_dot_underscore('.')