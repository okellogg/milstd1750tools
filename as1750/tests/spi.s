	name spi
spi
	normal
	xio r0, enbl
	lim r0, 020ec
	xio r0, smk
	lim r0, 012f
	xio r0, spi
        lim r0, 07fff
	aisp r0, 1
	nop
	br  0
	end

