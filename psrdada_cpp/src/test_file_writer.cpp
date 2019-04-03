#include "psrdada_cpp/test_file_writer.hpp"

namespace psrdada_cpp {

    TestFileWriter::TestFileWriter(std::string filename, std::size_t filesize)
    : _basefilename(filename),
      _shptr(nullptr),
      _filesize(filesize),
      _filenum(0),
      _wsize(0)
    {
        _header = new char[4096];
        auto newfilename = _basefilename;
        _outfile.open(filename + std::to_string(_filenum) ,std::ifstream::out | std::ifstream::binary);
        ++_filenum;
        if (_outfile.is_open())
        {
            BOOST_LOG_TRIVIAL(debug) << "Opened file " << newfilename.append(std::to_string(_filenum));
        }
        else
        {
            std::stringstream stream;
            stream << "Could not open file " << newfilename.append(std::to_string(_filenum));
            throw std::runtime_error(stream.str().c_str());
        }
    }

    TestFileWriter::~TestFileWriter()
    {
        _outfile.close();
    }

    void TestFileWriter::init(RawBytes& block)
    {
    /* Find where the HEADER_END is */
        std::memcpy(_header, block.ptr(), _shptr->header_size());
        _outfile.write(block.ptr(), _shptr->header_size());
        block.used_bytes(block.total_bytes());
    }

    bool TestFileWriter::operator()(RawBytes& block)
    {
        if ( _filesize - _wsize >= block.total_bytes())
        {
            _outfile.write(block.ptr(), block.used_bytes());
            //This is specifying the number of bytes read.
            block.used_bytes(block.total_bytes());
            _wsize += block.used_bytes();
        }
        else
        {
            auto left_size = _filesize - _wsize;
            _outfile.write(block.ptr(), left_size);
            auto current_ptr = block.ptr() + left_size;
            _wsize = 0;
            _outfile.close();
            auto newfilename = _basefilename;
            std::cout << "FileName:" << newfilename << "\n" ;
            _outfile.open(newfilename + std::to_string(_filenum), std::ifstream::out | std::ifstream::binary);
            if (_outfile.is_open())
            {
                BOOST_LOG_TRIVIAL(debug) << "Opened file " << newfilename.append(std::to_string(_filenum));
            }
            else
            {
                std::stringstream stream;
                stream << "Could not open file " << newfilename.append(std::to_string(_filenum));
                throw std::runtime_error(stream.str().c_str());
                return true;
            }
            ++_filenum;
            _outfile.write(_header, _shptr->header_size());
            _outfile.write(current_ptr,block.total_bytes() - left_size);
            block.used_bytes(block.total_bytes());
            _wsize += block.total_bytes() - left_size;
        }
        return false;
    }

    void TestFileWriter::header(SigprocHeader& header)
    {
        _shptr.reset(&header);
    }

} //psrdada_cpp
