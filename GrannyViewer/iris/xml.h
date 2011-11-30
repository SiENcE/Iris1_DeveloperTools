/*****
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****/
/*
	File: xml.h
	Created: DarkStorm
	Purpose: xml parser class
	Changelog:
		- added asBool / asString (Gigi)
		- fixed mem leak (Gigi)
		- use tinyxml internally (harkon)
*/
#if !defined(__XMLDOM_H__)
#define __XMLDOM_H__

#include <vector>
#include <string>

#include "tinyxml/tinyxml.h"

namespace XML
{
class Node;

class Parser
{
	TiXmlDocument m_doc;
	void reportError(const std::string &error);
public:
	Node *parseDocument();
	void setData( const std::string &data );
	void loadData( const std::string &filename );

	Parser( const std::string &data ) { setData( data ); }
	Parser() {}
	virtual ~Parser();
};

enum eNodeType
{
	Document,
	Element,
	Attribute,
	ProcessingInstruction,
	XMLDeclaration
};

class Node
{
	TiXmlNode *m_node;
	std::vector<Node*> m_child;
	Node *allocNode(TiXmlElement *tnode);
public:
	Node(TiXmlNode *node=NULL) : m_node(node) {}
	virtual ~Node();

	TiXmlNode *getTinyNode() { return m_node; }
	operator bool() const { return m_node != NULL; }

	int  addNode( Node *node );
	bool hasNode( const std::string &name ) const;
	void removeNode( unsigned int index );
	void removeNode( Node *node );
	Node *findNode( const std::string &name, int index, bool raiseError = false );
	Node *findNode( const std::string &name, bool raiseError = false ) { return findNode( name, 0, raiseError ); }
	
	bool lookupAttribute( const std::string &name, unsigned int &data ) const;
	bool lookupAttribute( const std::string &name, int &data ) const;
	bool lookupAttribute( const std::string &name, std::string &data ) const;

	unsigned int getNodeCount() const;// { return nodes.size(); }
	Node *getNode( unsigned int index ) const;
	unsigned int getTotalNodeCount() const;
	
	int asInteger() const;
	bool asBool() const;
	std::string asString() const;
	float asFloat () const;

	// Overloading the int operator
	Node *operator[]( unsigned int index ) const;// { return getNode( index ); }
	Node *operator[]( int index ) const;// { return getNode( (unsigned int)index ); }
	Node *operator[]( const char* name ) const;// { return findNode( name ); }

	// Setters/Getters
	eNodeType nodeType() const;// { return _nodeType; }
	void setNodeType( eNodeType nodeType );// { _nodeType = nodeType; }
	const std::string &name() const;// { return _name; }
	const std::string &data() const;// { return _data; }
	void setName( const std::string &name );// { _name = name; }
	void setData( const std::string &data );// { _data = data; }

};
};

#endif
