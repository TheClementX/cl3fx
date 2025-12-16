#ifndef PARSER
#define PARSER

#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <cstring>


/**@brief a simple template function to read an arbitrary size
 * integer from a byte vector
 * @param[in] buff : the vector
 * @param[in] ind : the location to read from 
 * @return the read byte
 */
template <typename T> 
T le_read(const std::vector<uint8_t>& buff, size_t ind) {
	T result = 0; 

	//bounds check 
	if(ind >= buff.size()-sizeof(T)) {
		result = -1; 
		return result; 
	}

	for(size_t i = 0; i < sizeof(T); ++i) {
		result |= static_cast<T>(buff[ind+i]) << (i*8); 
	}

	return result; 
}

/**@brief a function to verify a wave files string tags
 * @param[in] buff : the buffer to read data from 
 * @param[in] offset : the offset into the buffer
 * @param[in] ref : the reference string
 * @return true on match false on failure or failed match
 */
bool verify_tag(const std::vector<uint8_t>& buff, size_t offset, const std::string& ref); 

/**@brief a wav file metadata struct
 */

typedef struct WavMetaData {
	//riff section
	uint32_t file_size; 
	//format section 
	uint32_t fmt_size; 
	uint16_t audio_fmt; 
	uint16_t num_channels; 
	uint32_t sample_rate; 
	uint32_t byte_rate; 
	uint16_t block_align; 
	uint16_t bits_per_sample; 
	//data section
	uint32_t data_size;
	uint32_t data_offset; 
} WavMetaData; 

/**@brief A simple class to parse wav files. This implements functionality 
 * to both read and write wav files. Writting is not yet implemented
 */
class WavParser {
public: 
	//constructor 
	WavParser(); 
	
	/**@brief the front facing function to parse a wav file 
	 * @param[in] file : the file to be parssed 
	 * @return 0 on success -1 on failure
	 */
	int parse_file(const std::string& file); 

private:
	/**@brief the file path for the currently parsed file
	 */
	std::string file; 

	/**@brief contains every raw byte of the wav file to be 
	 * parsed
	 */
	std::vector<uint8_t> raw_data; 

	/**@brief contains the signal from the wav file normalized
	 * between -1 and 1 as double precision floating point values. 
	 */
	std::vector<std::vector<double>> norm_channels;  

	/**@brief meta_data struct for the wav_file
	 */
	WavMetaData meta_data; 

	/**@brief reads the raw bytes of the wav file to be parsed
	 * @param[in] file : the file to be read
	 */
	int read_wav(const std::string& file); 

	/**@brief parses the metadata of the wav file in this->meta_data
	 * @return -1 on failure 0 on success
	 */
	int parse_wav(); 

	/**@brief takes a c style array of bytes and parses a double from it
	 * based on the fmt and byte width of this parsers current wav file. 
	 * @param[in] bytes : the bytes array
	 * @param[in] bytes_width : the width of a wav file data frame
	 * @param[in] fmt : the fmt either PCM or IEEE float 32
	 * @return the parsed double or 0.0 on failure
	 * @post -1.0 < return < 1.0
	 */
	double bytes_to_double(uint8_t *bytes, size_t byte_width, size_t fmt); 

	/**@brief prepares the raw PCM bytes of a wav file as 
	 * a normalized audio signal
	 * @pre read_wav has been called 
	 */
	int normalize_data(); 
}; 

#endif
