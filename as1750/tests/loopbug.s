; command > prolo xmn.abs

; command > di 2b0 10

	global loopbug
	normal
loopbug
	org 0196
      XIO     R0,RCFR
      ST      R0,#1ab3
      LIM     R13,#ffff
      ST      R13,#d000
      LIM     R14,#ffe0
      ST      R14,#f800
      ST      R14,#1ab1
      XIO     R0,DSBL
      LIM     R15,#a2fe
      SJS     R15,#2c3
      LIM     R12,0
      XIO     R0,GO
      LIM     R0,#a000
      LIM     R2,#29d
      LIM     R1,#2aa
      SR      R1,R2
      MOV     R0,R2
      JS      R7,#a000
      LIM     R1,0
      LIM     R0,#1fff
      JS      R9,#2aa
      CIM     R4,0
      BEZ     3
      LIM     R12,#3233
      ST      R13,#1ab1
      LR      R13,R8
      STC     0,#a251
      STC     0,#a252
      STC     0,#a22a
      STC     0,#a22b
      JS      R8,#15af
      LR      R8,R13
      LIM     R1,0
      LIM     R2,#010
      LIM     R0,#2f0
      LIM     R3,#014

	org 02aa
      LIM     R2,0
      LR      R4,R0
      L       R3,0,R4
      XOR     R2,0,R1
      AIM     R1,#001
      CR      R0,R1
      BNZ     -5
      CR      R2,R3
      BNZ     5
      LIM     R4,0
      JC      UC,0,R9
      LIM     R4,#001
      ST      R3,#1ab4
      ST      R2,#1ab5
      JC      UC,0,R9
      PSHM    R0,R3
      LIM     R0,0
      ST      R0,#f001
      JS      R3,#2e8
      ST      R0,#f001
      JS      R3,#2e8
      ST      R0,#f001
      JS      R3,#2e8
      LIM     R0,#040
      ST      R0,#f001
      JS      R3,#2e8
      LIM     R0,#07e
      ST      R0,#f001
      JS      R3,#2e8
      LIM     R0,#035
      ST      R0,#f001
      ST      R0,#1ab2
      JS      R3,#2e8
      POPM    R0,R3
      URS     R15

	END

; command > br 1bc

; command > go
;         XIO     a000 <  > 4005

; Error 1 : go address >>>  stop :  BREAKPOINT at 01bc

; command > br 2b5

; command > go
; sim_arith: integer FIXOFL during ADD,  op0=32767 op1=1 res=32768
; sim_arith: integer FIXOFL during ADD,  op0=32767 op1=1 res=32768
; Interrupt : go address >>>  1 Level(s) interrupted (1)

