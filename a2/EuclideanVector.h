//
// Created by yu on 16/08/17.
//

#ifndef A2_EUCLIDEANVECTOR_H
#define A2_EUCLIDEANVECTOR_H


#include <cmath>
#include <memory>
#include <cstring>
#include <cassert>
#include <vector>
#include <list>

namespace evec {
    class EuclideanVector {
    public:


        EuclideanVector(unsigned length = 1U, double mag = 0.0) : dimension{length},
                                                                  magnitude{new double[length]} {
            for (auto i = 0; i < length; ++i) {
                magnitude[i] = mag;
            }
        };

        template<typename IT>
        EuclideanVector(IT begin, IT end) {
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

        EuclideanVector(const EuclideanVector &a);

        EuclideanVector(EuclideanVector &&a);

        ~EuclideanVector();

        const unsigned getNumDimensions();

        template<typename T>
        const double get(const T &i);

        double getEuclideanNorm();

        EuclideanVector createUnitVector();

        double &operator[](unsigned i);

        double operator[](unsigned i) const;

        EuclideanVector &operator+=(EuclideanVector x);

        EuclideanVector &operator-=(EuclideanVector x);

        template<typename T>
        EuclideanVector &operator*=(T x);

        template<typename T>
        EuclideanVector &operator/=(T x);


        explicit operator std::vector<double>() const;

        explicit operator std::list<double>() const;


        friend std::ostream &operator<<(std::ostream &os, const EuclideanVector &v);

        friend bool operator==(const EuclideanVector &lhs, const EuclideanVector &rhs);

        friend bool operator!=(const EuclideanVector &lhs, const EuclideanVector &rhs);

        friend EuclideanVector &operator+(const EuclideanVector &lhs, const EuclideanVector &rhs);

        friend EuclideanVector &operator-(const EuclideanVector &lhs, const EuclideanVector &rhs);

        template<typename T>
        friend EuclideanVector &operator/(const EuclideanVector &lhs, T &scale);

        template<typename T>
        friend EuclideanVector &operator*(const EuclideanVector &lhs, T &scale);

        template<typename T>
        friend EuclideanVector &operator*(T &scale, const EuclideanVector &lhs);

        friend double &operator*(const EuclideanVector &scale, const EuclideanVector &lhs);

//        friend EuclideanVector &operator=(std::vector<double> &trans, const EuclideanVector &rhs);


        double *magnitude;
        unsigned dimension;
    };
}
/*
 * 
 * 
 */
#endif //A2_EUCLIDEANVECTOR_H
