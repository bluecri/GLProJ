#include <src/glMain/ShaderObj.h>

class ShaderShadow : public ShaderObj{
	public:


		ShaderShadow(const char * vertexShader, const char * fragmentShader) : ShaderObj(vertexShader, fragmentShader) {
			if (m_shaderID != -1) {
				depthMatrixID = glGetUniformLocation(m_shaderID, "depthMVP");
			}
		}
};