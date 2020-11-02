/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-C-file
//@!BEGIN = PRIVATE

/*!
  \file Settings.C
  \brief Definition of the Settings class.

  This file defines the Settings class.
  \ingroup dnlKernel
*/

#include <Settings.h>
#include <System.h>
#include <fstream>

//-----------------------------------------------------------------------------
Settings::Settings() : _isChanged(false)
//-----------------------------------------------------------------------------
{
    // Get the path value
    System sys = System();
    String path = sys.getEnvironmentValue("DYNELA_BIN");

    // Loads the settings from configuration file
    if (path != "")
    {
        loadFromFile(path + "/configuration.dnl");
        _filename = "";
    }
}

//-----------------------------------------------------------------------------
Settings::~Settings()
//-----------------------------------------------------------------------------
{
    saveToFile();
}

//-----------------------------------------------------------------------------
bool Settings::loadFromFile(const std::string &filename)
//-----------------------------------------------------------------------------
{
    _data.clear();
    _filename = filename;
    return read();
}

//-----------------------------------------------------------------------------
bool Settings::saveToFile()
//-----------------------------------------------------------------------------
{
    if (_isChanged)
    {
        _isChanged = false;
        return write();
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Settings::read()
//-----------------------------------------------------------------------------
{
    std::ifstream in(_filename);
    if (!in.is_open())
    {
        std::cerr << "Error: Unable to open settings file \"" << _filename << "\" for reading!" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(in, line))
    {
        // parse line
        std::pair<std::string, std::string> keyValuePair = parseLine(line);

        if (!keyValuePair.first.empty())
        {
            // if the line is not empty or a comment save it to the map
            _data[keyValuePair.first] = keyValuePair.second;
        }
    }

    in.close();
    _isChanged = false;
    return true;
}

//-----------------------------------------------------------------------------
bool Settings::write() const
//-----------------------------------------------------------------------------
{
    std::vector<std::pair<std::string, std::string>> fileContents;

    std::ifstream in(_filename);

    // read the file into a vector and replace the values of the keys that match with our map
    if (in.is_open())
    {
        std::string line;
        while (std::getline(in, line))
        {
            // parse line
            std::pair<std::string, std::string> keyValuePair = parseLine(line);

            if (!keyValuePair.first.empty())
            {
                // check if the key is found in the map
                auto it = _data.find(keyValuePair.first);
                if (it != _data.end())
                {
                    // if so take it's value, otherwise the value from the file is kept
                    keyValuePair.second = it->second;
                }
            }
            else
            {
                // if the line is empty or a comment simply take the whole line as the key
                keyValuePair.first = line;
            }
            fileContents.push_back(keyValuePair);
        }
    }
    else
    {
        // Can't open file for reading. Use only the data from the map
        for (auto it = _data.begin(); it != _data.end(); ++it)
            fileContents.push_back(std::make_pair(it->first, it->second));
    }

    in.close();

    // open the file for writing
    std::ofstream out(_filename);
    if (!out.is_open())
    {
        std::cerr << "Error: Unable to open settings file \"" << _filename << "\" for writing!" << std::endl;
        return false;
    }
    for (auto it = fileContents.begin(); it != fileContents.end(); ++it)
    {
        out << it->first; // write the key

        if (!it->second.empty())
            // if this line is not empty or a comment also write the assignment and the value
            out << " = " << it->second;

        out << std::endl;
    }
    out.close();
    return true;
}

/**
 * This method parses a line from our format ("key = value") into a std::pair<std::string, std::string>
 * containing the key and the value.
 * If the line is empty or a comment (starts with a '#') an empty pair is returned.
 */
//-----------------------------------------------------------------------------
std::pair<std::string, std::string> Settings::parseLine(const std::string &line) const
//-----------------------------------------------------------------------------
{
    if (line.size() > 0 && line[0] != '#')
    {
        size_t index = 0;
        // trim leading whitespace
        while (std::isspace(line[index], _locale))
            index++;
        // get the key string
        const size_t beginKeyString = index;
        while (!std::isspace(line[index], _locale) && line[index] != '=')
            index++;
        const std::string key = line.substr(beginKeyString, index - beginKeyString);

        // skip the assignment
        while (std::isspace(line[index], _locale) || line[index] == '=')
            index++;

        // get the value string
        const std::string value = line.substr(index, line.size() - index);

        // return the key value pair
        return std::make_pair(key, value);
    }

    // if this line is emtpy or a comment, return an empty pair
    return std::make_pair(std::string(), std::string());
}

//-----------------------------------------------------------------------------
void Settings::dump() const
//-----------------------------------------------------------------------------
{
    for (auto &element : _data)
        std::cout << element.first << " = " << element.second << std::endl;

    std::cout << std::endl
              << "Size: " << _data.size() << std::endl;
}

//-----------------------------------------------------------------------------
bool Settings::isChanged() const
//-----------------------------------------------------------------------------
{
    return _isChanged;
}
