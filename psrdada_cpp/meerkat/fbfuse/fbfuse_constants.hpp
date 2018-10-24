#ifndef PSRDADA_CPP_MEERKAT_FBFUSE_CONSTANTS_HPP
#define PSRDADA_CPP_MEERKAT_FBFUSE_CONSTANTS_HPP

#define FBFUSE_CB_TSCRUNCH 16
#define FBFUSE_CB_FSCRUNCH 1
#define FBFUSE_CB_NANTENNAS 42
#define FBFUSE_CB_ANTENNA_OFFSET 0
#define FBFUSE_CB_NBEAMS 1024
#define FBFUSE_IB_TSCRUNCH 16
#define FBFUSE_IB_FSCRUNCH 1
#define FBFUSE_IB_NANTENNAS 64
#define FBFUSE_IB_ANTENNA_OFFSET
#define FBFUSE_IB_NBEAMS 1
#define FBFUSE_TOTAL_ANTENNAS 64
#define FBFUSE_NCHANS 256
#define FBFUSE_NSAMPLES_PER_HEAP 256
#define FBFUSE_NPOL 2
#define FBFUSE_CB_NTHREADS 1024
#define FBFUSE_CB_WARP_SIZE 32
#define FBFUSE_CB_NSAMPLES_PER_BLOCK (FBFUSE_CB_TSCRUNCH * FBFUSE_CB_NTHREADS/FBFUSE_CB_WARP_SIZE);

#endif //PSRDADA_CPP_MEERKAT_FBFUSE_CONSTANTS_HPP