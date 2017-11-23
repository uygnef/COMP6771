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
    }

    EuclideanVector::~EuclideanVector() {
       // this->dimension = 0;
        if(magnitude != nullptr){
            delete[] this->magnitude;
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
        auto dimension = v.getNumDimensions();
        for (auto i = 0U; i < dimension; ++i) {
            os << v.get(i);
            if (i != dimension - 1) {
                os << " ";
            }
        }
        os << "]";
        return os;
    }

    bool operator==(const EuclideanVector &lhs, const EuclideanVector &rhs) {
        if (lhs.getNumDimensions() != rhs.getNumDimensions()) {
            return false;
        }
        for (auto i = 0U; i < lhs.getNumDimensions(); ++i) {
            if (lhs.get(i) != rhs.get(i)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const EuclideanVector &lhs, const EuclideanVector &rhs) {
        if (lhs.getNumDimensions() != rhs.getNumDimensions()) {
            return true;
        }
        for (auto i = 0U; i < lhs.getNumDimensions(); ++i) {
            if (lhs.get(i) != rhs.get(i)) {
                return true;
            }
        }
        return false;
    }

    EuclideanVector operator+(const EuclideanVector &lhs, const EuclideanVector &rhs) {
        assert(lhs.getNumDimensions() == rhs.getNumDimensions());
        EuclideanVector ret(rhs.getNumDimensions());
        for (auto i = 0U; i < lhs.getNumDimensions(); ++i) {
            ret.magnitude[i] = rhs.get(i) + lhs.get(i);
        }
        return ret;
    }

    EuclideanVector operator-(const EuclideanVector &lhs, const EuclideanVector &rhs) {
        assert(lhs.getNumDimensions() == rhs.getNumDimensions());
        EuclideanVector ret(rhs.getNumDimensions());
        for (auto i = 0U; i < lhs.getNumDimensions(); ++i) {
            ret.magnitude[i] = lhs.get(i) - rhs.get(i);
        }
        return ret;
    }

    EuclideanVector operator/(const EuclideanVector &lhs, double scale) {
        EuclideanVector ret(lhs.getNumDimensions());
        for (auto i = 0U; i < lhs.getNumDimensions(); ++i) {
            ret.magnitude[i] = lhs.get(i) / scale;
        }
        return ret;
    }


    EuclideanVector operator*(const EuclideanVector &lhs, double scale) {
        EuclideanVector ret{lhs};
        for (auto i = 0U; i < lhs.getNumDimensions(); ++i) {
            ret.magnitude[i] *= scale;
        }
        return ret;
    }


    EuclideanVector operator*(double scale, const EuclideanVector &lhs) {
        EuclideanVector ret{lhs};
        for (auto i = 0U; i < lhs.getNumDimensions(); ++i) {
            ret.magnitude[i] *= scale;
        }
        return ret;
    }


    double operator*(const EuclideanVector &rhs, const EuclideanVector &lhs) {
        assert(lhs.dimension == rhs.dimension);
        double ret = 0;
        for (auto i = 0U; i < lhs.getNumDimensions(); ++i) {
            ret += (lhs.get(i) * rhs.get(i));
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









