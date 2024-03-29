#include "basic/stl_decl.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "xmlelement.h"
#include "xmlnsstack.h"
#include "xmlconstants.h"

namespace base {

XmlnsStack::XmlnsStack() :
  pxmlnsStack_(new std::vector<std::string>),
  pxmlnsDepthStack_(new std::vector<size_t>) {
}

XmlnsStack::~XmlnsStack() {}

void
XmlnsStack::PushFrame() {
  pxmlnsDepthStack_->push_back(pxmlnsStack_->size());
}

void
XmlnsStack::PopFrame() {
  size_t prev_size = pxmlnsDepthStack_->back();
  pxmlnsDepthStack_->pop_back();
  if (prev_size < pxmlnsStack_->size()) {
    pxmlnsStack_->erase(pxmlnsStack_->begin() + prev_size,
                        pxmlnsStack_->end());
  }
}
const std::pair<std::string, bool> NS_NOT_FOUND(STR_EMPTY, false);
const std::pair<std::string, bool> EMPTY_NS_FOUND(STR_EMPTY, true);
const std::pair<std::string, bool> XMLNS_DEFINITION_FOUND(NS_XMLNS, true);

const std::string *
XmlnsStack::NsForPrefix(const std::string & prefix) {
  if (prefix.length() >= 3 &&
      (prefix[0] == 'x' || prefix[0] == 'X') &&
      (prefix[1] == 'm' || prefix[1] == 'M') &&
      (prefix[2] == 'l' || prefix[2] == 'L')) {
    if (prefix == "xml")
      return &(NS_XML);
    if (prefix == "xmlns")
      return &(NS_XMLNS);
    return NULL;
  }

  std::vector<std::string>::iterator pos;
  for (pos = pxmlnsStack_->end(); pos > pxmlnsStack_->begin(); ) {
    pos -= 2;
    if (*pos == prefix)
      return &(*(pos + 1));
  }

  if (prefix == STR_EMPTY)
    return &(STR_EMPTY); // default namespace

  return NULL; // none found
}

bool
XmlnsStack::PrefixMatchesNs(const std::string & prefix, const std::string & ns) {
  const std::string * match = NsForPrefix(prefix);
  if (match == NULL)
    return false;
  return (*match == ns);
}

std::pair<std::string, bool>
XmlnsStack::PrefixForNs(const std::string & ns, bool isattr) {
  if (ns == NS_XML)
    return std::make_pair(std::string("xml"), true);
  if (ns == NS_XMLNS)
    return std::make_pair(std::string("xmlns"), true);
  if (isattr ? ns == STR_EMPTY : PrefixMatchesNs(STR_EMPTY, ns))
    return std::make_pair(STR_EMPTY, true);

  std::vector<std::string>::iterator pos;
  for (pos = pxmlnsStack_->end(); pos > pxmlnsStack_->begin(); ) {
    pos -= 2;
    if (*(pos + 1) == ns &&
        (!isattr || !pos->empty()) && PrefixMatchesNs(*pos, ns))
      return std::make_pair(*pos, true);
  }

  return std::make_pair(STR_EMPTY, false); // none found
}

std::string
XmlnsStack::FormatQName(const QName & name, bool isAttr) {
  std::string prefix(PrefixForNs(name.Namespace(), isAttr).first);
  if (prefix == STR_EMPTY)
    return name.LocalPart();
  else
    return prefix + ':' + name.LocalPart();
}

void
XmlnsStack::AddXmlns(const std::string & prefix, const std::string & ns) {
  pxmlnsStack_->push_back(prefix);
  pxmlnsStack_->push_back(ns);
}

void
XmlnsStack::RemoveXmlns() {
  pxmlnsStack_->pop_back();
  pxmlnsStack_->pop_back();
}

static bool IsAsciiLetter(char ch) {
  return ((ch >= 'a' && ch <= 'z') ||
          (ch >= 'A' && ch <= 'Z'));
}

static std::string AsciiLower(const std::string & s) {
  std::string result(s);
  size_t i;
  for (i = 0; i < result.length(); i++) {
    if (result[i] >= 'A' && result[i] <= 'Z')
      result[i] += 'a' - 'A';
  }
  return result;
}

static std::string SuggestPrefix(const std::string & ns) {
  size_t len = ns.length();
  size_t i = ns.find_last_of('.');
  if (i != std::string::npos && len - i <= 4 + 1)
    len = i; // chop off ".html" or ".xsd" or ".?{0,4}"
  size_t last = len;
  while (last > 0) {
    last -= 1;
    if (IsAsciiLetter(ns[last])) {
      size_t first = last;
      last += 1;
      while (first > 0) {
        if (!IsAsciiLetter(ns[first - 1]))
          break;
        first -= 1;
      }
      if (last - first > 4)
        last = first + 3;
      std::string candidate(AsciiLower(ns.substr(first, last - first)));
      if (candidate.find("xml") != 0)
        return candidate;
      break;
    }
  }
  return "ns";
}


std::pair<std::string, bool>
XmlnsStack::AddNewPrefix(const std::string & ns, bool isAttr) {
  if (PrefixForNs(ns, isAttr).second)
    return std::make_pair(STR_EMPTY, false);

  std::string base(SuggestPrefix(ns));
  std::string result(base);
  int i = 2;
  while (NsForPrefix(result) != NULL) {
    std::stringstream ss;
    ss << base;
    ss << (i++);
    ss >> result;
  }
  AddXmlns(result, ns);
  return std::make_pair(result, true);
}

void XmlnsStack::Reset() {
  pxmlnsStack_->clear();
  pxmlnsDepthStack_->clear();
}

}
