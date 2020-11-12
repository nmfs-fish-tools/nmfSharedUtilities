
#include "nmfConstants.h"
#include "nmfLogger.h"

nmfLogger::nmfLogger()
{
}


nmfLogger::~nmfLogger()
{

}

std::string nmfLogger::getTimestamp(bool withUnderscores)
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[100];

    // Find current timestamp
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (withUnderscores)
        strftime(buffer,sizeof(buffer),"%Y_%m_%d_%H_%M_%S",timeinfo);
    else
        strftime(buffer,sizeof(buffer),"%Y_%m_%d %H:%M:%S",timeinfo);
    std::string timestamp(buffer);

    return timestamp;
}

void nmfLogger::initLogger(std::string appName)
{
    filename = "." + appName + "//logs//"+ appName + "_" + getTimestamp(true) + ".log";
    std::ofstream outputFileStream;
    outputFileStream.open (filename, std::ofstream::out);
    outputFileStream.close();
}


std::string
nmfLogger::getLogFile()
{
    if (filename == "")
        return "";
    else
        return filename;
}

void nmfLogger::logMsg(int color, std::string msg)
{
  if (filename == "")
      return;

  std::cout << msg << std::endl;

  // write out message with timestamp
  std::ofstream outputFileStream;
  outputFileStream.open (filename,
            std::ofstream::out | std::ofstream::app);
  outputFileStream << color << " " << getTimestamp(false) << " - " << msg << "\n";

  outputFileStream.close();
}
