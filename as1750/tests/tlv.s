	name tlv
	nolist
	include "ms1750.inc"
	list

	global	__main



	konst

	Konst
Kxg.2
	dataef	0.036000
	Static
yg.3 	block   3	
	Konst
Kyl.4
	dataef	0.070000
	Static
ve.5 	block   9	
somx.6 	block   3	
fbt.7 	block   3	
iwz1.8 	block   3	
iwz2.9 	block   3	
isig.10 	block   1	
ls.11 	block   1	
ms.12 	block   1	
ima.13 	block   1	
laq.14 	block   3	
	Konst
Kkmax.15
	data	10
	global  fool_gcc	
	global  ns	
	global  inistc	
	global  omx	
	global  star	
	global  dvecp	
	global  lik	
	global  idsf	
	global  stc	
	global  aword	
	global  marg	
	global  idfstat	
	global  stcstat	
	global  dmamul	
	global  dcmr	
	global  dscalp	
	global  omrm	
	global  tis	
	global  zsmin	
	global  fabs	
	global  abvt	
	global  sign	
	global  dte	
	global  issc	
	global  dtmin	
	global  dtmax	
	global  cword	
	global  dword	
	global  bword	
	global  isrw	
	global  putbits	
	global  iwz2max	
	global  ist	
	global  rm	
	global  dmamax	
	global  ym	
	global  psc	
	global  xm	
	global  daqmax	
	global  iaqf	
	global  getbits	
	global  iwz1max	
	global  nrs	
	konst
KLC0
	dataef	0.000150
	konst
KLC1
	dataef	-0.051000
	konst
KLC2
	dataef	0.000000
	konst
KLC3
	dataef	0.000255
	konst
KLC4
	dataef	0.045000

	normal

	global  lvlhstcmd	
lvlhstcmd

	sim	r15,73
	pshm	r14,r14
	lr	r14,r15
	stc	1,39,r14
	lisp	r2,3
	stb	r14,41
	stb	r14,42
	lim	r7,42,r14
	lim	r6,41,r14
	lim	r5,40,r14
	lim	r4,39,r14
	lim	r3,38,r14
	lim	r2,37,r14
	lim	r1,ls.11
	lim	r0,ns
	sjs	r15,fool_gcc
	l	r2,inistc
	jc	le,L3
	efl	r2,omx
	jc	ez,L2
	efm	r2,somx.6
	jc	gt,L2
L3
	l	r2,ns
	jc	ge,L4
	aisp	r2,3
L4
	sra	r2,2
	stb	r14,40
	jc	gt,L5
	stc	1,40,r14
L5
	stc	0,70,r14
	lim	r7,1,r14
	lim	r8,18
	lim	r6,star
	l	r0,40,r14
	lr	r2,r0
	sll	r2,1
	ar	r2,r0
	sll	r2,2
	lr	r0,r2
	ar	r0,r6
L9
	lb	r14,70
	sll	r2,1
	ab	r14,70
	lr	r5,r2
	ar	r5,r7
	lr	r3,r5
	ar	r3,r8
	lr	r1,r2
	ar	r1,r6
	efl	r9,0,r1
	efst	r9,0,r3
	ar	r2,r0
	efl	r2,0,r2
	efs	r2,18,r5
	efst	r2,27,r5
	incm	1,70,r14
	l	r10,70,r14
	cisp	r10,2
	ble	L9
	lim	r2,ve.5
	lim	r1,28,r14
	lim	r0,19,r14
	sjs	r15,dvecp
	stc	0,70,r14
L14
	lisn	r12,1
	l	r11,70,r14
	st	r12,lik,r11
	stc	0,idsf,r11
	stc	0,iwz2.9,r11
	stc	0,fbt.7,r11
	incm	1,70,r14
	l	r9,70,r14
	cisp	r9,2
	ble	L14
	stc	1,isig.10
	efl	r2,omx
	jc	ge,L16
	lisn	r10,1
	st	r10,isig.10
L16
	l	r2,isig.10
	dsra	r2,16
	eflt	r2,r2
	efm	r2,LC1
	efst	r2,yg.3
	efl	r9,omx
	efst	r9,somx.6
	stc	0,ls.11
	stc	1,ms.12
	l	r7,marg
	xorr	r6,r6
	efl	r3,omx
	lim	r2,fbt.7
	lisp	r1,6
	lisp	r0,1
	sjs	r15,aword
	st	r0,15+stc
	stc	1,14+stc
	stc	0,idfstat
	stc	1,inistc
L2
	stc	0,stcstat
	stc	0,70,r14
	l	r10,70,r14
	st	r10,51,r14
	lim	r11,1,r14
	st	r11,54,r14
