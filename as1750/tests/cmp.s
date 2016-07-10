	name cmp
cmp	lim	r0,-32768
	cim	r0,-32768
	bez	go_on
	bpt
go_on	lim	r2,0x8000
	dcr	r0,r2
	bez okay
	bpt
okay	br	0
	end
