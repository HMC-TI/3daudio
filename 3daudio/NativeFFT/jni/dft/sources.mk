sources = 	dftw-direct.c \
	indirect.c \
	vrank-geq1.c \
	problem.c \
	plan.c \
	nop.c \
	buffered.c \
	dftw-generic.c \
	dftw-genericbuf.c \
	zero.c \
	dftw-directsq.c \
	kdft.c \
	conf.c \
	rader.c \
	ct.c \
	generic.c \
	kdft-difsq.c \
	rank-geq2.c \
	indirect-transpose.c \
	kdft-dit.c \
	kdft-dif.c \
	solve.c \
	direct.c \
	bluestein.c 

LOCAL_SRC_FILES += $(sources:%=dft/%)