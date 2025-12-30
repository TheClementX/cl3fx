#ifndef DFT_ALGS
#define DFT_ALGS

#include "utils.h"

using complex = std::complex<double>
using r_vector = std::vector<double>; 
using z_vector = std::vector<complex>; 

class DFT {
public:
	DFT(); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int dft(std::vector<double>& signal); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int idft(std::vector<double>& signal); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int flip(std::vector<double>& signal); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int shift(std::vector<double>& signal, int delta); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int convolute_circ(std::vector<double>& s1, std::vector<double>& sig2); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int stretch(std::vector<double>& signal, int L); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int zeropad(std::vector<double>& signal); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int interpolate(std::vector<double>& signal); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int repeat(std::vector<double>& signal, int L); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int downsample(std::vector<double>& signal, int L); 
	
	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int alias(std::vector<double>& signal, int L); 

private:
	const double PI = 3.141592653589793238460; 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	std::vector<std::complex<double>> r_to_z(std::vector<double> r); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	std::vector<double> z_to_r(std::vecotr<std::complex<double>> z); 

	std::vector<std::complex<double>> h_dft(std::vector<std::complex<double>> sig); 

	std::vector<std::complex<double>> h_idft(std::vector<std::complex<double>> sig); 

}; 

#endif 
