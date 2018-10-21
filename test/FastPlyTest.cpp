#include "fastply.h"
#include "DataLayouts.h"
#include "gtest/gtest.h"
#include <iostream>

using namespace fastply;

using Vertex = PlyElement<VertexLayout>;
using Camera = PlyElement<CameraLayout>;
using Alltypes = PlyElement<AlltypesLayout>;
using Face = PlyElement<FaceLayout>;


class FastPlyDataLayout : public testing::Test {};

TEST_F(FastPlyDataLayout, LayoutTests)
{
    ASSERT_EQ(sizeof(VertexLayout), 27);
    ASSERT_EQ(sizeof(CameraLayout), 84);
    ASSERT_EQ(sizeof(AlltypesLayout), 26);
    ASSERT_EQ(sizeof(FaceLayout), 17);
}

class FastPlyEmptyDefinition : public testing::Test {

    using FastPlyC = FastPly<>;

    void SetUp() override {
        fp = std::make_unique<FastPlyC>();
    }

public:
    std::unique_ptr<FastPlyC> fp;
};

TEST_F(FastPlyEmptyDefinition, MismatchElementDefinitions)
{
    auto path = std::string("test_no_element.ply");
    ASSERT_EQ(fp->init(path), false);
}

class FastPlyEmptyElements : public testing::Test {

    using FastPlyC = FastPly<Vertex, Camera, Alltypes, Face>;

    void SetUp() override {
        fp = std::make_unique<FastPlyC>();
    }

public:
    std::unique_ptr<FastPlyC> fp;
};

TEST_F(FastPlyEmptyElements, Initialization)
{
    auto path = std::string("test_empty_element.ply");
    ASSERT_EQ(fp->init(path), true);
    ASSERT_EQ(fp->isHeaderParsed(), true);
    ASSERT_EQ(fp->get<Vertex>().size, 0);
    ASSERT_EQ(fp->get<Vertex>().start, fp->get<Vertex>().end);
}

TEST_F(FastPlyEmptyElements, AtRangeCheck)
{
    auto path = std::string("test_empty_element.ply");
    ASSERT_EQ(fp->init(path), true);
    ASSERT_THROW(fp->get<Vertex>().at(-1), std::out_of_range);
    ASSERT_THROW(fp->get<Vertex>().at(0), std::out_of_range);
    ASSERT_THROW(fp->get<Vertex>().at(fp->get<Vertex>().size), std::out_of_range);
}

TEST_F(FastPlyEmptyElements, PointerState)
{
    auto path = std::string("test_empty_element.ply");
    ASSERT_EQ(fp->init(path), true);
    ASSERT_EQ(fp->get<Vertex>().start, fp->get<Vertex>().end);
    ASSERT_EQ(fp->get<Camera>().start, reinterpret_cast<const CameraLayout*>(fp->get<Vertex>().end));
    ASSERT_EQ(fp->get<Alltypes>().start, reinterpret_cast<const AlltypesLayout*>(fp->get<Vertex>().end));
    ASSERT_EQ(fp->get<Face>().start, reinterpret_cast<const FaceLayout*>(fp->get<Vertex>().end));
    ASSERT_EQ(fp->get<Face>().start, fp->get<Face>().end);
}

class FastPlyBasicFunctionality : public testing::Test {

    using FastPlyC = FastPly<Vertex, Camera, Alltypes, Face>;

    void SetUp() override {
        fp = std::make_unique<FastPlyC>();
    }

public:
    std::unique_ptr<FastPlyC> fp;
};

TEST_F(FastPlyBasicFunctionality, NonInitState)
{
    ASSERT_EQ(fp->isBigEndian(), false);
    ASSERT_EQ(fp->isHeaderParsed(), false);
    ASSERT_EQ(fp->getInputPath(), "");
}

TEST_F(FastPlyBasicFunctionality, IncorrectFilepath)
{
    // Todo: Windows compatibility?
    auto path = std::string("/dev/null/i_assume_this_does_not_exist");
    ASSERT_THROW(fp->init(path), std::system_error);
}

TEST_F(FastPlyBasicFunctionality, Initialization)
{
    auto path = std::string("test_many.ply");
    ASSERT_EQ(fp->isBigEndian(), false);
    ASSERT_EQ(fp->isHeaderParsed(), false);
    ASSERT_EQ(fp->init(path), true);
    ASSERT_EQ(fp->isHeaderParsed(), true);
    ASSERT_EQ(fp->isBigEndian(), false);
}

TEST_F(FastPlyBasicFunctionality, ElementAccessMethods)
{
    auto path = std::string("test_many.ply");
    ASSERT_EQ(fp->init(path), true);
    const auto& elements = fp->getElements();
    ASSERT_EQ(sizeof(VertexLayout), std::get<0>(elements).sizeT);
    ASSERT_EQ(sizeof(VertexLayout), std::get<Vertex>(elements).sizeT);
    ASSERT_EQ(sizeof(VertexLayout), fp->get<Vertex>().sizeT);
}

