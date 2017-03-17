#include "Slocum.h"

// wind is rounded to the nearest Beaufort Force.
std::vector<double> wind_bins = {0., 1., 3., 6., 10., 16., 21.,
                                 27., 33., 40., 47., 55., 63., 75.};

// Directions are stored in PI / 8 increments which let's
// us hold them in a 4 bit integer.  Note that these are
// the bin's edges.
std::vector<double> direction_bins = {
                           -15. * M_PI / 16.,
                           -13. * M_PI / 16.,
                           -11. * M_PI / 16.,
                           -9. * M_PI / 16.,
                           -7. * M_PI / 16.,
                           -5. * M_PI / 16.,
                           -3. * M_PI / 16.,
                           -M_PI / 16.,
                           M_PI / 16.,
                           3. * M_PI / 16.,
                           5. * M_PI / 16.,
                           7. * M_PI / 16.,
                           9. * M_PI / 16.,
                           11. * M_PI / 16.,
                           13. * M_PI / 16.,
                           15. * M_PI / 16.,
                          };

variable_definition_t time_variable_def = {.name = "time",
                                           .long_name = "Time (utc)",
                                           .dims = {TIME_ID},
                                           };

variable_definition_t wind_variable_def = {.name = "wind",
                                           .long_name = "Surface Wind",
                                           .dims = {TIME_ID, REALIZATION_ID, LONGITUDE_ID, LATITUDE_ID},
                                           };

/*
 * Expands a sequence of packed 32 bit (4 byte) integers.
 * The integer arrays are simply a zlib compressed
 */
std::vector<int> expand_small_ints(std::string compressed){

    // Decompress the payload which is zlib compressed
    std::string ints_as_bytes = zlib_decompress(compressed);
    int n = ints_as_bytes.size() / 4;
    // convert the byte string to 32 integers
    int32_t *ints = (int32_t*) ints_as_bytes.c_str();

    std::vector<int> out(n);
    for (int i = 0; i < n; i++){
        out[i] = (int) ints[i];
    }

    return out;
}


std::vector<double> expand_small_floats(std::string compressed,
                                        int least_significant_digit){

    // Decompress the payload which is zlib compressed
    std::string floats_as_bytes = zlib_decompress(compressed);
    int n = floats_as_bytes.size() / 4;
    // convert the byte string to 32 bit floats
    float *floats = (float*) floats_as_bytes.c_str();

    std::vector<double> out;
    float denom = pow10((double) least_significant_digit);
    for (int i = 0; i < n; i++){
        out.push_back(floats[i] / denom);
    }
    return out;
}


std::vector<int> unpack_ints(std::string packed_array, int bits, int size){
    uint8_t *bytes = (uint8_t*) packed_array.c_str();

    int values_per_int = 8 / bits;

    // Creates a mask that we can AND compare to each integer to retrieve the
    // 1, 2, or 4 bit integers.
    uint8_t masks[values_per_int];
    for (int i = 0; i < values_per_int; i++) {
        masks[i] = ((uint8_t) pow(2., (float) bits) - 1) << (i * bits);
    }
    int cnt = 0;
    std::vector<int> out(size);

    for (uint i = 0; i < packed_array.size(); i++){
        // The number of values in the next integer.  This is typically
        // just `values_per_int` unless we're at the end of the array.
        int nvals = std::min(size - cnt, values_per_int);
        for (int j = 0; j < nvals; j++){
            // The masks are used in reverse order, so k indexes backwards.
            int k = nvals - j - 1;
            out[cnt] = (bytes[i] & masks[k]) >> (k * bits);
            cnt += 1;
        }
    }
    return out;
}


std::vector<double> expand_tiny_array(std::string compressed,
                                      std::vector<double> bins,
                                      std::vector<int> shape,
                                      bool wrap, double wrap_value){

    // The total size is the product of all the shape values.
    int size = 1;
    for (uint i = 0; i < shape.size(); i++)
        size *= shape[i];

    int bits = 4;
    std::vector<int> bin_values = unpack_ints(compressed, bits, size);

    std::vector<double> data(bin_values.size());

    int low, high;
    for (uint i = 0; i < bin_values.size(); i++){
        if (wrap) {
            low = bin_values[i] - 1 % (int) bin_values.size();
            high = bin_values[i];
            if (bin_values[i] == 0) {
                data[i] = wrap_value;
            } else {
                data[i] = 0.5 * (bins[low] + bins[high]);
            }
        } else {
            low = std::max(bin_values[i] - 1, 0);
            high = std::min(bin_values[i], (int) bins.size() - 1);
            data[i] = 0.5 * (bins[low] + bins[high]);
        }
    }
    return data;
}


std::vector<double> expand_tiny_direction(std::string compressed,
                                          std::vector<int> shape){
    return expand_tiny_array(compressed, direction_bins, shape, true, -M_PI);
}


