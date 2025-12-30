#include "dft_algs.h"

DFT::DFT() {}

int DFT::dft(std::vector<double>& signal) {

}

int DFT::idft(std::vector<double>& signal) {

}

int flip(std::vector<double>& signal) {

}

int shift(std::vector<double>& signal, int delta) {

}

int convolute_circ(std::vector<double>& s1, std::vector<double>& sig2) {

}

int stretch(std::vector<double>& signal, int L) {

}

int zeropad(std::vector<double>& signal) {

}

int interpolate(std::vector<double>& signal) {

}

int repeat(std::vector<double>& signal, int L) {

}

int downsample(std::vector<double>& signal, int L) {

}

int alias(std::vector<double>& signal, int L) {

}

z_vector DFT::r_to_z(r_vector r) {
	z_vector result; 
	for(int i = 0; i < r.size(); ++i) {
		result.push_back(std::complex<double>(r[i], 0.0)); 
	}

	return result; 
}

r_vector DFT::z_to_r(z_vector z) {
	r_vector result; 
	for(int i = 0; i < z.size(); ++i) {
		result.push_back(z.real()); 
	}

	return result;
}

z_vector DFT::even(z_vector& sig) {
	z_vector result; 
	for(int i = 0; i < sig.size(); ++i) {
		if((i % 2) == 0) 
			result.push_back(sig[i]); 
	}

	return result; 
}

z_vector DFT::odd(z_vector& sig) {
	z_vector result; 
	for(int i = 0; i < sig.size(); ++i) {
		if((i % 2) != 0) 
			result.push_back(sig[i]); 
	}

	return result; 
}

z_vector DFT::h_dft(z_vector sig) {
	const size_t size = sig.size(); 
	//base case
	if(size <= 1) return sig; 
	
	//split
	z_vector even = this->even(sig); //even split
	z_vector odd = this->odd(sig); //odd split

	//recurse 
	z_vector edft = this->h_dft(even); //even dft
	z_vector odft = this->h_dft(odd); //odd dft

	z_vector result; 
	//calculate sum for the step 
	for(int k = 0; k < size; ++k) {
		complex t = std::polar(1.0, 2.0 * this->PI * k / size) * odft[k]; 
		result[k] = edft[k] + t; 
		result[k + (N/2)] = edft[k] - t; 
	}

	return result; 
}

z_vector DFT::h_idft(z_vector sig) {
	z_vector result; 

	for(int i = 0; i < sig.size(); ++i) 
		result.push_back(std::conj(sig[i])); 

	result = this->h_dft(result); 

	for(int i = 0; i < sig.size(); ++i) 
		sig[i] = std::conj(sig[i]); 

	return result; 
}

