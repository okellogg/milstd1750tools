	global	putstr
	global	putchar  ; import
putstr
	lr	r1,r0	; because r0 can't be index reg
loop
	l	r0,0,r1
	bez	done
	sjs	r15,putchar
	aisp	r1,1
	br	loop
done
	urs	r15
	end
