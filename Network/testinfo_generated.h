// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TESTINFO_H_
#define FLATBUFFERS_GENERATED_TESTINFO_H_

#include "flatbuffers/flatbuffers.h"

struct TestInfo;

struct TestInfo FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_LEVEL = 6
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  int32_t level() const {
    return GetField<int32_t>(VT_LEVEL, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<int32_t>(verifier, VT_LEVEL) &&
           verifier.EndTable();
  }
};

struct TestInfoBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(TestInfo::VT_NAME, name);
  }
  void add_level(int32_t level) {
    fbb_.AddElement<int32_t>(TestInfo::VT_LEVEL, level, 0);
  }
  explicit TestInfoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  TestInfoBuilder &operator=(const TestInfoBuilder &);
  flatbuffers::Offset<TestInfo> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<TestInfo>(end);
    return o;
  }
};

inline flatbuffers::Offset<TestInfo> CreateTestInfo(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    int32_t level = 0) {
  TestInfoBuilder builder_(_fbb);
  builder_.add_level(level);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<TestInfo> CreateTestInfoDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    int32_t level = 0) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return CreateTestInfo(
      _fbb,
      name__,
      level);
}

inline const TestInfo *GetTestInfo(const void *buf) {
  return flatbuffers::GetRoot<TestInfo>(buf);
}

inline const TestInfo *GetSizePrefixedTestInfo(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<TestInfo>(buf);
}

inline bool VerifyTestInfoBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<TestInfo>(nullptr);
}

inline bool VerifySizePrefixedTestInfoBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<TestInfo>(nullptr);
}

inline void FinishTestInfoBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<TestInfo> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedTestInfoBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<TestInfo> root) {
  fbb.FinishSizePrefixed(root);
}

#endif  // FLATBUFFERS_GENERATED_TESTINFO_H_
