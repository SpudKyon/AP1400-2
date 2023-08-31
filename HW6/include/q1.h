#ifndef Q1_H
#define Q1_H

#include <functional>

namespace q1 {

    template<typename T, typename F>
    inline T gradient_descent(
            T init_val,
            T step,
            F func) {
        T current = init_val;
        T cur_val = func(current);
        T e = step / 10000000;

        while (true) {
            T grad = (func(current + step) - cur_val) / step;
            T nxt = current - step * grad;
            T nxt_val = func(nxt);
            if (std::abs(current - nxt) < e) {
                break;
            }
            current = nxt;
            cur_val = nxt_val;
        }
        return current;
    }

    template<typename T, typename F>
    inline T gradient_descent(T init_val, T step) {
        return gradient_descent( init_val,  step,  F{});
    }
}

#endif //Q1_H