#ifndef CSV_READER_H
#define CSV_READER_H

#include <vector>

class CsvReader
{
public:
	std::vector<std::string> readCsv(const std::string &) const;

private:
	std::string read(const std::string &) const;
	std::vector<std::string> split(const std::string &, char delim = '\n') const;
};

#endif // CSV_READER_H
