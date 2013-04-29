sources = 	tile2d.c \
	solver.c \
	tensor5.c \
	md5.c \
	align.c \
	hash.c \
	problem.c \
	tensor2.c \
	plan.c \
	md5-1.c \
	tensor8.c \
	scan.c \
	buffered.c \
	extract-reim.c \
	ops.c \
	cpy2d.c \
	tensor.c \
	timer.c \
	print.c \
	primes.c \
	pickdim.c \
	assert.c \
	alloc.c \
	tensor1.c \
	stride.c \
	rader.c \
	ct.c \
	minmax.c \
	iabs.c \
	planner.c \
	debug.c \
	cpy2d-pair.c \
	trig.c \
	solvtab.c \
	tensor7.c \
	cpy1d.c \
	tensor9.c \
	tensor4.c \
	twiddle.c \
	tensor3.c \
	kalloc.c \
	transpose.c \
	awake.c 

LOCAL_SRC_FILES += $(sources:%=kernel/%)