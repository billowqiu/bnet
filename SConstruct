import os

debug = ARGUMENTS.get('debug', 0)

env = Environment(ENV=os.environ)
if int(debug):
    env.Append(CCFLAGS = ['-ggdb3', '-D DEBUG'])
else:
    env.Append(CCFLAGS = ['-O3', '-DNDEBUG'])

env.Append(CCFLAGS = '-I../')
env.Library("bnet", Glob('*.cpp'))
