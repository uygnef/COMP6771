//
// Created by yu on 16/08/17.
//
#include <iostream>
#include "EuclideanVector.h"
//
//double &EuclideanVector::Node::operator[](const double &i) {
//    unsigned count{0U};
//    Node *temp = this;
//
//    for(count; count < i; ++count){
//        if(temp == nullptr){
//            std::cerr << "[] out of range.\n";
//            exit(-1);
//        }
//        temp = temp->next;
//    }
//    return temp->value;
//}
//
//double EuclideanVector::Node::operator[](const double &i) const {
//
//    unsigned count{0U};
//    Node *temp = this;
//
//    for(count; count < i; ++count){
//        if(temp == nullptr){
//            std::cerr << "[] out of range.\n";
//            exit(-1);
//        }
//        temp = temp->next;
//    }
//    return temp->value;
//}



//EuclideanVector& EuclideanVector::operator= (const EuclideanVector x) {
//    this->magnitude = x.magnitude;
//    this->dimension = x.dimension;
//    return *this;
//}




EuclideanVector::EuclideanVector(const EuclideanVector &a) {
    dimension = a.dimension;
    magnitude = new double[dimension];
    for(auto i=0U; i< dimension; ++i){
        magnitude[i] = a.magnitude[i];
    }
}

EuclideanVector::EuclideanVector(EuclideanVector &&a) {
    dimension = a.dimension;
    magnitude = new double[dimension];
    for(auto i=0U; i< dimension; ++i){
        magnitude[i] = a.magnitude[i];
    }
    delete a.magnitude;

}

EuclideanVector::~EuclideanVector() {
    delete[] magnitude;
}


