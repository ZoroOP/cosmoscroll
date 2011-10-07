#ifndef ITEMDATA_HPP
#define ITEMDATA_HPP

#include "tinyxml/tinyxml.h"

class ItemData
{
public:
	enum Type
	{
		ARMOR,
		ENGINE,
		SHIELD,
		HEATSINK,
		LASER1,
		LASER2,
		_COUNT,
		_UNSET
	};
	static int const MAX_ITEM_LEVEL = 3;

	ItemData();

	static const char* TypeToString(Type type);

	const char* TypeToString() const;

	virtual bool LoadFromXml(TiXmlElement* elem);

	int GetPrice() const;
	int GetLevel() const;

	inline Type GetType() const { return type_; }
	void SetType(Type type) { type_ = type; }

	virtual std::wstring BuildDescriptionString(bool include_price=false) const = 0;


private:
	int price_;
	int level_;
	Type type_;
};

#endif // ITEMDATA_HPP