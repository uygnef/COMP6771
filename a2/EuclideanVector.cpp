//
// Created by yu on 16/08/17.
//
#include <iostream>
#include "EuclideanVector.h"



EuclideanVector::EuclideanVector(const EuclideanVector &a){
    dimension = a.dimension;
    magnitude = a.magnitude;
}

EuclideanVector::EuclideanVector(EuclideanVector &&a){
    dimension = std::move(a.dimension);
    magnitude = std::move(a.magnitude);
}

unsigned int EuclideanVector::getNumDimensions(){
    return this->dimension;
}

double EuclideanVector::get(unsigned int i){
    if(i >= this->magnitude.size()){
        std::cerr << "get() out of range.\n";
        exit(-1);
    }
    return this->magnitude[i];
}

double_t EuclideanVector::getEuclideanNorm(){
    double sum_power{0};
    for(auto &i:this->magnitude){
        sum_power +=  i*i;
    }
    return sqrt(sum_power);
}

EuclideanVector EuclideanVector::createUnitVector(){
    double norm_dist = this->getEuclideanNorm();
    std::vector<double> normal_value;
    for(auto i:this->magnitude){
        normal_value.push_back(i/norm_dist);
    }
    EuclideanVector ret{normal_value.begin(), normal_value.end()};
    return ret;
}

double& EuclideanVector::operator[] (unsigned i)  {
    if(i >= this->magnitude.size()){
        std::cerr << "[] operator out of range.\n";
        exit(-1);
    }
    return this->magnitude.at(i);
}

double EuclideanVector::operator[](unsigned i) const{
    if(i >= this->magnitude.size()){
        std::cerr << "[] operator out of range.\n";
        exit(-1);
    }
    return this->magnitude.at(i);
}

EuclideanVector& EuclideanVector::operator+= (EuclideanVector x){
    if(x.dimension != this->dimension){
        std::cerr << "+ operation need be same dimension: can not add"
                  << x.dimension << " with " << this->dimension <<std::endl;
        exit(-1);
    }

    unsigned i = 0;
    for(i; i < x.dimension; ++i ){
        this->magnitude[i] += x.magnitude[i];
    }
    return *this;
}

EuclideanVector& EuclideanVector::operator-= (EuclideanVector x){
    if(x.dimension != this->dimension){
        std::cerr << "- operation need be same dimension: can not add"
                  << x.dimension << " with " << this->dimension <<std::endl;
        exit(-1);
    }

    unsigned i = 0;
    for(i; i < x.dimension; ++i ){
        this->magnitude[i] -= x.magnitude[i];
    }
    return *this;
}



//EuclideanVector& EuclideanVector::operator= (const EuclideanVector x) {
//    this->magnitude = x.magnitude;
//    this->dimension = x.dimension;
//    return *this;
//}




