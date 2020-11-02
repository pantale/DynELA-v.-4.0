/***************************************************************************
 *                                                                         *
 *  DynELA Finite Element Code v 3.0                                       *
 *  by Olivier PANTALE                                                     *
 *                                                                         *
 *  (c) Copyright 1997-2020                                                *
 *                                                                         *
 **************************************************************************/
//@!CODEFILE = DynELA-H-file
//@!BEGIN = PRIVATE

/*!
  \file Settings.h
  \brief Definition of the Settings class.

  This file defines the Settings class.
  \ingroup dnlKernel
*/

#ifndef __dnlKernel_Settings_h__
#define __dnlKernel_Settings_h__

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>

class Settings
{
    bool _isChanged;
    const std::locale _locale;
    std::map<std::string, std::string> _data;
    std::string _filename;

    bool read();
    bool write() const;
    std::pair<std::string, std::string> parseLine(const std::string &line) const;
    template <typename T>
    std::string convertToStr(const T input) const;
    template <typename T>
    T convertToType(const std::string &input) const;

public:
    Settings();
    ~Settings();

    bool isChanged() const;
    bool loadFromFile(const std::string &filename);
    bool saveToFile();
    template <typename T>
    void getValue(const std::string &key, std::vector<T> &value) const;
    template <typename T>
    void getValue(const std::string &key, T &value) const;
    template <typename T>
    void setValue(const std::string &key, const std::vector<T> value);
    template <typename T>
    void setValue(const std::string &key, const T value);
    void dump() const;
};

//-----------------------------------------------------------------------------
template <typename T>
inline std::string Settings::convertToStr(T input) const
//-----------------------------------------------------------------------------
{
    throw "Unsupported type supplied, either change types, or write a correct conversion function for the template type.";
}

//-----------------------------------------------------------------------------
template <>
inline std::string Settings::convertToStr<std::string>(std::string value) const
//-----------------------------------------------------------------------------
{
    return value;
}

//-----------------------------------------------------------------------------
template <>
inline std::string Settings::convertToStr<const char *>(const char *value) const
//-----------------------------------------------------------------------------
{
    return std::string(value);
}

//-----------------------------------------------------------------------------
template <>
inline std::string Settings::convertToStr<bool>(bool value) const
//-----------------------------------------------------------------------------
{
    return (value) ? "TRUE" : "FALSE";
}

//-----------------------------------------------------------------------------
template <>
inline std::string Settings::convertToStr<char>(char value) const
//-----------------------------------------------------------------------------
{
    std::string tmp = "";
    tmp = value;
    return tmp;
}

//-----------------------------------------------------------------------------
template <>
inline std::string Settings::convertToStr<int>(int value) const
//-----------------------------------------------------------------------------
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

//-----------------------------------------------------------------------------
template <>
inline std::string Settings::convertToStr<unsigned int>(unsigned int value) const
//-----------------------------------------------------------------------------
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

//-----------------------------------------------------------------------------
template <>
inline std::string Settings::convertToStr<float>(float value) const
//-----------------------------------------------------------------------------
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

//-----------------------------------------------------------------------------
template <>
inline std::string Settings::convertToStr<short>(short value) const
//-----------------------------------------------------------------------------
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

//-----------------------------------------------------------------------------
template <>
inline std::string Settings::convertToStr<double>(double value) const
//-----------------------------------------------------------------------------
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

//-----------------------------------------------------------------------------
template <typename T>
inline T Settings::convertToType(const std::string &input) const
//-----------------------------------------------------------------------------
{
    throw "Unconvertable type encountered, please use a different type, or define the handle case in Settings.h";
}

//-----------------------------------------------------------------------------
template <>
inline int Settings::convertToType<int>(const std::string &input) const
//-----------------------------------------------------------------------------
{
    int value;
    std::stringstream ss(input);
    ss >> value;

    return value;
}

//-----------------------------------------------------------------------------
template <>
inline unsigned int Settings::convertToType<unsigned int>(const std::string &input) const
//-----------------------------------------------------------------------------
{
    unsigned int value;
    std::stringstream ss(input);
    ss >> value;

    return value;
}

//-----------------------------------------------------------------------------
template <>
inline double Settings::convertToType<double>(const std::string &input) const
//-----------------------------------------------------------------------------
{
    double value;
    std::stringstream ss(input);
    ss >> value;

    return value;
}

//-----------------------------------------------------------------------------
template <>
inline float Settings::convertToType<float>(const std::string &input) const
//-----------------------------------------------------------------------------
{
    float value;
    std::stringstream ss(input);
    ss >> value;

    return value;
}

//-----------------------------------------------------------------------------
template <>
inline short Settings::convertToType<short>(const std::string &input) const
//-----------------------------------------------------------------------------
{
    short value;
    std::stringstream ss(input);
    ss >> value;

    return value;
}

//-----------------------------------------------------------------------------
template <>
inline bool Settings::convertToType<bool>(const std::string &input) const
//-----------------------------------------------------------------------------
{
    return input == "TRUE" ? true : false;
}

//-----------------------------------------------------------------------------
template <>
inline char Settings::convertToType<char>(const std::string &input) const
//-----------------------------------------------------------------------------
{
    return input[0];
}

//-----------------------------------------------------------------------------
template <>
inline std::string Settings::convertToType<std::string>(const std::string &input) const
//-----------------------------------------------------------------------------
{
    return input;
}

//-----------------------------------------------------------------------------
template <typename T>
inline void Settings::getValue(const std::string &key, T &value) const
//-----------------------------------------------------------------------------
{
    auto it = _data.find(key);

    if (it != _data.end())
    {
        value = convertToType<T>(it->second);
    }
/*     else
    {
        int itr = 0;
        std::string locKey;
        bool endSeek = false;
        while (!endSeek)
        {
            locKey = settingsDefauts[itr].key;
            if (locKey == "END")
                endSeek = true;
            if (locKey == key)
            {
                value = convertToType<T>(settingsDefauts[itr].value);
                endSeek = true;
            }
            itr++;
        }
    }
 */}

/**
 * This method tries to read the value of a key into a vector. The values have to be
 * seperated by comma. The vector is cleared before it is filled.
 */
//-----------------------------------------------------------------------------
template <typename T>
inline void Settings::getValue(const std::string &key, std::vector<T> &value) const
//-----------------------------------------------------------------------------
{
    auto it = _data.find(key);
    if (it != _data.end())
    {
        std::string output;
        std::istringstream parser(it->second);
        value.clear();

        //split by comma
        while (getline(parser, output, ','))
        {
            value.push_back(convertToType<T>(output));
        }
    }
}

//-----------------------------------------------------------------------------
template <typename T>
inline void Settings::setValue(const std::string &key, const T value)
//-----------------------------------------------------------------------------
{
    // the [] operator replaces the value if the key is found, if not it creates a new element
    _data[key] = convertToStr<T>(value);
    _isChanged = true;
}

//-----------------------------------------------------------------------------
template <typename T>
inline void Settings::setValue(const std::string &key, const std::vector<T> value)
//-----------------------------------------------------------------------------
{
    // transform the vector into a string that seperates the elements with a comma
    std::string valueAsString;
    for (size_t i = 0; i < value.getSize() - 1; ++i)
    {
        valueAsString += convertToStr<T>(value.at(i)) + ",";
    }
    valueAsString += convertToStr<T>(value.back());

    // the [] operator replaces the value if the key is found, if not it creates a new element
    _data[key] = valueAsString;
    _isChanged = true;
}

#endif
