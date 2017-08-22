//
// Created by yu on 16/08/17.
//

#ifndef A2_EUCLIDEANVECTOR_H
#define A2_EUCLIDEANVECTOR_H
#pragma once

#include <cmath>
#include <memory>
#include <cstring>
#include <cassert>
#include <vector>
#include <list>
#include <stdarg.h>
#include <iostream>

namespace evec {
    class EuclideanVector {
    public:
        EuclideanVector(): dimension{1}{
            magnitude = new double[1]; std::cout <<"3 allocate" << magnitude<<"\n";
            magnitude[0] = 0.0;
        }

        EuclideanVector(double length): dimension{static_cast<unsigned>(length)},
                                        magnitude{new double[dimension]}{
            ; std::cout <<"4 allocate" << magnitude<<"\n";
            for(auto i=0U; i<dimension; ++i){
                magnitude[i] = 0.0;
            }
        }

        EuclideanVector(unsigned length, double mag) : dimension{length},
                                                       magnitude{new double[length]} {
            std::cout <<"5 allocate" << magnitude<<"\n";
            for (auto i = 0U; i < length; ++i) {
                magnitude[i] = mag;
            }
        };

        template<typename IT>
        EuclideanVector(IT begin, IT end, typename std::enable_if<!std::is_arithmetic<IT>::value>::type* = 0) {
            dimension = 0U;
            for (auto i = begin; i != end; ++i) {
                dimension += 1;
            }

            magnitude = new double[dimension];std::cout <<"6 allocate" << magnitude<<"\n";
            unsigned j = 0U;
            for (auto i = begin; i != end; ++i) {
                magnitude[j] = static_cast<double>(*i);
                j += 1;
            }
        }

        EuclideanVector(std::initializer_list<double> values){
            dimension = static_cast<unsigned >(values.size());
            magnitude = new double[dimension];            std::cout <<"7 allocate" << magnitude<<"\n";

            unsigned i = 0U;
            for(auto& value: values){
                magnitude[i] = value;
                ++i;
            }
        }

        EuclideanVector(const EuclideanVector &a);

        EuclideanVector(EuclideanVector &&a);

        ~EuclideanVector();

        EuclideanVector& operator=(const EuclideanVector &ev);
        EuclideanVector& operator=(EuclideanVector &&ev);

        unsigned getNumDimensions() const;

        double get(const int& i) const;

        double getEuclideanNorm() const;

        EuclideanVector createUnitVector() const;

        double &operator[](unsigned i);

        double operator[](unsigned i) const;

        EuclideanVector &operator+=(const EuclideanVector& x);

        EuclideanVector &operator-=(EuclideanVector x);

        EuclideanVector &operator*=(const int& x);

        EuclideanVector& operator/=(const double& x);



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

  //  private:
        unsigned dimension;
        double *magnitude;

    };
}
/*
 * 
 * 
 */
#endif //A2_EUCLIDEANVECTOR_H
