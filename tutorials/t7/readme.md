CS6771 Tutorial 2015

Week Seven

Function Templates

Create the file variadicPrint.cpp and include the following functionality:
Add the following template and template specialisation:
template <typename T>
bool printIfWholeNumber(const T&) {
	return false;
}

template <> 
bool printIfWholeNumber<int>(const int& i) {
	std::cout << i;
	return true;
}
What will this template do if it is given a std::string, a double, or a EuclideanVector?
What will this template do if it is given an int?
What will this template do if it is given a long?
Add the following function template to your code:
template <typename T>
unsigned int printAndCountWholeNumbers(const T& d) {
	if (printIfWholeNumber(d)) {
		std::cout << " ";
		return 1;
	}
	return 0;
}
What will this template do if it is given a std::string, a double, or a EuclideanVector?
What will this template do if it is given an int?
What will this template do if it is given a long?
Use the following code to write a recursive variadic template function to print the whole numbers in a variable number of parameters:
template <typename T, typename... U>
unsigned int printAndCountWholeNumbers(T head, U... tail) {
	// call a function to work out if the "head" is a whole number
	// call a function to process the tail
	// return the number of whole numbers in the variadic parameters
}
Test your code by writing a main function that contains:
auto c = printAndCountWholeNumbers(1, 2.5, 3, 4.5, 5.5, 6, 7.0, -5, "2" );
std::cout << "count = " << c << std::endl;
The numbers 1 3 6 -5 should be printed out. Why is the number 7 not printed? Why is the number 2 not printed?
Add the following code to your main function:
std::vector<double> d = {1.2, 32.0, 3.2, 5.30, 5.4, 6, -5};
auto dc = printAndCountWholeNumbers(d);
std::cout << "count = " << dc << std::endl;

std::vector<unsigned int> vui = {65, 343, 21, 3};
dc = printAndCountWholeNumbers(vui);
std::cout << "count = " << dc << std::endl;
This code will compile and run, but it won't print out the correct number of whole numbers in either vector. Why?
Write another overloaded template function to correctly work through a vector:
template <typename T>
unsigned int printAndCountWholeNumbers(const std::vector<T>& vd) {
	unsigned int count = 0;
	for (auto d : vd) {
		auto dCopy = d;
		// work out if d is a whole number
		dCopy -= static_cast<int>(d);
		if (dCopy == 0) {
			// call: printIfWholeNumber 
			// and add to count. 
		}
	}
	return count;
}
In this function the number 32 gets printed, why?
Create the file extraSmart.cpp and include the following functionality:
Add the following template function:
template <typename T>
std::unique_ptr<T> make_unique(T t) {
	return std::unique_ptr<T>(new T(t));
}
This function will create a copy of the data passed into it on the heap and return a unique_ptr object around the data.
Add the following code to a main function to test this:
auto dptr = make_unique(3.14);
std::cout << *dptr << std::endl;
Now try to add to the main function:
auto dptr2 = make_unique(dptr);
std::cout << *dptr2 << std::endl;
This won't compile as you can't copy construct a unique_ptr. Why?
Create an overloaded templated function to allow the duplication of a unique_ptr, that is a complete copy of the pointee data
template <typename T>
std::unique_ptr<T> make_unique(std::unique_ptr<T>& up) {
	// TODO: create a copy of the ptr data.
	// note the unique_ptr is passed by reference
} 
Consider the following function templates:
template <typename T>
T getBase(T t) {
	return t;
}

template <typename T>
auto getBase(T *t) -> decltype(*t) {
	return *t;
}
What do these functions do? Add the following code to your main method to test them:
int i = 9;
int* iptr = &i;
std::cout << getBase(i) << std::endl;
std::cout << getBase(iptr) << std::endl;
Now add to main:
std::cout << getBase(dptr) << std::endl;
Why does this not compile? Can you write a third overloaded function template that will get the base type from the unique_ptr?
