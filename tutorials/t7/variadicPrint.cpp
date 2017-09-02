#include<iostream>
#include<vector>
template <typename T>
bool printIfWholeNumber(const T&) {
	return false;
}

template <> 
bool printIfWholeNumber<int>(const int& i) {
	std::cout << i;
	return true;
}

template <typename T>
unsigned int printAndCountWholeNumbers(const T& d) {
	if (printIfWholeNumber(d)) {
		std::cout << " ";
		return 1;
	}
	return 0;
}

template <typename T, typename... U>
unsigned int printAndCountWholeNumbers(T head, U... tail) {
	// call a function to work out if the "head" is a whole number
	// call a function to process the tail
	// return the number of whole numbers in the variadic parameters
	
}



int main(){
	long a = 1;
	printAndCountWholeNumbers(a);	

}
