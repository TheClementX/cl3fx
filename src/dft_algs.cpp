#include "dft_algs.h"

DFT::DFT() {}

z_vector DFT::dft(r_vector& signal) {
	//zeropad 
	int N = signal.size(); 
	if(!this->is_p2(N)) 
		this->causal_zeropad(signal, this->next_p2(N)); 
	
	//compute
	z_vector z_sig = this->r_to_z(signal); 
	return this->h_dft(z_sig); 
}

r_vector DFT::idft(z_vector& signal) {
	//compute
	z_vector result = this->h_idft(signal); 
	return this->z_to_r(result); 
}

int DFT::flip(r_vector& signal) {
	int L = signal.size() - 1; 
	for(int i = 1; i < (L/2 + 1); ++i) {
		int i1 = i; 
		int i2 = L - i - 1; 
		double tmp = signal[i1]; 
		signal[i1] = signal[i2]; 
		signal[i2] = tmp; 
	}

	return 0; 
}

//std::rotate rotates left 
//shift rotates right
int DFT::shift(r_vector& signal, int delta) {
	int left_delta = signal.size() - delta; 
	std::rotate(signal.begin(), signal.begin() + left_delta, signal.end()); 
}

int DFT::convolute_circ(r_vector& s1, r_vector& s2) {
	//assert signals are same length
	if(s1.size() != s2.size()) return -1; 

	//use convolution theorem
	z_vector X = this->dft(s1); 
	z_vector Y = this->dft(s2); 

	z_vector z_result; 
	z_result.resize(s1.size()); 

	for(int i = 0; i < z_result.size(); ++i)
		z_result[i] = X[i] * Y[i]; 

	return this->idft(z_result); 
}

r_vector correlate(r_vector& s1, r_vector& s2) {
	//assert signals are same length
	if(s1.size() != s2.size()) return -1; 

	//use correlate theorem
	z_vector X = this->dft(s1); 
	z_vector Y = this->dft(s2); 

	z_vector z_result; 
	z_result.resize(s1.size()); 

	for(int i = 0; i < z_result.size(); ++i)
		z_result[i] = std::conj(X[i]) * Y[i]; 

	return this->idft(z_result); 
}

int DFT::stretch(r_vector& signal, int L) {
	int zeros = L - 1; 
	for(int i = signal.size() - 1; i >= 0; --i) {
		signal.insert(signal.begin() + i + 1, zeros, 0.0); 
	}
	
	return 0.0; 
}

//assumes signal is stored in circular form 
//with positive time from 0 to N/2 + 1
int DFT::periodic_zeropad(r_vector& signal, int L) {
	if(L <= signal.size()) return 0;
	int zeros = signal.size() - L;
	int ipos = signal.size() / 2 + 1; 

	signal.insert(signal.begin() + ipos, zeros, 0.0); 

	return 0; 
}

int DFT::causal_zeropad(r_vector& signal, int L) {
	if(L <= signal.size()) return 0; 
	
	int left = L - signal.size(); 
	for(int i = 0; i < left; ++i) {
		signal.push_back(0.0); 
	}

	return 0
}

int DFT::repeat(r_vector& signal, int L) {
	r_vector init = signal; 
	for(int i = 0; i < L; ++i)
		signal.insert(signal.end(), init.begin(), init.end()); 

	return 0; 
}

r_vector DFT::downsample(r_vector& signal, int L) {
	r_vector result; 
	for(int i = 0; i < signal.size(); ++i) {
		if(i % L == 0)	
			result.push_back(signal[i]); 
	}
	
	return result; 
}

//split the signal into L even parts at sum 
//their corresponding values
r_vector DFT::alias(r_vector& signal, int L) {
	int N = signal.size(); 
	int pad = L - (N % L); 

	//make signal length a multiple of L 
	this->causal_zeropad(signal, pad); 

	//store new lengths
	int padded_len = N + pad; 
	int new_len = padded_len / L; 

	r_vector result; 
	result.resize(new_len); 

	//populate result
	for(int i = 0; i < new_len; ++i) {
		for(int j = 0; j < L; ++j) {
			result[i] += signal[i + L * j]; 
		}
	}

	return result; 
}

int DFT::periodic_interpolation(r_vector& signal, int L) {

	return 0; 
}

//causal zeropadding
int DFT::nonperiodic_interpolation(r_vector& signal, int L) {

	return 0; 
}

bool is_p2(int n) {
	return (n > 0) && ((n-1) & n) == 0; 
}

int next_p2(int n) {
	int logb2 = std::bit_width(n); 
	return 1 << logb2; 
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

	for(int i = 0; i < result.size(); ++i) 
		result[i] = std::conj(result[i]); 

	return result; 
}

