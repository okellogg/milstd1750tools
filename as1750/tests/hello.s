	global	hello
	global	puts  ; import
hello
	lim	r0,string
	sjs	r15,puts
	urs	r15

	konst
string	data	'H','e','l','l','o','!',10,0
	end