L20
	l	r12,70,r14
	l	r0,idsf,r12
	jc	nz,L21
	l	r2,lik,r12
	jc	ge,L22
	l	r9,51,r14
	jc	nz,L19
	stc	1,51,r14
	lr	r13,r0
	l	r10,51,r14
	st	r10,49,r14
	l	r3,41,r14
	l	r5,39,r14
	lr	r4,r3
	lb	r14,54
	aim	r2,27
	lim	r1,ve.5
	lim	r0,dcmr
	st	r13,64,r14
	sjs	r15,dmamul
	lb	r14,41
	lim	r1,omrm
	lim	r0,28,r14
	sjs	r15,dscalp
	l	r13,64,r14
	dlr	r0,r0
	jc	ge,L25
	lisn	r11,1
	st	r11,49,r14
L25
	aisp	r13,1
	lb	r14,49
	a	r2,ls.11
	st	r2,ls.11
	jc	gt,L28
	l	r12,ns
	st	r12,ls.11
	jc	15,L29
L28
	l	r2,ls.11
	c	r2,ns
	jc	le,L29
	stc	1,ls.11
L29
	xorr	r4,r4
	c	r4,42,r14
	jc	ge,L32
	l	r0,ls.11
	sisp	r0,1
	lr	r2,r0
	sll	r2,1
	ar	r2,r0
	sll	r2,2
	lim	r0,star,r2
L34
	lr	r2,r4
	sll	r2,1
	ar	r2,r4
	l	r1,54,r14
	ar	r1,r2
	ar	r2,r0
	efl	r9,0,r2
	efst	r9,0,r1
	aisp	r4,1
	c	r4,42,r14
	blt	L34
L32
	lb	r14,42
	lim	r1,1,r14
	lim	r0,18+tis
	st	r13,64,r14
	sjs	r15,dscalp
	eflr.m	3,0
	stc	1,69,r14
	l	r13,64,r14
	efc	r3,zsmin
	jc	le,L36
	lb	r14,42
	lim	r1,1,r14
	lim	r0,tis
	efst	r3,56,r14
	st	r13,64,r14
	sjs	r15,dscalp
	efl	r3,56,r14
	efdr	r0,r3
	efst	r0,43,r14
	lb	r14,42
	lim	r1,1,r14
	lim	r0,9+tis
	sjs	r15,dscalp
	efl	r3,56,r14
	eflr.m	6,0
	efdr	r6,r3
	l	r2,isig.10
	dsra	r2,16
	eflt	r2,r2
	efmr	r2,r6
	efa	r2,yl.4
	l	r13,64,r14
	dlr	r2,r2
	jc	le,L36
	stc	0,69,r14
L36
	l	r2,ms.12
	jc	nz,L38
	l	r10,69,r14
	cisp	r10,1
	jc	nz,L38
	efl	r2,43,r14
	jc	ge,L40
	efl	r2,LC2
	efs	r2,43,r14
L40
	efc	r2,xg.2
	jc	gt,L27
	l	r12,ls.11
	sisp	r12,1
	l	r11,70,r14
	st	r12,lik,r11
	lb	r14,70
	sll	r2,12
	aim	r2,4096
	st	r2,stcstat
	jc	15,L27
L38
	l	r9,69,r14
	st	r9,ms.12
L27
	c	r13,kmax.15
	jc	ge,L19
	l	r10,70,r14
	l	r2,lik,r10
	jc	lt,L25
	jc	15,L19
L22
	xorr	r4,r4
L48
	lr	r1,r4
	sll	r1,1
	ar	r1,r4
	l	r3,54,r14
	ar	r3,r1
	l	r11,70,r14
	l	r0,lik,r11
	lr	r2,r0
	sll	r2,1
	ar	r2,r0
	sll	r2,2
	aim	r2,star
	ar	r1,r2
	efl	r9,0,r1
	efst	r9,9,r3
	aisp	r4,1
	cisp	r4,2
	ble	L48
	l	r5,39,r14
	l	r4,41,r14
	lr	r3,r4
	lim	r2,1,r14
	lim	r1,10,r14
	lim	r0,tis
	sjs	r15,dmamul
	stc	0,65,r14
	lim	r0,abvt
