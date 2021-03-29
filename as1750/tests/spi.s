	name spi
spi
	normal
	xio r0, enbl
	lim r0, #20ec
	xio r0, smk
	lim r0, #12f
	xio r0, spi
        lim r0, #7fff
	aisp r0, 1
	nop
	br  0
	end

