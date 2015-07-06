#ifndef __RANDOM__
#define __RANDOM___

#include <tucano.hpp>

#define NBINS 16

namespace Effects
{

/**
 * @brief Picks a 3D position from screen position
 */
class Random : public Tucano::Effect
{

public:

    /**
     * @brief Default constructor.
     */
    Random ()
    {
    }

    /**
     * @brief Default destructor
     */
    virtual ~Random (void ) {}

    /**
     * @brief Initializes the effect, creating and loading the shader.
     */
    virtual void initialize()
    {
        loadShader(shader, "random");
        scale = 0.01;
        quad.createQuad();
    }

    /**
     * @brief Renders the given texture.
     *
     * Renders the given texture using a proxy geometry, a quad the size of the viewport
     * to hold the texture.
     */
    void renderTexture (Texture& tex, Eigen::Vector2i viewport)
    {
        glViewport(0, 0, viewport[0], viewport[1]);

        shader.bind();
        shader.setUniform("frameTexture", tex.bind());
        shader.setUniform("viewport", viewport);
        shader.setUniform("scale", scale);
        quad.render();

        shader.unbind();
        tex.unbind();
    }

    void incScale(){
        scale += 0.01;
    }

    void decScale(){
        scale -= 0.01;
    }

private:

    /// The mean filter shader.
    Shader shader;

    /// A quad to be rendered forcing one call of the fragment shader per image pixel (its just a proxy geometry)
    Mesh quad;

    ///
    float scale;
};

}



#endif

