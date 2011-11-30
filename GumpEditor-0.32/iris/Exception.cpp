//
// File: Exception.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "Exception.h"

using namespace std;

cException::cException (std::string message, std::string file, int line)
{
      this->m_message = message;
      this->m_file = file;
      this->m_line = line;
}

cException::~cException ()
{
}

std::string cException::message ()
{
      return m_message;
}


std::string cException::file ()
{
      return m_file;
}

int cException::line ()
{
      return m_line;
}

std::string cException::debug_message ()
{
      char str_line[32];
      sprintf(str_line, "%i", line ());
      return ("an exception occured: " + message () + "(" + file () + ", " + string(str_line) + ")");
}



cAssertException::cAssertException (std::string file, int line)
     : cException("", file, line)
{
}

std::string cAssertException::message ()
{
   return "Assertion failed"; 
}

cXMLException::cXMLException (std::string message, std::string file, int line)
     : cException(message, file, line)
{
}


std::string cXMLException::message ()
{
      return "XML: " + cException::message();
}




