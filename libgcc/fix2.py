import os, glob
for fn in glob.glob('*.o'):
    try:
        symoff, nsyms = map(int, os.popen("otool -lvv %s  | grep -A 1 ' symoff' | awk '{print $2}' | xargs echo" % fn).read().split(' '))
    except:
        continue
    f = open(fn, 'r+b')
    f.seek(symoff)
    for i in xrange(nsyms):
        a = f.tell()
        f.seek(a + 4)
        q = chr((ord(f.read(1)) | 1) & ~0x10)
        f.seek(a + 4)
        f.write(q)
        f.seek(a + 12)
