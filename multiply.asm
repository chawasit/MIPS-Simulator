	lw	0	2	mcand
	lw	0	4	mcomp
loop	lw	0	3	mplier
	nand	3	4	3
	nand	3	3	3
	beq	3	4	sum
	beq	0	0	icnt
sum	add	1	2	1
icnt	add	2	2	2
	add	4	4	4
	lw	0	6	cnt
	lw	0	7	one
	add	6	7	6
	sw	0	6	cnt
	lw	0	7	stop
	beq	6	7	exit
	beq	0	0	loop
exit	halt
mcand	.fill	30000
mplier	.fill	10383
mcomp	.fill	1
cnt	.fill	0
stop	.fill	16
one	.fill	1