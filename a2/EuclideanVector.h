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

        EuclideanVector& operator=(const EuclideanVector &ev);
        EuclideanVector& operator=(EuclideanVector &&ev);

        const unsigned getNumDimensions();

        double get(const int& i) const;

        double getEuclideanNorm();

        EuclideanVector createUnitVector();

        double &operator[](unsigned i);

        double operator[](unsigned i) const;

        EuclideanVector &operator+=(const EuclideanVector& x);

        EuclideanVector &operator-=(EuclideanVector x);

        EuclideanVector &operator*=(const int& x);

        template<typename T>
        EuclideanVector &operator/=(T x);


        operator std::vector<double>() const;

        operator std::list<double>() const;


        friend std::ostream &operator<<(std::ostream &os, const EuclideanVector &v);

        friend bool operator==(const EuclideanVector &lhs, const EuclideanVector &rhs);

        friend bool operator!=(const EuclideanVector &lhs, const EuclideanVector &rhs);

        friend EuclideanVector operator+(const EuclideanVector &lhs, const EuclideanVector &rhs);

        friend EuclideanVector operator-(const EuclideanVector &lhs, const EuclideanVector &rhs);

        friend EuclideanVector operator/(const EuclideanVector &lhs, int scale);


        friend EuclideanVector operator*(const EuclideanVector &lhs, int &scale);

        friend EuclideanVector operator*(int &scale, const EuclideanVector &lhs);

        friend double operator*(const EuclideanVector &scale, const EuclideanVector &lhs);

       // friend EuclideanVector &operator=(const EuclideanVector &rhs);

        double *magnitude;
        unsigned dimension;
    };
}
/*
 * 
 * 
 */
#endif //A2_EUCLIDEANVECTOR_H
