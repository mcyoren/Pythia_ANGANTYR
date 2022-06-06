#!/usr/bin/python3.8

import os
import sys
from time import sleep


options = ['SoftQCD:all=on ', 'SoftQCD:inelastic=on ', 'HardQCD:allwdi=on ', 'HardQCD:all=on ',#0-3
           'HardQCD:onlydi=on ', 'PromptPhoton:all=on ', 'PhaseSpace:pTHatMin=2. ', 'PDF:pSet=8 ',#4-7
           'PDF:pSetB=3 ', 'MultipartonInteractions:Kfactor=0.5 ', 'PDF:pSet=7 ', 'PDF:pSet=5 ', 'PDF:pSet=13 ',#8-12
           'PDF:pSet=14 ', 'PDF:pSet=4 ', 'PDF:pSet=10 ', 'PDF:pSet=18 ', 'PDF:pSet=12 ', 'PDF:pSet=9 ',#13-18
           'PDF:pSet=17 ', 'MultipartonInteractions:ecmRef=200 ', 'MultipartonInteractions:bprofile=2 ',  #19-21
           'MultipartonInteractions:pT0Ref=1.4 ', 'MultipartonInteractions:ecmPow=0.135 ',   # 22 - 23
           'MultipartonInteractions:coreRadius=0.56 ', 'MultipartonInteractions:coreFraction=0.78 ', #24-25
           'ColourReconnection:range=5.4 '] #26
names = ['soft', 'inelastic', 'allwdi', 'hard', 'onlydi', 'promt', 'pthat', 'nPDFSet', 'nPDFSetB', 'Kfac',#0-9
        'nPDFSet7', 'nPDFSet5', 'nPDFSet13', 'nPDFSet14', 'nPDFSet4', 'nPDFSet10', 'nPDFSet18', 'nPDFSet12', 'nPDFSet9',#10-18
         'tune', '', '', '', '', '', '', '']#19-26
system = ['pp1', 'pAl1', 'pAu1', 'dAu1', 'HeAu1']
triggers = ['', 'trig']
projectile = [2212, 2212, 2212, 1000010020, 1000020030]
target = [2212, 1000130270, 1000791970, 1000791970, 1000791970]
opt = [0, 7, 9] #7, 10, 15, 13; 16 17 --- 18
syst = 4
trigs = 0
option = ''
name = ''
for i in opt:
    option = option + options[i]
    name = name + names[i]

nEvents = 4e4
name = name + '_pip_syst'

file = 'main113_pAu'
file_out = 'SmallSystems/'+system[syst]+'/'+system[syst]+name+triggers[trigs]
os.system('make ' + file)

n_threads = 25
start = 941020
for i in range(0, n_threads):
    os.system(f'./{file} {start + i * 10000} {i} {n_threads} {nEvents} {file_out} {projectile[syst]} '
              f'{target[syst]} {trigs} {len(opt)} {option} &')

try:
    while True:
        sleep(10)
        print("yolo")
        ss = os.popen('ps -fu | grep '+file).read()
        if ss.find('./'+file) == -1:
            break
    os.system('rm ' + file_out + '.root')
except:
    print("first try "*10)
if n_threads > 1:
    os.system('hadd ' + file_out + '.root ' + file_out + '_*' ' && rm ' + file_out + '_*')
print("\a"*10)