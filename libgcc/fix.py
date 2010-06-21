import glob
for fn in glob.glob('*.o'):
    f = open(fn, 'r+b')
    f.seek(8)
    f.write('\x09')
