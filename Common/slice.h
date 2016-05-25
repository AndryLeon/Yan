
#ifndef YAN_SLICE_H
#define YAN_SLICE_H

#include <string>
#include <string.h>

namespace Yan{

    namespace Common{

        class Slice{
        public:
            Slice() : data_(""), size_(0){}
            Slice(const char* d, size_t n) : data_(d), size_(n){}
            Slice(const std::string& s) : data_(s.c_str()), size_(s.size()) {}

            const char* Data() const{
                return data_;
            }

            size_t GetSize() const{
                return size_;
            }

            void Clear(){
                data_ = "";
                size_ = 0;
            }

            std::string ToString() const{
                return std::string(data_, size_);
            }

        private:
            const char* data_;
            size_t  size_;
        };
    }

}

#endif //YAN_SLICE_H
