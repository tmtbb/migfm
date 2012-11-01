// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "response.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace response {
namespace type {

namespace {

const ::google::protobuf::Descriptor* Inlink_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Inlink_reflection_ = NULL;
const ::google::protobuf::Descriptor* Inlinks_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Inlinks_reflection_ = NULL;
const ::google::protobuf::Descriptor* ResponseDatum_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ResponseDatum_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_response_2eproto() {
  protobuf_AddDesc_response_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "response.proto");
  GOOGLE_CHECK(file != NULL);
  Inlink_descriptor_ = file->message_type(0);
  static const int Inlink_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Inlink, from_url_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Inlink, anchor_),
  };
  Inlink_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Inlink_descriptor_,
      Inlink::default_instance_,
      Inlink_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Inlink, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Inlink, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Inlink));
  Inlinks_descriptor_ = file->message_type(1);
  static const int Inlinks_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Inlinks, inlink_),
  };
  Inlinks_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Inlinks_descriptor_,
      Inlinks::default_instance_,
      Inlinks_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Inlinks, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Inlinks, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Inlinks));
  ResponseDatum_descriptor_ = file->message_type(2);
  static const int ResponseDatum_offsets_[7] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ResponseDatum, fetch_time_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ResponseDatum, retries_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ResponseDatum, fetch_interval_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ResponseDatum, score_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ResponseDatum, signature_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ResponseDatum, modified_time_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ResponseDatum, metadatas_),
  };
  ResponseDatum_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ResponseDatum_descriptor_,
      ResponseDatum::default_instance_,
      ResponseDatum_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ResponseDatum, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ResponseDatum, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ResponseDatum));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_response_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Inlink_descriptor_, &Inlink::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Inlinks_descriptor_, &Inlinks::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ResponseDatum_descriptor_, &ResponseDatum::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_response_2eproto() {
  delete Inlink::default_instance_;
  delete Inlink_reflection_;
  delete Inlinks::default_instance_;
  delete Inlinks_reflection_;
  delete ResponseDatum::default_instance_;
  delete ResponseDatum_reflection_;
}

void protobuf_AddDesc_response_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::net::type::protobuf_AddDesc_metadata_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\016response.proto\022\rresponse.type\032\016metadat"
    "a.proto\"*\n\006Inlink\022\020\n\010from_url\030\001 \002(\t\022\016\n\006a"
    "nchor\030\002 \002(\t\"0\n\007Inlinks\022%\n\006inlink\030\001 \002(\0132\025"
    ".response.type.Inlink\"\301\001\n\rResponseDatum\022"
    "\025\n\nfetch_time\030\001 \001(\004:\0010\022\022\n\007retries\030\002 \001(\r:"
    "\0010\022\034\n\016fetch_interval\030\003 \001(\r:\0041000\022\020\n\005scor"
    "e\030\004 \001(\002:\0011\022\023\n\tsignature\030\005 \001(\014:\000\022\030\n\rmodif"
    "ied_time\030\006 \001(\004:\0010\022&\n\tmetadatas\030\007 \001(\0132\023.n"
    "et.type.Metadatas", 337);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "response.proto", &protobuf_RegisterTypes);
  Inlink::default_instance_ = new Inlink();
  Inlinks::default_instance_ = new Inlinks();
  ResponseDatum::default_instance_ = new ResponseDatum();
  Inlink::default_instance_->InitAsDefaultInstance();
  Inlinks::default_instance_->InitAsDefaultInstance();
  ResponseDatum::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_response_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_response_2eproto {
  StaticDescriptorInitializer_response_2eproto() {
    protobuf_AddDesc_response_2eproto();
  }
} static_descriptor_initializer_response_2eproto_;


// ===================================================================

const ::std::string Inlink::_default_from_url_;
const ::std::string Inlink::_default_anchor_;
#ifndef _MSC_VER
const int Inlink::kFromUrlFieldNumber;
const int Inlink::kAnchorFieldNumber;
#endif  // !_MSC_VER

