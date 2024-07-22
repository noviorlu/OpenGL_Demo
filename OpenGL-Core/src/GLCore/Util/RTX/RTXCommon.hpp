#include <vector>

namespace GLCore::Utils {
	class IIterator {
	public:
		enum SceneDataType
		{
			GROUP = 0,
			TRANSFORM,
			MODEL,
			MESH,
			SUBMESH,
			TRIANGLE
		};

		static int SeralizeGroup(std::vector<float>& SceneData, int childCount, int ret) {
			// Since we are using 4 channel texture, to accomodate we round up to the nearest 4
			// in glsl, check type of Group, if it is GROUP, then iterate number of frac pixels
			// depth first search through all childs and if meet negative value, then return to parent
			int childPos = SceneData.size();
			
			// leave enough space for childs, if childCount+1 divisible by 4, no need add 1
			int frac = (childCount + 1) / 4;
			frac += ((childCount + 1 - frac * 4) != 0);

			SceneData.push_back(GROUP);
			SceneData.push_back(frac+1); // occupy frac+1 pixels
			SceneData.push_back(-1);
			SceneData.push_back(-1);
			for(int i = 0; i < frac * 4; i++) SceneData.push_back(-ret);

			return childPos;
		}
	};
}