L53
	lb	r14,65
	sll	r2,1
	ab	r14,65
	l	r1,54,r14
	ar	r1,r2
	ar	r2,r0
	efl	r3,0,r1
	efa	r3,0,r2
	efst	r3,0,r1
	incm	1,65,r14
	l	r10,65,r14
	cisp	r10,2
	ble	L53
	efl	r2,1,r14
	efd	r2,7,r14
	efst	r2,43,r14
	efl	r2,4,r14
	eflr.m	6,2
	efd	r6,7,r14
	efl	r9,43,r14
	efst	r9,46,r14
	efl	r2,46,r14
	dlr	r9,r9
	jc	ge,L56
	efl	r2,LC2
	efs	r2,46,r14
L56
	efc	r2,xg.2
	jc	le,L55
	efl	r3,43,r14
	efl	r0,xg.2
	efst	r6,59,r14
	sjs	r15,sign
	efst	r0,46,r14
	efl	r6,59,r14
L55
	efl	r2,yg.3
	efsr	r2,r6
	l	r5,isig.10
	dsra	r5,16
	eflt	r5,r5
	efmr	r2,r5
	efst	r2,dte
	l	r2,issc
	jc	nz,L57
	efl	r5,omx
	jc	ge,L58
	efl	r2,LC2
	efsr	r2,r5
	eflr.m	5,2
L58
	eflr.m	2,5
	efm	r2,dtmin
	efl	r10,dte
	efcr	r10,r2
	jc	lt,L57
	efl	r5,omx
	jc	ge,L59
	efl	r2,LC2
	efsr	r2,r5
	eflr.m	5,2
L59
	eflr.m	2,5
	efm	r2,dtmax
	efl	r9,dte
	efcr	r9,r2
	jc	gt,L57
	l	r2,14+stc
	cisp	r2,4
	jc	gt,L57
	l	r10,70,r14
	stc	1,fbt.7,r10
	l	r0,lik,r10
	lr	r2,r0
	sll	r2,1
	ar	r2,r0
	sll	r2,2
	efl	r2,9+star,r2
	efar	r2,r2
	efix	r2,r2
	l	r0,marg
	aisp	r0,3
	lr	r2,r3
	ar	r2,r0
	sll	r2,3
	cisp	r2,15
	jc	le,L60
	lisp	r2,15
L60
	efl	r3,46,r14
	efd	r3,LC3
	efix	r3,r3
	l	r1,70,r14
	lr	r0,r4
	sjs	r15,cword
	lim	r8,15+stc
	l	r2,14+stc
	ar	r2,r8
	st	r0,0,r2
	l	r1,70,r14
	xorr	r0,r0
	st	r8,61,r14
	sjs	r15,dword
	l	r8,61,r14
	lim	r2,1,r8
	a	r2,14+stc
	st	r0,0,r2
	xorr	r1,r1
	l	r0,isrw
	sjs	r15,bword
	l	r8,61,r14
	lim	r2,2,r8
	a	r2,14+stc
	st	r0,0,r2
	l	r7,marg
	xorr	r6,r6
	efl	r3,omx
	lim	r2,fbt.7
	lisp	r1,6
	lisp	r0,1
	sjs	r15,aword
	l	r8,61,r14
	aisp	r8,3
	l	r2,14+stc
	ar	r2,r8
	st	r0,0,r2
	incm	4,14+stc
	l	r11,70,r14
	stc	1,idsf,r11
	stc	0,iwz1.8,r11
	l	r0,70,r14
	aisp	r0,1
	sll	r0,12
	st	r0,stcstat
	lisp	r3,12
	xorr	r2,r2
	l	r1,lik,r11
	aisp	r1,1
	sjs	r15,putbits
	st	r0,stcstat
	jc	15,L19
L57
	efl	r2,dte
	jc	ge,L19
	lisn	r9,1
	l	r12,70,r14
	st	r9,lik,r12
	stc	0,idsf,r12
	stc	0,iwz2.9,r12
	stc	0,fbt.7,r12
	l	r7,marg
	xorr	r6,r6
	efl	r3,omx
	lim	r2,fbt.7
	lisp	r1,6
	lisp	r0,1
	sjs	r15,aword
	l	r2,14+stc
	aim	r2,15+stc
	st	r0,0,r2
	incm	1,14+stc
	jc	15,L19
L21
	l	r10,70,r14
	l	r2,iwz2.9,r10
	c	r2,iwz2max
	jc	ge,L64
	l	r2,ist,r10
	cisp	r2,2
	jc	nz,L65
	lr	r3,r10
	sll	r3,1
	a	r3,70,r14
	l	r2,lik,r10
	lr	r1,r2
	sll	r1,1
	ar	r1,r2
	sll	r1,2
	efl	r2,rm,r3
	eflr.m	5,2
	efs	r5,9+star,r1
	jc	ge,L67
	efl	r2,LC2
	efsr	r2,r5
	eflr.m	5,2