Inlink::Inlink()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Inlink::InitAsDefaultInstance() {
}

Inlink::Inlink(const Inlink& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Inlink::SharedCtor() {
  _cached_size_ = 0;
  from_url_ = const_cast< ::std::string*>(&_default_from_url_);
  anchor_ = const_cast< ::std::string*>(&_default_anchor_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Inlink::~Inlink() {
  SharedDtor();
}

void Inlink::SharedDtor() {
  if (from_url_ != &_default_from_url_) {
    delete from_url_;
  }
  if (anchor_ != &_default_anchor_) {
    delete anchor_;
  }
  if (this != default_instance_) {
  }
}

void Inlink::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Inlink::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Inlink_descriptor_;
}

const Inlink& Inlink::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_response_2eproto();  return *default_instance_;
}

Inlink* Inlink::default_instance_ = NULL;

Inlink* Inlink::New() const {
  return new Inlink;
}

void Inlink::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (from_url_ != &_default_from_url_) {
        from_url_->clear();
      }
    }
    if (_has_bit(1)) {
      if (anchor_ != &_default_anchor_) {
        anchor_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Inlink::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string from_url = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_from_url()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->from_url().data(), this->from_url().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_anchor;
        break;
      }
      
      // required string anchor = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_anchor:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_anchor()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->anchor().data(), this->anchor().length(),
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

void Inlink::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string from_url = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->from_url().data(), this->from_url().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->from_url(), output);
  }
  
  // required string anchor = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->anchor().data(), this->anchor().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->anchor(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Inlink::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string from_url = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->from_url().data(), this->from_url().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->from_url(), target);
  }
  
  // required string anchor = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->anchor().data(), this->anchor().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->anchor(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Inlink::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string from_url = 1;
    if (has_from_url()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->from_url());
    }
    
    // required string anchor = 2;
    if (has_anchor()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->anchor());
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

