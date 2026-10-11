#!/usr/bin/env python3
# Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
# Temporary inspection only: do not write into the baseline source or package UF2s.
import argparse, difflib, shlex, shutil, subprocess, tempfile
from pathlib import Path
parser=argparse.ArgumentParser()
parser.add_argument('--arm-build',type=Path,help='Existing alpha19 build directory for recorded compiler flags')
parser.add_argument('--arm-prefix',default='/Applications/ArmGNUToolchain/15.2.rel1/arm-none-eabi/bin/arm-none-eabi-')
args=parser.parse_args()
here=Path(__file__).resolve().parent
source=here.parents[1]/'src'
work=Path(tempfile.mkdtemp(prefix='spatial-delay-slew-check-'))
print('Temporary inspection:',work,flush=True)
for variant in ('baseline','delay','slew','combined'):
    target=work/variant
    shutil.copytree(source,target/'src')
    patches=() if variant=='baseline' else ('delay','slew') if variant=='combined' else (variant,)
    for patch in patches:
        subprocess.run(['patch','-p1','-i',str(here/(patch+'.patch'))],cwd=target,check=True,stdout=subprocess.DEVNULL)
    if variant!='baseline':
        tests=['reference_test.cpp','scene_test.cpp']
        if variant=='combined': tests.append('primitive_test.cpp')
        for name in tests:
            content=(here/(name+'.in')).read_text().replace('@BASELINE@',str(source)).replace('@CANDIDATE@',str(target/'src'))
            test=target/name;test.write_text(content)
            binary=target/test.stem
            subprocess.run(['clang++','-std=c++17','-O2','-fsanitize=address,undefined','-DSPATIAL_HRTF_TAPS=64',str(test),'-o',str(binary)],check=True)
            subprocess.run([str(binary)],check=True)
    if args.arm_build:
        flags=(args.arm_build/'CMakeFiles/spatial_disorientation.dir/flags.make').read_text()
        options=[]
        for key in ('CXX_DEFINES','CXX_INCLUDES','CXX_FLAGS'):
            line=next(line for line in flags.splitlines() if line.startswith(key+' ='))
            options.extend(shlex.split(line.split('=',1)[1]))
        obj=target/'main.o'
        subprocess.run([args.arm_prefix+'g++',*options,'-c',str(target/'src/main.cpp'),'-o',str(obj)],check=True)
        (target/'main.asm').write_text(subprocess.check_output([args.arm_prefix+'objdump','-d','-C',str(obj)],text=True))
        print(variant,subprocess.check_output([args.arm_prefix+'nm','-S','-C',str(obj)],text=True).split('spatial::Source::Process(long)')[0].splitlines()[-1]+' Source::Process',flush=True)
print('All requested checks passed. No firmware was linked or packaged.',flush=True)
