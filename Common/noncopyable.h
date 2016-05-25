
#ifndef YAN_NONCOPYABLE_H
#define YAN_NONCOPYABLE_H

namespace Yan {

    namespace Common {
        class noncopyable {
        protected:
            noncopyable() { }
            ~noncopyable() { }
        private:
            noncopyable(const noncopyable &);
            const noncopyable &operator=(const noncopyable &);
        };
    }

    typedef Common::noncopyable noncopyable;
}

#endif //YAN_NONCOPYABLE_H