void Inlink::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Inlink* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Inlink*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Inlink::MergeFrom(const Inlink& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_from_url(from.from_url());
    }
    if (from._has_bit(1)) {
      set_anchor(from.anchor());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Inlink::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Inlink::CopyFrom(const Inlink& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Inlink::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  return true;
}

void Inlink::Swap(Inlink* other) {
  if (other != this) {
    std::swap(from_url_, other->from_url_);
    std::swap(anchor_, other->anchor_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Inlink::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Inlink_descriptor_;
  metadata.reflection = Inlink_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int Inlinks::kInlinkFieldNumber;
#endif  // !_MSC_VER

Inlinks::Inlinks()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Inlinks::InitAsDefaultInstance() {
  inlink_ = const_cast< ::response::type::Inlink*>(&::response::type::Inlink::default_instance());
}

Inlinks::Inlinks(const Inlinks& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Inlinks::SharedCtor() {
  _cached_size_ = 0;
  inlink_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Inlinks::~Inlinks() {
  SharedDtor();
}

void Inlinks::SharedDtor() {
  if (this != default_instance_) {
    delete inlink_;
  }
}

void Inlinks::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Inlinks::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Inlinks_descriptor_;
}

const Inlinks& Inlinks::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_response_2eproto();  return *default_instance_;
}

Inlinks* Inlinks::default_instance_ = NULL;

Inlinks* Inlinks::New() const {
  return new Inlinks;
}

void Inlinks::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (inlink_ != NULL) inlink_->::response::type::Inlink::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Inlinks::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .response.type.Inlink inlink = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_inlink()));
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

void Inlinks::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .response.type.Inlink inlink = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->inlink(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Inlinks::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .response.type.Inlink inlink = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->inlink(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Inlinks::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .response.type.Inlink inlink = 1;
    if (has_inlink()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->inlink());
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

void Inlinks::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Inlinks* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Inlinks*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Inlinks::MergeFrom(const Inlinks& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      mutable_inlink()->::response::type::Inlink::MergeFrom(from.inlink());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Inlinks::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Inlinks::CopyFrom(const Inlinks& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Inlinks::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  if (has_inlink()) {
    if (!this->inlink().IsInitialized()) return false;
  }
  return true;
}

void Inlinks::Swap(Inlinks* other) {
  if (other != this) {
    std::swap(inlink_, other->inlink_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Inlinks::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Inlinks_descriptor_;
  metadata.reflection = Inlinks_reflection_;
  return metadata;
}


// ===================================================================

const ::std::string ResponseDatum::_default_signature_;
#ifndef _MSC_VER
const int ResponseDatum::kFetchTimeFieldNumber;
const int ResponseDatum::kRetriesFieldNumber;
const int ResponseDatum::kFetchIntervalFieldNumber;
const int ResponseDatum::kScoreFieldNumber;
const int ResponseDatum::kSignatureFieldNumber;
const int ResponseDatum::kModifiedTimeFieldNumber;
const int ResponseDatum::kMetadatasFieldNumber;
#endif  // !_MSC_VER

ResponseDatum::ResponseDatum()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ResponseDatum::InitAsDefaultInstance() {
  metadatas_ = const_cast< ::net::type::Metadatas*>(&::net::type::Metadatas::default_instance());
}

ResponseDatum::ResponseDatum(const ResponseDatum& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ResponseDatum::SharedCtor() {
  _cached_size_ = 0;
  fetch_time_ = GOOGLE_ULONGLONG(0);
  retries_ = 0u;
  fetch_interval_ = 1000u;
  score_ = 1;
  signature_ = const_cast< ::std::string*>(&_default_signature_);
  modified_time_ = GOOGLE_ULONGLONG(0);
  metadatas_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ResponseDatum::~ResponseDatum() {
  SharedDtor();
}

void ResponseDatum::SharedDtor() {
  if (signature_ != &_default_signature_) {
    delete signature_;
  }
  if (this != default_instance_) {
    delete metadatas_;
  }
}

void ResponseDatum::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ResponseDatum::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ResponseDatum_descriptor_;
}

const ResponseDatum& ResponseDatum::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_response_2eproto();  return *default_instance_;
}

ResponseDatum* ResponseDatum::default_instance_ = NULL;

ResponseDatum* ResponseDatum::New() const {
  return new ResponseDatum;
}

void ResponseDatum::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    fetch_time_ = GOOGLE_ULONGLONG(0);
    retries_ = 0u;
    fetch_interval_ = 1000u;
    score_ = 1;
    if (_has_bit(4)) {
      if (signature_ != &_default_signature_) {
        signature_->clear();
      }
    }
    modified_time_ = GOOGLE_ULONGLONG(0);
    if (_has_bit(6)) {
      if (metadatas_ != NULL) metadatas_->::net::type::Metadatas::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ResponseDatum::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint64 fetch_time = 1 [default = 0];
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &fetch_time_)));
          _set_bit(0);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_retries;
        break;
      }
      
      // optional uint32 retries = 2 [default = 0];
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_retries:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &retries_)));
          _set_bit(1);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_fetch_interval;
        break;
      }
      
      // optional uint32 fetch_interval = 3 [default = 1000];
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_fetch_interval:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &fetch_interval_)));
          _set_bit(2);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(37)) goto parse_score;
        break;
      }
      
      // optional float score = 4 [default = 1];
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_score:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &score_)));
          _set_bit(3);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_signature;
        break;
      }
      
      // optional bytes signature = 5 [default = ""];
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_signature:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_signature()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_modified_time;
        break;
      }
      
      // optional uint64 modified_time = 6 [default = 0];
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_modified_time:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &modified_time_)));
          _set_bit(5);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(58)) goto parse_metadatas;
        break;
      }
      
      // optional .net.type.Metadatas metadatas = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_metadatas:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_metadatas()));
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

