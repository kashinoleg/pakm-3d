#include "CppUnitTest.h"

#include <iostream>
#include <sstream>
#include "tinyxml.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	void NullLineEndings(char* p)
	{
		while (p && *p) {
			if (*p == '\n' || *p == '\r') {
				*p = 0;
				return;
			}
			++p;
		}
	}

	TEST_CLASS(TinyXMLIntegrationTest)
	{
	public:
		TEST_METHOD(StringTest)
		{
			double value = 1.223232323;
			
			//char buf[256];
			//sprintf(buf, "%g", value);

			char buffer[256];
			snprintf(buffer, sizeof(buffer), "%g", value);
			string str = buffer;

			string var = to_string(value);
			Assert::AreEqual(true, str == "1.22323");
			Assert::AreEqual(true, var == "1.223232");
		}
		

		TEST_METHOD(XmlSaveTest)
		{
			const char* demoStart =
				"<?xml version=\"1.0\"  standalone='no' >\n"
				"<!-- Our to do list data -->"
				"<ToDo>\n"
				"<!-- Do I need a secure PDA? -->\n"
				"<Item priority=\"1\" distance='close'> Go to the <bold>Toy store!</bold></Item>"
				"<Item priority=\"2\" distance='none'> Do bills   </Item>"
				"<Item priority=\"2\" distance='far &amp; back'> Look for Evil Dinosaurs! </Item>"
				"</ToDo>";
			TiXmlDocument doc("demotest.xml");
			doc.Parse(demoStart);
			Assert::AreEqual(doc.Error(), false);
			bool save = doc.SaveFile();
			Assert::AreEqual(save, true);
		}

		TEST_METHOD(XmlLoadTest)
		{
			//	What the todo list should look like after processing.
			// In stream (no formatting) representation.
			const char* demoEnd =
				"<?xml version=\"1.0\" standalone=\"no\" ?>"
				"<!-- Our to do list data -->"
				"<ToDo>"
				"<!-- Do I need a secure PDA? -->"
				"<Item priority=\"2\" distance=\"close\">Go to the"
				"<bold>Toy store!"
				"</bold>"
				"</Item>"
				"<Item priority=\"1\" distance=\"far\">Talk to:"
				"<Meeting where=\"School\">"
				"<Attendee name=\"Marple\" position=\"teacher\" />"
				"<Attendee name=\"Voel\" position=\"counselor\" />"
				"</Meeting>"
				"<Meeting where=\"Lunch\" />"
				"</Item>"
				"<Item priority=\"2\" distance=\"here\">Do bills"
				"</Item>"
				"</ToDo>";



			TiXmlDocument doc("demotest.xml");
			bool loadOkay = doc.LoadFile();

			if (!loadOkay)
			{
				printf("Could not load test file 'demotest.xml'. Error='%s'. Exiting.\n", doc.ErrorDesc());
				exit(1);
			}

			printf("** Demo doc read from disk: ** \n\n");
			printf("** Printing via doc.Print **\n");
			doc.Print(stdout);

			{
				printf("** Printing via TiXmlPrinter **\n");
				TiXmlPrinter printer;
				doc.Accept(&printer);
				fprintf(stdout, "%s", printer.CStr());
			}
			{
				printf("** Printing via operator<< **\n");
				std::cout << doc;
			}
			TiXmlNode* node = 0;
			TiXmlElement* todoElement = 0;
			TiXmlElement* itemElement = 0;


			// --------------------------------------------------------
			// An example of changing existing attributes, and removing
			// an element from the document.
			// --------------------------------------------------------

			// Get the "ToDo" element.
			// It is a child of the document, and can be selected by name.
			node = doc.FirstChild("ToDo");
			assert(node);
			todoElement = node->ToElement();
			assert(todoElement);

			// Going to the toy store is now our second priority...
			// So set the "priority" attribute of the first item in the list.
			node = todoElement->FirstChildElement();	// This skips the "PDA" comment.
			assert(node);
			itemElement = node->ToElement();
			assert(itemElement);
			itemElement->SetAttribute("priority", 2);

			// Change the distance to "doing bills" from
			// "none" to "here". It's the next sibling element.
			itemElement = itemElement->NextSiblingElement();
			assert(itemElement);
			itemElement->SetAttribute("distance", "here");

			// Remove the "Look for Evil Dinosaurs!" item.
			// It is 1 more sibling away. We ask the parent to remove
			// a particular child.
			itemElement = itemElement->NextSiblingElement();
			todoElement->RemoveChild(itemElement);

			itemElement = 0;

			// --------------------------------------------------------
			// What follows is an example of created elements and text
			// nodes and adding them to the document.
			// --------------------------------------------------------

			// Add some meetings.
			TiXmlElement item("Item");
			item.SetAttribute("priority", "1");
			item.SetAttribute("distance", "far");

			TiXmlText text("Talk to:");

			TiXmlElement meeting1("Meeting");
			meeting1.SetAttribute("where", "School");

			TiXmlElement meeting2("Meeting");
			meeting2.SetAttribute("where", "Lunch");

			TiXmlElement attendee1("Attendee");
			attendee1.SetAttribute("name", "Marple");
			attendee1.SetAttribute("position", "teacher");

			TiXmlElement attendee2("Attendee");
			attendee2.SetAttribute("name", "Voel");
			attendee2.SetAttribute("position", "counselor");

			// Assemble the nodes we've created:
			meeting1.InsertEndChild(attendee1);
			meeting1.InsertEndChild(attendee2);

			item.InsertEndChild(text);
			item.InsertEndChild(meeting1);
			item.InsertEndChild(meeting2);

			// And add the node to the existing list after the first child.
			node = todoElement->FirstChild("Item");
			assert(node);
			itemElement = node->ToElement();
			assert(itemElement);

			todoElement->InsertAfterChild(itemElement, item);

			printf("\n** Demo doc processed: ** \n\n");
			doc.Print(stdout);


			printf("** Demo doc processed to stream: ** \n\n");
			cout << doc << endl << endl;

			// --------------------------------------------------------
			// Different tests...do we have what we expect?
			// --------------------------------------------------------

			int count = 0;
			TiXmlElement*	element;

			//////////////////////////////////////////////////////
			ostringstream outputStream(ostringstream::out);
			outputStream << doc;
			Assert::AreEqual(string(demoEnd).c_str(), outputStream.str().c_str());

			node = doc.RootElement();
			assert(node);
			Assert::AreEqual(true, (node != 0 && node->ToElement()));
			Assert::AreEqual("ToDo", node->Value());

			node = node->FirstChild();
			Assert::AreEqual(true, (node != 0 && node->ToComment()));
			node = node->NextSibling();
			Assert::AreEqual(true, (node != 0 && node->ToElement()));
			Assert::AreEqual("Item", node->Value());

			node = node->FirstChild();
			Assert::AreEqual(true, (node != 0 && node->ToText()));
			Assert::AreEqual("Go to the", node->Value());


			//////////////////////////////////////////////////////

			// Walk all the top level nodes of the document.
			count = 0;
			for (node = doc.FirstChild();
				node;
				node = node->NextSibling())
			{
				count++;
			}
			Assert::AreEqual(3, count);

			count = 0;
			for (node = doc.LastChild();
				node;
				node = node->PreviousSibling())
			{
				count++;
			}
			Assert::AreEqual(3, count);

			// Walk all the top level nodes of the document,
			// using a different syntax.
			count = 0;
			for (node = doc.IterateChildren(0);
				node;
				node = doc.IterateChildren(node))
			{
				count++;
			}
			Assert::AreEqual(3, count);

			// Walk all the elements in a node.
			count = 0;
			for (element = todoElement->FirstChildElement();
				element;
				element = element->NextSiblingElement())
			{
				count++;
			}
			Assert::AreEqual(3, count);

			// Walk all the elements in a node by value.
			count = 0;
			for (node = todoElement->FirstChild("Item");
				node;
				node = node->NextSibling("Item"))
			{
				count++;
			}
			Assert::AreEqual(3, count);

			count = 0;
			for (node = todoElement->LastChild("Item");
				node;
				node = node->PreviousSibling("Item"))
			{
				count++;
			}
			Assert::AreEqual(3, count);

			{
				cout << "\n** Parsing. **\n";
				istringstream parse0("<Element0 attribute0='foo0' attribute1= noquotes attribute2 = '&gt;' />");
				TiXmlElement element0("default");
				parse0 >> element0;

				Assert::AreEqual("Element0", element0.Value());
				Assert::AreEqual("foo0", element0.Attribute("attribute0"));
				Assert::AreEqual("noquotes", element0.Attribute("attribute1"));
				Assert::AreEqual(">", element0.Attribute("attribute2"));
			}

			{
				const char* error = "<?xml version=\"1.0\" standalone=\"no\" ?>\n"
					"<passages count=\"006\" formatversion=\"20020620\">\n"
					"    <wrong error>\n"
					"</passages>";

				TiXmlDocument docTest;
				docTest.Parse(error);
				Assert::AreEqual(docTest.ErrorRow(), 3);
				Assert::AreEqual(docTest.ErrorCol(), 17);
			}

			{
				//////////////////////////////////////////////////////
				cout << "\n** Streaming. **\n";

				// Round trip check: stream in, then stream back out to verify. The stream
				// out has already been checked, above. We use the output

				istringstream inputStringStream(outputStream.str());
				TiXmlDocument document0;

				inputStringStream >> document0;

				ostringstream outputStream0(ostringstream::out);
				outputStream0 << document0;
				Assert::AreEqual(string(demoEnd).c_str(), outputStream0.str().c_str());

				std::string str;
				str << document0;
				Assert::AreEqual(string(demoEnd).c_str(), str.c_str());
			}
		}

		TEST_METHOD(XmlQueryTest)
		{
			const char* str = "<doc attr0='1' attr1='2.0' attr2='foo' />";

			TiXmlDocument doc;
			doc.Parse(str);

			TiXmlElement* ele = doc.FirstChildElement();

			int iVal;
			TiXmlQueryType result;
			double dVal;

			result = ele->QueryDoubleAttribute("attr0", &dVal);
			Assert::AreEqual(result == TIXML_SUCCESS, true);
			Assert::AreEqual((int)dVal, 1);
			result = ele->QueryDoubleAttribute("attr1", &dVal);
			Assert::AreEqual((int)dVal, 2);
			result = ele->QueryIntAttribute("attr1", &iVal);
			Assert::AreEqual(result == TIXML_SUCCESS, true);
			Assert::AreEqual(iVal, 2);
			result = ele->QueryIntAttribute("attr2", &iVal);
			Assert::AreEqual(result == TIXML_WRONG_TYPE, true);
			result = ele->QueryIntAttribute("bar", &iVal);
			Assert::AreEqual(result == TIXML_NO_ATTRIBUTE, true);
		}

		TEST_METHOD(XmlAttributeTest) {
			const char* str = "<doc/>";

			TiXmlDocument doc;
			doc.Parse(str);

			TiXmlElement* ele = doc.FirstChildElement();

			int iVal;
			double dVal;

			ele->SetAttribute("str", "strValue");
			ele->SetAttribute("int", 1);
			ele->SetDoubleAttribute("double", -1.0);

			const char* cStr = ele->Attribute("str");
			ele->QueryIntAttribute("int", &iVal);
			ele->QueryDoubleAttribute("double", &dVal);

			Assert::AreEqual("strValue", cStr);
			Assert::AreEqual(1, iVal);
			Assert::AreEqual(-1.0, dVal);
		}

		TEST_METHOD(XmlLocationTest)
		{
			const char* str = "\t<?xml version=\"1.0\" standalone=\"no\" ?>\t<room doors='2'>\n"
				"</room>";

			TiXmlDocument doc;
			doc.SetTabSize(8);
			doc.Parse(str);

			TiXmlHandle docHandle(&doc);
			TiXmlHandle roomHandle = docHandle.FirstChildElement("room");

			assert(docHandle.Node());
			assert(roomHandle.Element());

			TiXmlElement* room = roomHandle.Element();
			assert(room);
			TiXmlAttribute* doors = room->FirstAttribute();
			assert(doors);

			Assert::AreEqual(room->Row(), 1);
			Assert::AreEqual(room->Column(), 49);
			Assert::AreEqual(doors->Row(), 1);
			Assert::AreEqual(doors->Column(), 55);
		}

		TEST_METHOD(XmlLocationLongTest)
		{
			const char* str = "\t<?xml version=\"1.0\" standalone=\"no\" ?>\t<room doors='2'>\n"
				"  <!-- Silly example -->\n"
				"    <door wall='north'>A great door!</door>\n"
				"\t<door wall='east'/>"
				"</room>";

			TiXmlDocument doc;
			doc.Parse(str);

			TiXmlHandle docHandle(&doc);
			TiXmlHandle roomHandle = docHandle.FirstChildElement("room");
			TiXmlHandle commentHandle = docHandle.FirstChildElement("room").FirstChild();
			TiXmlHandle textHandle = docHandle.FirstChildElement("room").ChildElement("door", 0).FirstChild();
			TiXmlHandle door0Handle = docHandle.FirstChildElement("room").ChildElement(0);
			TiXmlHandle door1Handle = docHandle.FirstChildElement("room").ChildElement(1);

			assert(docHandle.Node());
			assert(roomHandle.Element());
			assert(commentHandle.Node());
			assert(textHandle.Text());
			assert(door0Handle.Element());
			assert(door1Handle.Element());

			TiXmlDeclaration* declaration = doc.FirstChild()->ToDeclaration();
			assert(declaration);
			TiXmlElement* room = roomHandle.Element();
			assert(room);
			TiXmlAttribute* doors = room->FirstAttribute();
			assert(doors);
			TiXmlText* text = textHandle.Text();
			TiXmlComment* comment = commentHandle.Node()->ToComment();
			assert(comment);
			TiXmlElement* door0 = door0Handle.Element();
			TiXmlElement* door1 = door1Handle.Element();

			Assert::AreEqual(declaration->Row(), 1);
			Assert::AreEqual(declaration->Column(), 5);
			Assert::AreEqual(room->Row(), 1);
			Assert::AreEqual(room->Column(), 45);
			Assert::AreEqual(doors->Row(), 1);
			Assert::AreEqual(doors->Column(), 51);
			Assert::AreEqual(comment->Row(), 2);
			Assert::AreEqual(comment->Column(), 3);
			Assert::AreEqual(text->Row(), 3);
			Assert::AreEqual(text->Column(), 24);
			Assert::AreEqual(door0->Row(), 3);
			Assert::AreEqual(door0->Column(), 5);
			Assert::AreEqual(door1->Row(), 4);
			Assert::AreEqual(door1->Column(), 5);
		}

		TEST_METHOD(XmlUTF8Test)
		{
			// --------------------------------------------------------
			// UTF-8 testing. It is important to test:
			//	1. Making sure name, value, and text read correctly
			//	2. Row, Col functionality
			//	3. Correct output
			// --------------------------------------------------------
			{
				TiXmlDocument doc("utf8test.xml");
				doc.LoadFile();
				if (doc.Error() && doc.ErrorId() == TiXmlError::TIXML_ERROR_OPENING_FILE) {
					printf("WARNING: File 'utf8test.xml' not found.\n"
						"(Are you running the test from the wrong directory?)\n"
						"Could not test UTF-8 functionality.\n");
				}
				else
				{
					TiXmlHandle docH(&doc);
					// Get the attribute "value" from the "Russian" element and check it.
					TiXmlElement* element = docH.FirstChildElement("document").FirstChildElement("Russian").Element();
					const unsigned char correctValue[] = { 0xd1U, 0x86U, 0xd0U, 0xb5U, 0xd0U, 0xbdU, 0xd0U, 0xbdU,
						0xd0U, 0xbeU, 0xd1U, 0x81U, 0xd1U, 0x82U, 0xd1U, 0x8cU, 0 };

					Assert::AreEqual((const char*)correctValue, element->Attribute("value"));
					Assert::AreEqual(4, element->Row());
					Assert::AreEqual(5, element->Column());

					const unsigned char russianElementName[] = { 0xd0U, 0xa0U, 0xd1U, 0x83U,
						0xd1U, 0x81U, 0xd1U, 0x81U,
						0xd0U, 0xbaU, 0xd0U, 0xb8U,
						0xd0U, 0xb9U, 0 };
					const char russianText[] = "<\xD0\xB8\xD0\xBC\xD0\xB5\xD0\xB5\xD1\x82>";

					TiXmlText* text = docH.FirstChildElement("document").FirstChildElement((const char*)russianElementName).Child(0).Text();
					Assert::AreEqual(russianText, text->Value());
					Assert::AreEqual(7, text->Row());
					Assert::AreEqual(47, text->Column());

					TiXmlDeclaration* dec = docH.Child(0).Node()->ToDeclaration();
					Assert::AreEqual(1, dec->Column());
					Assert::AreEqual(1, doc.Column());

					// Now try for a round trip.
					doc.SaveFile("utf8testout.xml");

					// Check the round trip.
					char savedBuf[256];
					char verifyBuf[256];
					int okay = 1;

					FILE* saved = fopen("utf8testout.xml", "r");
					FILE* verify = fopen("utf8testverify.xml", "r");

					//bool firstLineBOM=true;
					if (saved && verify)
					{
						while (fgets(verifyBuf, 256, verify))
						{
							fgets(savedBuf, 256, saved);
							NullLineEndings(verifyBuf);
							NullLineEndings(savedBuf);

							if ( /*!firstLineBOM && */ strcmp(verifyBuf, savedBuf))
							{
								printf("verify:%s<\n", verifyBuf);
								printf("saved :%s<\n", savedBuf);
								okay = 0;
								break;
							}
							//firstLineBOM = false;
						}
					}
					if (saved)
						fclose(saved);
					if (verify)
						fclose(verify);
					Assert::AreEqual(1, okay);

					// On most Western machines, this is an element that contains
					// the word "resume" with the correct accents, in a latin encoding.
					// It will be something else completely on non-wester machines,
					// which is why TinyXml is switching to UTF-8.
					const char latin[] = "<element>r\x82sum\x82</element>";

					TiXmlDocument latinDoc;
					latinDoc.Parse(latin, 0, TIXML_ENCODING_LEGACY);

					text = latinDoc.FirstChildElement()->FirstChild()->ToText();
					Assert::AreEqual("r\x82sum\x82", text->Value());
				}
			}
		}

		TEST_METHOD(XmlCondenseWhiteSpaceTest)
		{
			TiXmlBase::SetCondenseWhiteSpace(false);
			{
				istringstream parse1("<start>This  is    \ntext</start>");
				TiXmlElement text1("text");
				parse1 >> text1;
				Assert::AreEqual(text1.FirstChild()->Value(), "This  is    \ntext");
			}
			TiXmlBase::SetCondenseWhiteSpace(true);
		}

		TEST_METHOD(XmlGetTextTest)
		{
			const char* str = "<foo>This is text</foo>";
			TiXmlDocument doc;
			doc.Parse(str);
			const TiXmlElement* element = doc.RootElement();
			Assert::AreEqual(element->GetText(), "This is text");

			str = "<foo><b>This is text</b></foo>";
			doc.Clear();
			doc.Parse(str);
			element = doc.RootElement();
			Assert::AreEqual(element->GetText() == 0, true);

			str = "<foo>This is <b>text</b></foo>";
			doc.Clear();
			TiXmlBase::SetCondenseWhiteSpace(false);
			doc.Parse(str);
			TiXmlBase::SetCondenseWhiteSpace(true);
			element = doc.RootElement();
			Assert::AreEqual(element->GetText(), "This is ");
		}
		
		TEST_METHOD(XmlCDATATest1482728)
		{
			// [ 1482728 ] Wrong wide char parsing
			char buf[256];
			buf[255] = 0;
			for (int i = 0; i<255; ++i) {
				buf[i] = (char)((i >= 32) ? i : 32);
			}
			std::string str("<xmlElement><![CDATA[");
			str += buf;
			str += "]]></xmlElement>";

			TiXmlDocument doc;
			doc.Parse(str.c_str());

			TiXmlPrinter printer;
			printer.SetStreamPrinting();
			doc.Accept(&printer);
			Assert::AreEqual(str.c_str(), printer.CStr());

			doc.Clear();
			istringstream iss(printer.Str());
			iss >> doc;
			std::string out;
			out << doc;
			Assert::AreEqual(out.c_str(), printer.CStr());
		}

		TEST_METHOD(XmlCDATATest1480107)
		{
			// [ 1480107 ] Bug-fix for STL-streaming of CDATA that contains tags
			// CDATA streaming had a couple of bugs, that this tests for.
			const char* str = "<xmlElement>"
				"<![CDATA["
				"<b>I am > the rules!</b>\n"
				"...since I make symbolic puns"
				"]]>"
				"</xmlElement>";
			TiXmlDocument doc;
			doc.Parse(str);
			Assert::AreEqual(doc.FirstChildElement()->FirstChild()->Value(), "<b>I am > the rules!</b>\n...since I make symbolic puns");

			doc.Clear();

			istringstream parse0(str);
			parse0 >> doc;
			Assert::AreEqual(doc.FirstChildElement()->FirstChild()->Value(), "<b>I am > the rules!</b>\n...since I make symbolic puns");

			TiXmlDocument doc1 = doc;
			Assert::AreEqual(doc1.FirstChildElement()->FirstChild()->Value(), "<b>I am > the rules!</b>\n...since I make symbolic puns");
		}

		TEST_METHOD(XmlFuzzingTest)
		{
			//////////////////////////////////////////////////////
			printf("\n** Fuzzing... **\n");

			const int FUZZ_ITERATION = 300;
			const char* demoStart =
				"<?xml version=\"1.0\"  standalone='no' >\n"
				"<!-- Our to do list data -->"
				"<ToDo>\n"
				"<!-- Do I need a secure PDA? -->\n"
				"<Item priority=\"1\" distance='close'> Go to the <bold>Toy store!</bold></Item>"
				"<Item priority=\"2\" distance='none'> Do bills   </Item>"
				"<Item priority=\"2\" distance='far &amp; back'> Look for Evil Dinosaurs! </Item>"
				"</ToDo>";
			// The only goal is not to crash on bad input.
			int len = (int)strlen(demoStart);
			for (int i = 0; i<FUZZ_ITERATION; ++i)
			{
				char* demoCopy = new char[len + 1];
				strcpy(demoCopy, demoStart);

				demoCopy[i%len] = (char)((i + 1) * 3);
				demoCopy[(i * 7) % len] = '>';
				demoCopy[(i * 11) % len] = '<';

				TiXmlDocument xml;
				xml.Parse(demoCopy);

				delete[] demoCopy;
			}
			printf("** Fuzzing Complete. **\n");
		}//*/

		TEST_METHOD(XmlInsertBeforeChildTest)
		{
			TiXmlElement parent("Parent");
			TiXmlElement childText0("childText0");
			TiXmlElement childText1("childText1");
			TiXmlNode* childNode0 = parent.InsertEndChild(childText0);
			TiXmlNode* childNode1 = parent.InsertBeforeChild(childNode0, childText1);
			Assert::AreEqual(childNode1 == parent.FirstChild(), true);
		}

		TEST_METHOD(XmlInsertAfterChildTest)
		{
			// InsertBeforeChild and InsertAfterChild causes crash.
			TiXmlElement parent("Parent");
			TiXmlElement childText0("childText0");
			TiXmlElement childText1("childText1");
			TiXmlNode* childNode0 = parent.InsertEndChild(childText0);
			TiXmlNode* childNode1 = parent.InsertAfterChild(childNode0, childText1);
			Assert::AreEqual(childNode1 == parent.LastChild(), true);
		}

		TEST_METHOD(XmlTextSTLTest)
		{
			// Missing constructor implementation. No test -- just compiles.
			TiXmlText text("Missing");

			// Missing implementation:
			TiXmlDocument doc;
			string name = "missing";
			doc.LoadFile(name);

			TiXmlText textSTL(name);
		}

		/*TEST_METHOD(XmlLongFilenamesTest)
		{
			// Long filenames crashing STL version
			TiXmlDocument doc("midsummerNightsDreamWithAVeryLongFilenameToConfuseTheStringHandlingRoutines.xml");
			bool loadOkay = doc.LoadFile();
			//loadOkay = true;	// get rid of compiler warning.
								// Won't pass on non-dev systems. Just a "no crash" check.
								//XmlTest( "Long filename. ", true, loadOkay );
			Assert::AreEqual(loadOkay, true);
		}//*/

		TEST_METHOD(XmlWrittenTest)
		{
			// Entities not being written correctly.
			// From Lynn Allen

			const char* passages =
				"<?xml version=\"1.0\" standalone=\"no\" ?>"
				"<passages count=\"006\" formatversion=\"20020620\">"
				"<psg context=\"Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;."
				" It also has &lt;, &gt;, and &amp;, as well as a fake copyright &#xA9;.\"> </psg>"
				"</passages>";

			TiXmlDocument doc("passages.xml");
			doc.Parse(passages);
			TiXmlElement* psg = doc.RootElement()->FirstChildElement();
			const char* context = psg->Attribute("context");
			const char* expected = "Line 5 has \"quotation marks\" and 'apostrophe marks'. It also has <, >, and &, as well as a fake copyright \xC2\xA9.";

			//Entity transformation: read.
			Assert::AreEqual(expected, context);

			FILE* textfile = fopen("textfile.txt", "w");
			if (textfile)
			{
				psg->Print(textfile, 0);
				fclose(textfile);
			}
			textfile = fopen("textfile.txt", "r");
			assert(textfile);
			if (textfile)
			{
				char buf[1024];
				fgets(buf, 1024, textfile);
				Assert::AreEqual(buf,
					"<psg context=\'Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;."
					" It also has &lt;, &gt;, and &amp;, as well as a fake copyright \xC2\xA9.' />");
				
			}
			fclose(textfile);
		}
		
		TEST_METHOD(XmlPutTest)
		{
			FILE* textfile = fopen("test5.xml", "w");
			if (textfile)
			{
				fputs("<?xml version='1.0'?><a.elem xmi.version='2.0'/>", textfile);
				fclose(textfile);

				TiXmlDocument doc;
				doc.LoadFile("test5.xml");
				Assert::AreEqual(doc.Error(), false);
			}
		}

		TEST_METHOD(XmlPutTextFileTest)
		{
			FILE* textfile = fopen("test6.xml", "w");
			if (textfile)
			{
				fputs("<element><Name>1.1 Start easy ignore fin thickness&#xA;</Name></element>", textfile);
				fclose(textfile);

				TiXmlDocument doc;
				bool result = doc.LoadFile("test6.xml");
				Assert::AreEqual(result, true);

				TiXmlText* text = doc.FirstChildElement()->FirstChildElement()->FirstChild()->ToText();
				Assert::AreEqual(text->Value(), "1.1 Start easy ignore fin thickness\n");
			}
		}

		TEST_METHOD(XmlDOCTYPETest)
		{
			// DOCTYPE not preserved (950171)
			// 
			const char* doctype =
				"<?xml version=\"1.0\" ?>"
				"<!DOCTYPE PLAY SYSTEM 'play.dtd'>"
				"<!ELEMENT title (#PCDATA)>"
				"<!ELEMENT books (title,authors)>"
				"<element />";

			TiXmlDocument doc;
			doc.Parse(doctype);
			doc.SaveFile("test7.xml");
			doc.Clear();
			doc.LoadFile("test7.xml");

			TiXmlHandle docH(&doc);
			TiXmlUnknown* unknown = docH.Child(1).Unknown();
			Assert::AreEqual(unknown->Value(), "!DOCTYPE PLAY SYSTEM 'play.dtd'");

			TiXmlNode* node = docH.Child(2).Node();
			std::string str;
			str << (*node);
			Assert::AreEqual(str.c_str(), "<!ELEMENT title (#PCDATA)>");
		}

		TEST_METHOD(XmlCommentFormattingTest)
		{
			// [ 791411 ] Formatting bug
			// Comments do not stream out correctly.
			const char* doctype = "<!-- Somewhat<evil> -->";
			TiXmlDocument doc;
			doc.Parse(doctype);

			TiXmlHandle docH(&doc);
			TiXmlComment* comment = docH.Child(0).Node()->ToComment();

			Assert::AreEqual(comment->Value(), " Somewhat<evil> ");
			std::string str;
			str << (*comment);
			Assert::AreEqual(str.c_str(), "<!-- Somewhat<evil> -->");
		}

		TEST_METHOD(XmlWhiteSpaceTest)
		{
			// [ 870502 ] White space issues
			TiXmlDocument doc;
			TiXmlText* text;
			TiXmlHandle docH(&doc);

			const char* doctype0 = "<element> This has leading and trailing space </element>";
			const char* doctype1 = "<element>This has  internal space</element>";
			const char* doctype2 = "<element> This has leading, trailing, and  internal space </element>";

			TiXmlBase::SetCondenseWhiteSpace(false);
			doc.Clear();
			doc.Parse(doctype0);
			text = docH.FirstChildElement("element").Child(0).Text();
			Assert::AreEqual(text->Value(), " This has leading and trailing space ");

			doc.Clear();
			doc.Parse(doctype1);
			text = docH.FirstChildElement("element").Child(0).Text();
			Assert::AreEqual(text->Value(), "This has  internal space");

			doc.Clear();
			doc.Parse(doctype2);
			text = docH.FirstChildElement("element").Child(0).Text();
			Assert::AreEqual(text->Value(), " This has leading, trailing, and  internal space ");

			TiXmlBase::SetCondenseWhiteSpace(true);
			doc.Clear();
			doc.Parse(doctype0);
			text = docH.FirstChildElement("element").Child(0).Text();
			Assert::AreEqual(text->Value(), "This has leading and trailing space");

			doc.Clear();
			doc.Parse(doctype1);
			text = docH.FirstChildElement("element").Child(0).Text();
			Assert::AreEqual(text->Value(), "This has internal space");

			doc.Clear();
			doc.Parse(doctype2);
			text = docH.FirstChildElement("element").Child(0).Text();
			Assert::AreEqual(text->Value(), "This has leading, trailing, and internal space");
		}

		/*TEST_METHOD(XmlDoubleAttributesTest)
		{
			const char* doctype = "<element attr='red' attr='blue' />";
			TiXmlDocument doc;
			doc.Parse(doctype);
			Assert::AreEqual(doc.Error(), true);
		}//*/

		TEST_METHOD(XmlNullStreamTest)
		{
			// Embedded null in stream.
			const char* doctype = "<element att\0r='red' attr='blue' />";

			TiXmlDocument doc;
			doc.Parse(doctype);
			//Embedded null throws error.
			Assert::AreEqual(doc.Error(), true);

			istringstream strm(doctype);
			doc.Clear();
			doc.ClearError();
			strm >> doc;
			//Embedded null throws error.
			Assert::AreEqual(doc.Error(), true);
		}
		
		TEST_METHOD(XmlWesternSystemTest)
		{
			// Legacy mode test. (This test may only pass on a western system)
			const char* str =
				"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
				"<д>"
				"CцntдntЯдцьДЦЬ"
				"</д>";

			TiXmlDocument doc;
			doc.Parse(str);

			TiXmlHandle docHandle(&doc);
			TiXmlHandle aHandle = docHandle.FirstChildElement("д");
			TiXmlHandle tHandle = aHandle.Child(0);
			assert(aHandle.Element());
			assert(tHandle.Text());
			//ISO-8859-1 Parsing.
			Assert::AreEqual(tHandle.Text()->Value(), "CцntдntЯдцьДЦЬ");
		}

		/*TEST_METHOD(XmlEmptyDocumentsTest)
		{
			// Empty documents should return TIXML_ERROR_PARSING_EMPTY, bug 1070717
			const char* str = "    ";
			TiXmlDocument doc;
			doc.Parse(str);
			//Empty document error TIXML_ERROR_DOCUMENT_EMPTY
			int id = TiXmlBase::TIXML_ERROR_DOCUMENT_EMPTY;
			Assert::AreEqual(doc.ErrorId(), id);
		}//*/

		TEST_METHOD(XmlBugTest1195696)
		{
			// Bug [ 1195696 ] from marlonism
			TiXmlBase::SetCondenseWhiteSpace(false);
			TiXmlDocument xml;
			xml.Parse("<text><break/>This hangs</text>");
			//Test safe error return
			Assert::AreEqual(xml.Error(), false);
		}

		/*TEST_METHOD(XmlBugTest1243992)
		{
			// Bug [ 1243992 ] - another infinite loop
			TiXmlDocument doc;
			doc.SetCondenseWhiteSpace(false);
			doc.Parse("<p><pb></pb>test</p>");
		}//*/

		TEST_METHOD(XmlLowEntitiesTest)
		{
			TiXmlDocument xml;
			const char result[] = { 0x0e, 0 };
			xml.Parse("<test>&#x0e;</test>");
			Assert::AreEqual(xml.FirstChildElement()->GetText(), result);
		}

		TEST_METHOD(XmlBugTest1451649)
		{
			// Bug [ 1451649 ] Attribute values with trailing quotes not handled correctly
			TiXmlDocument xml;
			xml.Parse("<foo attribute=bar\" />");
			//Throw error with bad end quotes.
			Assert::AreEqual(xml.Error(), true);
		}

		/*TEST_METHOD(XmlBugTest1449463)
		{
			// Bug [ 1449463 ] Consider generic query
			TiXmlDocument xml;
			xml.Parse("<foo bar='3' barStr='a string'/>");

			TiXmlElement* ele = xml.FirstChildElement();
			double d;
			int i;
			float f;
			bool b;
			std::string str;

			XmlTest("QueryValueAttribute", ele->QueryValueAttribute("bar", &d), TIXML_SUCCESS);
			XmlTest("QueryValueAttribute", ele->QueryValueAttribute("bar", &i), TIXML_SUCCESS);
			XmlTest("QueryValueAttribute", ele->QueryValueAttribute("bar", &f), TIXML_SUCCESS);
			XmlTest("QueryValueAttribute", ele->QueryValueAttribute("bar", &b), TIXML_WRONG_TYPE);
			XmlTest("QueryValueAttribute", ele->QueryValueAttribute("nobar", &b), TIXML_NO_ATTRIBUTE);
			XmlTest("QueryValueAttribute", ele->QueryValueAttribute("barStr", &str), TIXML_SUCCESS);

			XmlTest("QueryValueAttribute", (d == 3.0), true);
			XmlTest("QueryValueAttribute", (i == 3), true);
			XmlTest("QueryValueAttribute", (f == 3.0f), true);
			XmlTest("QueryValueAttribute", (str == std::string("a string")), true);
		}//*/

		TEST_METHOD(XmlGetAttributeTest)
		{
			// [ 1505267 ] redundant malloc in TiXmlElement::Attribute
			TiXmlDocument xml;
			xml.Parse("<foo bar='3' />");
			TiXmlElement* ele = xml.FirstChildElement();
			double d;
			int i;

			string bar = "bar";
			const string* atrrib = ele->Attribute(bar);
			ele->Attribute(bar, &d);
			ele->Attribute(bar, &i);
			//Attribute
			Assert::AreEqual(atrrib->empty(), false);
			Assert::AreEqual(d, 3.0);
			Assert::AreEqual(i, 3);
		}

		/*TEST_METHOD(XmlInsertEndChildTest)
		{
			// [ 1356059 ] Allow TiXMLDocument to only be at the top level
			TiXmlDocument xml;
			TiXmlDocument xml2;
			xml.InsertEndChild(xml2);
			//Document only at top level.
			Assert::AreEqual(xml.Error(), true);
			//Document only at top level.
			int id = TiXmlBase::TIXML_ERROR_DOCUMENT_TOP_ONLY;
			Assert::AreEqual(xml.ErrorId(), id);
		}//*/

		/*TEST_METHOD(XmlMissingEndTagTest)
		{
			// [ 1663758 ] Failure to report error on bad XML
			TiXmlDocument xml;
			xml.Parse("<x>");
			//"Missing end tag at end of input"
			Assert::AreEqual(xml.Error(), true);
			xml.Parse("<x> ");
			//"Missing end tag with trailing whitespace"
			Assert::AreEqual(xml.Error(), true);
		}//*/

		/*TEST_METHOD(XmlNewLineTest)
		{
			// [ 1635701 ] fail to parse files with a tag separated into two lines
			// I'm not sure this is a bug. Marked 'pending' for feedback.
			TiXmlDocument xml;
			xml.Parse("<title><p>text</p\n><title>");
			//xml.Print();
			//XmlTest( "Tag split by newline", xml.Error(), false );
			Assert::AreEqual(xml.Error(), false);
		}//*/

		/*TEST_METHOD(XmlParsesCommentsTest)
		{
			// [ 1475201 ] TinyXML parses entities in comments
			TiXmlDocument xml;
			xml.Parse("<!-- declarations for <head> & <body> -->"
				"<!-- far &amp; away -->");

			TiXmlNode* e0 = xml.FirstChild();
			TiXmlNode* e1 = e0->NextSibling();
			TiXmlComment* c0 = e0->ToComment();
			TiXmlComment* c1 = e1->ToComment();

			//Comments ignore entities.
			Assert::AreEqual(c0->Value(), " declarations for <head> & <body> ");
			//Comments ignore entities.
			Assert::AreEqual(c1->Value(), " far &amp; away ");
		}//*/

		TEST_METHOD(XmlCommentsIterateTest)
		{
			TiXmlDocument xml;
			xml.Parse("<Parent>"
				"<child1 att=''/>"
				"<!-- With this comment, child2 will not be parsed! -->"
				"<child2 att=''/>"
				"</Parent>");
			int count = 0;
			TiXmlNode* ele = 0;
			while ((ele = xml.FirstChildElement("Parent")->IterateChildren(ele)) != 0) {
				++count;
			}
			//"Comments iterate correctly."
			Assert::AreEqual(count, 3);
		}

		/*TEST_METHOD(XmlBugTest1874301)
		{
			// trying to repro ]1874301]. If it doesn't go into an infinite loop, all is well.
			unsigned char buf[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?><feed><![CDATA[Test XMLblablablalblbl";
			buf[60] = 239;
			buf[61] = 0;

			TiXmlDocument doc;
			doc.Parse((const char*)buf);
		}//*/

		/*TEST_METHOD(XmlBugTest1827248)
		{
			// bug 1827248 Error while parsing a little bit malformed file
			// Actually not malformed - should work.
			TiXmlDocument xml;
			xml.Parse("<attributelist> </attributelist >");
			//Handle end tag whitespace
			Assert::AreEqual(xml.Error(), false);
		}//*/
		
		/*TEST_METHOD(XmlInfiniteLoopTest)
		{
			// This one must not result in an infinite loop
			TiXmlDocument xml;
			xml.Parse("<infinite>loop");
			XmlTest("Infinite loop test.", true, true);
		}//*/

		TEST_METHOD(XmlParseTagTest)
		{
			// 1709904 - can not repro the crash
			TiXmlDocument xml;
			xml.Parse("<tag>/</tag>");
			//"Odd XML parsing."
			Assert::AreEqual(xml.FirstChild()->Value(), "tag");
		}


		TEST_METHOD(XmlSampleTest)
		{
			//////////////////////
			// Copy and assignment
			//////////////////////
			/*printf("\n** Copy and Assignment **\n");
			{
				TiXmlElement element("foo");
				element.Parse("<element name='value' />", 0, TIXML_ENCODING_UNKNOWN);

				TiXmlElement elementCopy(element);
				TiXmlElement elementAssign("foo");
				elementAssign.Parse("<incorrect foo='bar'/>", 0, TIXML_ENCODING_UNKNOWN);
				elementAssign = element;

				XmlTest("Copy/Assign: element copy #1.", "element", elementCopy.Value());
				XmlTest("Copy/Assign: element copy #2.", "value", elementCopy.Attribute("name"));
				XmlTest("Copy/Assign: element assign #1.", "element", elementAssign.Value());
				XmlTest("Copy/Assign: element assign #2.", "value", elementAssign.Attribute("name"));
				XmlTest("Copy/Assign: element assign #3.", true, (0 == elementAssign.Attribute("foo")));

				TiXmlComment comment;
				comment.Parse("<!--comment-->", 0, TIXML_ENCODING_UNKNOWN);
				TiXmlComment commentCopy(comment);
				TiXmlComment commentAssign;
				commentAssign = commentCopy;
				XmlTest("Copy/Assign: comment copy.", "comment", commentCopy.Value());
				XmlTest("Copy/Assign: comment assign.", "comment", commentAssign.Value());

				TiXmlUnknown unknown;
				unknown.Parse("<[unknown]>", 0, TIXML_ENCODING_UNKNOWN);
				TiXmlUnknown unknownCopy(unknown);
				TiXmlUnknown unknownAssign;
				unknownAssign.Parse("incorrect", 0, TIXML_ENCODING_UNKNOWN);
				unknownAssign = unknownCopy;
				XmlTest("Copy/Assign: unknown copy.", "[unknown]", unknownCopy.Value());
				XmlTest("Copy/Assign: unknown assign.", "[unknown]", unknownAssign.Value());

				TiXmlText text("TextNode");
				TiXmlText textCopy(text);
				TiXmlText textAssign("incorrect");
				textAssign = text;
				XmlTest("Copy/Assign: text copy.", "TextNode", textCopy.Value());
				XmlTest("Copy/Assign: text assign.", "TextNode", textAssign.Value());

				TiXmlDeclaration dec;
				dec.Parse("<?xml version='1.0' encoding='UTF-8'?>", 0, TIXML_ENCODING_UNKNOWN);
				TiXmlDeclaration decCopy(dec);
				TiXmlDeclaration decAssign;
				decAssign = dec;

				XmlTest("Copy/Assign: declaration copy.", "UTF-8", decCopy.Encoding());
				XmlTest("Copy/Assign: text assign.", "UTF-8", decAssign.Encoding());

				TiXmlDocument doc;
				elementCopy.InsertEndChild(textCopy);
				doc.InsertEndChild(decAssign);
				doc.InsertEndChild(elementCopy);
				doc.InsertEndChild(unknownAssign);

				TiXmlDocument docCopy(doc);
				TiXmlDocument docAssign;
				docAssign = docCopy;

				std::string original, copy, assign;
				original << doc;
				copy << docCopy;
				assign << docAssign;
				XmlTest("Copy/Assign: document copy.", original.c_str(), copy.c_str(), true);
				XmlTest("Copy/Assign: document assign.", original.c_str(), assign.c_str(), true);
			}//*/



			//printf("\nPass %d, Fail %d\n", gPass, gFail);
			//return gFail;
		}
	};
}
