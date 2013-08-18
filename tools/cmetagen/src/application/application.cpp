#include "application.h"

#include "model/cppcontext.h"
#include "model/cppnamespace.h"
#include "buildermodel/buildercontext.h"
#include "buildermodel/builderutil.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"
#include "codewriter/codeblock.h"
#include "exception.h"
#include "logger.h"
#include "util.h"
#include "constants.h"
#include "commandlineparser.h"

#include "Poco/Glob.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"

#include <set>

#include <iostream>
#include <fstream>

using namespace std;
using namespace cpgf;

namespace metagen {


Application::Application()
{
}

Application::~Application()
{
}

int Application::run(int argc, char * argv[])
{
	int exitCode = 1;
	try {
		this->doRun(argc, argv);
		exitCode = 0;
	}
	catch(const AbortException &) {
	}
	catch(const Exception & e) {
		getLogger().error(string("\n") + e.what());
	}
	catch(const Poco::Exception & e) {
		getLogger().error(string("\n") + e.displayText());
	}
	cout << endl;
	return exitCode;
}

void Application::doRun(int argc, char * argv[])
{
	CommandLineParser commandLineParser(&this->project);
	commandLineParser.parse(argc, argv);

	this->processFiles();

	getLogger().info("Generating main register file...");
	globFiles(this->project.getHeaderOutputPath(), "*" + this->project.getHeaderFileExtension(),
		makeCallback(this, &Application::loadCreationFunctionsFromFile));
	generateMainRegisterFiles(this->creationFunctionNameList, &this->project);
	getLogger().info("done.\n");
}

void Application::processFiles()
{
	for(StringArrayType::const_iterator it = this->project.getFiles().begin();
		it != this->project.getFiles().end();
		++it) {
		this->processOnePath(Poco::Path(*it).makeAbsolute(Poco::Path(this->project.getSourceRootPath())).toString());
	}
}

void Application::processOnePath(const std::string & path)
{
	set<string> fileSet;
	string absolutePath = this->project.getAbsoluteFileName(path);

	Poco::Glob::glob(absolutePath, fileSet, Poco::Glob::GLOB_DOT_SPECIAL);
	if(fileSet.empty()) {
		getLogger().warn(Poco::format("Can't find any files in %s.\n", absolutePath));
	}
	for(set<string>::iterator it = fileSet.begin(); it != fileSet.end(); ++it) {
		if(Poco::File(*it).isDirectory()) {
			continue;
		}
		else {
			this->processOneFile(*it);
		}
	}
}

void Application::processOneFile(const std::string & fileName)
{
	if(! this->project.processFileByScript(fileName)) {
		getLogger().info(Poco::format("File %s is skipped by script.\n", fileName));
		return;
	}

	if(isFileAutoGenerated(fileName)) {
		getLogger().info(Poco::format("File %s is auto generated, skipped.\n", fileName));
		return;
	}

	string outputHeaderFileName = this->project.getOutputHeaderFileName(fileName);
	if(this->project.doesForce()
		|| shouldTargetFileBeUpdated(fileName, outputHeaderFileName)) {
		getLogger().info(Poco::format("Generate for fileName %s...", fileName));

		CppContext context(&this->project);
		string absoluteFileName = this->project.getAbsoluteFileName(fileName);
		context.process(absoluteFileName);

		BuilderContext builderContext(&this->project, absoluteFileName,
			makeCallback(this, &Application::onGenerateCreationFunction));
		builderContext.process(&context);

		getLogger().info("done.\n");
	}
	else {
		getLogger().info(Poco::format("File %s is up to date, skipped.\n", fileName));
	}
}

void Application::loadCreationFunctionsFromFile(const std::string & fileName)
{
	ifstream file(fileName.c_str());
	string line;
	bool isInCreations = false;
	while(getline(file, line)) {
		if(isInCreations) {
			if(line == GeneratedCreationFunctionEndMark) {
				return;
			}
			this->onGenerateCreationFunction(line);
		}
		else {
			if(line == GeneratedCreationFunctionBeginMark) {
				isInCreations = true;
			}
		}
	}
}

void Application::onGenerateCreationFunction(const std::string & creationFunctionName)
{
	if(this->creationFunctionNameList.find(creationFunctionName) != this->creationFunctionNameList.end()) {
		getLogger().warn(Poco::format("Creation function %s is duplicated.", creationFunctionName));
	}
	this->creationFunctionNameList.insert(creationFunctionName);
}


} // namespace metagen
