//
// Created by yu on 16/08/17.
//
#include "EuclideanVector.h"

namespace evec {


    EuclideanVector::EuclideanVector(const EuclideanVector &a){
        dimension = a.dimension;
        magnitude = new double[dimension];
        is_change = a.is_change;
        normal = a.normal;
#ifdef DEBUG
        std::cout <<"1 allocate" << magnitude<<"\n";
#endif
        for (auto i = 0U; i < dimension; ++i) {
            magnitude[i] = a.magnitude[i];
        }
    }

    EuclideanVector::EuclideanVector(EuclideanVector &&a) {
            dimension = a.dimension;
            magnitude = a.magnitude;
            normal = a.normal;
            is_change = a.is_change;

            a.dimension = 0;
            a.magnitude = nullptr;
          //  delete[] a.magnitude; std::cout << "9 free "<<a.magnitude<<"\n";
    }

    EuclideanVector::~EuclideanVector() {
       // this->dimension = 0;
        if(magnitude != nullptr){
            delete[] this->magnitude;
#ifdef DEBUG
            std::cout << * magnitude;
            std::cout <<"        1 free" << magnitude<<"\n";
#endif
        }
    }

    unsigned EuclideanVector::getNumDimensions() const{
        return this->dimension;
    }

    double EuclideanVector::get(const int &i) const{
        assert(static_cast<unsigned >(i) < getNumDimensions());
        return this->magnitude[static_cast<unsigned>(i)];
    }

    double EuclideanVector::getEuclideanNorm() const{
        if(!is_change){
            return normal;
        }
        double sum_power{0};;
        for (auto i = 0U; i < this->dimension; ++i) {
            sum_power += pow(this->magnitude[i], 2);
        }
        normal = sqrt(sum_power);
        is_change = false;
        return normal;
    }

    EuclideanVector EuclideanVector::createUnitVector() const{
        double norm_dist = this->getEuclideanNorm();
        EuclideanVector ret{*this};
        for (auto i = 0U; i < this->dimension; ++i) {
            ret.magnitude[i] /= norm_dist;
        }
        ret.is_change = true;
        return ret;
    }

    double &EuclideanVector::operator[](unsigned i) {
        assert(i < this->dimension);
        is_change = true;
        return this->magnitude[i];
    }

    double EuclideanVector::operator[](unsigned i) const {
        assert(i < this->dimension);
        return this->magnitude[i];
    }

    EuclideanVector& EuclideanVector::operator+=(const EuclideanVector &x) {
        assert(this->dimension == x.dimension);
        for (auto i = 0U; i < this->dimension; ++i) {
            this->magnitude[i] += x.magnitude[i];
        }
        is_change = true;
        return *this;
    }

    EuclideanVector &EuclideanVector::operator-=(EuclideanVector x) {
        assert(this->dimension == x.dimension);
        for (auto i = 0U; i < this->dimension; ++i) {
            this->magnitude[i] -= x.magnitude[i];
        }
        is_change = true;
        return *this;
    }

    EuclideanVector &EuclideanVector::operator*=(const double & x) {
        for (auto i = 0U; i < this->dimension; ++i) {
            this->magnitude[i] *= x;
        }
        is_change = true;
        return *this;
    }

    EuclideanVector &EuclideanVector::operator/=(const double& x){
        assert(x != 0);
        for (auto i = 0U; i < this->dimension; ++i) {
            magnitude[i] /= x;
        }
        is_change = true;
        return *this;
    }


    EuclideanVector::operator std::vector<double>() const {
        std::vector<double> ret;
        for (auto i = 0U; i < this->dimension; ++i) {
            ret.push_back(this->magnitude[i]);
        }
        return ret;
    }

    EuclideanVector::operator std::list<double>() const {
        std::list<double> ret;
        for (auto i = 0U; i < this->dimension; ++i) {
            ret.push_back(this->magnitude[i]);
        }
        return ret;
    }

    std::ostream &operator<<(std::ostream &os, const EuclideanVector &v) {
        os << "[";
        for (auto i = 0U; i < v.dimension; ++i) {
            os << v.magnitude[i];
            if (i != v.dimension - 1) {
                os << " ";
            }
        }
        os << "]";
        return os;
    }

    bool operator==(const EuclideanVector &lhs, const EuclideanVector &rhs) {
        if (lhs.dimension != rhs.dimension) {
            return false;
        }
        for (auto i = 0U; i < lhs.dimension; ++i) {
            if (lhs.magnitude[i] != rhs.magnitude[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const EuclideanVector &lhs, const EuclideanVector &rhs) {
        if (lhs.dimension != rhs.dimension) {
            return true;
        }
        for (auto i = 0U; i < lhs.dimension; ++i) {
            if (lhs.magnitude[i] != rhs.magnitude[i]) {
                return true;
            }
        }
        return false;
    }

    EuclideanVector operator+(const EuclideanVector &lhs, const EuclideanVector &rhs) {
        assert(lhs.dimension == rhs.dimension);
        EuclideanVector ret(rhs.dimension);
        for (auto i = 0U; i < lhs.dimension; ++i) {
            ret.magnitude[i] = rhs.magnitude[i] + lhs.magnitude[i];
        }
        return ret;
    }

    EuclideanVector operator-(const EuclideanVector &lhs, const EuclideanVector &rhs) {
        assert(lhs.dimension == rhs.dimension);
        EuclideanVector ret(rhs.dimension);
        for (auto i = 0U; i < lhs.dimension; ++i) {
            ret.magnitude[i] = lhs.magnitude[i] - rhs.magnitude[i];
        }
        return ret;
    }

//template<typename T>
    EuclideanVector operator/(const EuclideanVector &lhs, double scale) {
       // assert(scale != 0);
        EuclideanVector ret(lhs.dimension);
        for (auto i = 0U; i < lhs.dimension; ++i) {
            ret.magnitude[i] = lhs.magnitude[i] / scale;
        }
        return ret;
    }


    EuclideanVector operator*(const EuclideanVector &lhs, double scale) {
        EuclideanVector ret{lhs};
        for (auto i = 0U; i < lhs.dimension; ++i) {
            ret.magnitude[i] *= scale;
        }
        return ret;
    }


    EuclideanVector operator*(double scale, const EuclideanVector &lhs) {
        EuclideanVector ret{lhs};
        for (auto i = 0U; i < lhs.dimension; ++i) {
            ret.magnitude[i] *= scale;
        }
        return ret;
    }


    double operator*(const EuclideanVector &rhs, const EuclideanVector &lhs) {
        assert(lhs.dimension == rhs.dimension);
        double ret = 0;
        for (auto i = 0U; i < lhs.dimension; ++i) {
            ret += (lhs.magnitude[i] * rhs.magnitude[i]);
        }
        return ret;
    }

    EuclideanVector& EuclideanVector::operator=(const EuclideanVector &rhs) {
        if(this == &rhs){
            return *this;
        }
        if (magnitude != nullptr){
            delete[] magnitude;
        }
        dimension = rhs.dimension;
        magnitude = new double[dimension];
        for(auto i=0U; i < dimension; ++i){
            magnitude[i] = rhs.magnitude[i];
        }
        is_change = true;
        return *this;
    }

    EuclideanVector &EuclideanVector::operator=(EuclideanVector &&ev) {
        if(this != &ev) {
            delete[] magnitude;
            dimension = ev.dimension;
            magnitude = ev.magnitude;

            ev.magnitude = nullptr;
            ev.dimension = 0;
        }
        is_change = true;
        return *this;
    }

}









