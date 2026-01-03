#include "wav_parser.h"

WavParser::WavParser() {
	this->file = ""; 
}

bool verify_tag(const std::vector<uint8_t>& buff, size_t offset, const std::string& ref) {
	if(offset + ref.size() > buff.size()) 
		return false; 

	return std::equal(ref.begin(), ref.end(), buff.begin() + offset); 
}

channel_v WavParser::read_file(const std::string& file) {
	this->file = file; 

	//process wave file
	if(-1 == this->read_wav(file)) {
		//return -1.0 signal
		return channel_v{{-1.0}}; 
	}
	if(-1 == this->parse_wav()) {
		//return -1.0 signal
		return channel_v{{-1.0}}; 
	}
	if(-1 == this->normalize_data()) {
		//return -1.0 signal
		return channel_v{{-1.0}}; 
	}

	//return success
	return this->norm_channels; 
}

int WavParser::write_file(
	std::string& name, 
	const channel_v& data, 
	uint32_t sample_rate
) {
	std::ofstream outfile(name + ".wav", std::ios::binary); 
	if(!outfile) {
		return -1; 
	}
	std::vector<float> to_write = this->interleave_channels(data); 

	//riff header
	outfile << "RIFF"; 
	//file size
	uint32_t dsize = to_write.size() * 4; 
	uint32_t fsize = 36 + dsize; 
	outfile.write(reinterpret_cast<const char*>(&fsize), sizeof(fsize)); 
	//wave header
	outfile << "WAVE"; 
	//fmt header
	outfile << "fmt "; 
	//fmt size
	uint32_t fmt_size = 16; 
	outfile.write(reinterpret_cast<const char*>(&fmt_size), sizeof(fmt_size)); 
	//fmt type IEEE float 32
	uint16_t type = 3; 
	outfile.write(reinterpret_cast<const char*>(&type), sizeof(type)); 
	//number of channels
	uint16_t num_chan = data.size(); 
	outfile.write(reinterpret_cast<const char*>(&num_chan), sizeof(num_chan)); 
	//sample rate
	outfile.write(reinterpret_cast<const char*>(&sample_rate), sizeof(sample_rate)); 

	//bit rate fields
	uint16_t bits_per_sample = 32; 
	uint16_t block_align = num_chan * bits_per_sample / 8; 
	uint32_t byte_rate = sample_rate * bits_per_sample * num_chan / 8; 

	outfile.write(reinterpret_cast<const char*>(&byte_rate), sizeof(byte_rate)); 
	outfile.write(reinterpret_cast<const char*>(&block_align), sizeof(block_align)); 
	outfile.write(reinterpret_cast<const char*>(&bits_per_sample), sizeof(bits_per_sample)); 

	//write data size
	outfile << "data"; 
	outfile.write(reinterpret_cast<const char*>(&dsize), sizeof(dsize)); 
	//write data
	outfile.write(reinterpret_cast<const char*>(to_write.data()), to_write.size() * sizeof(float)); 

	return 0; 
}

void WavParser::print_wav() {
	std::cout << "______________________________________" << std::endl; 
	std::cout << "file_path : " << this->file << std::endl; 
	std::cout << "file_size : " << this->meta_data.file_size << std::endl; 
	std::cout << "fmt_size : " << this->meta_data.fmt_size << std::endl; 
	std::cout << "audio_fmt : " << this->meta_data.audio_fmt << std::endl; 
	std::cout << "num_channels : " << this->meta_data.num_channels << std::endl; 
	std::cout << "sample_rate : " << this->meta_data.sample_rate << std::endl; 
	std::cout << "byte_rate : " << this->meta_data.byte_rate << std::endl; 
	std::cout << "block_align : " << this->meta_data.block_align << std::endl; 
	std::cout << "bits_per_sample : " << this->meta_data.bits_per_sample << std::endl; 
	std::cout << "data_size : " << this->meta_data.data_size << std::endl; 
	std::cout << "data_offset : " << this->meta_data.data_offset << std::endl; 
	std::cout << "______________________________________\n" << std::endl; 
}

int WavParser::read_wav(const std::string& file) {
	std::ifstream infile(file, std::ios::binary); 
	if(!infile) {
		std::cout << "file open failed in read_wav" << std::endl; 
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

//must fix parsing error chunk skipping for the fmt section
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

	//seek fmt_section
	while(!verify_tag(this->raw_data, curpos, "fmt ")) {
		curpos += 4; 
		uint32_t chunk_size = le_read<uint32_t>(this->raw_data, curpos); 
		curpos += 4 + static_cast<size_t>(chunk_size); 
	}
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
	this->norm_channels = channel_v(num_channels); 

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

std::vector<float> WavParser::interleave_channels(const channel_v& c) {
	int num_chan = c.size(); 
	int chan_len = c[0].size(); 

	std::vector<float> result; 

	for(int i = 0; i < chan_len; ++i) {
		for(int j = 0; j < num_chan; ++j)
			result.push_back(static_cast<float>(c[j][i])); 
	}

	return result; 
}
