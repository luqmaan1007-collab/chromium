#include "corex_document_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace blink {

CorexDocumentParser::CorexDocumentParser(Document& doc) : document_(doc) {}

void CorexDocumentParser::LoadSyntaxMap(const std::string& file) {
    std::ifstream in(file);
    std::string line;
    while (std::getline(in, line)) {
        KnownCorexSyntax.insert(line);
    }
    std::cout << "Loaded " << KnownCorexSyntax.size() << " Corex keywords/functions.\n";
}

void CorexDocumentParser::Parse(const std::string& code) {
    std::istringstream stream(code);
    std::string line;

    while (std::getline(stream, line)) {
        // Trim whitespace (simple version)
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        ExecuteCorexLine(line);
    }
}

void CorexDocumentParser::ExecuteCorexLine(const std::string& line) {
    // Check if line matches known syntax
    for (const auto& keyword : KnownCorexSyntax) {
        if (line.find(keyword) == 0) {
            // You can expand this to full execution later
            if (keyword == "print") HandlePrint(line);
            else if (keyword == "button") HandleButton(line);
            else if (keyword == "image") HandleImage(line);
            else {
                // For unknown-but-registered functions, just show placeholder
                HandleUnknown(line);
            }
            return;
        }
    }
    // Unknown syntax (not in syntax map)
    HandleUnknown(line);
}

// Simple implementations
void CorexDocumentParser::HandlePrint(const std::string& content) {
    auto start = content.find('"') + 1;
    auto end = content.rfind('"');
    if (start != std::string::npos && end > start) {
        std::string text = content.substr(start, end - start);
        auto* p = document_.CreateElement("p");
        auto* tn = document_.CreateTextNode(text.c_str(), text.size());
        p->AppendChild(tn);
        document_.AppendChild(p);
    }
}

void CorexDocumentParser::HandleButton(const std::string& label) {
    auto start = label.find('"') + 1;
    auto end = label.rfind('"');
    std::string text = (start != std::string::npos && end > start) ? label.substr(start, end - start) : "Button";
    auto* btn = document_.CreateElement("button");
    auto* tn = document_.CreateTextNode(text.c_str(), text.size());
    btn->AppendChild(tn);
    document_.AppendChild(btn);
}

void CorexDocumentParser::HandleImage(const std::string& src) {
    auto start = src.find('"') + 1;
    auto end = src.rfind('"');
    std::string path = (start != std::string::npos && end > start) ? src.substr(start, end - start) : "";
    auto* img = document_.CreateElement("img");
    img->SetAttribute("src", path.c_str());
    document_.AppendChild(img);
}

void CorexDocumentParser::HandleUnknown(const std::string& line) {
    auto* div = document_.CreateElement("div");
    div->SetAttribute("class", "corex-unknown");
    auto* tn = document_.CreateTextNode(line.c_str(), line.size());
    div->AppendChild(tn);
    document_.AppendChild(div);
}

} // namespace blink
