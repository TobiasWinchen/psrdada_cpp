#ifndef PSRDADA_CPP_DOUBLE_BUFFER_HPP
#define PSRDADA_CPP_DOUBLE_BUFFER_HPP

namespace psrdada_cpp {

/**
 * @brief      Class for double buffer.
 *
 * @tparam     T     The internal data type for the buffers
 *
 * @detail     An implementation of the double buffer concept
 *             using thrust::device_vector. Provides double
 *             buffers in GPU memory.
 *
 *             This class exposes two buffers "a" and "b"
 *             which can be independently accessed. The buffers
 *             can be swapped to make b=a and a=b which is useful
 *             for staging inputs and outputs in a streaming pipeline
 */
template <typename VectorType>
class DoubleBuffer
{
public:
    /**
     * @brief      Constructs the object.
     */
    DoubleBuffer();
    ~DoubleBuffer();
    DoubleBuffer(DoubleBuffer const&) = delete;

    /**
     * @brief      Resize the buffer
     *
     * @param[in]  size  The desired size in units of the data type
     */
    void resize(std::size_t size);

    /**
     * @brief      Resize the buffer
     *
     * @param[in]  size        The desired size in units of the data type
     * @param[in]  fill_value  The fill value for newly allocated memory
     */
    void resize(std::size_t size, typename VectorType::value_type fill_value);

    /**
     * @brief   Return the size of the buffer in elements
     */
    std::size_t size() const;

    /**
     * @brief      Swap the a and b buffers
     */
    void swap();

    /**
     * @brief      Return a reference to the "a" vector
     */
    VectorType& a();

    /**
     * @brief      Return a reference to the "b" vector
     */
    VectorType& b();

    /**
     * @brief      Return a pointer to the contents of the "a" vector
     */
    typename VectorType::value_type* a_ptr();

    /**
     * @brief      Return a pointer to the contents of the "b" vector
     */
    typename VectorType::value_type* b_ptr();

private:
    VectorType _buf0;
    VectorType _buf1;
};

} //namespace psrdada_cpp

#include "psrdada_cpp/detail/double_buffer.cu"

#endif //PSRDADA_CPP_DOUBLE_DEVICE_BUFFER_HPP
