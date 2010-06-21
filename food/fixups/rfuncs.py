import re
pthread = open('/usr/include/pthread.h').read()
for func in open('rfuncs.txt'):
    func = func.strip()
    m = re.search(re.compile('^[^\n]*   %s.*?;' % func, re.M | re.S), pthread)
    if m:
        #print '--', func
        print m.group(0)
        #print '--'
    else:
        print '???', func
