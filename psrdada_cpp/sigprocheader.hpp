#ifndef PSRDADA_CPP_SIGPROCHEADER_HPP
#define PSRDADA_CPP_SIGPROCHEADER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include"psrdada_cpp/psrdadaheader.hpp"
#include"psrdada_cpp/raw_bytes.hpp"

/* @detail: A SigProc Header writer class. This class will parse values
 *          from a PSRDADA header object and write that out as a standard
 *          SigProc format. This is specific for PSRDADA stream.
 */

namespace psrdada_cpp
{

class SigprocHeader
{
public:
    SigprocHeader();
    ~SigprocHeader();
    void write_header(RawBytes& block,PsrDadaHeader ph);

    std::size_t header_size() const;

private:
    std::size_t _header_size;
    /*
     * @brief write string to the header
     */
    void header_write(char*& ptr, std::string const& str);
    void header_write(char*& ptr, std::string const& str, std::string const& name);

    /*
     * @brief write a value to the stream
     */
    template<typename NumericT>
    void header_write(char*& ptr, std::string const& name, NumericT val);

};

} // namespace psrdada_cpp
#include "psrdada_cpp/detail/sigprocheader.cpp"
#endif //PSRDADA_CPP_SIGPROCHEADER_HPP
