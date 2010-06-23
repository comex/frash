import re
iface = None
for fn in ['android_npapi.h', 'ANPSystem_npapi.h', 'ANPSurface_npapi.h']:
    for line in open(fn):
        m = re.match('struct ANP(.*)InterfaceV0 ', line)
        if m:
            iface = m.group(1)
            print '    case k%sInterfaceV0_ANPGetValue: {' % iface
            print '        ANP%sInterfaceV0 *iface = (ANP%sInterfaceV0 *) ptr;' % (iface, iface)
            print '        _assert(iface->inSize == sizeof(*iface));'
        m = re.search('\(\*([^\)]*)\)\(', line)
        if iface is not None and m:
            print '        *((void **) (&iface->%s)) = stub2(%s_impl_%s);' % (m.group(1), iface.lower(), m.group(1))

        if line.startswith('}') and iface is not None:
            print '        break; }'
            print
            iface = None
