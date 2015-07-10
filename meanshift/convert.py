import os,sys
name = '%d' 
path = './'

for i in range(0,200):
    os.system('convert %s%s %s%s'% (path,(name%i)+'.pgm',path,(name%i)+'.jpg'))