void ResponseDatum::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional uint64 fetch_time = 1 [default = 0];
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(1, this->fetch_time(), output);
  }
  
  // optional uint32 retries = 2 [default = 0];
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->retries(), output);
  }
  
  // optional uint32 fetch_interval = 3 [default = 1000];
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->fetch_interval(), output);
  }
  
  // optional float score = 4 [default = 1];
  if (_has_bit(3)) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(4, this->score(), output);
  }
  
  // optional bytes signature = 5 [default = ""];
  if (_has_bit(4)) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      5, this->signature(), output);
  }
  
  // optional uint64 modified_time = 6 [default = 0];
  if (_has_bit(5)) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(6, this->modified_time(), output);
  }
  
  // optional .net.type.Metadatas metadatas = 7;
  if (_has_bit(6)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      7, this->metadatas(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ResponseDatum::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional uint64 fetch_time = 1 [default = 0];
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(1, this->fetch_time(), target);
  }
  
  // optional uint32 retries = 2 [default = 0];
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->retries(), target);
  }
  
  // optional uint32 fetch_interval = 3 [default = 1000];
  if (_has_bit(2)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->fetch_interval(), target);
  }
  
  // optional float score = 4 [default = 1];
  if (_has_bit(3)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(4, this->score(), target);
  }
  
  // optional bytes signature = 5 [default = ""];
  if (_has_bit(4)) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        5, this->signature(), target);
  }
  
  // optional uint64 modified_time = 6 [default = 0];
  if (_has_bit(5)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(6, this->modified_time(), target);
  }
  
  // optional .net.type.Metadatas metadatas = 7;
  if (_has_bit(6)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        7, this->metadatas(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ResponseDatum::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional uint64 fetch_time = 1 [default = 0];
    if (has_fetch_time()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->fetch_time());
    }
    
    // optional uint32 retries = 2 [default = 0];
    if (has_retries()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->retries());
    }
    
    // optional uint32 fetch_interval = 3 [default = 1000];
    if (has_fetch_interval()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->fetch_interval());
    }
    
    // optional float score = 4 [default = 1];
    if (has_score()) {
      total_size += 1 + 4;
    }
    
    // optional bytes signature = 5 [default = ""];
    if (has_signature()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->signature());
    }
    
    // optional uint64 modified_time = 6 [default = 0];
    if (has_modified_time()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->modified_time());
    }
    
    // optional .net.type.Metadatas metadatas = 7;
    if (has_metadatas()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->metadatas());
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

void ResponseDatum::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ResponseDatum* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ResponseDatum*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ResponseDatum::MergeFrom(const ResponseDatum& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_fetch_time(from.fetch_time());
    }
    if (from._has_bit(1)) {
      set_retries(from.retries());
    }
    if (from._has_bit(2)) {
      set_fetch_interval(from.fetch_interval());
    }
    if (from._has_bit(3)) {
      set_score(from.score());
    }
    if (from._has_bit(4)) {
      set_signature(from.signature());
    }
    if (from._has_bit(5)) {
      set_modified_time(from.modified_time());
    }
    if (from._has_bit(6)) {
      mutable_metadatas()->::net::type::Metadatas::MergeFrom(from.metadatas());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ResponseDatum::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ResponseDatum::CopyFrom(const ResponseDatum& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ResponseDatum::IsInitialized() const {
  
  if (has_metadatas()) {
    if (!this->metadatas().IsInitialized()) return false;
  }
  return true;
}

void ResponseDatum::Swap(ResponseDatum* other) {
  if (other != this) {
    std::swap(fetch_time_, other->fetch_time_);
    std::swap(retries_, other->retries_);
    std::swap(fetch_interval_, other->fetch_interval_);
    std::swap(score_, other->score_);
    std::swap(signature_, other->signature_);
    std::swap(modified_time_, other->modified_time_);
    std::swap(metadatas_, other->metadatas_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ResponseDatum::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ResponseDatum_descriptor_;
  metadata.reflection = ResponseDatum_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace type
}  // namespace response

// @@protoc_insertion_point(global_scope)
