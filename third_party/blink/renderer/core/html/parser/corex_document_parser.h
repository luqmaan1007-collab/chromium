#ifndef COREX_DOCUMENT_PARSER_H_
#define COREX_DOCUMENT_PARSER_H_

#include "third_party/blink/renderer/core/dom/document.h"
#include <string>
#include <unordered_set>

namespace blink {

class CorexDocumentParser {
public:
    CorexDocumentParser(Document& doc);
    void LoadSyntaxMap(const std::string& file);
    void Parse(const std::string& code);

private:
    Document& document_;
    std::unordered_set<std::string> KnownCorexSyntax;

    void HandlePrint(const std::string& content);
    void HandleButton(const std::string& label);
    void HandleImage(const std::string& src);
    void HandleUnknown(const std::string& line);
    void ExecuteCorexLine(const std::string& line);
};

} // namespace blink

#endif
