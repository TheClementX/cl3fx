#include "wav_parser.h"

WavParser::WavParser() {
	this->file = ""; 
}

bool verify_tag(const std::vector<uint8_t>& buff, size_t offset, const std::string& ref) {
	if(offset + ref.size() > buff.size()) 
		return false; 
	return std::equal(ref.begin(), ref.end(), buff.begin() + offset); 
}

int WavParser::parse_file(const std::string& file) {
	this->file = file; 

	//process wave file
	if(this->read_wav(file) < 0) return -1; 
	if(this->parse_wav() < 0) return -1;
	if(this->normalize_data() < 0) return -1; 

	//return success
	return 0; 
}

int WavParser::read_wav(const std::string& file) {
	std::ifstream infile(file, std::ios::binary); 
	if(!infile) {
		return -1; 
	}

	infile.seekg(0, std::ios::end); 

	std::streamsize size = infile.tellg(); 
	if(size <= 0) {
		return -1; 
	}

	infile.seekg(0, std::ios::beg); 

	this->raw_data = std::vector<uint8_t>(size); 
	if(!infile.read(reinterpret_cast<char *>(this->raw_data.data()), size)) {
		return -1; 
	}

	return 0; 
}

int WavParser::parse_wav() {
//read RIFF chunk Header
	size_t curpos = 0; 
	if(!verify_tag(this->raw_data, curpos, "RIFF"))
		return -1; 
	curpos += 4; 

	this->meta_data.file_size = le_read<uint32_t>(this->raw_data, curpos); 
	curpos += 4; 

	if(!verify_tag(this->raw_data, curpos, "WAVE"))
		return -1; 
	curpos += 4; 
	
	//read format chunk 
	if(!verify_tag(this->raw_data, curpos, "fmt "))
		return -1; 
	curpos += 4; 

	this->meta_data.fmt_size = le_read<uint32_t>(this->raw_data, curpos); 
	curpos += 4;
	uint32_t next_chunk = this->meta_data.fmt_size + curpos; 
	this->meta_data.audio_fmt = le_read<uint16_t>(this->raw_data, curpos); 
	curpos += 2;
	this->meta_data.num_channels = le_read<uint16_t>(this->raw_data, curpos); 
	curpos += 2;
	this->meta_data.sample_rate = le_read<uint32_t>(this->raw_data, curpos); 
	curpos += 4;
	this->meta_data.byte_rate = le_read<uint32_t>(this->raw_data, curpos); 
	curpos += 4;
	this->meta_data.block_align = le_read<uint16_t>(this->raw_data, curpos); 
	curpos += 2;
	this->meta_data.bits_per_sample = le_read<uint16_t>(this->raw_data, curpos); 
	curpos += 2;
	
	curpos = next_chunk; 
	//seek data section
	while(!verify_tag(this->raw_data, curpos, "data")) {
		curpos += 4; 
		uint32_t chunk_size = le_read<uint32_t>(this->raw_data, curpos); 
		curpos += 4 + static_cast<size_t>(chunk_size); 
	}
	curpos += 4; 
	this->meta_data.data_size = le_read<uint32_t>(this->raw_data, curpos); 
	curpos += 4; 

	this->meta_data.data_offset = curpos; 
	return 0; 
}

double WavParser::bytes_to_double(uint8_t *bytes, size_t byte_width, size_t fmt) {
	//IEEE FLOAT 32
	if(fmt == 3 && byte_width == 4) {
		float result; 
		std::memcpy(&result, bytes, byte_width); 
		return static_cast<double>(result); 
	//32 bit signed int
	} else if(fmt == 1 && byte_width == 4) {
		int32_t result; 
		std::memcpy(&result, bytes, byte_width); 
		double scale = 2147483648.0; 
		return static_cast<double>(result) / scale; 
	//24 bit signed int
	} else if(fmt == 1 && byte_width == 3) {
		//extract bits
		int32_t result = (static_cast<int32_t>(bytes[2]) << 16) 
					   | (static_cast<int32_t>(bytes[1]) << 8) 
					   | static_cast<int32_t>(bytes[0]); 

		//correct sign
		if(result & 0x00800000) 
			result |= 0xff000000; 

		//scale
		double scale = 8388608.0; 
		return static_cast<double>(result) / scale; 
	//16 bit signed int
	} else if(fmt == 1 && byte_width == 2) {
		int16_t result; 
		std::memcpy(&result, bytes, byte_width); 
		double scale = 32768.0; 
		return static_cast<double>(result) / scale; 
	//8 bit unsigned int
	} else if(fmt == 1 && byte_width == 1) {
		double scale = 128.0; 
		return (static_cast<double>(*bytes) - scale) / scale; 
	}

	//error = no amplitude
	//note this is a silent error
	return 0.0; 
}

int WavParser::normalize_data() {
	//get neccessary metadata
	uint16_t num_channels = this->meta_data.num_channels; 
	uint32_t offset = this->meta_data.data_offset; 
	uint16_t bit_depth = this->meta_data.bits_per_sample; 
	uint32_t data_size = this->meta_data.data_size; 
	uint16_t fmt = this->meta_data.audio_fmt; 

	if(num_channels == 0 || bit_depth == 0 || data_size == 0) {
		return -1; 
	}

	//init channels
	this->norm_channels = std::vector<std::vector<double>>(num_channels); 

	uint16_t byte_depth = bit_depth / 8; 
	uint16_t frame_size = num_channels * byte_depth; 
	uint8_t *raw_data_bytes = this->raw_data.data(); 


	//main processing loop
	for(uint32_t i = offset; i < (offset+data_size); i += frame_size) {
		if(i + frame_size > this->raw_data.size())
			return -1; 

		for(int c = 0; c < num_channels; ++c) {
			uint8_t *cur_data = &raw_data_bytes[i + (c * byte_depth)]; 
			double val = this->bytes_to_double(cur_data, (size_t)byte_depth, (size_t)fmt); 
			this->norm_channels[c].push_back(val);  
		}
	}
	//success
	return 0; 
}
