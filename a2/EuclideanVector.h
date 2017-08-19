//
// Created by yu on 16/08/17.
//

#ifndef A2_EUCLIDEANVECTOR_H
#define A2_EUCLIDEANVECTOR_H


#include <cmath>
#include <memory>
#include <cstring>
#include <cassert>

class EuclideanVector {
public:
    template <typename T, typename D>
    explicit EuclideanVector(T length=0U, D mag = 0): dimension{static_cast<unsigned>(length)},
                                                      magnitude{new double[dimension]} {};

    template <typename IT> EuclideanVector(IT begin, IT end){
        dimension = 0;
        for (auto i = begin; i != end; ++i) {
            dimension += 1;
        }

        magnitude = new double[dimension];
        unsigned j = 0U;
        for (auto i = begin; i != end; ++i) {
            magnitude[j] = *i;
            j += 1;
        }
    }

    EuclideanVector( const EuclideanVector& a);
    EuclideanVector( EuclideanVector&& a);

    ~EuclideanVector();
//private:
//    class Array{
//    public:
//        friend class EuclideanVector;
//
//        template<typename T, typename D>
//        Array(T s=1U, D v = 0): size{static_cast<unsigned>(s)},
//                                value{new double[static_cast<int>(s)]} {
//            if (v != 0){
//                auto i = 0U;
//                for(i; i<size; ++i){
//                    value[i] = v;
//                }
//            }
//        };
//
//        double *value;
//        int size;
//    };
    double *magnitude;
    unsigned dimension;
};

/*
 * 
 * 
 */
#endif //A2_EUCLIDEANVECTOR_H
