	name trigtest
	global	fsqrt,fsin,fcos,puts,pr_fp_num  ;imports
	
	konst
pi	dataf	3.1415926
eight	dataf	8.0
zero	dataf	0.0
spc	datac	"   ",0
sine	datac	" => sine: ",0
cosine	datac	" ;  cosine: ",0
eol	datac   10,13,0

	static
pi_8	block	2
input	block	2

	normal
trigtest
	dl	r0,pi
	fd	r0,eight
	dst	r0,pi_8
	dl	r0,zero
	dst	r0,input
loop
	dl	r0,input
	sjs	r15,pr_fp_num
	lim	r0,sine
	sjs	r15,puts
	dl	r0,input
	sjs	r15,fsin
	sjs	r15,pr_fp_num
	lim	r0,cosine
	sjs	r15,puts
	dl	r0,input
	sjs	r15,fcos
	sjs	r15,pr_fp_num
	lim	r0,eol
	sjs	r15,puts
	dl	r0,input
	fa	r0,pi_8
	dst	r0,input
	fc	r0,pi
	ble	loop
	urs	r15

	end

