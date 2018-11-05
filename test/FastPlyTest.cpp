#include <iostream>
#include "DataLayout.h"
#include "fastply/fastply.h"
#include "gtest/gtest.h"

using namespace fastply;

/********************************************************************
 * Verify size of structs match the definition of the ply format.   *
 * This ensures that the current platform is suitable with out      *
 * further adjustments.                                             *
 *******************************************************************/
class FastPlyDataLayout : public testing::Test {};

TEST_F(FastPlyDataLayout, LayoutTests) {
  ASSERT_EQ(sizeof(Vertex), 27);
  ASSERT_EQ(sizeof(Camera), 84);
  ASSERT_EQ(sizeof(Alltypes), 26);
  ASSERT_EQ(sizeof(Face), 17);
}

/********************************************************************
 * Check basic funtionality, such as initialization, accessing      *
 * iterators, derefencing etc.                                      *
 *******************************************************************/
class FastPlyBasicFunctionality : public testing::Test {
  using FastPlyC = FastPly<Vertex, Camera, Alltypes, Face>;

  void SetUp() override { fp = std::make_unique<FastPlyC>(); }

 public:
  std::unique_ptr<FastPlyC> fp;
};

TEST_F(FastPlyBasicFunctionality, NonInitState) {
  ASSERT_EQ(fp->isBigEndian(), false);
  ASSERT_EQ(fp->isHeaderParsed(), false);
  ASSERT_EQ(fp->getInputPath(), "");
}

TEST_F(FastPlyBasicFunctionality, IncorrectFilepath) {
  auto path = std::string("/dev/null/i_assume_this_does_not_exist");
  ASSERT_THROW(fp->open(path), std::system_error);
}

// Ensures opening and closing a file works as expected
TEST_F(FastPlyBasicFunctionality, Initialization) {
  auto path = std::string("test_many.ply");
  ASSERT_EQ(fp->isBigEndian(), false);
  ASSERT_EQ(fp->isHeaderParsed(), false);
  ASSERT_EQ(fp->open(path), true);
  ASSERT_EQ(fp->isHeaderParsed(), true);
  ASSERT_EQ(fp->isBigEndian(), false);
  ASSERT_EQ(fp->get<Vertex>().size(), 1232);
  ASSERT_NO_THROW(fp->close());
}

TEST_F(FastPlyBasicFunctionality, RandomAccessMethods) {
  auto path = std::string("test_many.ply");
  ASSERT_EQ(fp->open(path), true);
  ASSERT_EQ(fp->get<Vertex>()[0], fp->get<Vertex>().at(0));
  ASSERT_EQ(fp->get<Vertex>()[0], fp->get<Vertex>().front());
  ASSERT_EQ(fp->get<Vertex>()[fp->get<Vertex>().size() - 1],
            fp->get<Vertex>().back());
  ASSERT_THROW(fp->get<Vertex>().at(-1), std::out_of_range);
  ASSERT_THROW(fp->get<Vertex>().at(fp->get<Vertex>().size()),
               std::out_of_range);
}

TEST_F(FastPlyBasicFunctionality, Iterators) {
  auto path = std::string("test_many.ply");
  ASSERT_EQ(fp->open(path), true);
  ASSERT_EQ(sizeof(Vertex), sizeof(fp->get<Vertex>().front()));
  ASSERT_EQ(fp->get<Vertex>()[fp->get<Vertex>().size() - 1],
            fp->get<Vertex>().back());
  ASSERT_EQ(fp->get<Vertex>()[0], *fp->get<Vertex>().data());
  ASSERT_EQ(fp->get<Vertex>()[0], *fp->get<Vertex>().begin());
  ASSERT_EQ(fp->get<Vertex>().back(), *(fp->get<Vertex>().end() - 1));
}

TEST_F(FastPlyBasicFunctionality, PointerDifferences) {
  auto path = std::string("test_many.ply");
  ASSERT_EQ(fp->open(path), true);
  ASSERT_EQ(fp->get<Vertex>().end() - fp->get<Vertex>().begin(), 1232);
  ASSERT_EQ(fp->get<Vertex>().end() - fp->get<Vertex>().data(), 1232);
}

