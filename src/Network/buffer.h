
#ifndef YAN_BUFFER_H
#define YAN_BUFFER_H

#include "../Common/slice.h"
#include "../Common/log.h"
#include <string>
#include <vector>
#include <unistd.h>
#include <cassert>

namespace Yan{

    class Buffer {
    public:
        Buffer()
                : mem_(1024),
                  readIdx_(0),
                  writeIdx_(0){
        };

        int ReadFd(int fd, int* saved_errno){
            char buf[65536];
            int total = 0;
            while(true){
                int n = ::read(fd, buf, size_t(sizeof(buf)));
                if( -1 == n ){
                    *saved_errno = errno;
                    if( EAGAIN != errno){
                        LOG_WARN("Buffer::ReadFd, error : %s\n", strerror(errno));
                    }
                    return total > 0? total : n;
                }else if( 0 == n){
                    return n;
                }else{
                    assert( n > 0);
                    total += n;
                    Append(buf, n);
                }
            }
        }

        void Append(const char* data, size_t byteSize){
            resize(byteSize);
            std::copy(data, data+byteSize, begin()+writeIdx_);
            writeIdx_ += byteSize;
        }

        void ReadIndexInc(size_t n){
            assert(readIdx_ + n < writeIdx_);
            readIdx_ += n;
        }

        size_t ReadableBytes() const{
            return writeIdx_ - readIdx_;
        }

        size_t WritableBytes() const{
            return mem_.size() - writeIdx_;
        }

        std::string RetriveString() {
            std::string str(begin() + readIdx_, writeIdx_ - readIdx_);
            readIdx_ = 0;
            writeIdx_ = 0;
            return str;
        }

        const Common::Slice ToSlice() const {
            return Common::Slice(begin() + readIdx_, writeIdx_ - readIdx_);
        }

    private:
        void resize(size_t need){
            if(mem_.size() - readIdx_ >= need){
                return;
            }

            if(readIdx_ + mem_.size() - writeIdx_ >= need) {
                std::copy(begin() + readIdx_, begin() + writeIdx_, begin());
                writeIdx_ = writeIdx_ - readIdx_;
                readIdx_ = 0;
            }else{
                mem_.resize(writeIdx_ + need);
            }
        }


        char* begin() {
            return &*mem_.begin();
        }

        const char* begin() const{
            return &*mem_.begin();
        }

        std::vector<char> mem_;
        size_t readIdx_;
        size_t writeIdx_;
    };


}

#endif //YAN_BUFFER_H