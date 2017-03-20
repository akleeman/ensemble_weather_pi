#include "Slocum.h"
#include <gtest/gtest.h>


TEST(slocum, test_read_file) {
    auto fcst = read_slocum_forecast("/home/kleeman/dev/slocum/queries/east_atlantic_query.fcst");

    auto wind_speed = fcst.get_variable(WIND_SPEED_ID);
    auto wind_direction = fcst.get_variable(WIND_DIRECTION_ID);

    //spot check some of the values
    Tensor<double> dir = wind_direction.get_data();
    EXPECT_DOUBLE_EQ(2.748893571891069, dir.get({13, 11, 15, 12}));
    EXPECT_DOUBLE_EQ(2.3561944901923448, dir.get({1, 9, 3, 5}));

    Tensor<double> speed = wind_speed.get_data();
    EXPECT_DOUBLE_EQ(8., speed.get({13, 11, 15, 12}));
    EXPECT_DOUBLE_EQ(24., speed.get({1, 9, 3, 5}));

    Matrix<double> *lons = wind_speed.get_lons();
    EXPECT_DOUBLE_EQ(lons->get(3, 0), 3.);
    EXPECT_DOUBLE_EQ(lons->get(3, 3), 3.);

    Matrix<double> *lats = wind_speed.get_lats();
    EXPECT_DOUBLE_EQ(lats->get(3, 1), -29.);
    EXPECT_DOUBLE_EQ(lats->get(0, 1), -29.);
}

TEST(slocum, test_unpack_ints) {
    // This takes a packed array containing and odd number of values
    // from 0-14, and unpacks it making sure the correct values are
    // recovered.
    std::string packed = "\x01#Eg\x89\xab\xcd\x0e";
    int bits = 4;
    int size = 15;
    std::vector<int> unpacked = unpack_ints(packed, bits, size);
    for (uint i = 0; i < unpacked.size(); i++)
        EXPECT_EQ(unpacked[i], i);
}


TEST(slocum, test_expand_tiny) {
    // This takes a packed array containing and odd number of values
    // from 0-14, and wind bins and expands it as a tiny packed array
    std::string packed = "\x01#Eg\x89\xab\xcd\x0e";
    std::vector<double> bins = {0., 1., 3., 6., 10., 16., 21., 27., 33., 40., 47., 55., 63., 75.};
    std::vector<int> shape = {15};

    std::vector<double> data = expand_tiny_array(packed, bins, shape);

    // The result should incrementally follow the midpoints except at the end
    // where they are pinned to the edge values.
    std::vector<double> expected = {0., 0.5, 2., 4.5, 8., 13., 18.5,
                                    24., 30., 36.5, 43.5, 51., 59., 69., 75.};

    EXPECT_EQ(data.size(), expected.size());
    for (uint i = 0; i < data.size(); i++){
        EXPECT_EQ(data[i], expected[i]);
    }
}


TEST(slocum, test_expand_tiny_direction) {
    std::string packed = "\x01#Eg\x89\xab\xcd\xef\x00";
    std::vector<double> out = expand_tiny_direction(packed, {17});

    double expected;
    for (uint i = 0; i < out.size(); i++){
        expected = (-16. + 2 * (double) i) / 16. * M_PI;
        expected = fmod(expected + M_PI, 2 * M_PI) - M_PI;
        EXPECT_TRUE(expected < out[i] + 0.0001);
        EXPECT_TRUE(expected > out[i] - 0.0001);
    }
}


TEST(slocum, test_expand_time) {
    char data [] = {'x', '\xda', '{', 'g', '\xce', '\xcd', 'p', '`', '\x05', '\x03',
                    '\x18', '0', '\x12', '\x81', '\x01', 'v', '\xcf', '\x02', '\xa2'};
    std::string packed(data, 19);
    std::vector<time_t> out = expand_time(packed);

    // Expected origin is Sun Dec 12 12:00:00 2013
    tm expected_origin = {.tm_sec = 0,
                          .tm_min = 0,
                          .tm_hour = 12,
                          .tm_mday = 12,
                          .tm_mon = 11,
                          .tm_year = 113};

    // Now iterate through each of the expected times and make sure
    // it matches the actual time.
    tm expected = expected_origin;
    time_t one_time;
    for (uint i = 0; i < out.size(); i++){
        expected.tm_hour = expected_origin.tm_hour + i;
        one_time = mktime(&expected);
        EXPECT_EQ(one_time, out[i]);
    }
}