/********************************************************************
 * Edge case: Element definition, but not instances present in      *
 * binary section.                                                  *
 *******************************************************************/
class FastPlyEmptyElements : public testing::Test {
  using FastPlyC = FastPly<Vertex, Camera, Alltypes, Face>;

  void SetUp() override { fp = std::make_unique<FastPlyC>(); }

 public:
  std::unique_ptr<FastPlyC> fp;
};

TEST_F(FastPlyEmptyElements, Initialization) {
  auto path = std::string("test_empty_element.ply");
  ASSERT_EQ(fp->open(path), true);
  ASSERT_EQ(fp->isHeaderParsed(), true);
  ASSERT_EQ(fp->get<Vertex>().size(), 0);
  ASSERT_EQ(fp->get<Vertex>().begin(), fp->get<Vertex>().end());
}

TEST_F(FastPlyEmptyElements, AtRangeCheck) {
  auto path = std::string("test_empty_element.ply");
  ASSERT_EQ(fp->open(path), true);
  ASSERT_THROW(fp->get<Vertex>().at(-1), std::out_of_range);
  ASSERT_THROW(fp->get<Vertex>().at(0), std::out_of_range);
  ASSERT_THROW(fp->get<Vertex>().at(fp->get<Vertex>().size()),
               std::out_of_range);
}

TEST_F(FastPlyEmptyElements, PointerState) {
  auto path = std::string("test_empty_element.ply");
  ASSERT_EQ(fp->open(path), true);
  ASSERT_EQ(fp->get<Vertex>().begin(), fp->get<Vertex>().end());
  ASSERT_EQ(fp->get<Camera>().begin(),
            reinterpret_cast<const Camera*>(fp->get<Vertex>().end()));
  ASSERT_EQ(fp->get<Alltypes>().begin(),
            reinterpret_cast<const Alltypes*>(fp->get<Vertex>().end()));
  ASSERT_EQ(fp->get<Face>().begin(),
            reinterpret_cast<const Face*>(fp->get<Vertex>().end()));
  ASSERT_EQ(fp->get<Face>().begin(), fp->get<Face>().end());
}

/********************************************************************
 * Verifies that multiple instances don't conflict, in particular   *
 * when opened on the same file.                                    *
 *******************************************************************/
class FastPlyMultiInstance : public testing::Test {
  using FastPlyC = FastPly<Vertex, Camera, Alltypes, Face>;

  void SetUp() override {
    fp1 = std::make_unique<FastPlyC>();
    fp2 = std::make_unique<FastPlyC>();
  }

 public:
  std::unique_ptr<FastPlyC> fp1;
  std::unique_ptr<FastPlyC> fp2;
};

TEST_F(FastPlyMultiInstance, PointerState) {
  auto path = std::string("test_many.ply");
  ASSERT_EQ(fp1->open(path), true);
  ASSERT_EQ(fp2->open(path), true);

  auto& el1 = fp1->get<Vertex>();
  auto& el2 = fp2->get<Vertex>();
  ASSERT_NE(&el1, &el2);  // sanity check
  ASSERT_NE(el1.data(), el2.data());
  ASSERT_EQ(el1.front(), el2.front());
}

/********************************************************************
 * Test class when no template arguments are provided (ply file     *
 * without any element definitions.                                 *
 * EDIT: Not needed anymore, since static_assert inside the class   *
 * will emit a warning to the user.                                 *
 *******************************************************************/
// class FastPlyEmptyDefinition : public testing::Test {

//     using FastPlyC = FastPly<>;

//     void SetUp() override {
//         fp = std::make_unique<FastPlyC>();
//     }

// public:
//     std::unique_ptr<FastPlyC> fp;
// };

// TEST_F(FastPlyEmptyDefinition, MismatchElementDefinitions)
// {
//     auto path = std::string("test_no_element.ply");
//     ASSERT_EQ(fp->open(path), false);
// }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}