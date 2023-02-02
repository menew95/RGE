#include "Importer_pch.h"
#include "Deserializer/Deserializer.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/archive/binary_iarchive.hpp" 
#include "boost/iostreams/filtering_stream.hpp"
#include "boost/iostreams/filter/zlib.hpp"

namespace Utility
{
	template<>
	bool Deserializer::Deserialize(BinaryData::MaterialData& meterialData, uuid uuid)
	{
		std::string materialPathStr = "Asset/BinaryFile/Material/";

		std::string _uuid = StringHelper::WStringToString(uuid);

		std::ifstream ifs(materialPathStr + _uuid + ".mat", std::ios_base::binary);

		if (!ifs.is_open())
			return false;

		boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
		buffer.push(boost::iostreams::zlib_decompressor());
		buffer.push(ifs);
		boost::archive::binary_iarchive iaMat(buffer);
		iaMat >> meterialData;

		return true;
	}

	template<>
	bool Deserializer::Deserialize(BinaryData::MeshData& meshData, uuid uuid)
	{
		std::string meshPathStr = "Asset/BinaryFile/Mesh/";

		std::string _uuid = StringHelper::WStringToString(uuid);

		std::ifstream ifs(meshPathStr + _uuid + ".mesh", std::ios_base::binary);

		if (!ifs.is_open())
			return false;

		boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
		buffer.push(boost::iostreams::zlib_decompressor());
		buffer.push(ifs);
		boost::archive::binary_iarchive iaMesh(buffer);
		iaMesh >> meshData;

		return true;
	}

	template<>
	bool Deserializer::Deserialize(BinaryData::SkinnedData& skinnedData, uuid uuid)
	{
		std::string bonePathStr = "Asset/BinaryFile/Bone/";

		std::string _uuid = StringHelper::WStringToString(uuid);

		std::ifstream ifs(bonePathStr + _uuid + ".bone", std::ios_base::binary);

		if (!ifs.is_open())
			return false;

		boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
		buffer.push(boost::iostreams::zlib_decompressor());
		buffer.push(ifs);
		boost::archive::binary_iarchive iaBone(buffer);
		iaBone >> skinnedData;

		return true;
	}

	template<>
	bool Deserializer::Deserialize(BinaryData::AnimationClipData& animationClipData, uuid uuid)
	{
		std::string animationPathStr = "Asset/BinaryFile/Animation/";

		std::string _uuid = StringHelper::WStringToString(uuid);

		std::ifstream ifs(animationPathStr + _uuid + ".anim", std::ios_base::binary);

		if (!ifs.is_open())
			return false;

		boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
		buffer.push(boost::iostreams::zlib_decompressor());
		buffer.push(ifs);
		boost::archive::binary_iarchive iaAnim(buffer);
		iaAnim >> animationClipData;

		return true;
	}
}