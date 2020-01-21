#ifndef MAPPING_CHECKER_H
#define MAPPING_CHECKER_H

namespace mappers {
	class ProvinceMapper;
	class ReligionMapper;
	class CultureMapper;
}

namespace EU4 {
	class World;
}

namespace V2
{
	class MappingChecker
	{
	public:
		void check(const EU4::World& sourceWorld, 
			const mappers::ProvinceMapper& provinceMapper, 
			const mappers::ReligionMapper& religionMapper, 
			const mappers::CultureMapper& cultureMapper);
		
	private:
		void checkAllEU4ProvincesMapped(const EU4::World& sourceWorld, const mappers::ProvinceMapper& provinceMapper) const;
		void checkAllEU4ReligionsMapped(const EU4::World& sourceWorld, const mappers::ReligionMapper& religionMapper) const;
		void checkAllEU4CulturesMapped(const EU4::World& sourceWorld, const mappers::CultureMapper& cultureMapper) const;
	};
}
#endif // MAPPING_CHECKER_H
