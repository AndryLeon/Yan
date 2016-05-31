
#ifndef YAN_TIMESTAMP_H
#define YAN_TIMESTAMP_H

#include <stdint.h>
#include <string>
#include <sys/time.h>
#include <string.h>
#include <cinttypes>

namespace Yan{

    namespace Common{

        class Timestamp{
        public:
            static const int kMicroSecondPerSecond = 1000000;

            static Timestamp Now(){
                /*
                struct timeval
                {
                    __time_t tv_sec;
                    __suseconds_t tv_usec;
                };
                */

                struct timeval tv;
                gettimeofday(&tv, NULL);

                return Timestamp(tv.tv_sec * kMicroSecondPerSecond + tv.tv_usec);
            }

            Timestamp()
                    :microSecondsSinceEpoch_(0){
            }

            explicit Timestamp(int64_t m)
                    :microSecondsSinceEpoch_(m){
            }

            std::string ToString() const{
                char  buf[32];
                bzero(buf, sizeof(buf));

                int64_t sec = microSecondsSinceEpoch_ / kMicroSecondPerSecond;
                int64_t micro_sec = microSecondsSinceEpoch_ % kMicroSecondPerSecond;

                snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", sec, micro_sec );

                return buf;
            }

            int64_t microSeconds() const{
                return  microSecondsSinceEpoch_;
            }

            void AddTime(double seconds){
                int64_t delta = static_cast<int64_t > (seconds * Timestamp::kMicroSecondPerSecond);
                microSecondsSinceEpoch_ += delta;
            }

        private:
            int64_t microSecondsSinceEpoch_;
        };

        inline bool operator<(const Timestamp& lhs, const Timestamp& rhs){
            return lhs.microSeconds() < rhs.microSeconds();
        }

        inline bool operator==(const Timestamp& lhs, const Timestamp& rhs){
            return lhs.microSeconds() == rhs.microSeconds();
        }

    }

}

#endif //YAN_TIMESTAMP_H