std::vector<time_t> expand_time(std::string compressed){
    std::vector<int> encoded_times = expand_small_ints(compressed);

    // Start with a time info object for the origin, day 00/00/0001 00:00:00.
    struct tm calendar_start = {.tm_sec = 0,
                                .tm_min = 0,
                                .tm_hour = 0,
                                .tm_mday = 0,
                                .tm_mon = 0,
                                .tm_year = -1899};

    // The first integer is the number of days since the origin.
    int origin_ordinal = (int) encoded_times[0];
    calendar_start.tm_mday += origin_ordinal;
    // The second integer is the number of seconds into that day.
    int origin_seconds = (int) encoded_times[1];
    calendar_start.tm_sec += origin_seconds;

    std::vector<time_t> data;
    // Now create the array of data by iteratively adding each
    // time offset and pushing it to the array.
    for (uint i=2; i<encoded_times.size(); i++){
        calendar_start.tm_hour += (int) encoded_times[i];
        std::time_t next_time = mktime(&calendar_start);
        data.push_back(next_time);
    }
    return data;
}


std::vector<int> expand_realization(std::string compressed){
    int16_t *ints = (int16_t*) compressed.c_str();
    PRINT_DEBUG("\t" << ints[0] << " realizations");

    std::vector<int> out;
    for (int i = 0; i < ints[0]; i++){
      out.push_back(i);
    }
    return out;
}


double expand_wind(std::string compressed, std::vector<int> shape) {

    int n = compressed.size() / 2;

    std::string compressed_speed = compressed.substr(0, n);
    std::vector<double> speed = expand_tiny_array(compressed_speed, wind_bins, shape);

    assert(shape.size() == 4);
    std::cout << "about to build the data" << std::endl;
//    std::vector<std::vector<std::vector<std::vector<double>>>> data(shape[0]);
    double data[shape[0]][shape[1]][shape[2]][shape[3]];

    for (int i = 0; i < shape[0]; i++){
        for (int j = 0; j < shape[1]; j++){
            for (int k = 0; k < shape[2]; k++){
                for (int l = 0; l < shape[3]; l++){
                  data[i][j][k][l] = speed[i +
                                           shape[0] * j +
                                           shape[1] * shape[0] * k +
                                           shape[2] * shape[1] * shape[0] * l];
                }
            }
        }
    }
    return data[0][0][0][0];
//    return &data;
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


/*
 * Takes a string which may hold multiple variables and returns a map from variable
 * ID to a struct holding the (still compressed) data for that variable.
 */
std::map<VariableID, compressed_variable_t> split_by_variable(std::string encoded_variables){

    int offset;
    std::map<VariableID, compressed_variable_t> variables;

    while (encoded_variables.size()) {
        // A string of encoded variables consists of multiple variable strings
        // concatenated together.  In order to pull them out we need to read one,
        // then inspect it's length to know where in the string we'll find the
        // next.
        compressed_variable_t compressed = extract_one_variable(encoded_variables);
        PRINT_DEBUG("Found a variable with id: " << compressed.id
                    << " length: " << compressed.length);
        variables[compressed.id] = compressed;
        // we need to add 4 here because the first four bytes hold the variable
        // ID and length.
        offset = compressed.length + 4;
        encoded_variables = encoded_variables.substr(offset,
                                                     encoded_variables.size() - offset);
    }
    return variables;
}


EnsembleForecast read_slocum_forecast(std::string filename) {

    std::ifstream file(filename.c_str());
    std::string str;
    std::string file_contents;

    while (std::getline(file, str))
    {
        file_contents += str;
        file_contents.push_back('\n');
    }

    std::string encoded_forecast = zlib_decompress(file_contents);

    int version = encoded_forecast[0];
    assert(version == 1);
    // shorten the data by one so it no longer contains the version.
    encoded_forecast = encoded_forecast.substr(1, encoded_forecast.size() - 1);
    PRINT_DEBUG("Reading forecast file with version: " << version);

    auto variables = split_by_variable(encoded_forecast);

    for (auto it=variables.begin(); it!=variables.end(); it++) {
        std::cout << "id: " << it->first << " length: " << it->second.length << std::endl;
    }

    PRINT_DEBUG("About to parse times");
    std::vector<time_t> times = expand_time(variables[TIME_ID].payload);
    // Maybe print out what times we just read in.
    if (DEBUG) {
        for (auto t = times.begin(); t != times.end(); ++t){
            tm *timeinfo = localtime(&(*t));
            PRINT_DEBUG("\t" << asctime(timeinfo));
        }
    }

    PRINT_DEBUG("About to parse realization");
    std::vector<int> realizations = expand_realization(variables[REALIZATION_ID].payload);

    PRINT_DEBUG("About to parse latitude");
    std::vector<double> lats = expand_small_floats(variables[LATITUDE_ID].payload, 3);
    PRINT_DEBUG("\tFirst Lat: " << lats[0]);
    PRINT_DEBUG("\tLast Lat: " << lats[lats.size() - 1]);

    PRINT_DEBUG("About to parse longitude");
    std::vector<double> lons = expand_small_floats(variables[LONGITUDE_ID].payload, 3);
    PRINT_DEBUG("\tFirst Lon: " << lons[0]);
    PRINT_DEBUG("\tLast Lon: " << lons[lons.size() - 1]);

    PRINT_DEBUG("About to parse the data");

    std::vector<int> shape = {(int) times.size(),
                              (int) realizations.size(),
                              (int) lons.size(),
                              (int) lats.size()};
    expand_wind(variables[WIND_ID].payload, shape);

    EnsembleForecast var;

    return var;
}