L67
	efc	r5,dmamax
	jc	ge,L66
	l	r11,70,r14
	stc	2,idsf,r11
	stc	15,iwz2.9,r11
	jc	15,L19
L66
	l	r12,70,r14
	incm	1,iwz2.9,r12
	stc	1,idsf,r12
	jc	15,L19
L65
	l	r9,70,r14
	l	r2,ist,r9
	cisp	r2,3
	jc	nz,L70
	lr	r2,r9
	sll	r2,1
	ab	r14,70
	efl	r5,ym,r2
	jc	ge,L71
	efl	r2,LC2
	efsr	r2,r5
	eflr.m	5,2
L71
	efc	r5,LC4
	jc	gt,L130
L70
	l	r9,70,r14
	incm	1,iwz2.9,r9
	stc	1,idsf,r9
	jc	15,L19
L64
	l	r2,14+stc
	cisp	r2,7
	jc	gt,L19
L130
	l	r10,70,r14
	stc	0,fbt.7,r10
	l	r7,marg
	xorr	r6,r6
	efl	r3,omx
	lim	r2,fbt.7
	lisp	r1,6
	lisp	r0,1
	sjs	r15,aword
	l	r2,14+stc
	aim	r2,15+stc
	st	r0,0,r2
	incm	1,14+stc
	l	r11,70,r14
	stc	0,idsf,r11
	lisn	r12,1
	st	r12,lik,r11
	stc	0,iwz2.9,r11
L19
	incm	1,70,r14
	l	r9,70,r14
	cisp	r9,2
	jc	le,L20
	stc	0,70,r14
	l	r10,70,r14
	st	r10,50,r14
	lim	r11,1,r14
	st	r11,55,r14
L79
	l	r12,70,r14
	l	r2,idsf,r12
	cisp	r2,1
	jc	le,L80
	incm	1,50,r14
	xorr	r4,r4
	c	r4,42,r14
	jc	ge,L82
L84
	lr	r1,r4
	sll	r1,1
	ar	r1,r4
	l	r3,55,r14
	ar	r3,r1
	l	r9,70,r14
	l	r0,lik,r9
	lr	r2,r0
	sll	r2,1
	ar	r2,r0
	sll	r2,2
	aim	r2,star
	ar	r1,r2
	efl	r10,0,r1
	efst	r10,9,r3
	aisp	r4,1
	c	r4,42,r14
	blt	L84
L82
	l	r5,39,r14
	l	r4,41,r14
	lr	r3,r4
	lim	r2,1,r14
	lim	r1,10,r14
	lim	r0,tis
	sjs	r15,dmamul
	l	r1,70,r14
	sll	r1,3
	a	r1,70,r14
	efl	r2,1,r14
	efd	r2,7,r14
	efst	r2,psc,r1
	efl	r2,4,r14
	efd	r2,7,r14
	efst	r2,3+psc,r1
	l	r11,70,r14
	l	r0,lik,r11
	lr	r2,r0
	sll	r2,1
	ar	r2,r0
	sll	r2,2
	efl	r9,9+star,r2
	efst	r9,6+psc,r1
L80
	l	r10,70,r14
	stc	0,laq.14,r10
	incm	1,70,r14
	l	r11,70,r14
	cisp	r11,2
	ble	L79
	stc	2,70,r14
	stc	0,65,r14
L90
	l	r13,65,r14
	aisp	r13,1
	cisp	r13,2
	jc	gt,L89
	lb	r14,65
	sll	r2,1
	ab	r14,65
	stb	r14,52
	lim	r12,xm,r2
	st	r12,53,r14
	l	r0,65,r14
	sll	r0,3
	a	r0,65,r14
L94
	l	r9,65,r14
	l	r2,idsf,r9
	cisp	r2,1
	jc	le,L95
	l	r2,idsf,r13
	cisp	r2,1
	jc	le,L95
	lr	r1,r13
	sll	r1,1
	ar	r1,r13
	lim	r2,xm,r1
	l	r11,53,r14
	efl	r10,0,r11
	efs	r10,0,r2
	efst	r10,66,r14
	efm	r10,66,r14
	efst	r10,66,r14
	lim	r2,ym
	l	r3,52,r14
	ar	r3,r2
	ar	r1,r2
	efl	r2,0,r3
	efs	r2,0,r1
	efmr	r2,r2
	efa	r2,66,r14
	efst	r2,66,r14
	lr	r1,r13
	sll	r1,3
	ar	r1,r13
	lr	r12,r0
	efl	r5,psc,r12
	efs	r5,psc,r1
	efmr	r5,r5
	efl	r2,3+psc,r12
	efs	r2,3+psc,r1
	efmr	r2,r2
	efar	r5,r2
	efl	r9,66,r14
	efsr	r9,r5
	efst	r9,66,r14
	efm	r9,66,r14
	efst	r9,66,r14
	efdr	r9,r5
	efst	r9,66,r14
	jc	ge,L97
	efl	r2,LC2
	efs	r2,66,r14
	efst	r2,66,r14
