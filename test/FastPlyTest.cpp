#include "fastply.h"

#include "gtest/gtest.h"
#include <iostream>

using namespace fastply;


struct __attribute__ ((packed)) VertexLayout {
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;
};

class FastPlyTest : public testing::Test {

    using FastPlyFixed = FastPly<VertexLayout, int>;

    void SetUp() override {
        fp = std::make_unique<FastPlyFixed>();
    }

public:
    std::unique_ptr<FastPlyFixed> fp;
};

TEST_F(FastPlyTest, NonInitState)
{
    ASSERT_EQ(fp->isBigEndian(), false);
    ASSERT_EQ(fp->isHeaderParsed(), false);
    ASSERT_EQ(fp->getInputPath(), "");
}


TEST_F(FastPlyTest, InitLittleEndian)
{
    auto path = std::string("test_little_endian.ply");
    ASSERT_EQ(fp->isBigEndian(), false);
    ASSERT_EQ(fp->isHeaderParsed(), false);
    ASSERT_EQ(fp->init(path), true);
    ASSERT_EQ(fp->isHeaderParsed(), true);
    ASSERT_EQ(fp->isBigEndian(), false);
}

TEST_F(FastPlyTest, BigEndian)
{
    auto path = std::string("test_big_endian.ply");

    ASSERT_EQ(fp->isBigEndian(), false);
    ASSERT_EQ(fp->isHeaderParsed(), false);
    ASSERT_EQ(fp->init(path), true);
    ASSERT_EQ(fp->isHeaderParsed(), true);
    ASSERT_EQ(fp->isBigEndian(), true);
}

TEST_F(FastPlyTest, HeaderOffset)
{
    auto path = std::string("test_big_endian.ply");
    ASSERT_EQ(fp->init(path), true);
    ASSERT_EQ(fp->getHeaderOffset(), 229);
    path = std::string("test_little_endian.ply");
    ASSERT_EQ(fp->init(path), true);
    ASSERT_EQ(fp->getHeaderOffset(), 232);
}

TEST_F(FastPlyTest, DataSizes)
{
    auto path = std::string("test_big_endian.ply");
    ASSERT_EQ(fp->init(path), true);
    ASSERT_EQ(fp->getHeaderOffset(), 229);
    ASSERT_EQ(fp->getVertexHeader().size, 1337);
    ASSERT_EQ(fp->getVertexHeader().sizeT, 27);
}

TEST_F(FastPlyTest, DataCheck)
{
    auto path = std::string("test_little_endian.ply");
    ASSERT_EQ(fp->init(path), true);
    ASSERT_FLOAT_EQ(fp->getVertexHeader().start->x, 0.15807361694047417);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}