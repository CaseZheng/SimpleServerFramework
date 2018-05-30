// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: basic_protocol.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "basic_protocol.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace basic_protocol {

namespace {

const ::google::protobuf::Descriptor* PbBasicProtocol_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  PbBasicProtocol_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_basic_5fprotocol_2eproto() {
  protobuf_AddDesc_basic_5fprotocol_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "basic_protocol.proto");
  GOOGLE_CHECK(file != NULL);
  PbBasicProtocol_descriptor_ = file->message_type(0);
  static const int PbBasicProtocol_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbBasicProtocol, strversion_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbBasicProtocol, strserverid_),
  };
  PbBasicProtocol_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      PbBasicProtocol_descriptor_,
      PbBasicProtocol::default_instance_,
      PbBasicProtocol_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbBasicProtocol, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PbBasicProtocol, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(PbBasicProtocol));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_basic_5fprotocol_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    PbBasicProtocol_descriptor_, &PbBasicProtocol::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_basic_5fprotocol_2eproto() {
  delete PbBasicProtocol::default_instance_;
  delete PbBasicProtocol_reflection_;
}

void protobuf_AddDesc_basic_5fprotocol_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\024basic_protocol.proto\022\016basic_protocol\":"
    "\n\017PbBasicProtocol\022\022\n\nstrVersion\030\001 \002(\t\022\023\n"
    "\013strServerId\030\002 \002(\t", 98);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "basic_protocol.proto", &protobuf_RegisterTypes);
  PbBasicProtocol::default_instance_ = new PbBasicProtocol();
  PbBasicProtocol::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_basic_5fprotocol_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_basic_5fprotocol_2eproto {
  StaticDescriptorInitializer_basic_5fprotocol_2eproto() {
    protobuf_AddDesc_basic_5fprotocol_2eproto();
  }
} static_descriptor_initializer_basic_5fprotocol_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int PbBasicProtocol::kStrVersionFieldNumber;
const int PbBasicProtocol::kStrServerIdFieldNumber;
#endif  // !_MSC_VER

PbBasicProtocol::PbBasicProtocol()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void PbBasicProtocol::InitAsDefaultInstance() {
}

PbBasicProtocol::PbBasicProtocol(const PbBasicProtocol& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void PbBasicProtocol::SharedCtor() {
  _cached_size_ = 0;
  strversion_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  strserverid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

PbBasicProtocol::~PbBasicProtocol() {
  SharedDtor();
}

void PbBasicProtocol::SharedDtor() {
  if (strversion_ != &::google::protobuf::internal::kEmptyString) {
    delete strversion_;
  }
  if (strserverid_ != &::google::protobuf::internal::kEmptyString) {
    delete strserverid_;
  }
  if (this != default_instance_) {
  }
}

void PbBasicProtocol::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PbBasicProtocol::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PbBasicProtocol_descriptor_;
}

const PbBasicProtocol& PbBasicProtocol::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_basic_5fprotocol_2eproto();
  return *default_instance_;
}

PbBasicProtocol* PbBasicProtocol::default_instance_ = NULL;

PbBasicProtocol* PbBasicProtocol::New() const {
  return new PbBasicProtocol;
}

void PbBasicProtocol::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_strversion()) {
      if (strversion_ != &::google::protobuf::internal::kEmptyString) {
        strversion_->clear();
      }
    }
    if (has_strserverid()) {
      if (strserverid_ != &::google::protobuf::internal::kEmptyString) {
        strserverid_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool PbBasicProtocol::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string strVersion = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_strversion()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->strversion().data(), this->strversion().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_strServerId;
        break;
      }

      // required string strServerId = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_strServerId:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_strserverid()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->strserverid().data(), this->strserverid().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void PbBasicProtocol::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string strVersion = 1;
  if (has_strversion()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->strversion().data(), this->strversion().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->strversion(), output);
  }

  // required string strServerId = 2;
  if (has_strserverid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->strserverid().data(), this->strserverid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->strserverid(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* PbBasicProtocol::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string strVersion = 1;
  if (has_strversion()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->strversion().data(), this->strversion().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->strversion(), target);
  }

  // required string strServerId = 2;
  if (has_strserverid()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->strserverid().data(), this->strserverid().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->strserverid(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int PbBasicProtocol::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string strVersion = 1;
    if (has_strversion()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->strversion());
    }

    // required string strServerId = 2;
    if (has_strserverid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->strserverid());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PbBasicProtocol::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const PbBasicProtocol* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const PbBasicProtocol*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void PbBasicProtocol::MergeFrom(const PbBasicProtocol& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_strversion()) {
      set_strversion(from.strversion());
    }
    if (from.has_strserverid()) {
      set_strserverid(from.strserverid());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void PbBasicProtocol::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PbBasicProtocol::CopyFrom(const PbBasicProtocol& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PbBasicProtocol::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void PbBasicProtocol::Swap(PbBasicProtocol* other) {
  if (other != this) {
    std::swap(strversion_, other->strversion_);
    std::swap(strserverid_, other->strserverid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata PbBasicProtocol::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = PbBasicProtocol_descriptor_;
  metadata.reflection = PbBasicProtocol_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace basic_protocol

// @@protoc_insertion_point(global_scope)