TEST_F(FastPlyBasicFunctionality, PointerDifferences)
{
    auto path = std::string("test_many.ply");
    ASSERT_EQ(fp->init(path), true);
    ASSERT_EQ(fp->get<Vertex>().size, 1232);
    ASSERT_EQ(fp->get<Vertex>().end - fp->get<Vertex>().start, 1232);
}


TEST_F(FastPlyBasicFunctionality, DataReadOutLittleEndian)
{
    auto path = std::string("test_many.ply");
    ASSERT_EQ(fp->init(path), true);

    int count = 0;

    // Validate regular element with content
    ASSERT_EQ(fp->get<Vertex>().size, 1232);
    for(std::size_t i = 0; i < fp->get<Vertex>().size; ++i) {
        ASSERT_EQ(fp->get<Vertex>()[i].x, count++%128);
        ASSERT_EQ(fp->get<Vertex>()[i].y, count++%128);
        ASSERT_EQ(fp->get<Vertex>()[i].z, count++%128);
        ASSERT_EQ(fp->get<Vertex>()[i].nx, count++%128);
        ASSERT_EQ(fp->get<Vertex>()[i].ny, count++%128);
        ASSERT_EQ(fp->get<Vertex>()[i].nz, count++%128);
        ASSERT_EQ(fp->get<Vertex>()[i].red, count++%128);
        ASSERT_EQ(fp->get<Vertex>()[i].green, count++%128);
        ASSERT_EQ(fp->get<Vertex>()[i].blue, count++%128);
    }

    // Validate single entry element
    ASSERT_EQ(fp->get<Camera>().size, 1);
    for(std::size_t i = 0; i < fp->get<Camera>().size; ++i) {
        ASSERT_EQ(fp->get<Camera>()[i].view_px, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].view_py, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].view_pz, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].x_axisx, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].x_axisy, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].x_axisz, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].y_axisx, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].y_axisy, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].y_axisz, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].z_axisx, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].z_axisy, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].z_axisz, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].focal, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].scalex, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].scaley, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].centerx, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].centery, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].viewportx, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].viewporty, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].k1, count++%128);
        ASSERT_EQ(fp->get<Camera>()[i].k2, count++%128);
    }

    // Validate empty element
    ASSERT_EQ(fp->get<Alltypes>().size, 0);

    // Validate list
    ASSERT_EQ(fp->get<Face>().size, 15);
    for(std::size_t i = 0; i < fp->get<Face>().size; ++i) {
        ASSERT_EQ(static_cast<unsigned int>(fp->get<Face>()[i].list_length), 4);
        for(std::size_t j = 0; j < static_cast<unsigned int>(fp->get<Face>()[i].list_length); ++j)
        {
            ASSERT_EQ(fp->get<Face>()[i].vertex_index[j], count++%128);
        }
    }
}


// TEST_F(FastPlyBasicFunctionality, FilledElementAtRangeCheck)
// {
//     // Ply with at least one element and 2 or more entries for that element
//     auto path = std::string("test15117.ply");
//     ASSERT_EQ(fp->init(path), true);
//     ASSERT_THROW(fp->get<Vertex>().at(-1), std::out_of_range);
//     ASSERT_THROW(fp->get<Vertex>().at(15117), std::out_of_range);
//     ASSERT_THROW(fp->get<Vertex>().at(fp->get<Vertex>().size), std::out_of_range);
//     ASSERT_NO_THROW(fp->get<Vertex>().at(0));
//     ASSERT_NO_THROW(fp->get<Vertex>().at(15116));
//     ASSERT_NO_THROW(fp->get<Vertex>().at(fp->get<Vertex>().size - 1));
// }

// TEST_F(FastPlyBasicFunctionality, BigEndian)
// {
//     auto path = std::string("test_big_endian.ply");

//     ASSERT_EQ(fp->isBigEndian(), false);
//     ASSERT_EQ(fp->isHeaderParsed(), false);
//     ASSERT_EQ(fp->init(path), true);
//     ASSERT_EQ(fp->isHeaderParsed(), true);
//     ASSERT_EQ(fp->isBigEndian(), true);
// }

// TEST_F(FastPlyBasicFunctionality, HeaderOffset)
// {
//     auto path = std::string("test_big_endian.ply");
//     ASSERT_EQ(fp->init(path), true);
//     ASSERT_EQ(fp->getHeaderOffset(), 229);
//     path = std::string("test_little_endian.ply");
//     ASSERT_EQ(fp->init(path), true);
//     ASSERT_EQ(fp->getHeaderOffset(), 232);
// }

// TEST_F(FastPlyBasicFunctionality, DataSizes)
// {
//     auto path = std::string("test_big_endian.ply");
//     ASSERT_EQ(fp->init(path), true);
//     ASSERT_EQ(fp->getHeaderOffset(), 229);
//     ASSERT_EQ(fp->getVertexHeader().size, 1337);
//     ASSERT_EQ(fp->getVertexHeader().sizeT, 27);
// }

// TEST_F(FastPlyBasicFunctionality, DataCheck)
// {
//     auto path = std::string("test_little_endian.ply");
//     ASSERT_EQ(fp->init(path), true);
//     ASSERT_FLOAT_EQ(fp->getVertexHeader().start->x, 0.15807361694047417);
// }

// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}