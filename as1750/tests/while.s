	macro xchg
		xwr r`1`,r`2`
	endmacro

	macro doit
		ab	r14,14
cnt1		set	1
		while `cnt1` < `1`
			lim	r`cnt1`,`1`
cnt2			set	`cnt1`
			while `cnt2` > 0
				lim	r`cnt1`,`2`,r`cnt2`
cnt2				set	`cnt2` - 2
			endwhile
			xchg	`cnt1`,`3`
cnt1			set	`cnt1` + 1
		endwhile
		mb	r15,15
	endmacro

	lisp r0,1
	doit 10, 7, 14
	lisp r15,15
	end
