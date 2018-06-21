# Evan Pezent (epezent@rice.edu)
# MEL Universal Clean Script
# 1/2018

# This script cleans the build and binary directories associated with MEL. If
# a particular directory does not exist, it is created.

import os
import shutil

keep = ['bin/sfml-audio-2.dll',
        'bin/sfml-graphics-2.dll',
        'bin/sfml-network-2.dll',
        'bin/sfml-system-2.dll',
        'bin/sfml-window-2.dll',
        'bin/Release',
        'bin/Debug',
        'bin/Release/sfml-audio-2.dll',
        'bin/Release/sfml-graphics-2.dll',
        'bin/Release/sfml-network-2.dll',
        'bin/Release/sfml-system-2.dll',
        'bin/Release/sfml-window-2.dll',
        'bin/Debug/sfml-audio-d-2.dll',
        'bin/Debug/sfml-graphics-d-2.dll',
        'bin/Debug/sfml-network-d-2.dll',
        'bin/Debug/sfml-system-d-2.dll',
        'bin/Debug/sfml-window-d-2.dll'
        ]
folders = ['build',
           'lib',
           'bin']

files = []

keep    = ['../' + f for f in keep]
folders = ['../' + f for f in folders]

for folder in folders:
    try:
        files += [folder + "/" + f for f in os.listdir(folder)]
    except:
        print 'Creating: ', folder[3:]
        os.mkdir(folder)
        files += [folder + "/" + f for f in os.listdir(folder)]

files = [f for f in files if f not in folders + keep]

if not files:
    print 'Already Clean'
else:
    for file in files:
        try:
            print 'Removing: ', file[3:]
            os.remove(file)
        except:
            pass
        try:
           print 'Removing: ', file[3:]
           shutil.rmtree(file)
        except:
            pass
    print 'Finished Cleaning'

