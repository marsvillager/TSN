#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <tinyxml2/tinyxml2.h>
#include <iostream>
#include <sstream>

using namespace faker_tsn;
using namespace std;
using namespace tinyxml2;

static void TestTinyXML2_Read() {
    XMLDocument doc;
    doc.LoadFile("/home/reptile/下载/TSN/config/tinyxml2_test.xml");
    XMLElement* root = doc.RootElement();
    INFO(root->Attribute("name"));
    XMLElement* surface = root->FirstChildElement("surface");
    while (surface) {
        const XMLAttribute* surfaceAttr = surface->FirstAttribute();
        while (surfaceAttr) {
            stringstream ss;
            ss << "Name: " << surfaceAttr->Name();
            ss << " | ";
            ss << "Value: " << surfaceAttr->Value();
            INFO(ss.str());
            surfaceAttr = surfaceAttr->Next();
        }
        XMLElement* surfaceChlid = surface->FirstChildElement();
        while (surfaceChlid) {
            stringstream ss;
            ss << "Name: " << surfaceChlid->Name();
            ss << " | ";
            ss << "Value: " << surfaceChlid->GetText();
            INFO(ss.str());
            surfaceChlid = surfaceChlid->NextSiblingElement();
        }
        surface = surface->NextSiblingElement("surface");
    }
}

TEST(TEST_TINYXML2, TEST_TINYXML2_READ) {
    TestTinyXML2_Read();
}