#include "Slocum.h"


void TimeVariable::decompress(compressed_variable_t compressed){

  // Decompress the payload which is zlib compressed
  std::string ints_as_bytes = zlib_decompress(compressed.payload);
  int n = ints_as_bytes.size() / 4;
  // convert the byte string to 32 integers
  int *ints = (int*) ints_as_bytes.c_str();

  // Start with a time info object for the origin, day 00/00/0001 00:00:00.
  struct tm calendar_start = {.tm_sec = 0,
                              .tm_min = 0,
                              .tm_hour = 0,
                              .tm_mday = 0,
                              .tm_mon = 0,
                              .tm_year = -1899};

  // The first integer is the number of days since the origin.
  int origin_ordinal = ints[0];
  calendar_start.tm_mday += origin_ordinal;
  // The second integer is the number of seconds into that day.
  int origin_seconds = ints[1];
  calendar_start.tm_sec += origin_seconds;

  // Now create the array of data by iteratively adding each
  // time offset and pushing it to the array.
  for (int i=2; i<n; i++){
    calendar_start.tm_hour += ints[i];
    std::time_t next_time = mktime(&calendar_start);
    m_data.push_back(next_time);
  }

}


/** Decompress an std::string using zlib and return the original data. */
std::string zlib_decompress(const std::string& str)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") "
            << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}


compressed_variable_t extract_one_variable(std::string encoded_variables){

  // The variable id is simply an integer which we've mapped to an enum
  VariableID variable_id = (VariableID) encoded_variables[0];
  // The variable length is a 24 bit integer which isn't standard so
  // we have to manually combine the three bits.
  int l0 = encoded_variables[1];
  int l1 = encoded_variables[2];
  int l2 = encoded_variables[3];
  int length = (l0 << 16) + (l1 << 8) + l2;

  compressed_variable_t compressed = {.id = variable_id,
                                      .length = length,
                                      .payload = encoded_variables.substr(4, length)};
  return compressed;
}


GriddedVariable read_slocum_forecast(std::string filename) {

  std::ifstream file(filename.c_str());
  std::string str;
  std::string file_contents;

  while (std::getline(file, str))
  {
      file_contents += str;
      file_contents.push_back('\n');
  }

  std::string encoded_variables = zlib_decompress(file_contents);

//  int version = encoded_variables[0];
  encoded_variables = encoded_variables.substr(1, encoded_variables.size() - 1);

  int offset;
  std::map<VariableID, compressed_variable_t> variables;

  while (encoded_variables.size()) {
      compressed_variable_t compressed = extract_one_variable(encoded_variables);
      variables[compressed.id] = compressed;
      offset = compressed.length + 4;
      if (compressed.id == TIME_ID) {
        TimeVariable var;
        var.decompress(compressed);
      }
      encoded_variables = encoded_variables.substr(offset,
                                                   encoded_variables.size() - offset);
  }

  for (auto it=variables.begin(); it!=variables.end(); it++) {
    std::cout << "id: " << it->first << " length: " << it->second.length << std::endl;
  }

  GriddedVariable var;

  return var;
}