L97
	efl	r10,66,r14
	efc	r10,daqmax
	jc	ge,L95
	l	r11,70,r14
	stc	1,laq.14,r11
L95
	decm	1,70,r14
	aisp	r13,1
	cisp	r13,2
	ble	L94
L89
	incm	1,65,r14
	l	r12,65,r14
	cisp	r12,1
	jc	le,L90
	l	r0,laq.14
	sll	r0,2
	l	r2,1+laq.14
	sll	r2,1
	ar	r2,r0
	a	r2,2+laq.14
	l	r9,50,r14
	cisp	r9,1
	jc	le,L100
	l	r2,iaqf,r2
	stb	r14,37
	stc	0,70,r14
L104
	lb	r14,70
	sll	r2,1
	lisp	r1,5
	sr	r1,r2
	st	r1,38,r14
	lisp	r2,1
	l	r0,37,r14
	sjs	r15,getbits
	l	r10,70,r14
	a	r0,idsf,r10
	st	r0,idsf,r10
	cisp	r0,2
	jc	nz,L103
	l	r2,iwz1.8,r10
	c	r2,iwz1max
	jc	ge,L106
	incm	1,iwz1.8,r10
	jc	15,L103
L106
	l	r11,70,r14
	stc	0,idsf,r11
	lisn	r12,1
	st	r12,lik,r11
	stc	0,iwz2.9,r11
L103
	incm	1,70,r14
	l	r9,70,r14
	cisp	r9,2
	ble	L104
	jc	15,L109
L100
	stc	0,70,r14
	lim	r13,xm
	lim	r0,ym
L113
	l	r10,70,r14
	l	r2,idsf,r10
	cisp	r2,2
	jc	nz,L112
	lr	r2,r10
	sll	r2,1
	ab	r14,70
	ar	r2,r13
	lr	r1,r10
	sll	r1,3
	a	r1,70,r14
	efl	r2,0,r2
	efs	r2,psc,r1
	efst	r2,66,r14
	efc	r2,LC2
	jc	ge,L115
	efl	r9,LC2
	efs	r9,66,r14
	efst	r9,66,r14
L115
	lb	r14,70
	sll	r2,1
	ab	r14,70
	ar	r2,r0
	l	r1,70,r14
	sll	r1,3
	a	r1,70,r14
	efl	r2,0,r2
	eflr.m	5,2
	efs	r5,3+psc,r1
	jc	ge,L116
	efl	r2,LC2
	efsr	r2,r5
	eflr.m	5,2
L116
	efl	r10,66,r14
	efc	r10,LC0
	jc	ge,L112
	efc	r5,LC0
	jc	ge,L112
	l	r11,70,r14
	stc	3,idsf,r11
L112
	incm	1,70,r14
	l	r12,70,r14
	cisp	r12,2
	ble	L113
L109
	stc	0,nrs
	stc	0,70,r14
L122
	l	r9,70,r14
	l	r2,idsf,r9
	cisp	r2,3
	jc	nz,L121
	incm	1,nrs
L121
	incm	1,70,r14
	l	r10,70,r14
	cisp	r10,2
	ble	L122
	efl	r9,omx
	efst	r9,somx.6
	stc	0,70,r14
L128
	lb	r14,70
	sll	r2,2
	lisp	r3,4
	lisp	r10,8
	sr	r10,r2
	lr	r2,r10
	l	r11,70,r14
	l	r1,idsf,r11
	l	r0,idfstat
	sjs	r15,putbits
	st	r0,idfstat
	incm	1,70,r14
	l	r12,70,r14
	cisp	r12,2
	ble	L128
	lisp	r3,12
	xorr	r2,r2
	l	r1,ls.11
	l	r0,stcstat
	sjs	r15,putbits
	st	r0,stcstat
	lr	r15,r14
	popm	r14,r14
	aim	r15,73
	urs	r15

	static
init_srel
xg.2 	block 3
yl.4 	block 3
kmax.15 	block 1
LC0 	block 3
LC1 	block 3
LC2 	block 3
LC3 	block 3
LC4 	block 3

	init
	lim	r0,init_srel
	lim	r1,22
	lim	r2,Kxg.2
	mov	r0,r2

	normal

	end
