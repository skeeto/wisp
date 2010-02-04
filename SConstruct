# SConstruct - Wisp build system
#
# Documentation and test suite will be built automatically. To run the
# test suite, use the "check" target.
#
#   scons check
#
import os

# Documentation building
docbld = Builder(action = 'asciidoc -o $TARGET $SOURCE')

# Create new environement
normal = Environment(
           CC = 'gcc',
           CFLAGS = ['-W', '-Wall', '-g', '-O2'],
           LIBPATH = ['/usr/lib', '/usr/local/lib'],
           CPPPATH = ['/usr/include', '/usr/local/include'],
           BUILDERS = {'AsciiDoc' : docbld})

# Configure the environment
conf = Configure(normal)
print 'Configuring...'
if not conf.CheckFunc('printf'):
    print 'error: couldn\'t find a working C compiler'
    Exit(1)
if not conf.CheckLibWithHeader('gmp', 'gmp.h', 'c'):
    print 'error: could not find libgmp and/or gmp.h'
    print '  GNU Multiple Precision (GMP) library with headers required'
    Exit(1)
if not os.path.exists('/usr/bin/asciidoc'):
    print 'AsciiDoc not found - docs will not be built'
    conf.env['noasciidoc'] = True
if 'CC' in os.environ:
    conf.env.Replace(CC = os.environ['CC'])
    conf.env.Replace(CFLAGS = '')
    print(">> Using compiler " + os.environ['CC'])
if 'CFLAGS' in os.environ:
    conf.env.Append(CFLAGS = os.environ['CFLAGS'])
    print(">> Appending custom build flags : " + os.environ['CFLAGS'])
if 'LDFLAGS' in os.environ:
    conf.env.Append(LINKFLAGS = os.environ['LDFLAGS'])
    print(">> Appending custom link flags : " + os.environ['LDFLAGS'])
normal = conf.Finish()
Export('normal')

# Include subdirectories
SConscript('lib/SConscript')
SConscript('test/SConscript')
SConscript('doc/SConscript')

# Main program
normal.Program(target  = 'wisp',
               LIBS = ['wisp', 'gmp'],
               LIBPATH = normal['LIBPATH'] + ['lib'],
               source = 'wisp.c')
