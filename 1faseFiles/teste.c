#include <stdio.h>
#include"maq.h"
INSTR prog[] = {
Keys in the first if:['1']
This is op: PUSH
This is keys 2nd if: []
This is arg: 1
Tabsim depois de arg: {}
 {PUSH, 1},
Keys in the first if:['0']
This is op: STO
This is keys 2nd if: []
This is arg: 0
Tabsim depois de arg: {}
 {STO, 0},
Keys in the first if:['1']
This is op: STO
This is keys 2nd if: []
This is arg: 1
Tabsim depois de arg: {}
 {STO, 1},

Keys in the first if:['10']
This is op: PUSH
This is keys 2nd if: []
This is arg: 10
Tabsim depois de arg: {}
 {PUSH, 10},
Keys in the first if:['2']
This is op: STO
This is keys 2nd if: []
This is arg: 2
Tabsim depois de arg: {}
 {STO, 2},

This is the label: LOOP
This are the keys: ['RCL', '0']
Keys in the first if:['0']
This is op: RCL
This is keys 2nd if: []
This is arg: 0
Tabsim depois de arg: {'LOOP': 7}
 {RCL, 0},
Keys in the first if:['1']
This is op: RCL
This is keys 2nd if: []
This is arg: 1
Tabsim depois de arg: {'LOOP': 7}
 {RCL, 1},
Keys in the first if:[]
This is op: DUP
 {DUP, 0},
Keys in the first if:['0']
This is op: STO
This is keys 2nd if: []
This is arg: 0
Tabsim depois de arg: {'LOOP': 7}
 {STO, 0},
Keys in the first if:[]
This is op: ADD
 {ADD, 0},
Keys in the first if:[]
This is op: DUP
 {DUP, 0},
Keys in the first if:['1']
This is op: STO
This is keys 2nd if: []
This is arg: 1
Tabsim depois de arg: {'LOOP': 7}
 {STO, 1},
Keys in the first if:[]
This is op: PRN
 {PRN, 0},
Keys in the first if:['2']
This is op: RCL
This is keys 2nd if: []
This is arg: 2
Tabsim depois de arg: {'LOOP': 7}
 {RCL, 2},
Keys in the first if:['1']
This is op: PUSH
This is keys 2nd if: []
This is arg: 1
Tabsim depois de arg: {'LOOP': 7}
 {PUSH, 1},
Keys in the first if:[]
This is op: SUB
 {SUB, 0},
Keys in the first if:[]
This is op: DUP
 {DUP, 0},
Keys in the first if:['2']
This is op: STO
This is keys 2nd if: []
This is arg: 2
Tabsim depois de arg: {'LOOP': 7}
 {STO, 2},
Keys in the first if:['0']
This is op: PUSH
This is keys 2nd if: []
This is arg: 0
Tabsim depois de arg: {'LOOP': 7}
 {PUSH, 0},
Keys in the first if:[]
This is op: EQ
 {EQ, 0},
Keys in the first if:['LOOP']
This is op: JIF
This is keys 2nd if: []
This is arg: LOOP
Tabsim depois de arg: {'LOOP': 7}
This is arg if arg in tab: 7
 {JIF, 7},

Keys in the first if:[]
This is op: END
 {END, 0},
};
