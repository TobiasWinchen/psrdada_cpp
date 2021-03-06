#include "psrdada_cpp/effelsberg/edd/EDDRoach_merge.hpp"
#include "ascii_header.h"
#include <immintrin.h>
#include <time.h>
#include <iomanip>
#include <cmath>

namespace psrdada_cpp {
namespace effelsberg {
namespace edd {

	uint64_t interleave(uint32_t x, uint32_t y) {
  	__m128i xvec = _mm_cvtsi32_si128(x);
	__m128i yvec = _mm_cvtsi32_si128(y);
 	__m128i interleaved = _mm_unpacklo_epi8(yvec, xvec);
	  return _mm_cvtsi128_si64(interleaved);
	}

    EDDRoach_merge::EDDRoach_merge(std::size_t nsamps_per_heap, std::size_t nchunck, DadaWriteClient& writer)
    : _nsamps_per_heap(nsamps_per_heap)
    , _nchunck(nchunck)
    , _writer(writer)
    {
    }

    EDDRoach_merge::~EDDRoach_merge()
    {
    }

    void EDDRoach_merge::init(RawBytes& block)
    {
        RawBytes& oblock = _writer.header_stream().next();
        if (block.used_bytes() > oblock.total_bytes())
	{
	    _writer.header_stream().release();
	    throw std::runtime_error("Output DADA buffer does not have enough space for header");
	}
        std::memcpy(oblock.ptr(), block.ptr(), block.used_bytes());
        char buffer[1024];
	ascii_header_get(block.ptr(), "SAMPLE_CLOCK_START", "%s", buffer);
	std::size_t sample_clock_start = std::strtoul(buffer, NULL, 0);
	ascii_header_get(block.ptr(), "CLOCK_SAMPLE", "%s", buffer);
	long double sample_clock = std::strtold(buffer, NULL);
	ascii_header_get(block.ptr(), "SYNC_TIME", "%s", buffer);
	long double sync_time = std::strtold(buffer, NULL);
        BOOST_LOG_TRIVIAL(debug) << "this is sample_clock_start "<< sample_clock_start;
        BOOST_LOG_TRIVIAL(debug)<< "this is sample_clock "<< sample_clock;
        BOOST_LOG_TRIVIAL(debug) << "this is sync_time "<< sync_time;
	BOOST_LOG_TRIVIAL(debug) << "this is sample_clock_start / sample_clock "<< sample_clock_start / sample_clock;
	long double unix_time = sync_time + (sample_clock_start / sample_clock);
	long double mjd_time = unix_time / 86400 - 40587.5;
	char time_buffer[80];
	std::time_t unix_time_int;
	struct std::tm * timeinfo;
	double fractpart, intpart;
	fractpart = std::modf (static_cast<double>(unix_time) , &intpart);
	unix_time_int = static_cast<std::time_t>(intpart);
	timeinfo = std::gmtime (&unix_time_int);
	std::strftime(time_buffer, 80, "%Y-%m-%d-%H:%M:%S", timeinfo);
	std::stringstream utc_time_stamp;
	BOOST_LOG_TRIVIAL(debug) << "unix_time" << unix_time;
	BOOST_LOG_TRIVIAL(debug) << "fractional part " << fractpart;
        //BOOST_LOG_TRIVIAL(debug) << "fractional part ." << static_cast<std::size_t>(fractpart*10000000000);
        //utc_time_stamp<< time_buffer << "." <<fractpart;
	utc_time_stamp<< time_buffer << "." << std::setw(10) << std::setfill('0') << std::size_t(fractpart*10000000000) << std::setfill(' ');
	//BOOST_LOG_TRIVIAL(debug) << "fractional part" <<static_cast<std::size_t>(fractpart * 10000000000);
	//utc_time_stamp<< time_buffer << "." << static_cast<std::size_t>(fractpart * 10000000000);
	BOOST_LOG_TRIVIAL(info) << "this is start time in utc "<< utc_time_stamp.str().c_str()<< "\n";
//	std::cout << "this is sync_time MJD "<< mjd_time<< "\n";
	ascii_header_set(oblock.ptr(), "UTC_START", "%s", utc_time_stamp.str().c_str());
	ascii_header_set(oblock.ptr(), "UNIX_TIME", "%Lf", unix_time);
	oblock.used_bytes(oblock.total_bytes());
        _writer.header_stream().release();
    }

    bool EDDRoach_merge::operator()(RawBytes& block)
    {
	BOOST_LOG_TRIVIAL(info) << "nchucnk "<< _nchunck << "\n";
	RawBytes& oblock = _writer.data_stream().next();
        std::size_t bytes_per_chunk= 32;
        std::size_t heap_size = 262144;
        std::size_t nbands = _nchunck;
        #pragma omp parallel for
        for (std::size_t xx=0; xx< block.used_bytes()/_nchunck/heap_size; xx++)
        {
	    	std::vector<char*> ptrs(_nchunck);
		for (std::size_t ii=0; ii<_nchunck; ++ii)
		{
	    		ptrs[ii] = block.ptr() + xx * nbands * heap_size + ii * heap_size;
		}
       		const char *target = oblock.ptr() + xx * nbands * heap_size;

        	for (std::size_t yy=0; yy< heap_size/bytes_per_chunk; yy++)
        	{
            		for (std::size_t ii=0; ii<_nchunck; ++ii)
			{
            	    		std::memcpy((void*)target, (void*)ptrs[ii], bytes_per_chunk);
            	    		ptrs[ii] += bytes_per_chunk;
                    		target += bytes_per_chunk;
			}
        	}
        }

	//std::memcpy(oblock.ptr(), block.ptr(), block.used_bytes());
	oblock.used_bytes(block.used_bytes());
	_writer.data_stream().release();
        return false;
    }
}//edd
}//effelsberg
}//psrdada_cpp

