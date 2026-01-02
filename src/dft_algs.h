#ifndef DFT_ALGS
#define DFT_ALGS

#include "utils.h"


class DFT {
public:
	DFT(); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	z_vector dft(r_vector& signal); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	r_vector idft(z_vector& signal); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int flip(r_vector& signal); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int shift(r_vector& signal, int delta); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	r_vector convolute_circ(r_vector& s1, r_vector& s2); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	r_vector correlate(r_vector& s1, r_vector& s2); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int stretch(r_vector& signal, int L); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int periodic_zeropad(r_vector& signal, int L); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int causal_zeropad(r_vector& signal, int L); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	int repeat(r_vector& signal, int L); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	r_vector downsample(r_vector& signal, int L); 
	
	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	r_vector alias(r_vector& signal, int L); 

	/*
	implement after learning filters
	periodic interpolation
	int p_interpolation(r_vector& signal, int L); 

	non periodic interpolation
	int np_interpolation(r_vector& signal, int L); 
	*/
private:
	const double PI = 3.141592653589793238460; 

	bool is_p2(int n); 

	int next_p2(int n); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	z_vector r_to_z(r_vector& r); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	r_vector z_to_r(z_vector& z); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	z_vector even(z_vector& sig); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	z_vector odd(z_vector& sig); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	z_vector h_dft(z_vector sig); 

	/**@brief
	 * @param[in]
	 * @return
	 * @pre
	 * @post
	 */
	z_vector h_idft(z_vector sig); 
}; 

#endif 
