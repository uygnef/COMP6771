//
// Created by yu on 16/08/17.
//

#ifndef A2_EUCLIDEANVECTOR_H
#define A2_EUCLIDEANVECTOR_H


#include <cmath>
#include <vector>

class EuclideanVector {
public:
    EuclideanVector():dimension{1U},magnitude{0}{};
    EuclideanVector(int i):dimension{static_cast<unsigned>(i)} ,magnitude{0}{};

    EuclideanVector(int d, double m):dimension{static_cast<unsigned int>(d)}, magnitude{m}{};

    template <typename IT> EuclideanVector(IT begin, IT end){
        magnitude = std::vector<double>(begin, end);
        dimension = magnitude.size();
    }

    EuclideanVector( const EuclideanVector& a);
    EuclideanVector(EuclideanVector&& a);

    unsigned int getNumDimensions();

    double get(unsigned int);
    double_t getEuclideanNorm();

    EuclideanVector createUnitVector();

    double& operator[] (unsigned x);
    double operator[](unsigned i) const;
    EuclideanVector& operator+= (EuclideanVector x);
    EuclideanVector& operator-= (EuclideanVector x);

    template<typename NUM> EuclideanVector& operator*= (NUM x){
        for(auto &i: this->magnitude){
            i *= x;
        }
        return *this;
    }

    template<typename NUM> EuclideanVector& operator/= (NUM x){
        for(auto &i: this->magnitude){
            i /= x;
        }
        return *this;
    }


//    template<typename> EuclideanVector& operator= (const EuclideanVector& x){
//
//        return *this;
//    }




    std::vector<double>magnitude;

private:
    unsigned int dimension;


};


#endif //A2_EUCLIDEANVECTOR_H
