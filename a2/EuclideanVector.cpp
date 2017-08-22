//
// Created by yu on 16/08/17.
//
#include "EuclideanVector.h"

namespace evec {


    EuclideanVector::EuclideanVector(const EuclideanVector &a){
        dimension = a.dimension;
        magnitude = new double[dimension]; std::cout <<"1 allocate" << magnitude<<"\n";
        for (auto i = 0U; i < dimension; ++i) {
            magnitude[i] = a.magnitude[i];
        }
    }

    EuclideanVector::EuclideanVector(EuclideanVector &&a) {
            dimension = a.dimension;
            magnitude = new double[dimension];std::cout <<"2 allocate" << magnitude<<"\n";
            for (auto i = 0U; i < dimension; ++i) {
                magnitude[i] = a.magnitude[i];
            }
            a.dimension = 0;
            delete[] a.magnitude; std::cout << "9 free "<<a.magnitude<<"\n";
    }

    EuclideanVector::~EuclideanVector() {
       // this->dimension = 0;
        std::cout << * magnitude;
            delete[] this->magnitude; std::cout <<"        1 free" << magnitude<<"\n";
            this->magnitude = nullptr;
     //   std::cout << * magnitude << (magnitude == nullptr);


    }

    unsigned EuclideanVector::getNumDimensions() const{
        return this->dimension;
    }

    double EuclideanVector::get(const int &i) const{
        return this->magnitude[static_cast<unsigned>(i)];
    }

    double EuclideanVector::getEuclideanNorm() const{
        double sum_power{0};;
        for (auto i = 0U; i < this->dimension; ++i) {
            sum_power += pow(this->magnitude[i], 2);
        }
        return sqrt(sum_power);
    }

    EuclideanVector EuclideanVector::createUnitVector() const {
        double norm_dist = this->getEuclideanNorm();
        EuclideanVector ret{*this};
        for (auto i = 0U; i < this->dimension; ++i) {
            ret.magnitude[i] /= norm_dist;
        }
        return ret;
    }

    double &EuclideanVector::operator[](unsigned i) {
        assert(i < this->dimension);
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
        return *this;
    }

    EuclideanVector &EuclideanVector::operator-=(EuclideanVector x) {
        assert(this->dimension == x.dimension);
        for (auto i = 0U; i < this->dimension; ++i) {
            this->magnitude[i] -= x.magnitude[i];
        }
        return *this;
    }

    EuclideanVector &EuclideanVector::operator*=(const int& x) {
        for (auto i = 0U; i < this->dimension; ++i) {
            this->magnitude[i] *= x;
        }
        return *this;
    }

    EuclideanVector &EuclideanVector::operator/=(const double& x){
        assert(x != 0);
        for (auto i = 0U; i < this->dimension; ++i) {
            magnitude[i] /= x;
        }
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
    EuclideanVector operator/(const EuclideanVector &lhs, int scale) {
        EuclideanVector ret(lhs.dimension);
        for (auto i = 0U; i < lhs.dimension; ++i) {
            ret.magnitude[i] = lhs.magnitude[i] / scale;
        }
        return ret;
    }


    EuclideanVector operator*(const EuclideanVector &lhs, int &scale) {
        EuclideanVector ret(lhs.dimension);
        for (auto i = 0U; i < lhs.dimension; ++i) {
            ret.magnitude[i] *= scale;
        }
        return ret;
    }


    EuclideanVector operator*(int &scale, const EuclideanVector &lhs) {
        EuclideanVector ret(lhs.dimension);
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
        if(this != &rhs){
            for(auto i=0U; i < rhs.dimension; ++i){
                this->magnitude[i] = rhs.magnitude[i];
            }
        }
        return *this;
    }

    EuclideanVector &EuclideanVector::operator=(EuclideanVector &&ev) {
        if(this != &ev) {
            delete[] magnitude;
            dimension = ev.dimension;
            magnitude = ev.magnitude;
//            for (auto i = 0U; i < ev.dimension; ++i) {
//                this->magnitude[i] = ev.magnitude[i];
//            }
          //  delete[] ev.magnitude; std::cout <<"      2     free" << ev.magnitude<<"\n";
          //  ev.dimension = 0U;
        }
        return *this;
    }


}









