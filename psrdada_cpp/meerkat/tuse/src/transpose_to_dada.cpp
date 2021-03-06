#include "psrdada_cpp/meerkat/tuse/transpose_to_dada.hpp"
#include "psrdada_cpp/cli_utils.hpp"
#include <ctime>
#include <mutex>
#include <iostream>


namespace psrdada_cpp {
namespace meerkat {
namespace tuse {

namespace transpose{

    /*
     * @brief This is the actual block that performs the
     * transpose. The format is based on the heap format
     * of SPEAD2 packets. This can change in time
     */
    std::mutex MyMutex;
    void do_transpose(RawBytes& transposed_data, RawBytes& input_data,std::uint32_t nchans, std::uint32_t nsamples, std::uint32_t nfreq, std::uint32_t beamnum, std::uint32_t nbeams, std::uint32_t ngroups)
    {
        // make copies of arrays to be transposed
        size_t tocopy = ngroups * nsamples * nfreq * nchans;
        char *tmpindata = new char[tocopy / ngroups];
        char *tmpoutdata = new char[tocopy];
        size_t skipgroup = nchans * nsamples * nfreq * nbeams;
        size_t skipbeam = beamnum * nchans * nsamples * nfreq;
        size_t skipband = nchans * nsamples;
        size_t skipallchans = nchans * nfreq;
        // actual transpose
        for (unsigned int igroup = 0; igroup < ngroups; ++igroup)
        {
            memcpy(tmpindata, input_data.ptr() + skipbeam + igroup * skipgroup, tocopy / ngroups);

            for (unsigned int isamp = 0; isamp < nsamples; ++isamp)
            {
                for (unsigned int iband = 0; iband < nfreq; ++iband)
                {
                    memcpy(tmpoutdata + iband * nchans + isamp * skipallchans + igroup * tocopy/ngroups,
                        tmpindata + iband * skipband + isamp * nchans,
                        nchans * sizeof(char));
                } // BAND LOOP
            } // SAMPLES LOOP
        } // GROUP LOOP
        memcpy(transposed_data.ptr(), tmpoutdata, tocopy);
        delete [] tmpoutdata;
        delete [] tmpindata;
    }
} //transpose
} //tuse
} //meerkat
} //psrdada_cpp
